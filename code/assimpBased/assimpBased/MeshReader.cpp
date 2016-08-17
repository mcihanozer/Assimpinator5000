#include "MeshReader.h"

#include "RegularMesh.h"
#include "SkeletonMesh.h"

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

#include <iostream>
using std::cout;

#include <fstream>
using std::ifstream;

// Helper class for keeping mesh anme-mesh path pairs
// TODO can be a vector<pair<string, string>>?
class MeshInfo
{
public:
	MeshInfo(const string& name, const string& source, const string& dest)
	{
		meshName = name;
		sourcePath = source;
		destinationPath = dest;
	}

	~MeshInfo()
	{
		meshName.clear();
		sourcePath.clear();
		destinationPath.clear();
	}

	string meshName;
	string sourcePath;
	string destinationPath;
};

// PUBLIC METHODS

// Reads mesh name-mesh path pairs in options.txt
// TRUE: Opening and reading options.txt process happened
// FALSE: options.txt is not opened
bool MeshReader::start()
{
	// Read options.txt to get mesh list.
	std::vector<MeshInfo*> meshList;
	ifstream optionsFile("options.txt");
	if (optionsFile)
	{
		string name;

		// Read mesh name - mesh file path
		while (getline(optionsFile, name))
		{
			// You have mesh name, now get mesh path
			string source, dest;
			getline(optionsFile, source);	// TODO A CONTROL FOR THIS?
			getline(optionsFile, dest);	// TODO A CONTROL FOR THIS?
			MeshInfo* newInfo = new MeshInfo(name, source, dest);
			meshList.push_back(newInfo);
		}

		optionsFile.close();

		// Start reading and writing process
		generateMeshFiles(meshList);

		// Memory management
		for (int i = 0; i < meshList.size(); i++)
		{
			delete meshList[i];
		}
		meshList.clear();

		cout << "\n\nDONE!";
	}
	else
	{
		cout << "ERROR! CANNOT OPEN options.txt FILE to read mesh name-mesh path pairs!\n\n";
		return false;
	}

	return true;
}

// PRIVATE METHODS

// Generate bone structure + positions, bone weights, face, vertex, and normal files for given meshes
void MeshReader::generateMeshFiles(const std::vector<MeshInfo*>& meshList)
{
	for (int mi = 0; mi < meshList.size(); mi++)
	{
		if (!readMesh(meshList[mi]))
		{
			cout << "ERROR! A PROBLEM ACCORD WHILE READING " << meshList[mi]->meshName << "MESH AT " << meshList[mi]->sourcePath << "MESH PATH!\n\n";
		}
	}
}

// Read mesh file using Assimp and generates related files
bool MeshReader::readMesh(const MeshInfo* meshInfo)
{
	// Check whether the file exists
	std::ifstream meshFile(meshInfo->sourcePath.c_str());
	if (meshFile)
	{
		meshFile.close();	// File exists. Start mesh reading.

		// Read mesh file using Assimp
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(meshInfo->sourcePath.c_str(), aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

		if (scene && scene->HasMeshes())
		{
			if (!getMeshData(scene, meshInfo->meshName, meshInfo->destinationPath))
			{
				// Scene does not have any meshes!
				cout << "ERROR! ASSIMP SCENE OBJECT DOES NOT CONTAIN ANY MESHES!\n\n";
			}
		}
		else
		{
			// Mesh reading failed!
			cout << "ERROR! ASSIMP CANNOT IMPORT MESH!\n";
			cout << importer.GetErrorString() << std::endl << std::endl;
		}
	}
	else
	{
		return false;
	}

	return true;
}

// Get mesh info from the scene and write files
bool MeshReader::getMeshData(const aiScene* scene, const string& meshName, const string& meshPath)
{
	// TODO SOMETIMES ASSIMP DIVIDE SAME MESH INTO SUB-MESHES (Single mesh becomes 3 different meshes...)
	// CURRENTLY, THIS BEHAVIOR IS EXCLUDED
	aiMesh* mesh = scene->mMeshes[0];

	if (mesh->HasFaces() && mesh->HasPositions() && mesh->HasNormals())
	{
		// We can start the process.

		// TODO DO NOT FORGET TO HANDLE ANIMATIONS!!!
		// TODO WRITE A MESH-FACTORY AUTOMATICALLY GENERATES RELATED MESH. SO, THIS METHOD CAN USE POLYMORPHISM
		if (mesh->HasBones())
		{
			cout << "\nMesh has skeleton. Skeletal information will also be written.";

			// TODO CURRENTLY ONLY CONSIDER libigl.
			SkeletonMesh newMesh(mesh, scene->mRootNode, meshName, meshPath, true);
			newMesh.save();
		}
		else
		{
			cout << "\nMesh does not contain a skeleton.";
			RegularMesh newMesh(mesh, meshName, meshPath, true);
			newMesh.save();
		}
	}
	else
	{
		// Mesh has some problems
		cout << "\nERROR! MESH NAMED " << meshName << "DOES NOT INCLUDE";
		if (mesh->HasFaces())
		{
			cout << " FACES";
		}
		if (mesh->HasPositions())
		{
			cout << " VERTEX POSITIONS";
		}
		if (mesh->HasNormals())
		{
			cout << " NORMALS";
		}
		cout << "!!!\n";

		return false;
	}

	// ???
	//delete mesh;

	return true;
}