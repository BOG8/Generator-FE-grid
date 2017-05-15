#include "EllipsoidGenerator.h"
#include "ParallelepipedGenerator.h"

const string ELLIPSOID_PROPERTIES = "EllipsoidProperties.txt";
const string RESULT = "Result.txt";

class ProcessManager {
	EllipsoidGenerator ellipsoidGenerator;
	ParallelepipedGenerator parallelepipedGenerator;

public:
	ProcessManager();
	~ProcessManager();

	void run();

private:
	void runEllipsoidGenerator();
	void runParallelepipedGenerator();
};