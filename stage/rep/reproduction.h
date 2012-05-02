#ifndef __VYZKUM_REPRODUCTION__
#define __VYZKUM_REPRODUCTION__

#include<vyzkumak/topLevelHeader.h>
#include<stdlib.h>
#include<time.h>
#include<algorithm>

//template <int,typename,int,typename> class initializablePart;


template<int dim, typename vectorType, typename evalType, int mateSize>
class reproductionMethod{
	public:
	typedef basicCandidate<dim,vectorType> specBasCandidate; 
	typedef abstractBasicPopulation<dim,vectorType> specAbstBasPopulation;

	protected:
	specBasCandidate **pop,**mate,**offspr;
	int popSize, offsprSize;

	public:
	virtual int PerformReproduction() = 0;
	virtual int Init(specAbstBasPopulation *p){
		pop = p->GetPopulation();
		mate = new specBasCandidate*[mateSize];
		popSize = p->GetPopSize();
		offspr = p->GetOffspring();
		offsprSize = p->GetOffsprSize();
		return 1;
	}
};


//fill offspring with multiple population copies
template<int dim, typename vectorType, typename evalType, int mateSize>
class plainCopyReproduction : public reproductionMethod<dim,vectorType,evalType,mateSize>{
	//public typedef abstractPopulation<dim,vectorType,evalDim,evalType> specAbstPopulation;
	//public typedef candidate<dim,vectorType,evalDim,evalType> specCandidate;
	//public typedef initializablePart<dim,vectorType,evalDim,evalType> specInitializable;

	public:
	int PerformReproduction(){
		//if(mateSize != offsprSize) return PARAMETER_MISMATCH;

		for(int i=0; i<offsprSize;){
			*(offspr[i]) = *(pop[i % popSize]);
		}
		return 1;
	}
};

//-----------------------BIA repro-----------------------------

//classic GO reproduction with 2-tournalent selection and n-point crossover
template<int dim, typename vectorType, typename evalType, int mateSize, 
	template<int,typename,typename,int> class _selectionMethod,int parasitismRate, int maxCrossPoints>
class biaReproduction : public reproductionMethod<dim,vectorType,evalType,mateSize>{
	public:
		typedef _selectionMethod<dim,vectorType,evalType,mateSize> specSelMethod;
		typedef basicSingleObjPopulation<dim,vectorType,evalType> specSingleObjPopulation;
		typedef singleObjectiveCandidate<dim,vectorType,evalType> specSingleObjCandidate; 

	private: 
		specSelMethod sel;
		

	public: 
	int Init(specAbstBasPopulation *p){
		reproductionMethod<dim,vectorType,evalType,mateSize>::Init(p);
		sel.Init(reinterpret_cast<specSingleObjPopulation*>(p),reinterpret_cast<specSingleObjCandidate**>(mate));
		srand(time(NULL));
		return 1;
	}

	int PerformReproduction(){
		sel.PerformSelection();
		//now we have mating pool full of candidates to crossover

		//n-point crossover n from 1 to maxCrossPoints
		int cross[maxCrossPoints+1], crossPoints;
		int i,j,k;
		bool parasitism;
		vectorType distMax;

		for(i=0; i<offsprSize; i++){
			//randomize number of cross points and initialize them
			crossPoints = (rand() % maxCrossPoints)+1;
			for(j=0;j<crossPoints;j++){
				cross[j] = (rand() % (dim/2))*2;	//aligned to whole atom coordinates
			}
			sort(cross,cross+crossPoints);
			//just as a stopper
			cross[crossPoints] = dim;
			
			//find out if crossover is with parasite or not
			parasitism = false;
			if(rand()%100 <= parasitismRate){
				parasitism=true;
				//init distMax
				distMax = abs(mate[2*i]->components[0]);
				for(j=1;j<dim; j++){
					if(distMax < abs(mate[2*i]->components[j])) distMax = abs(mate[2*i]->components[j]);
				}
			}
			//crossover
			k=0;
			for(j=0;j<crossPoints+1;j++){
				if(!(j % 2)){	//copy 1st parent part
					for(;k<cross[j];k++) offspr[i]->components[k] = mate[2*i]->components[k];
				}else{			//copy 2nd parent or parasite
					if(!parasitism){	
						for(;k<cross[j];k++) offspr[i]->components[k] = mate[2*i+1]->components[k];
					}else{
						//parasite
						for(;k<cross[j];k++) offspr[i]->components[k] = 
							(rand()%((vectorType)(2*distMax*1.1))) - (vectorType)(distMax*1.1);
					}
				}
			}

			////pick crossover point (aligned to whole atom positions)
			//cross[0] = (rand() % (dim/2))*2;
			////do crossover
			//for(j=0; j<cross[0]; j++) offspr[i]->components[j] = mate[2*i]->components[j];
			////parasitism
			//if(rand()%100 > parasitismRate){

			//	//ok, normal crossover
			//	for(;j<dim; j++) offspr[i]->components[j] = mate[2*i+1]->components[j];

			//}else{ //crosover with appropriate parasite
			//	//find candidate extreme values
			//	distMax = abs(mate[2*i]->components[0]);
			//	for(k=1;k<dim; k++){
			//		if(distMax < abs(mate[2*i]->components[k])) distMax = abs(mate[2*i]->components[k]);
			//	}
			//	distMax++; //in rare zero case 
			//	for(;j<dim; j++) offspr[i]->components[j] = (rand()%((vectorType)(2*distMax*1.1))) - (vectorType)(distMax*1.1);
			//}
		}
		return 1;
	}
};

#endif