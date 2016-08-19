// M. Cihan Ozer - August 2016, Montreal
//				^-_-^

// Definiton of SkeletonMesh class for serialization
// of mesh including skeletal information which is read by Assimp library

#include "SkeletonMesh.h"

#include <iostream>
using std::cout;


// CONSTRUCTOR

SkeletonMesh::SkeletonMesh(const aiMesh* mesh, const aiNode* rootNode, const string& meshName, const string& filePath, bool libigl)
	:Mesh(mesh, meshName, filePath, libigl)
{
	// Mesh name, face, vertex, normal etc. are initialized by super class (Mesh).
	
	// Set bone information
	mBoneNumber = mesh->mNumBones;
	mBoneInfo = mesh->mBones;
	mRootNode = rootNode;
}

// PUBLIC METHODS
bool SkeletonMesh::save()
{
	writeFaces();
	writeVertices();
	writePerVerNormals();
	writeBoneInfo();

	return true;
}

// PRIVATE METHODS

void SkeletonMesh::getStructureInfo(map<int, bool>& normalBoneIndices, std::vector<pair<int, int>>& boneStructure)
{
	// Loop over each bones and their children to find out skeleton structure and abnormal bones:
	// Experiments with abnormal meshes show that, abnormal bones cannot find a place for themselves in any pairs.
	// Thus, later "boneStructure" pairs are used to detect and eliminate these kinds of bones.
	for (int bi = 0; bi < mBoneNumber; bi++)
	{
		// Get next bone and its node
		const aiBone* nextBone = mBoneInfo[bi];
		const aiNode* nextNode = mRootNode->FindNode(nextBone->mName.C_Str());

		// Loop over the children:
		// This loop acts as breadth first search, and right know do not consider the hierarchy that much
		// (Since I do not consider skeletal animation right now).
		// A recursive traversal such as deep first seach may be used but, currently I do not know how Assimp
		// treats skeleton information. Is it keeping skeleton information from root to children or random?
		// That's why using a recursive might be problematic.
		// Plus, Assimp also suggests similar approach: http://www.assimp.org/lib_html/data.html (Bones section)
		for (int ci = 0; ci < nextNode->mNumChildren; ci++)
		{
			string childName = nextNode->mChildren[ci]->mName.C_Str();

			for (int si = 0; si < mBoneNumber; si++)
			{
				if (childName.compare(mBoneInfo[si]->mName.C_Str()) == 0)	// Check whether the child is in the bones
				{
					boneStructure.push_back(std::pair<int, int>(bi, si));	// If child is in the structure, make a pair.

					// Mark pairs as safe to use
					normalBoneIndices.insert(std::map<int, bool>::value_type(bi, true));
					normalBoneIndices.insert(std::map<int, bool>::value_type(si, true));

					break;
				}

			}	// End of for si = 0

		}	// End of for ci = 0

	}	// End of for each bones
}

void SkeletonMesh::getSkinningInfo(map<int, bool>& normalBoneIndices, float** bonePosition, std::vector<std::vector <float>>& vertexWeights)
{
	// Get bone position, and weight information
	for (map<int, bool>::iterator it = normalBoneIndices.begin(); it != normalBoneIndices.end(); it++)
	{
		int boneId = it->first;

		// Get bone position
		aiVector3D scale, pos;
		aiQuaternion rot;
		aiMatrix4x4 bomeMatrix = mBoneInfo[boneId]->mOffsetMatrix.Inverse();
		bomeMatrix.Decompose(scale, rot, pos);

		bonePosition[boneId][0] = pos.x;
		bonePosition[boneId][1] = pos.y;
		bonePosition[boneId][2] = pos.z;

		// Get weights
		for (int wi = 0; wi < mBoneInfo[boneId]->mNumWeights; wi++)
		{
			vertexWeights[mBoneInfo[boneId]->mWeights[wi].mVertexId].at(boneId) = mBoneInfo[boneId]->mWeights[wi].mWeight;
		}
	}

}	// End of getSkinningInfo()

void SkeletonMesh::getBoneInfo(map<int, bool>& normalBoneIndices, std::vector<pair<int, int>>& boneStructure, float** bonePosition, std::vector<std::vector <float>>& vertexWeights)
{
	getStructureInfo(normalBoneIndices, boneStructure);
	getSkinningInfo(normalBoneIndices, bonePosition, vertexWeights);
}

