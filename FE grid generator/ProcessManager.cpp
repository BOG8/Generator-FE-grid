#include "ProcessManager.h"

ProcessManager::ProcessManager() {

}

ProcessManager::~ProcessManager() {

}

void ProcessManager::runEllipsoidGenerator() {
	ellipsoidGenerator.loadEllipsoidProperties(ELLIPSOID_PROPERTIES);
	ellipsoidGenerator.createAllNodes();
	ellipsoidGenerator.createExternalGrid();
}

void ProcessManager::run() {
	runEllipsoidGenerator();
}