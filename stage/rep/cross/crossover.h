#ifndef __VYZKUM_CROSSOVER__
#define __VYZKUM_CROSSOVER__

template<int dim, typename vectorType, int mateSize>
class crossoverMethod{
	public:
	typedef basicCandidate<dim,vectorType> specBasCandidate; 



	public:
	virtual int Init(){};

	virtual int PerformCrossover() = 0;
	
};

#endif