// M. Cihan Ozer - August 2016, Montreal
//				^-_-^

// Decleration of MeshReader class to controll reading operations depending on
// Assimp library. The process starts by reading meshName-meshPath pairs in options.txt file

#ifndef MESHREADER_H
#define MESHREADER_H

class MeshInfo;
struct aiScene;

#include <vector>

#include <string>
using std::string;

class MeshReader
{
public:
	// CONSTRUCTOR
	MeshReader()	{}

	// METHODS
	// Reads mesh name-mesh path pairs in options.txt
	// TRUE: Opening and reading options.txt process happened
	// FALSE: options.txt is not opened
	bool start();

private:

	void generateMeshFiles(const std::vector<MeshInfo*>& meshList);	// Generate bone structure + positions, bone weights, face, vertex, and normal files for given meshes
	bool readMesh(const MeshInfo* meshInfo);	// Read mesh file using Assimp and generates related files
	bool getMeshData(const aiScene* scene, const string& meshName, const string& meshPath);	// Get mesh info from the scene and write files
};

#endif