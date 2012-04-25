#ifndef __VYZKUM_POPULATION__
#define __VYZKUM_POPULATION__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
using namespace std;

/*
	Inheritance is organized as cube directed acyclic graph, one pair of sides representing
	anneling/non-annealing population, next pair single/multiobjective
	and last small/large candidates
	All traits are aditive with respect to inheritance (other traits 
	can be added by adding a dimension to the cube)

	basic
	  ^
	  |
	sigle <----------------	multi
	  ^	^					  ^ ^
	  |	 \					  |	 \
	  |	  ann-single <----------- ann-multi
	  |			^			  |			^
	  |			|			  |			|
	large-single<----------	large-multi	|	
		^		|				^		|
		 \		|				 \		|
		  ann-large-single <----- ann-large-multi

	This structure allows parts to properly use different levels of
	abstraction without conflicts (arising for example form one part 
	using annealing pop and other using its non-annealing counterpart)
*/


template<int dim, typename vectorType>
class abstractBasicPopulation{
	protected:
	typedef basicCandidate<dim,vectorType> specBasCandidate; 

	const int populationSize, offspringSize, bestArcSize;
	specBasCandidate **pop, **offspr, **best;
	double gridStep;	//?
	vectorType upperLimit[dim],lowerLimit[dim];		//grid mesh

	//friend declaration, subject to change
	/*template<int,typename,int,typename> friend class selectAllMethod;
	template<int,typename,int,typename> friend class inverseHilbMtrxNorm;
	template<int,typename,int,typename> friend class randomNbDisplace;
	template<int,typename,int,typename> friend class fightParentAnnealing;
	template<int,typename,int,typename> friend class plainCopyReproduction;

	template<int,typename,int,typename> friend class evaluationMethod;*/

	public:
	abstractBasicPopulation<dim,vectorType>(int pSize, int oSize, int bestA):
	  populationSize(pSize),offspringSize(oSize),bestArcSize(bestA){
		pop = new specBasCandidate*[populationSize+offspringSize];		//for possible sorting in selection
		offspr = &(pop[populationSize]);								//to allow parent-child comparison
		best = new specBasCandidate*[bestArcSize];
	}
	
	//convenience methods for process stages
	  inline specBasCandidate** GetPopulation(){return pop;}
	  inline specBasCandidate** GetOffspring(){return offspr;}
	  inline int GetPopSize(){return populationSize;}
	  inline int GetOffsprSize(){return offspringSize;}
	  inline int GetLimits(vectorType **upper, vectorType **lower){(*upper) = upperLimit; (*lower) = lowerLimit; return 1;}

	virtual bool Create() = 0;									//(random) creation and initial evaluation
	const specBasCandidate *GetBestCandidate(const int idx) const {return (best[idx]);}
};

//------------------------------------------------------------------------------------------------------------------------
/*
	More spetialized populations follow:
		- if some part does not need more specific information, it can use pointer to theese populations
*/

template<int dim, typename vectorType, typename evalType>
class basicSingleObjPopulation : public abstractBasicPopulation<dim,vectorType>{
	public:
	typedef singleObjectiveCandidate<dim,vectorType,evalType> specSingleObjCandidate; 

	basicSingleObjPopulation<dim,vectorType,evalType>(int pSize, int oSize, int bestA) :
		abstractBasicPopulation<dim,vectorType>(pSize,oSize,bestA){}

	inline specSingleObjCandidate** GetPopulation(){return reinterpret_cast<specSingleObjCandidate**>(pop);}
	inline specSingleObjCandidate** GetOffspring(){return reinterpret_cast<specSingleObjCandidate**>(offspr);}
};


template<int dim, typename vectorType, typename evalType>
class basicSingleObjAnnePopulation : public  basicSingleObjPopulation<dim,vectorType,evalType>{
	double temperature;
	public:
	basicSingleObjAnnePopulation<dim,vectorType,evalType>(int pSize, int oSize, int bestA, double t) :
		basicSingleObjPopulation<dim,vectorType,evalType>(pSize,oSize,bestA), temperature(t){}
};


