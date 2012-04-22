#ifndef __VYZKUM_EVAL__
#define __VYZKUM_EVAL__

#include<vyzkumak/topLevelHeader.h>
#include<iostream>
#include<iomanip>
#include<math.h>

//using namespace std;


template<int dim, typename vectorType, typename evalType>
class evaluationMethod{
	public:
	typedef singleObjectiveCandidate<dim,vectorType,evalType> specSingleObjCandidate; 
	typedef basicSingleObjPopulation<dim,vectorType,evalType> specSingleObjPopulation;

	protected:
	specSingleObjCandidate **pop,**offspr;
	int popSize, offsprSize;

	public: 
	virtual int Init(specSingleObjPopulation *p){
		pop = p->GetPopulation();
		popSize = p->GetPopSize();
		offspr = p->GetOffspring();
		offsprSize = p->GetOffsprSize();
		return 1;
	}	

//public:
	//typedef abstractBasicPopulation<dim,vectorType> specAbstBasPopulation;
	//typedef basicCandidate<dim,vectorType> specBasCandidate;
	//typedef initializablePart<dim,vectorType> specInitializable;

	protected:
	//using specInitializable::p;
	virtual int EvaluateSet(specSingleObjCandidate **set, int size) = 0;

	public:
	int PerformEvaluation(bool initial=false){
		if(initial) EvaluateSet(pop,popSize);
		else EvaluateSet(offspr,offsprSize);
		return 1;
	}
};

/*----------------------------------------------------------------------------------------*/

template<int dim, typename vectorType, typename evalType>
class inverseHilbMtrxNorm: public evaluationMethod<dim,vectorType,evalType>{
	public:
	//typedef basicSingleObjPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	//typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

	private:
	//using specInitializable::p;

		//{H11,H12,...,H1n,H21,...,H2n, ... ,Hnn}
	static signed long *inverseHilbert;

	int EvaluateSet(specSingleObjCandidate **set, int size){
		double norm, sum;
		for(int i=0; i < size; i++){
			//compute norm od H^(-1)x
			norm = 0;
			for(int j=0; j<dim; j++){
				sum=0;
				//row*vector
				for(int k=0;k<dim;k++){
					sum += inverseHilbert[j*dim + k]*(set[i]->components[k]*GRID_STEP);
				}
				norm += sum*sum;
			}
			set[i]->evaluation[0] = sqrt(norm);
		}
		return 1;
	}

	long comb(long n, long k){
		long res=1;
		if(n<0 || k<0) return 1;
		if(k < n-k) k = n-k;	//symetry
		for(long i=k+1, div=1; i<=n; i++, div++){
		 res = (res*i)/div;
		}
		return res;
	}

	public:
	int Init(specSingleObjPopulation *p){
		if(evalDim != 1) return BAD_EVAL_DIM;
		evaluationMethod<dim,vectorType,evalDim,evalType>::Init(p);

		//create Hilbert inverse if not already created
		if(inverseHilbert != NULL) return 1;

		signed long tmp;
		inverseHilbert = new signed long[dim*dim];
		for(int i=1; i<dim+1; i++){
			for(int j=1; j<dim+1; j++){
				tmp = comb(i+j-2,i-1);
				inverseHilbert[dim*(i-1)+j-1] = (((i+j)%2)?-1:1)*(i+j-1)*comb(dim+i-1,dim-j)*comb(dim+j-1,dim-i)*tmp*tmp;
				//cout << setw(8);
				//cout << inverseHilbert[dim*(i-1)+j-1]<<" ";
			}
			//cout << '\n';
		}
		return 1;
	}
};
template<int dim, typename vectorType, typename evalType>
signed long *inverseHilbMtrxNorm<dim,vectorType,evalType>::inverseHilbert = NULL;

//------------------BIA evaluation -----------------------------


template<int dim, typename vectorType, typename evalType>
class moleculePotentialEnergy: public evaluationMethod<dim,vectorType,evalType>{
	int dist[5];

	public:
	moleculePotentialEnergy<dim,vectorType,evalType>(){};

	int EvaluateSet(specSingleObjCandidate **set, int size){
		return 1;
	}
};

#endif