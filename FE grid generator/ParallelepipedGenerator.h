#include <string>

using namespace std;

class ParallelepipedGenerator {
	double x, y, z;
	double xStep, yStep, zStep;
	int xStepsNumber, yStepsNumber, zStepsNumber;
	int minNumber;

public:
	ParallelepipedGenerator();
	~ParallelepipedGenerator();

	void setMinNumber(int number);
	void loadEllipsoidProperties(string fileName);
	void correctAllSteps();

private:
	void correctStep(double &x, double &xStep, int &xStepsNumber);
};