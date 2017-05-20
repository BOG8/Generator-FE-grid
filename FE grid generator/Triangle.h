#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

using namespace std;

struct Triangle {
	vector<int> nodesNumbers;
	vector<int> neighbour;
	vector<int> ribs;
};

#endif