#ifndef __VYZKUM_TOPLEVEL__
#define __VYZKUM_TOPLEVEL__

/*We MINIMIZE in whole program!*/

//template<int,typename,int, typename>

//whole program definitions
#define GRID_STEP 0.0001

//error definitions
#include <vyzkumak\errors.h>

//pop include
#include <vyzkumak\parts\candidates.h>
#include <vyzkumak\population.h>

#include<vyzkumak\parts\initializable.h>

//parts
#include <vyzkumak\parts\eva\evaluation.h>
#include <vyzkumak\parts\mer\merge.h>
#include <vyzkumak\parts\mut\mutation.h>
#include <vyzkumak\parts\sel\selection.h>
#include <vyzkumak\parts\rep\reproduction.h>

//additionals
#include <vyzkumak\probability.h>


#endif