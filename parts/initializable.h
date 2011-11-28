#ifndef __VYZKUM_INITIALIZABLE__
#define __VYZKUM_INITIALIZABLE__

#include<vyzkumak\population.h>

/*
	Contains parent of all parts containing all 
	necessarry typedefs and basic initialization.

	It should be inherited through public inheritance.
*/

template<int,typename,int,typename> class reproductionMethod;
template<int,typename,int,typename> class evaluationMethod;
template<int,typename,int,typename> class mutationMethod;
template<int,typename,int,typename> class selectionMethod;
template<int,typename,int,typename> class mergingMethod;

template<int dim, typename vectorType, int evalDim, typename evalType>
class initializablePart{
public:
	//spec for fully specialized
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	typedef abstractAnnealingPopulation<dim,vectorType,evalDim,evalType> specAbstAnnePopulation;
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;

	typedef selectionMethod<dim,vectorType,evalDim,evalType> specSelMethod;
	typedef mutationMethod<dim,vectorType,evalDim,evalType> specMutMethod;
	typedef reproductionMethod<dim,vectorType,evalDim,evalType> specRepMethod;
	typedef evaluationMethod<dim,vectorType,evalDim,evalType> specEvaMethod;
	typedef mergingMethod<dim,vectorType,evalDim,evalType> specMerMethod;
protected:
	specAbstPopulation *p;
public:
	virtual int Init(specAbstPopulation *pop){p=pop; return 1;}
};

#endif