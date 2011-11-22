#ifndef __VYZKUM_EVAL__
#define __VYZKUM_EVAL__

#include<vyzkumak\prototypes.h>
#include<iostream>
#include<iomanip>
#include<math.h>

using namespace std;


template<int dim, typename vectorType, int evalDim, typename evalType>
class evaluationMethod{
public:
	typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
private:
	inline int EvaluateSet(specCandidate *set, int size){};
public:
	typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstractPopulation;

	static int Init(const specAbstractPopulation *p){};
	static int EvaluatePopulation(const specAbstractPopulation *p, bool initial=false){
		if(initial) EvaluateSet(p->pop,p->populationSize);
		else EvaluateSet(p->offspr,p->offspringSize);
	}
};

/*----------------------------------------------------------------------------------------*/

template<int dim, typename vectorType, int evalDim, typename evalType>
class inverseHilbMtrxNorm: public evaluationMethod<dim,vectorType,evalDim,evalType>{
		//{H11,H12,...,H1n,H21,...,H2n, ... ,Hnn}
	static signed long *inverseHilbert;

	inline int EvaluateSet(specCandidate *set, int size){
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
	}

	static long comb(long n, long k){
		long res=1;
		if(n<0 || k<0) return 1;
		if(k < n-k) k = n-k;	//symetry
		for(long i=k+1, div=1; i<=n; i++, div++){
		 res = (res*i)/div;
		}
		return res;
	}
public:
	static int Init(const specAbstractPopulation *p){
		if(evalDim != 1) return BAD_EVAL_DIM;
		//create Hilbert inverse
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

template<int dim, typename vectorType, int evalDim, typename evalType>
signed long *inverseHilbMtrxNorm<dim,vectorType,evalDim,evalType>::inverseHilbert = NULL;

#endif