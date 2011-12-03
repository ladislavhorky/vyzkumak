#ifndef __VYZKUM_SELECTION__
#define __VYZKUM_SELECTION__

#include<vyzkumak/topLevelHeader.h>
#include<time.h>
#include<stdlib.h>

template<int dim, typename vectorType, int evalDim, typename evalType>
class selectionMethod : public initializablePart<dim,vectorType,evalDim,evalType>{
	virtual int PerformSelection() = 0;
};



template<int dim, typename vectorType, int evalDim, typename evalType>
class selectAllMethod : public selectionMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

private:
	using specInitializable::p;

public:

	int Init(specAbstPopulation *pop){
		if(pop->populationSize != pop->mateSize) return PARAMETER_MISMATCH;
		pop->mate = pop->pop;				//so all population will be "selected" to the mating pool
		srand((unsigned)time(NULL));
		return 1;
	}
	virtual int PerformSelection(){return 1;}	// do nothing
};



template<int dim, typename vectorType, int evalDim, typename evalType>
class tournamentSelectionMethod : public selectionMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

private:
	using specInitializable::p;

public:

	int Init(specAbstPopulation *pop){
		p = pop;
		p->mate = new specCandidate*[p->mateSize];
		return 1;
	}
	int PerformSelection(){			//basic 2-tournament, pop must be smaller than randmax!!
		specCandidate *one, *two;
		for(int i=0;i<p->mateSize;i++){
			one = p->pop[rand() % p->popSize];
			two = p->pop[rand() % p->popSize];
			p->mate[i] = (one->fitness() > two->fitness())?one:two;
		}
		return 1;
	}
};

#endif