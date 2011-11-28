#ifndef __VYZKUM_REPRODUCTION__
#define __VYZKUM_REPRODUCTION__

#include<vyzkumak\topLevelHeader.h>
#include<stdlib.h>
#include<time.h>

//template <int,typename,int,typename> class initializablePart;


template<int dim, typename vectorType, int evalDim, typename evalType>
class reproductionMethod : public initializablePart<dim,vectorType,evalDim,evalType>{
public:
	virtual int PerformReproduction() = 0;
};



template<int dim, typename vectorType, int evalDim, typename evalType>
class plainCopyReproduction : public reproductionMethod<dim,vectorType,evalDim,evalType>{
public:
	int PerformReproduction(){
		if(p->mateSize != p->offspringSize) return PARAMETER_MISMATCH;

		for(unsigned i=0; i<p->mateSize; i++){
			*(p->offspr[i]) = *(p->mate[i]);
		}
		return 1;
	}
};


#endif