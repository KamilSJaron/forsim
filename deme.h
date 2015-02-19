#include "individual.h"
#include "g2_gd.h"

static int DEMEsize = 512;
// comment demesize in manual, it should be divideable by 2*number_of_edges
// comment demesize in manual, it should be power of 3 for easy square plotting (64, 216, 512, 1000, 1728, 4096)

using namespace std;

// fctions
double selectionRand(){
		return (double)rand() / RAND_MAX;
}

int pickAnIndividual(){
		return rand() % DEMEsize;
}

class Deme  
{
	public:
// constructor / destructors
		Deme(){};
// 		Deme(int ind){index = ind;};
// 		Deme(vector<int> neigb){neigbours = neigb;};
		Deme(int ind, char init);
		Deme(int ind, vector<int> neigb){index = ind;neigbours = neigb;};
		Deme(int ind, vector<int> neigb, char init);
		~Deme(){delete[] deme;}; // ????
		
// communication functions
		vector<int> getNeigbours(){return neigbours;};
		Individual getIndividual(int index){return deme[index];}
		int getDemeIndex(){return index;};
		Deme(const vector<int>&);
		static void setDEMEsize(int demesize){DEMEsize = demesize;};
		static int getDEMEsize(){return DEMEsize;};
		
// computing functions
		void quickBreed();
		void Breed();
		void getFitnessVector(vector<double>& fitnessVector);
		void permutation(); // permutation function
		void integrateMigrantVector(vector<Individual>& migBuffer);
		vector<double> getBproportion() const; 
		
// plotting functions
		void showDeme();
		void viewDeme();
		void plotHeadOfDeme();
		void plotDeme();
		void plotDeme(const char* filename);
	
	private:
		void swap(int ind1, int ind2);
		
		Individual *deme = new Individual[DEMEsize];
		int index;
		vector<int> neigbours;
};

  // // // // // // // // // // // // // //
 // constructor/destructors functions / //
// // // // // // // // // // // // // //


Deme::Deme(int ind, vector<int> neigb, char init){
	index = ind;
	neigbours = neigb;
	if(init == 'A' or init == 'B'){
		Individual temp(init);
		for(int i=0;i<DEMEsize;i++){
			deme[i] = temp;
		}
	} else {
		Individual tempA('A');
		Individual tempB('B');
		int i = 0;
		while(i< (DEMEsize / 2)){
			deme[i] = tempA;
			i++;
		}
		while(i< DEMEsize){
			deme[i] = tempB;
			i++;
		}
	}
	
}

Deme::Deme(int ind, char init){
	index = ind;
	if(init == 'A' or init == 'B'){
		Individual temp(init);
		for(int i=0;i<DEMEsize;i++){
			deme[i] = temp;
		}
	} else {
		Individual tempA('A');
		Individual tempB('B');
		int i = 0;
		while(i< (DEMEsize / 2)){
			deme[i] = tempA;
			i++;
		}
		while(i< DEMEsize){
			deme[i] = tempB;
			i++;
		}
	}
}

  // // // // // // // // // // // // // //
 // // // // computing functions / // // /
// // // // // // // // // // // // // //

void Deme::quickBreed(){
	Individual *metademe = new Individual[DEMEsize];
	vector<Chromosome> gamete1, gamete2;
	for(int i=0;i<DEMEsize;i++){
		deme[pickAnIndividual()].makeGamete(gamete1);
		deme[pickAnIndividual()].makeGamete(gamete2);
		metademe[i] = Individual(gamete1,gamete2);
	}
	for(int i=0;i<DEMEsize;i++){
		deme[i] = metademe[i];
	}
	delete[] metademe;
}

