#ifndef __VYZKUM_MUTATION__
#define __VYZKUM_MUTATION__

#include<vyzkumak\prototypes.h>
#include<stdlib.h>
#include<time.h>

using namespace std;

template<int dim, typename vectorType, int evalDim, typename evalType>
class mutationMethod{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){};
	static int PerformMutation(const specAbstractPopulation *p){};
};

//moves only by 1 (Hamming distance), it mutates offspring only, vectorType should be integer
template<int dim, typename vectorType, int evalDim, typename evalType>
class randomNbDisplace : public mutationMethod<dim,vectorType,evalDim,evalType>{
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	static int Init(const specAbstractPopulation *p){
		srand(time(NULL));
	};
	static int PerformMutation(const specAbstractPopulation *p){
		int com;
		int move;
		for(int i=0; i<p->offspringSize; i++){
			com = rand() % dim;
			move = (rand() & 1)*2 - 1;	//= +-1
			p->offspr[i]->components[com] += move;
			if(p->offspr[i]->components[com] > p->upperLimit[com]) p->offspr[i]->components[com] -= 2;
			if(p->offspr[i]->components[com] < p->lowerLimit[com]) p->offspr[i]->components[com] += 2;
		}
	};
};



#endif