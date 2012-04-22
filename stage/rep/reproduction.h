#ifndef __VYZKUM_REPRODUCTION__
#define __VYZKUM_REPRODUCTION__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>

//template <int,typename,int,typename> class initializablePart;


template<int dim, typename vectorType, typename evalType, int mateSize>
class reproductionMethod{
	public:
	typedef basicCandidate<dim,vectorType> specBasCandidate; 
	typedef abstractBasicPopulation<dim,vectorType> specAbstBasPopulation;

	protected:
	specBasCandidate **pop,**mate,**offspr;
	int popSize, offsprSize;

	public:
	virtual int PerformReproduction() = 0;
	virtual int Init(specAbstBasPopulation *p){
		pop = p->GetPopulation();
		mate = new specBasCandidate*[mateSize];
		popSize = p->GetPopSize();
		offspr = p->GetOffspring();
		offsprSize = p->GetOffsprSize();
		return 1;
	}
};



template<int dim, typename vectorType, typename evalType, int mateSize>
class plainCopyReproduction : public reproductionMethod<dim,vectorType,evalType,mateSize>{
	//public typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//public typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	//public typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

	public:
	int PerformReproduction(){
		if(mateSize != offsprSize) return PARAMETER_MISMATCH;

		for(unsigned i=0; i<mateSize; i++){
			*(offspr[i]) = *(mate[i]);
		}
		return 1;
	}
};

//-----------------------BIA repro-----------------------------

//classic GO reproduction with 2-tournalent selection and n-point crossover
template<int dim, typename vectorType, typename evalType, int mateSize, template<int,typename,typename,int> class _selectionMethod>
class biaReproduction : public reproductionMethod<dim,vectorType,evalType,mateSize>{
	public:
		typedef _selectionMethod<dim,vectorType,evalType,mateSize> specSelMethod;
		typedef basicSingleObjPopulation<dim,vectorType,evalType> specSingleObjPopulation;
		typedef singleObjectiveCandidate<dim,vectorType,evalType> specSingleObjCandidate; 

	private: 
		specSelMethod sel;

	public: 
	int Init(specAbstBasPopulation *p){
		reproductionMethod<dim,vectorType,evalType,mateSize>::Init(p);
		sel.Init(reinterpret_cast<specSingleObjPopulation*>(p),reinterpret_cast<specSingleObjCandidate**>(mate));
		srand(time(NULL));
		return 1;
	}

	int PerformReproduction(){
		sel.PerformSelection();
		//now we have mating pool full of candidates to crossover
		//1-point crossover
		int cross;
		int j;
		for(int i=0; i<offsprSize; i++){
			//pick crossover point (aligned to whole atom positions)
			cross = (rand() % (dim/2))*2;
			//do crossover
			for(j=0; j<cross; j++) offspr[i]->components[j] = mate[2*i]->components[j];
			for(;j<dim; j++)	   offspr[i]->components[j] = mate[2*i+1]->components[j];
		}
		//center to weight center?? or wait after mutation
		return 1;
	}
};

#endif