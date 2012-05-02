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
		srand((int)time(NULL));
		return 1;
	};
	int PerformMutation(){
		//component to change -- direction actually
		int com;
		int move;
		for(int i=0; i<offsprSize; i++){
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

//bia gaussian mutation (displacement) of n points
template<int dim, typename vectorType, int sigma, int mutRate, int maxMutPoints>
class gaussianDisplace : public mutationMethod<dim,vectorType>{

	public:
	int Init(specAbstBasPopulation *p){
		mutationMethod<dim,vectorType>::Init(p);
		srand((int)time(NULL));
		return 1;
	};

	bool BadCand(){
		for(int i=0;i<offsprSize;i++){
			for(int j=0;j<dim;j++){
				if(offspr[i]->components[j] == -2147483648) return true;
			}
		}
		return false;
	}

	//mutates one point
	int PerformMutation(){
		#define PI 3.1415926535897932
		#define MAXR 10000
		//mutate one point

		char b;

		double R,theta;
		int pos,mutPoints;
		for(int i=0; i<offsprSize; i++){
			if((rand() % 100) > mutRate) continue; // no mutation
			// +1 to eliminate NaN, +-INF
			R = sqrt(-2*log((double)(rand()+1)/(RAND_MAX+1)))*sigma;
			theta = 2*PI*(double)rand()/RAND_MAX;

			mutPoints = (rand() % maxMutPoints)+1;
			for(int j=0; j<mutPoints; j++){
				pos = rand() % (dim/2);
				//mutate all points with the same value
				offspr[i]->components[2*pos] += R*cos(theta);
				offspr[i]->components[2*pos+1] += R*sin(theta);
			}
		}

		/*if(BadCand()){
			cout << "BAD after primal mutation";
			cin >> b;
		}*/

		//normalize ... pertubation done automaticaly by overflow
		long wx,wy;
		for(int i=0; i<offsprSize; i++){
			wx=wy=0;
			for(int j=0; j<dim; j+=2) wx += offspr[i]->components[j];
			for(int j=1; j<dim; j+=2) wy += offspr[i]->components[j];
			wx /= dim/2;
			wy /= dim/2;
			for(int j=0; j<dim; j+=2) offspr[i]->components[j] -= wx;
			for(int j=1; j<dim; j+=2) offspr[i]->components[j] -= wy;
		}

		/*if(BadCand()){
			cout << "BAD after normalisation";
			cin >> b;
		}*/

		//and randomly rotate with mutation rate -- just try
		//double rotM[4], phi;
		//vectorType oldx, oldy;
		//for(int i=0; i<offsprSize; i++){
		//	if((rand() % 100) > mutRate) continue; // no mutation
		//	//init rotation matrix
		//	// +1 to eliminate NaN, +-INF
		//	R = sqrt(-2*log((double)(rand()+1)/(RAND_MAX+1)))*0.3;
		//	theta = 2*PI*(double)rand()/RAND_MAX;
		//	//phi with normal distribution.. 0.1 ~ 6 deg.
		//	phi = R*cos(theta);
		//	rotM[0]=rotM[3] = cos(phi);
		//	rotM[1] = -sin(phi);
		//	rotM[2] = -rotM[1];

		//	//rotate every point
		//	for(int j=0;j<dim/2;j++){
		//		oldx = offspr[i]->components[2*j];
		//		oldy = offspr[i]->components[2*j+1];
		//		offspr[i]->components[2*j] =  rotM[0]*oldx + rotM[1]*oldy;
		//		offspr[i]->components[2*j+1] =rotM[2]*oldx + rotM[3]*oldy;
		//	}
		//}
		/*if(BadCand()){
			cout << "BAD after rotation";
			cin >> b;
		}*/

		return 1;
	}
};

#endif