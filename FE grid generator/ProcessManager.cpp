#include "ProcessManager.h"

ProcessManager::ProcessManager() {

}

ProcessManager::~ProcessManager() {

}

void ProcessManager::runEllipsoidGenerator() {
	ellipsoidGenerator.loadEllipsoidProperties(ELLIPSOID_PROPERTIES);
	ellipsoidGenerator.createAllNodes();
	ellipsoidGenerator.createExternalGrid();
	ellipsoidGenerator.createTriangulation();
	ellipsoidGenerator.writeNodesInFile(RESULT);
	ellipsoidGenerator.writeTetrahedronsInFile(RESULT);
	ellipsoidGenerator.writeGidFile();
}

void ProcessManager::runParallelepipedGenerator() {
	parallelepipedGenerator.setMinNumber(ellipsoidGenerator.getMaxNumber());
}

void ProcessManager::run() {
	runEllipsoidGenerator();
	runParallelepipedGenerator();
}