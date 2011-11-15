#include<math.h>
#include<time.h>
#include<stdlib.h>


class rnd{
public:
	static unsigned int lrand(){return 0;}
public:
	static unsigned int Init(){srand(time(NULL)); return RAND_MAX;}
	static unsigned int U(unsigned int a, unsigned int b){return 0;}
};