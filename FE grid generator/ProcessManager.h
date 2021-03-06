#include "EllipsoidGenerator.h"
#include "ParallelepipedGenerator.h"
#include "GridConnector.h"
#include "DeloneRebuilder.h"

const string ELLIPSOID_PROPERTIES = "EllipsoidProperties.txt";
const string PARALLELEPIPED_PROPERTIES = "ParallelepipedProperties.txt";
const string RESULT = "Result.txt";

class ProcessManager {
	EllipsoidGenerator ellipsoidGenerator;
	ParallelepipedGenerator parallelepipedGenerator;
	GridConnector gridConnector;
	DeloneRebuilder deloneRebuilder;

public:
	ProcessManager();
	~ProcessManager();

	void run();

private:
	void runEllipsoidGenerator();
	void runParallelepipedGenerator();
	void runGridConnector();
	void runDeloneRebuilder();
};