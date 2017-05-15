#include <string>

using namespace std;

class ParallelepipedGenerator {
	double x, y, z;
	double xStep, yStep, zStep;
	int minNumber;

public:
	ParallelepipedGenerator();
	~ParallelepipedGenerator();

	void setMinNumber(int number);
	void loadEllipsoidProperties(string fileName);
};