void Deme::Breed(){
	vector<double> fitnessVector;
	vector<Chromosome> gamete1, gamete2;
	getFitnessVector(fitnessVector);
	double RandMax = fitnessVector[DEMEsize-1];
// 	if(RandMax > DEMEsize){
// 		cout << "WARNING: RandMax is " << RandMax << endl;
// 	}
	
	double roll;
	map<double, int> parentPick;
	map<double, int>::iterator it;
	vector<int> couples[2];
	
	for(int i=0;i < DEMEsize*2;i++){
		roll = (selectionRand() * RandMax);
// 		if(roll >= RandMax or roll <= 0){
// 			cout << "WARNING: " << roll << " !!! - bad roll\n" << endl;
// 		}
		it = parentPick.find(roll);
		while(it != parentPick.end()){
			roll -= 0.001;
			it = parentPick.find(roll);
		}
		parentPick[roll] = i;
	}
	
	int i = 0;
	int par_index = 0, parent = 0;
	auto pos=parentPick.begin();
	
	while(pos!=parentPick.end()){
		
		if(fitnessVector[i] > pos->first){
			if(pos->second >= DEMEsize){
				par_index = pos->second - DEMEsize;
				parent = 1;
			} else {
				par_index = pos->second;
				parent = 0;
			}
			couples[parent].push_back(par_index);
			++pos;
		} else {
			i++;
			if(i >= DEMEsize){
				cout << "WARNING: individual " << i << " with fitness limit " << fitnessVector[i] << endl;
			}
		}
	}
	
	if(couples[0].size() != couples[1].size()){
		cout << "WARNING: Mothers: " << couples[0].size() << endl;
		cout << "WARNING: Fathers: " << couples[1].size() << endl;
	}
	
	Individual *metademe = new Individual[DEMEsize];
	for(int i=0;i<DEMEsize;i++){
		if(couples[0][i] >= DEMEsize or couples[1][i] >= DEMEsize){
			cout << "WARNING: HAPPY " << couples[0][i] << " and HAPPY " << couples[1][i] << " got " << i << endl;
		}
		deme[couples[0][i]].makeGamete(gamete1);
		deme[couples[1][i]].makeGamete(gamete2);
		metademe[i] = Individual(gamete1,gamete2);
	}
	
	for(int i=0;i<DEMEsize;i++){
		deme[i] = metademe[i];
	}
	delete[] metademe;
}

void Deme::permutation(){
	int j = 0;
  for(int i = 0;i<DEMEsize;i++){
		j = rand() % (DEMEsize - i);
		j += i;
		swap(i, j);
	}
}

void Deme::integrateMigrantVector(vector<Individual>& migBuffer){
	unsigned int i = 0;
	while(i < migBuffer.size()){
// 		cout << i << ' ';
		deme[i] = migBuffer[i];
		i++;
// 		migBuffer[i].plotGenotype();
	}
// 	cout << endl;
	return;
}

vector<double> Deme::getBproportion() const{
	vector<double> props;
	for(int i = 0;i < DEMEsize;i++){
		props.push_back(deme[i].getBprop());
	}
	return props;
}

void Deme::getFitnessVector(vector<double> &fitnessVector){
	double sum = 0;
	for(int i = 0;i < DEMEsize;i++){
		sum += deme[i].getFitness();
		fitnessVector.push_back(sum);
	}
}
		

		
  // // // // // // // // // // // // // //
 // // // // plotting functions / // // //
// // // // // // // // // // // // // //

void Deme::showDeme(){
	int neigbsize = neigbours.size();
	cout << setw(5) << right << index << ":  ";
	for(int i = 0; i < neigbsize; i++){
		cout << setw(5) << left << neigbours[i] << " ";
	}
	cout << endl;
}

void Deme::viewDeme(){
	for(int i=0;i<DEMEsize;i++){
		cout << "Individual: " << i+1 << endl;
		deme[i].viewGenotype();
	}
}

void Deme::plotHeadOfDeme(){
	cout << "Individual: 0" << endl;
	deme[0].viewGenotype();
	cout << "Individual: 1" << endl;
	deme[1].viewGenotype();
}

void Deme::plotDeme(){
	int line = 0, dev = 0;
	int height = 1;
	int width = 50;
	dev = g2_open_X11(width*NUMBERofCHROMOSOMES,height*DEMEsize);
	g2_set_auto_flush(dev, 0);
	for(int i=0;i<DEMEsize;i++){
		line = i + 1;
// 		cout << "Individual: " << i+1 << endl;
		deme[i].plotGenotype(dev, line, height, width, DEMEsize);
	}
	g2_flush(dev);
}

void Deme::plotDeme(const char* filename){
// 	cout << filename << endl;
	int line = 0, dev = 0;
	int height = 1;
	int width = 50;
	dev = g2_open_gd(filename,width*NUMBERofCHROMOSOMES,height*DEMEsize, g2_gd_png);
	for(int i=0;i<DEMEsize;i++){
		line = i + 1;
// 		cout << "Individual: " << i+1 << endl;
		deme[i].plotGenotype(dev, line, height, width, DEMEsize);
	}
	g2_close(dev);
}
   // // // // //
  //  PRIVATE //
 // // // // //
 
void Deme::swap(int ind1, int ind2){
	Individual tempInd;
	tempInd = deme[ind2];
	deme[ind2] = deme[ind1];
	deme[ind1] = tempInd;
}