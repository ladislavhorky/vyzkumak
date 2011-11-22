#ifndef __VYZKUM_MERGE__
#define __VYZKUM_MERGE__

#include<vyzkumak\prototypes.h>

template<int dim, typename vectorType, int evalDim, typename evalType>
class mergingMethod{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){};
	static int PerformMerge(const specAbstractPopulation *p){};
};


template<int dim, typename vectorType, int evalDim, typename evalType>
class fightParent : public mergingMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){}
	static int PerformMerge(const specAbstractPopulation *p){
		if(p->offspringSize != p->populationSize) return 0;
		specCandidate *tmp;

		for(int i=0; i<p->populationSize; i++){
			if(p->pop[i]->fitness() > p->offspr[i]->fitness()){		//minimizing!!
				tmp = p->pop[i];
				p->pop[i] = p->offspr[i];
				p->offspr[i] = tmp;
			}
		}
		return 1;
	}
};

template<int dim, typename vectorType, int evalDim, typename evalType>
class fightPopulation : public mergingMethod<dim,vectorType,evalDim,evalType>{

	int qsort(unsigned first, unsigned last, const specAbstractPopulation *p){
	/*
		modified qsort - sorts only first popSize values (in ascending order!)
		last-first is supposed to be > 1
	*/
		specCandidate *tmp;
#define SWAP(X,Y) {tmp = p->pop[(X)]; p->pop[(X)] = p->pop[(Y)]; p->pop[(Y)] = tmp;}
#define FIT(X) p->pop[(X)].fitness()

		//find median of 3
		if(FIT(first) > FIT(first+1)) SWAP(first,first+1);
		if(FIT(first) > FIT(last)) SWAP(first,last);
		if(FIT(first+1) > FIT(last)) SWAP(first+1,last);

		//run qsort
		unsigned pivot=first, rising=first+1, falling=last;

		//while(){
			//dodelat
		//}

	}
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){}
	static int PerformMerge(const specAbstractPopulation *p){	//modified qsort
		
		return 1;
	}
};

template<int dim, typename vectorType, int evalDim, typename evalType>
class fightParentAnnealing : public mergingMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){}
	static int PerformMerge(const specAbstractPopulation *p, float temperature){
		if(p->offspringSize != p->populationSize) return 0;
		specCandidate *tmp;

		for(int i=0; i<p->populationSize; i++){
			if(p->pop[i]->fitness() > p->offspr[i]->fitness()){		//minimizing!!
				tmp = p->pop[i];
				p->pop[i] = p->offspr[i];
				p->offspr[i] = tmp;
			}
		}
		return 1;
	}
};


#endif