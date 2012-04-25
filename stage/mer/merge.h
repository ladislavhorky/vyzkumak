#ifndef __VYZKUM_MERGE__
#define __VYZKUM_MERGE__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define INSERTION_SORT_TRESHOLD 5

template<int dim, typename vectorType, typename evalType>
class mergingMethod{
	public: 
		typedef singleObjectiveCandidate<dim,vectorType,evalType> specSingleObjCandidate; 
		typedef basicSingleObjPopulation<dim,vectorType,evalType> specSingleObjPopulation;

	protected:
	specSingleObjCandidate **pop,**offspr;
	int popSize, offsprSize;

	public:
	virtual int PerformMerge() = 0;
	virtual int Init(specSingleObjPopulation *p){
		pop = p->GetPopulation();
		popSize = p->GetPopSize();
		offspr = p->GetOffspring();
		offsprSize = p->GetOffsprSize();
		return 1;
	}
};

#define SWAPO_P(X) tmp = pop[(X)]; pop[(X)] = offspr[(X)]; offspr[(X)] = tmp;

template<int dim, typename vectorType, typename evalType>
class fightParent : public mergingMethod<dim,vectorType,evalType>{
	public:
	//typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	//typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

	//private using specInitializable::p;

	public:
	int PerformMerge(){
		if(offsprSize != popSize) return 0;
		specSingleObjCandidate *tmp;

		for(int i=0; i<popSize; i++){
			if(pop[i]->fitness > offspr[i]->fitness){		//minimizing!!
				SWAPO_P(i);
			}
		}
		return 1;
	}
};

#define SWAP(X,Y) {tmp = pop[(X)]; pop[(X)] = pop[(Y)]; pop[(Y)] = tmp;}
#define FIT(X) (pop[(X)]->fitness)

template<int dim, typename vectorType, typename evalType>
class fightPopulation : public mergingMethod<dim,vectorType,evalType>{
	public:
	//typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	//typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

	protected:
	int qsortPartial(int first, int last, int split){
	/*
		Sorts in ascending order!!
		Modified qsort - partially sorts array from indice 'first' to indice 'last'
		splitting it into two partially ordered subarrays where first subarray 
		is 'split' long. Last-first is supposed to be > 1
	*/
		specSingleObjCandidate *tmp;

		//run qsort
		int pivot, rising, falling;
		int targetPos = first + split;
		evalType pivotFit;
		while(1){
			//find median of 3 (to be on first+1 pos)
			if(FIT(first) > FIT(last)) SWAP(first,last);
			if(FIT(first) > FIT(first+1)) SWAP(first,first+1);
			if(FIT(first+1) > FIT(last)) SWAP(first+1,last);
		
			pivotFit = FIT(first+1);//pivot is at 'first+1' pos (first is already smaller)
			rising = first+1;	//will be immediatelly incremented
			falling= last;	//will be immediatelly decremented but last is greated than pivot so OK
			while(1){
				do falling--; while(FIT(falling) > pivotFit);	//thanks to med. no need to check underrun
				//rise until first element >= pivot is found
				do rising++; while(FIT(rising) < pivotFit);		//also, no overrun check
				//done and continue or swap and go again
				if(rising > falling){	
					SWAP(first+1,falling);	//first+1=pivot index
					break;
				}else{					
					SWAP(rising,falling);
				}
			}
			//check and end/run again
			if((falling == targetPos)||(falling == targetPos-1)){return 1;}
			else if(falling > targetPos) last = falling-1; //pivot-1;
			else first = falling+1; //pivot+1;
		}
	}

	//sorts first cnt candidates in ascending order by insertion sort
	int insertionSortPartial(int first, int last, int cnt){
		evalType minFit;
		int minIdx, i,j;
		specSingleObjCandidate *tmp;

		for(i=0;i<cnt;i++){
			minIdx = i+first;
			minFit = FIT(minIdx);
			for(j=first+i+1; j<=last; j++){
				if(FIT(j) < minFit){
					minIdx = j; minFit = FIT(minIdx);
				}
			}
			SWAP(minIdx,i+first);
		}
		return 1;
	}

	public:
	int PerformMerge(){	//modified qsort
		qsortPartial(0, popSize + offsprSize - 1,popSize);
		return 1;
	}
};

//------------------BIA merge -----------------------------

/*
Merges pop and offspr so that elite from pop is preserved and the rest of pop is replaced
with best offspring. It inherits fightPopulation so that it can use quicksort

eliteCount determines elite volume
*/
template<int dim, typename vectorType, typename evalType, int eliteCount>
class elitismMerge : public fightPopulation<dim,vectorType,evalType>{
	
	public: 
	int PerformMerge(){
		//sort out elites in pop and popSize-eliteCount in offstr
		if(eliteCount < INSERTION_SORT_TRESHOLD) insertionSortPartial(0,popSize-1,eliteCount);
		else qsortPartial(0,popSize-1,eliteCount);
		//also include treshold ??
		qsortPartial(popSize,popSize+offsprSize-1,popSize-eliteCount);

		specSingleObjCandidate *tmp;
		/*
		preserve elite and SWAP the rest -- otherwise some pointers would 
		be duplicit and pop would be corrupted
		*/
		for(int i=eliteCount, j=0; i<popSize; i++,j++){
			tmp = pop[i];
			pop[i] = offspr[j];
			offspr[j] = tmp;
		}
		return 1;
	}
};




// TO RECONSIDER
//template<int dim, typename vectorType, int evalDim, typename evalType>
//class fightParentAnnealing : public mergingMethod<dim,vectorType,evalDim,evalType>{
//	public:
//	typedef abstractAnnealingPopulation<dim,vectorType,evalDim,evalType> specAbstAnnePopulation;
//	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
//	typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;
//
//	private:
//	using specInitializable::p;
//
//	public:
//	int Init(specAbstAnnePopulation *pop){ //we need temperature
//		p=pop;
//		srand((int)time(NULL));
//		return 1;
//	}	
//	int PerformMerge(){
//		if(p->offspringSize != p->populationSize) return 0;
//		specCandidate *tmp;
//		evalType delta;
//
//		for(int i=0; i<p->populationSize; i++){
//			delta = p->pop[i]->fitness() - p->offspr[i]->fitness();
//			if((delta > 0)||((double)rand()/RAND_MAX < exp(delta/static_cast<specAbstAnnePopulation *>(p)->temperature))){		//minimizing!!
//				SWAPO_P(i);
//			}
//		}
//		return 1;
//	}
//};


#endif