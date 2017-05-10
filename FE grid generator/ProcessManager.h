#include "EllipsoidGenerator.h"

const string ELLIPSOID_PROPERTIES = "EllipsoidProperties.txt";
const string RESULT = "Result.txt";

class ProcessManager {
	EllipsoidGenerator ellipsoidGenerator;

public:
	ProcessManager();
	~ProcessManager();

	void run();

private:
	void runEllipsoidGenerator();
};