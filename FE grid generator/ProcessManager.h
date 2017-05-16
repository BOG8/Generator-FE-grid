#include "EllipsoidGenerator.h"
#include "ParallelepipedGenerator.h"
#include "GridConnector.h"

const string ELLIPSOID_PROPERTIES = "EllipsoidProperties.txt";
const string PARALLELEPIPED_PROPERTIES = "ParallelepipedProperties.txt";
const string RESULT = "Result.txt";

class ProcessManager {
	EllipsoidGenerator ellipsoidGenerator;
	ParallelepipedGenerator parallelepipedGenerator;
	GridConnector gridConnector;

public:
	ProcessManager();
	~ProcessManager();

	void run();

private:
	void runEllipsoidGenerator();
	void runParallelepipedGenerator();
	void runGridConnector();
};