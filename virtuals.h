#ifndef __VYZKUM_VIRTUALS__
#define __VYZKUM_VIRTUALS__

#include<vyzkumak\prototypes.h>

// test


template<typename candidateType, typename evalType>
class singleObjectivePopulation : public abstractPopulation{



template<int dim, typename T>
class fitness{

public:
	static virtual int Init(){};
	static virtual int GetFitness(myVector v) = 0;
};

template<int dim, typename T>
class model{



};

#endif