//--------------------------------------------------------------------------------------------

//vectorType should be signed int. After mutation all candidates are normalised to have their center of gravity at (0,0)
template<int dim, typename vectorType, typename evalType, 
	template<int,typename,typename> class _evaluationMethod, 
	template<int,typename> class _mutationMethod,
	template<int,typename,typename> class _mergingMethod, 
	template<int,typename,typename> class _reproductionMethod>
class testingClassicGOPopulation : public basicSingleObjPopulation<dim,vectorType,evalType>{


	public:
	typedef _mutationMethod<dim,vectorType> specMutMethod;
	typedef _reproductionMethod<dim,vectorType,evalType> specRepMethod;
	typedef _evaluationMethod<dim,vectorType,evalType> specEvaMethod;
	typedef _mergingMethod<dim,vectorType,evalType> specMerMethod;
	typedef singleObjectiveCandidate<dim,vectorType,evalType> specSingleObjCandidate; 

	//typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	protected:
		specEvaMethod eva;
		specMerMethod mer;
		specMutMethod mut;
		specRepMethod rep;

	public:
	testingClassicGOPopulation<dim,vectorType,evalType,
		_evaluationMethod,_mutationMethod,
		_mergingMethod,_reproductionMethod>(int pSize, int oSize, int bestA) :
		basicSingleObjPopulation<dim,vectorType,evalType>(pSize,oSize,bestA){}

	bool Create(){
		eva.Init(this);
		mer.Init(this);
		mut.Init(this);
		rep.Init(this);

		//create initial population
		#define bigrand() (rand() | rand() << 16)
		for(int i=0;i<populationSize; i++){
			pop[i] = new specSingleObjCandidate();
			for(int j=0; j<dim; j++){
				pop[i]->components[j] = (rand()-RAND_MAX/2); //bigrand();
			}
		}
		//initialize offspring
		for(int i=0;i<offspringSize; i++){
			offspr[i] = new specSingleObjCandidate();
			for(int j=0; j<dim; j++){
				offspr[i]->components[j] = 0;
			}
		}
		//initially evaluate population
		eva.PerformEvaluation(true);
		
		//prepare graphics
		InitGraphics();
		return true;
	}

	bool BadCand(){
		for(int i=0;i<populationSize+offspringSize;i++){
			for(int j=0;j<dim;j++){
				if(pop[i]->components[j] == -2147483648) return true;
			}
		}
		return false;
	}
	void PrintPop(){
		cout.fill(' ');
		cout <<'\n';
		for(int i=0;8*i < populationSize;i++){
			for(int k=0;k<dim;k++){
				for(int j=0;j<8;j++){
					if(i*8+j >= populationSize){cout<<'\n';break;}
					cout <<setw(10)<<right<< GetPopulation()[i*8+j]->components[k];
				}
			}
			cout << "--------------------------------\n";
		}
	}

	bool NextGeneration(){
		char b;
		//PrintPop();
		rep.PerformReproduction();
		if(BadCand()){
			cout << "BAD after reproduction";
			//PrintPop();
			cin >> b;
		}
		mut.PerformMutation();
		if(BadCand()){
			cout << "BAD after mutation";
			//PrintPop();
			cin >> b;
		}
		eva.PerformEvaluation();
		if(BadCand()){
			cout << "BAD after evaluation";
			//PrintPop();
			cin >> b;
		}
		mer.PerformMerge();
		if(BadCand()){
			cout << "BAD after merge";
			//PrintPop();
			cin >> b;
		}
		//print info about population
		cout.precision(2);
		cout << fixed;
		cout << "New generation:-----------\n";
		for(int i=0;i<populationSize; i++){
			//if(i==populationSize) cout << "---\n";
			cout << /*GetPopulation()[i]->components[0] << " " <<*/ GetPopulation()[i]->fitness << '\n';
		}
		DrawScene<dim,vectorType,evalType>(populationSize,pop);
		return true;
	}

	//bool sdlInit(
};


#endif