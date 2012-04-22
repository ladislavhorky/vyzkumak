#ifndef __VYZKUM_MUTATION__
#define __VYZKUM_MUTATION__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

//using namespace std;

template<int dim, typename vectorType>
class mutationMethod{
	public:
	typedef basicCandidate<dim,vectorType> specBasCandidate; 
	typedef abstractBasicPopulation<dim,vectorType> specAbstBasPopulation;
	
	protected:
	specBasCandidate **offspr;
	int offsprSize;
	vectorType *upperLimit, *lowerLimit;

	public:
	virtual int PerformMutation() = 0;
	virtual int Init(specAbstBasPopulation *p){
		offspr = p->GetOffspring();
		offsprSize = p->GetOffsprSize();
		p->GetLimits(&upperLimit,&lowerLimit);
		return 1;
	}
};

//moves only by 1 (Hamming distance), it mutates offspring only, vectorType should be integer
template<int dim, typename vectorType>
class randomNbDisplace : public mutationMethod<dim,vectorType>{
	//public:
	//typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	//typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

	public:
	int Init(specAbstBasPopulation *p){
		mutationMethod<dim,vectorType>::Init(p);
		srand((unsigned)time(NULL));
		return 1;
	};
	int PerformMutation(){
		//component to change -- direction actually
		int com;
		int move;
		for(unsigned i=0; i<offsprSize; i++){
			com = rand() % dim;
			move = (rand() & 1)*2 - 1;	//= +-1
			offspr[i]->components[com] += move;
			//pertubation - back to border (bad but this is just example)
			if(offspr[i]->components[com] > upperLimit[com]) offspr[i]->components[com] -= 2;
			if(offspr[i]->components[com] < lowerLimit[com]) offspr[i]->components[com] += 2;
		}
		return 1;
	}
};

//-----------------------BIA----------------------------

//bia gaussian mutation (displacement) of one point
template<int dim, typename vectorType, int sigma, int mutRate>
class gaussianDisplace : public mutationMethod<dim,vectorType>{

	public:
	int Init(specAbstBasPopulation *p){
		mutationMethod<dim,vectorType>::Init(p);
		srand((unsigned)time(NULL));
		return 1;
	};

	//mutates one point
	int PerformMutation(){
		#define PI 3.1415926535897932
		//mutate one point
		double R,theta;
		int pos;
		for(int i=0; i<offsprSize; i++){
			if((rand() % 100) > mutRate) continue; // no mutation
			R = sqrt(-2*log((double)rand()/RAND_MAX))*sigma;
			theta = 2*PI*(double)rand()/RAND_MAX;
			pos = rand() % (dim/2);
			//mutate
			offspr[i]->components[2*pos] += R*cos(theta);
			offspr[i]->components[2*pos+1] += R*sin(theta);
		}

		//normalize ... pertubation done automaticaly by overflow
		long wx,wy;
		for(int i=0; i<offsprSize; i++){
			wx=wy=0;
			for(int j=0; j<dim; j+=2) wx += offspr[i]->components[j];
			for(int j=1; j<dim; j+=2) wy += offspr[i]->components[j];
			wx /= dim;
			wy /= dim;
			for(int j=0; j<dim; j+=2) offspr[i]->components[j] -= wx;
			for(int j=1; j<dim; j+=2) offspr[i]->components[j] -= wy;
		}
		return 1;
	}
};

#endif