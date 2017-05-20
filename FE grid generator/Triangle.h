#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

using namespace std;

struct Triangle {
	vector<int> nodesNumbers;
	vector<int> neighbours;
	vector<int> ribs;
};

#endif