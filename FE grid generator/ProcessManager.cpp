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
	parallelepipedGenerator.loadEllipsoidProperties(PARALLELEPIPED_PROPERTIES);
	parallelepipedGenerator.correctAllSteps();
	parallelepipedGenerator.createNodes(ELLIPSOID_PROPERTIES);
}

void ProcessManager::run() {
	runEllipsoidGenerator();
	runParallelepipedGenerator();
}