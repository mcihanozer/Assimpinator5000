// M. Cihan Ozer - August 2016, Montreal
//				^-_-^

// Decleration of SkeletonMesh class for serialization
// of mesh including skeletal information which is read by Assimp library


#ifndef SKELETONMESH_H
#define SKELETONMESH_h

#include "Mesh.h"

#include <vector>

#include <map>
using std::map;

#include <utility>
using std::pair;

class SkeletonMesh : Mesh
{
public:
	// CONSTRUCTOR
	SkeletonMesh(const aiMesh* baseMesh, const aiNode* rootNode, const string& meshName, const string& filePath, bool isLibigl = false);

	// METHODS
	bool save();

private:

	// METHODS
	bool writeBoneInfo();
	void getBoneInfo(	map<int, bool>& normalBoneIndices, std::vector<pair<int, int>>& boneStructure,
						float** bonePosition, std::vector<std::vector <float>>& vertexWeights);

	void getStructureInfo(map<int, bool>& normalBoneIndices, std::vector<pair<int, int>>& boneStructure);
	void getSkinningInfo(map<int, bool>& normalBoneIndices, float** bonePosition, std::vector<std::vector <float>>& vertexWeights);

	int mBoneNumber;
	aiBone** mBoneInfo;
	const aiNode* mRootNode;

};

#endif