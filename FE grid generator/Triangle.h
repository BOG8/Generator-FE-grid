#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Tetrahedron.h"
#include <vector>

using namespace std;

struct Triangle {
	vector<int> nodesNumbers;
	vector<int> neighbours;
	Tetrahedron* tetrahedron;
};

#endif