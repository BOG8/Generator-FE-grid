#include "Tetrahedron.h"
#include "Node.h"
#include <list>

class DeloneRebuilder {
	Tetrahedron tetrahedrons;
	double planeA, planeB, planeC, planeD;


public:
	DeloneRebuilder();
	~DeloneRebuilder();

	void rebuild(list<Tetrahedron> &tetrahedrons);

private:
	double calculateDistance(Node N1, Node N2);
	bool isDeloneCondition(Node N1, Node N2, Node N3, Node N4, Node P);
	void definePlaneCoefficients(Node one, Node two, Node three);
	Node definePerpendicularPlaneNode(Node node);
	Node defineVector(Node one, Node two);
	Node multiplyNodeVector(Node nodeVector, double multiplier);
	Node sumNodes(Node one, Node two);
	double defineABSCrossProduct(Node one, Node two);
	bool isAccessory(Node planeNode, Node A, Node B, Node C);
	bool isCanBeRebuild(Node V1, Node V2, Node M1, Node M2, Node M3);
	bool isNeedToRebuild(Node V1, Node V2, Node M1, Node M2, Node M3);

	void defineNodesNumbersAndIndexes(int &V1, int &V2, vector<int> &mVect, vector<int> &mTVect, vector<int> &mBVect, Tetrahedron* tetr1, Tetrahedron* tetr2);
};