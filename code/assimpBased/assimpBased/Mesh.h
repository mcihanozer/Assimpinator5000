// M. Cihan Ozer - August 2016, Montreal
//				^-_-^

// Decleration of abstract Mesh class for serialization
// of mesh information which is read by Assimp library

#ifndef MESH_H
#define MESH_H

#include <assimp\scene.h>

#include <string>
using std::string;

class Mesh
{
public:
	// CONSTRUCTOR
	Mesh(const aiMesh* baseMesh, const string& meshName, const string& filePath, bool isLibigl = false);

	// METHODS
	virtual bool save() = 0;	// Write information of the mesh file into file

protected:

	// METHODS

	// TODO DO NOT WRITE THIS INFORMATION INTO DIFFERENT FILES SUCH AS .fi, .vi, .ni etc.
	bool writeFaces();
	bool writeVertices();
	bool writePerVerNormals();

	// MEMBERS
	bool isLibigl;

	string mMeshName;
	string mFilePath;
	
	int mFaceNumber;
	const aiFace* mFaceInfo;
	
	int mVertexNumber;
	const aiVector3D* mVertexInfo;

	int mNormalNumber;
	const aiVector3D* mNormalInfo;

};

#endif