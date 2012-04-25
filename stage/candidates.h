#ifndef __VYZKUM_CANDIDATE__
#define __VYZKUM_CANDIDATE__

#include<vyzkumak/topLevelHeader.h>

/*
	Philosophy:
	to limit population assembly only to combinations
	that make sense through different number of template
	parameters for some parts

	Candidate hierarchy: 
	(GPU memory pattern and kernel structure 
	strongly depends on type of candidate)

____________________________________
basicCandidate:						|
only data, no evaluation, to reduce	|
number of template perameters		|
				^
________________|___________________	 ____________________________________
singleObjectiveCandidate:			|	|largeSingleObjectiveCandidate:		 |
small dimension, needs all data for |<--|large dimension, only few changes by|
reevaluation, single objective		|	|mutation/reproduction, need to track|
				^						|these minor changes for reevaluation|
				|										^
________________|___________________	 _______________|_______________
multiObjectiveCandidate:			|	|largeMultiObjectiveCandidate:	|
multiple evaluation, separated		|<--|...							|
fitness value						|	|								|
											
*/

/*
	Basic candidate, to reduce number of template parameters for
	parts that do not need to know much about actual candidate. 
	For better handling virtual operator= is provided.
*/
template<int dim, typename vectorType>
class basicCandidate{
	public:
	typedef basicCandidate<dim,vectorType> specBasCandidate; 

	vectorType components[dim];

	virtual specBasCandidate & operator=(const specBasCandidate &rval){
		for(int i=0;i<dim;i++){
			components[i]=rval.components[i];
		}
		return *this;
	}
};

template<int dim, typename vectorType, typename evalType>
class singleObjectiveCandidate : public basicCandidate<dim,vectorType>{

	public: 
		evalType fitness;
};

template<int dim, typename vectorType, typename evalType, int evalDim>
class multiObjectiveCandidate : public singleObjectiveCandidate<dim,vectorType,evalType>{

	public: 
		evalType evaluation[evalDim];
};


template<int dim, typename vectorType, typename evalType, int numChanges>
class largeSingleObjectiveCandidate : public singleObjectiveCandidate<dim,vectorType,evalType>{
	public:
	typedef largeSingleObjectiveCandidate<dim,vectorType,evalType,numChanges> specLarSinCandidate; 

	int changes[numChanges];
	vectorType deltas[numChanges];

	specLarSinCandidate & operator=(const specLarSinCandidate &rval){
		for(int i=0;i<dim;i++){
			components[i]=rval.components[i];
		}
		fitness = rval.fitness;
		return *this;
	}
};

template<int dim, typename vectorType, typename evalType, int evalDim, int numChanges>
class largeMultiObjectiveCandidate :
	public multiObjectiveCandidate<dim,vectorType,evalType,evalDim>,
	public largeSingleObjectiveCandidate<dim,vectorType,evalType,numChanges>{
	
	public: 
		typedef largeMultiObjectiveCandidate<dim,vectorType,evalType,evalDim,numChanges> specLarMulCandidate; 

		specLarMulCandidate & operator=(const specLarMulCandidate &rval){
			for(int i=0;i<dim;i++){
				components[i]=rval.components[i];
			}
			fitness = rval.fitness;
			for(int i=0;i<evalDim;i++){
				evaluation[i]=rval.evaluation[i];
			}
			return *this;
		}
};

/* OLD code-------------------------------------

/*
	numChanges != 0 means, that candidate is very large and it would be
	ineffective to reevaluate whole candidate (TSP + one transposition = 
	change in two points = change in at most four distances instead of ALL
	distances).
	Therefore two new arrays are introduced - the first holds indices of elemets
	that have been changed and the second holds deltas from original values (which
	stay unchanged for purposes of evaluation -- substracting old distances, adding 
	new).
	Evaluation method is then responsible for adjusting old values after evaluation.
*/
/*
template<int dim, typename vectorType, int evalDim, typename evalType, int numChanges=0>
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

template<int dim, typename vectorType, int evalDim, typename evalType>
class candidate<dim,vectorType,evalDim,evalType,0>{
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
	we think as evaluation and fitness are the same 
*/
/*
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
};*/

#endif