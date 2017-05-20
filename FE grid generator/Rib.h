#include "Tetrahedron.h"

struct Rib {
	vector<int> nodesNumbers;
	vector<Tetrahedron*> neighbour;
	bool isEmptyArea;
};