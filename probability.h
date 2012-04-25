#include<math.h>
#include<time.h>
#include<stdlib.h>


class rnd{
public:
	static int lrand(){return 0;}
public:
	static int Init(){srand((int)time(NULL)); return RAND_MAX;}
	static int U(int a, int b){return 0;}
};