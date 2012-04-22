#ifndef __VYZKUM_POPULATION__
#define __VYZKUM_POPULATION__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>

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

	unsigned const populationSize, offspringSize, bestArcSize;
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
	abstractBasicPopulation<dim,vectorType>(unsigned pSize, unsigned oSize, unsigned bestA):
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

	basicSingleObjPopulation<dim,vectorType,evalType>(unsigned pSize, unsigned oSize, unsigned bestA) :
		abstractBasicPopulation<dim,vectorType>(pSize,oSize,bestA){}

	inline specSingleObjCandidate** GetPopulation(){return reinterpret_cast<specSingleObjCandidate**>(pop);}
	inline specSingleObjCandidate** GetOffspring(){return reinterpret_cast<specSingleObjCandidate**>(offspr);}
};


template<int dim, typename vectorType, typename evalType>
class basicSingleObjAnnePopulation : public  basicSingleObjPopulation<dim,vectorType,evalType>{
	double temperature;
	public:
	basicSingleObjAnnePopulation<dim,vectorType,evalType>(unsigned pSize, unsigned oSize, unsigned bestA, double t) :
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
		_mergingMethod,_reproductionMethod>(unsigned pSize, unsigned oSize, unsigned bestA) :
		basicSingleObjPopulation<dim,vectorType,evalType>(pSize,oSize,bestA){}

	bool Create(){
		eva.Init(this);
		mer.Init(this);
		mut.Init(this);
		rep.Init(this);

		//create initial population
		#define bigrand() (rand() | rand() << 16)
		for(unsigned i=0;i<populationSize; i++){
			pop[i] = new specBasCandidate();
			for(unsigned j=0; i<dim; j++){
				pop[i]->components[j] = bigrand();
			}
		}
		//initialize offspring
		for(unsigned i=0;i<offspringSize; i++){
			offspr[i] = new specBasCandidate();
		}
		//initially evaluate population
		eva.PerformEvaluation(true);
		return true;
	}

	bool NextGeneration(){
		rep.PerformReproduction();
		mut.PerformMutation();
		eva.PerformEvaluation();
		mer.PerformMerge();
	}
};




/* --------------- OLD code ----------------------------
template<int dim, typename vectorType, int evalDim, typename evalType>
class abstractPopulation{
protected:
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate; 

	unsigned const populationSize, mateSize, offspringSize, bestArcSize;
	specCandidate **pop, **mate, **offspr, **best;
	double gridStep;	//?

	vectorType upperLimit[dim],lowerLimit[dim];		//grid mash

	template<int,typename,int,typename> friend class selectAllMethod;
	template<int,typename,int,typename> friend class inverseHilbMtrxNorm;
	template<int,typename,int,typename> friend class randomNbDisplace;
	template<int,typename,int,typename> friend class fightParentAnnealing;
	template<int,typename,int,typename> friend class plainCopyReproduction;

	template<int,typename,int,typename> friend class evaluationMethod;

public:
	abstractPopulation<dim,vectorType,evalDim,evalType>(unsigned pSize, unsigned mSize, unsigned oSize, unsigned bestA):
	  populationSize(pSize),mateSize(mSize),offspringSize(oSize),bestArcSize(bestA){
		pop = new specCandidate*[populationSize+offspringSize];		//for possible sorting in selection
		offspr = &(pop[populationSize]);								//to allow parent-child comparison
		best = new specCandidate*[bestArcSize];
	}

	virtual bool Create() = 0;								//(random) creation and initial evaluation
	virtual bool NextGeneration() = 0;
	virtual bool UpdateBest() = 0;
	//virtual bool EvaluateAndFitness() = 0;					//single-objective optimization does not necessarily need both steps 
	//virtual bool SelectAndProduceOffspringAndMerge() = 0;	//some algorithms do not employ selection
	const specCandidate *GetBestCandidate(const int idx) const {return (best[idx]);}
};



template<int dim, typename vectorType, int evalDim, typename evalType>
class abstractAnnealingPopulation : public abstractPopulation<dim,vectorType,evalDim,evalType>{
protected:
	double temperature;

	template<int,typename,int,typename> friend class fightParentAnnealing;

public:
	abstractAnnealingPopulation<dim,vectorType,evalDim,evalType>(unsigned pSize, unsigned mSize, unsigned oSize, unsigned bestA, double t) :
		abstractPopulation<dim,vectorType,evalDim,evalType>(pSize,mSize,oSize,bestA), temperature(t){}
};



template<int dim, typename vectorType, int evalDim, typename evalType, template<int,typename,int,typename> class _selectionMethod, 
	template<int,typename,int,typename> class _evaluationMethod, template<int,typename,int,typename> class _mutationMethod,
	template<int,typename,int,typename> class _mergingMethod, template<int,typename,int,typename> class _reproductionMethod>
class basicAnnealingPopulation : public abstractAnnealingPopulation<dim,vectorType,evalDim,evalType>{
public:
	typedef _selectionMethod<dim,vectorType,evalDim,evalType> specSelMethod;
	typedef _mutationMethod<dim,vectorType,evalDim,evalType> specMutMethod;
	typedef _reproductionMethod<dim,vectorType,evalDim,evalType> specRepMethod;
	typedef _evaluationMethod<dim,vectorType,evalDim,evalType> specEvaMethod;
	typedef _mergingMethod<dim,vectorType,evalDim,evalType> specMerMethod;
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

private:
	specSelMethod sel;
	specMutMethod mut;
	specRepMethod rep;
	specEvaMethod eva;
	specMerMethod mer;

protected:
	using specAbstPopulation::populationSize;
	using specAbstPopulation::offspringSize;
	using specAbstPopulation::pop;
	using specAbstPopulation::upperLimit;
	using specAbstPopulation::lowerLimit;
	//using specAbstPopulation::specCandidate;
public:
	basicAnnealingPopulation<dim,vectorType,evalDim,evalType,
		_selectionMethod,_evaluationMethod,
		_mutationMethod,_mergingMethod,
		_reproductionMethod>(unsigned pSize, unsigned mSize, unsigned oSize, unsigned bestA, double t) :
		abstractAnnealingPopulation<dim,vectorType,evalDim,evalType>(pSize,mSize,oSize,bestA,t){}

	bool Create(){
		for(unsigned i=0; i<populationSize+offspringSize; i++){
			pop[i] = new specCandidate;
		}
		srand((unsigned)time(NULL));
		for(unsigned i=0; i<populationSize; i++){
			for(unsigned j=0; j<dim; j++) 
				pop[i]->components[j] = (int)((upperLimit[j]-lowerLimit[j])*(float)(rand()/RAND_MAX) + lowerLimit[j]);
		}
		sel.Init(this);
		mut.Init(this);
		rep.Init(this);
		eva.Init(this);
		mer.Init(this);

		eva.PerformEvaluation(true);
		return true;
	}
	bool NextGeneration(){return true;}
	bool UpdateBest(){return true;}
};
*/
#endif