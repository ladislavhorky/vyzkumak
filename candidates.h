#ifndef __VYZKUM_CANDIDATE__
#define __VYZKUM_CANDIDATE__

#include<vyzkumak\prototypes.h>

template<int dim, typename vectorType, int evalDim, typename evalType>
class candidate{
	evalType _fitness;
public:
	vectorType components[dim];
	evalType evaluation[evalDim];
	inline evalType &fitness(){return _fitness;}
};

template<int dim, typename vectorType, typename evalType>
class candidate<dim,vectorType,1,evalType>{
public:
	vectorType components[dim];
	evalType evaluation[1];
	inline evalType &fitness(){return evaluation[0];}	
};

#endif