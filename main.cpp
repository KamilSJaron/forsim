#include <iostream> // preprocessor directive
#include <cmath>
#include <string>  // strings
#include <map>  // associative arrays
#include <algorithm> // sort
#include <fstream>
#include <new> // alocation of memory in heap
#include <iomanip>
#include <time.h>

#include "universe.h" // Class of individuals

#define SEEDtoRAND 605786180 //seed only for rand() fction, values of the poisson distribution are always generated with same initial seed
#define NUM_OF_IMIGRANTS 100 /* number of complete heterozygotious imigrants per generation (assuming, that Pois(1)*500 would be very close to 500, cause computationally it is not worth the computational time)*/

using namespace std;

template <class T> class auto_ptr{
	T* ptr;
	public:
		explicit auto_ptr(T* p = 0) : ptr(p) {}
		~auto_ptr(){delete ptr;}
		T& operator*(){return *ptr;}
		T* operator->(){return ptr;}

};

// v0.141020

int main()
{
	using namespace std; // std as default
	srand (SEEDtoRAND); // setting a seed
// 	Universe KamilWorld;
// 	clock_t t1,t2;
// 	
// 	KamilWorld.setLREdgesType("reflexive");
// 	KamilWorld.basicUnitCreator('b', 'A');
// 	KamilWorld.basicUnitCreator('r', 'B');
// 	KamilWorld.basicUnitCreator('l', 'A');
// 	KamilWorld.basicUnitCreator('r', 'B');
// 	KamilWorld.listOfDemes();
// 
// 	KamilWorld.migration();
// 	KamilWorld.plotOneDeme(0);
// 	for(int i=0; i < 10;i++){
// 		t1=clock();
// 		cout << "Generation: " << i << " in ";
// 		KamilWorld.migration();
// 		KamilWorld.globalNaiveBreeding();
// 		t2=clock();
// 		cout << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
// 	}
	
// 	KamilWorld.listOfDemes();	
// 	KamilWorld.plotOneDeme(0);
	Deme smallDeme(42,'C');
	smallDeme.plotDeme();
	smallDeme.Breed();
	smallDeme.plotDeme();
	smallDeme.Breed();
	smallDeme.plotDeme();
// 	smallDeme.Breed();
// 	smallDeme.plotDeme();
	return 0;
}











