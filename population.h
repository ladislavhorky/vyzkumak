#ifndef __VYZKUM_POPULATION__
#define __VYZKUM_POPULATION__

#include<vyzkumak\prototypes.h>

template<int dim, typename vectorType, int evalDim, typename evalType>
class abstractPopulation{

	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate; 

	unsigned const populationSize, mateSize, offspringSize, bestArcSize;
	specCandidate **pop, **mate, **offspr, **best;
	double gridStep,vectorLimit;	//?

	template<int,typename,int,typename> friend class selectionMethod;
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


#endif