bool SkeletonMesh::writeBoneInfo()
{
	// Get structure information

	// Decide which bones are included to the skeleton and get skeleton structure:
	// When you use Assimp, some poorly made meshes generates some "allien" bones
	// whose parent is not the root bone but, a different rig material.
	// This step eliminates these kinds of bones.
	// TODO: IS THERE A BETTER METHOD TO ELIMINATE THIS KIND OF ABNORMATILTIES?
	map<int, bool> normalBoneIndices;	// Seems safer and faster then std::unique
	std::vector<pair<int, int>> boneStructure;	// Keeps bone structure pairs

	getStructureInfo(normalBoneIndices, boneStructure);

	// Get bone position and weight information

	float** bonePosition = new float*[normalBoneIndices.size()];
	for (int row = 0; row < normalBoneIndices.size(); row++)
	{
		bonePosition[row] = new float[3];
	}
	std::vector<std::vector <float>> vertexWeights(mVertexNumber, std::vector<float>(normalBoneIndices.size()));

	getSkinningInfo(normalBoneIndices, bonePosition, vertexWeights);

	// Write into files

	// Write weight info
	string weightPath = mFilePath + "\\" + mMeshName + ".bw";
	FILE* weightFile = fopen(weightPath.c_str(), "wb");

	if (weightFile)
	{
		// Write file size
		int boneSize = normalBoneIndices.size();

		fwrite(&mVertexNumber, sizeof(int), 1, weightFile);
		fwrite(&boneSize, sizeof(int), 1, weightFile);

		for (int vi = 0; vi < vertexWeights.size(); vi++)
		{
			for (int bi = 0; bi < vertexWeights[vi].size(); bi++)
			{
				fwrite(&vertexWeights[vi].at(bi), sizeof(float), 1, weightFile);
			}
		}

		fclose(weightFile);
	}
	else
	{
		cout << "\n\nERROR!!! CANNOT OPEN BONE STRUCTURE FILE: " << weightPath << " FOR MESH " << mMeshName << std::endl << std::endl;
		return false;
	}

	// Write bone structure info
	string structurePath = mFilePath + "\\" + mMeshName + ".bs";
	FILE* structureFile = fopen(structurePath.c_str(), "wb");

	if (structureFile)
	{
		int rows = boneStructure.size();
		int cols = 2;

		fwrite(&rows, sizeof(int), 1, structureFile);
		fwrite(&cols, sizeof(int), 1, structureFile);

		for (int bs = 0; bs < rows; bs++)
		{
			int first = boneStructure[bs].first;
			int sec = boneStructure[bs].second;

			fwrite(&first, sizeof(int), 1, structureFile);
			fwrite(&sec, sizeof(int), 1, structureFile);
		}

		fclose(structureFile);
	}
	else
	{
		cout << "\n\nERROR!!! CANNOT OPEN BONE STRUCTURE FILE: " << structurePath << " FOR MESH " << mMeshName << std::endl << std::endl;
		return false;
	}

	// Write bone positions
	string bonePos = mFilePath + "\\" + mMeshName + ".bp";
	FILE* bonePosFile = fopen(bonePos.c_str(), "wb");

	if (bonePosFile)
	{
		int boneSize = normalBoneIndices.size();
		fwrite(&boneSize, sizeof(int), 1, bonePosFile);

		for (int bpi = 0; bpi < normalBoneIndices.size(); bpi++)
		{
			fwrite(&bonePosition[bpi][0], sizeof(float), 1, bonePosFile);
			fwrite(&bonePosition[bpi][1], sizeof(float), 1, bonePosFile);
			fwrite(&bonePosition[bpi][2], sizeof(float), 1, bonePosFile);
		}

		fclose(bonePosFile);
	}
	else
	{
		cout << "\n\nERROR!!! CANNOT OPEN BONE POSITION FILE: " << bonePos << " FOR MESH " << mMeshName << std::endl << std::endl;
		return false;
	}

	// Memory management
	for (int row = 0; row < normalBoneIndices.size(); row++)
	{
		delete[] bonePosition[row];
	}
	delete[] bonePosition;

	return true;
}