#include "Tetrahedron.h"

struct Rib {
	vector<int> nodesNumbers;
	vector<Tetrahedron*> neighbours;
	bool isEmptyArea;
};