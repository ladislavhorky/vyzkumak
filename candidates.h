#ifndef __VYZKUM_CANDIDATE__
#define __VYZKUM_CANDIDATE__

#include<vyzkumak\prototypes.h>

template<int dim, typename vectorType, int evalDim, typename evalType>
class candidate{
	evalType _fitness;
public:
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate; 

	vectorType components[dim];
	evalType evaluation[evalDim];
	inline evalType &fitness(){return _fitness;}

	specCandidate & operator=(const specCandidate &rval){
		//_fitness = rval._fitness;
#pragma unroll
		for(int i=0;i<dim;i++){
			components[i]=rval.components[i];
			//evaluation[i]=rval.evaluation[i];
		}
		return *this;
	}
};

template<int dim, typename vectorType, typename evalType>
class candidate<dim,vectorType,1,evalType>{
public:
	typedef candidate<dim,vectorType,1,evalType> specCandidate; 

	vectorType components[dim];
	evalType evaluation[1];
	inline evalType &fitness(){return evaluation[0];}	
	specCandidate & operator=(const specCandidate &rval){
		//evaluation[0]=rval.evaluation[0];
#pragma unroll
		for(int i=0;i<dim;i++){
			components[i]=rval.components[i];
		}
		return *this;
	}
};

#endif