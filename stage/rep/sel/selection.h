#ifndef __VYZKUM_SELECTION__
#define __VYZKUM_SELECTION__

#include<vyzkumak/topLevelHeader.h>
#include<time.h>
#include<stdlib.h>

template<int dim, typename vectorType, typename evalType, int mateSize>
class selectionMethod{
	public:
	typedef singleObjectiveCandidate<dim,vectorType,evalType> specSingleObjCandidate; 
	typedef basicSingleObjPopulation<dim,vectorType,evalType> specSingleObjPopulation;

	protected:
	specSingleObjCandidate **pop,**mate;
	int popSize;

	public:
	virtual int PerformSelection() = 0;
	virtual int Init(specSingleObjPopulation *p, specSingleObjCandidate **_mate){
		pop = p->GetPopulation();
		mate = _mate;
		popSize = p->GetPopSize();
		return 1;
	}
};


template<int dim, typename vectorType, typename evalType, int mateSize>
class selectAllMethod : public selectionMethod<dim,vectorType,evalType,mateSize>{
	public:
	//typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	//typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;
	//typedef reproductionMethod<dim,vectorType,evalDim,evalType> specReproductionMethod;

	int Init(specSingleObjPopulation *p, specSingleObjCandidate **_mate){
		selectionMethod<dim,vectorType,evalType>::Init(p,_mate);
		//pop = p->GetPopulation();
		//popSize = p->GetPopSize();

		if(popSize != mateSize) return PARAMETER_MISMATCH;
		mate = pop;				//so all population will be "selected" to the mating pool
		srand((int)time(NULL));
		return 1;
	}
	virtual int PerformSelection(){return 1;}	// do nothing
};



template<int dim, typename vectorType, typename evalType, int mateSize>
class tournamentSelection : public selectionMethod<dim,vectorType,evalType,mateSize>{
	public:
	//typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	//typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

	//private using specInitializable::p;

	public:
	/*int Init(specAbstPopulation *p, specSingleObjCandidate **_mate){
		selectionMethod<dim,vectorType,evalType>::Init(p,_mate);
		
		p->mate = new specCandidate*[p->mateSize];
		return 1;
	}*/

	int PerformSelection(){			//basic 2-tournament, pop must be smaller than randmax!!
		specSingleObjCandidate *one, *two;
		for(int i=0;i<mateSize;i++){
			one = pop[rand() % popSize];
			two = pop[rand() % popSize];
			mate[i] = (one->fitness > two->fitness)?one:two;
		}
		return 1;
	}
};

#endif