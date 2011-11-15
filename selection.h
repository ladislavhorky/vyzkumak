#ifndef __VYZKUM_SELECTION__
#define __VYZKUM_SELECTION__

#include<vyzkumak\prototypes.h>
#include<time.h>
#include<stdlib.h>

template<int dim, typename vectorType, int evalDim, typename evalType>
class selectionMethod{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){};
	static int PerformSelection(const specAbstractPopulation *p){};
};

template<int dim, typename vectorType, int evalDim, typename evalType>
class selectAllMethod : public selectionMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){
		if(p->populationSize != p->mateSize) return PARAMETER_MISMATCH;
		p->mate = p->pop;				//so all population will be "selected" to the mating pool
		srand(time(NULL));
		return 1;
	}
	static int PerformSelection(const specAbstractPopulation *p){return 1};	// do nothing
};

template<int dim, typename vectorType, int evalDim, typename evalType>
class tournamentSelectionMethod : public selectionMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){
		p->mate = new specCandidate*[p->mateSize];
		return 1;
	}
	static int PerformSelection(const specAbstractPopulation *p){	//basic 2-tournament, pop must be smaller than randmax!!
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