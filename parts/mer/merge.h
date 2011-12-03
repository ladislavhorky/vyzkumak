#ifndef __VYZKUM_MERGE__
#define __VYZKUM_MERGE__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

template<int dim, typename vectorType, int evalDim, typename evalType>
class mergingMethod : public initializablePart<dim,vectorType,evalDim,evalType>{
	virtual int PerformMerge() = 0;
};

#define SWAPO_P(X) tmp = p->pop[(X)]; p->pop[(X)] = p->offspr[(X)]; p->offspr[(X)] = tmp;

template<int dim, typename vectorType, int evalDim, typename evalType>
class fightParent : public mergingMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

private:
	using specInitializable::p;

public:

	int PerformMerge(){
		if(p->offspringSize != p->populationSize) return 0;
		specCandidate *tmp;

		for(int i=0; i<p->populationSize; i++){
			if(p->pop[i]->fitness() > p->offspr[i]->fitness()){		//minimizing!!
				SWAPO_P(i);
			}
		}
		return 1;
	}
};

#define SWAP(X,Y) {tmp = p->pop[(X)]; p->pop[(X)] = p->pop[(Y)]; p->pop[(Y)] = tmp;}
#define FIT(X) p->pop[(X)].fitness()

template<int dim, typename vectorType, int evalDim, typename evalType>
class fightPopulation : public mergingMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

private:
	using specInitializable::p;

	int qsort(unsigned first, unsigned last){
	/*
		modified qsort - partially sorts first popSize values (in ascending order!)
		last-first is supposed to be > 1
	*/
		specCandidate *tmp;

		//find median of 3
		if(FIT(first) > FIT(first+1)) SWAP(first,first+1);
		if(FIT(first) > FIT(last)) SWAP(first,last);
		if(FIT(first+1) > FIT(last)) SWAP(first+1,last);

		//run qsort
		unsigned pivot, rising, falling;
		while(1){
			pivot=first;
			rising=first+1;
			falling=last;
			while(1){
				do falling--; while(FIT(falling) < FIT(pivot));		//thanks to med. no need to check underrun
				//rise until first element >= pivot is found
				do rising++; while(FIT(rising) > FIT(pivot));		//also, no overrun check
				//done and continue or swap and go again
				if(rising > falling){	
					SWAP(pivot,falling);
					break;
				}else{					
					SWAP(rising,falling);
				}
			}
			//check and end/run again
			if((pivot == p->popSize)||(pivot == p->popSize-1)) return 1;
			else if(pivot > p->popSize) last = pivot-1;
			else first = pivot+1;
		}
	}
public:
	int PerformMerge(){	//modified qsort
		qsort(0, p->popSize + p->offSize - 1);
		return 1;
	}
};


template<int dim, typename vectorType, int evalDim, typename evalType>
class fightParentAnnealing : public mergingMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractAnnealingPopulation<dim,vectorType,evalDim,evalType> specAbstAnnePopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

private:
	using specInitializable::p;

public:

	int Init(specAbstAnnePopulation *pop){ //we need temperature
		p=pop;
		srand((unsigned)time(NULL));
		return 1;
	}	
	int PerformMerge(){
		if(p->offspringSize != p->populationSize) return 0;
		specCandidate *tmp;
		evalType delta;

		for(unsigned i=0; i<p->populationSize; i++){
			delta = p->pop[i]->fitness() - p->offspr[i]->fitness();
			if((delta > 0)||((double)rand()/RAND_MAX < exp(delta/static_cast<specAbstAnnePopulation *>(p)->temperature))){		//minimizing!!
				SWAPO_P(i);
			}
		}
		return 1;
	}
};


#endif