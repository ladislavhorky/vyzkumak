#ifndef __VYZKUM_CANDIDATE__
#define __VYZKUM_CANDIDATE__

#include<vyzkumak\topLevelHeader.h>

template<int dim, typename vectorType, int evalDim, typename evalType>
class candidate{
	evalType _fitness;
public:
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate; 

	vectorType components[dim];
	evalType evaluation[evalDim];
	inline evalType &fitness(){return _fitness;}

	specCandidate & operator=(const specCandidate &rval){
		for(int i=0;i<dim;i++){
			components[i]=rval.components[i];
		}
		return *this;
	}
};

/*
	for now (?) in single-objective optimization 
	we think evaluation and fitness are the same 
*/

template<int dim, typename vectorType, typename evalType>
class candidate<dim,vectorType,1,evalType>{
public:
	typedef candidate<dim,vectorType,1,evalType> specCandidate; 

	vectorType components[dim];
	evalType evaluation[1];
	inline evalType &fitness(){return evaluation[0];}	
	specCandidate & operator=(const specCandidate &rval){
		for(int i=0;i<dim;i++){
			components[i]=rval.components[i];
		}
		return *this;
	}
};

#endif