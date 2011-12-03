#ifndef __VYZKUM_POPULATION__
#define __VYZKUM_POPULATION__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>

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

#endif