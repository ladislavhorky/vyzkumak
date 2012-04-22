#ifndef __VYZKUM_TOPLEVEL__
#define __VYZKUM_TOPLEVEL__

/*We MINIMIZE in whole program!*/

//template<int,typename,int, typename>

//whole program definitions
#define GRID_STEP 0.0001

//error definitions
#include <vyzkumak/errors.h>

//pop include
#include <vyzkumak/stage/candidates.h>
#include <vyzkumak/population.h>

#include<vyzkumak/stage/initializable.h>

//stages
#include <vyzkumak/stage/eva/evaluation.h>
#include <vyzkumak/stage/mer/merge.h>
#include <vyzkumak/stage/mut/mutation.h>
#include <vyzkumak/stage/rep/sel/selection.h>
#include <vyzkumak/stage/rep/reproduction.h>

//additionals
#include <vyzkumak/probability.h>


#endif