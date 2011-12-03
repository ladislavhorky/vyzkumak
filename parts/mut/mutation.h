#ifndef __VYZKUM_MUTATION__
#define __VYZKUM_MUTATION__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>

using namespace std;

template<int dim, typename vectorType, int evalDim, typename evalType>
class mutationMethod : public initializablePart<dim,vectorType,evalDim,evalType>{
	virtual int PerformMutation() = 0;
};

//moves only by 1 (Hamming distance), it mutates offspring only, vectorType should be integer
template<int dim, typename vectorType, int evalDim, typename evalType>
class randomNbDisplace : public mutationMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

private:
	using specInitializable::p;

public:

	int Init(specAbstPopulation *pop){
		p = pop;
		srand((unsigned)time(NULL));
		return 1;
	};
	int PerformMutation(){
		int com;
		int move;
		for(unsigned i=0; i<p->offspringSize; i++){
			com = rand() % dim;
			move = (rand() & 1)*2 - 1;	//= +-1
			p->offspr[i]->components[com] += move;
			if(p->offspr[i]->components[com] > p->upperLimit[com]) p->offspr[i]->components[com] -= 2;
			if(p->offspr[i]->components[com] < p->lowerLimit[com]) p->offspr[i]->components[com] += 2;
		}
		return 1;
	}
};



#endif