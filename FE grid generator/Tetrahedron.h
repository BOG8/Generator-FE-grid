#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <vector>

using namespace std;

struct Tetrahedron {
	vector<int> nodesNumbers;
	vector<Tetrahedron*> neighbour;
};

#endif