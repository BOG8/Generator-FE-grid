#include <vector>

using namespace std;

struct Tetrahedron {
	vector<int> nodesNumbers;
	vector<Tetrahedron*> neighbour;
};