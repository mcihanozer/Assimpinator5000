// M. Cihan Ozer - August 2016, Montreal
//				^-_-^

// Implementation of abstract Mesh class for serialization
// of mesh information which is read by Assimp library

#include "Mesh.h"

#include <iostream>
using std::cout;

// CONSTRUCTOR

Mesh::Mesh(const aiMesh* mesh, const string& meshName, const string& filePath, bool libigl)
{
	// Set mesh info
	isLibigl = libigl;
	mMeshName = meshName + "_" + mesh->mName.C_Str();
	mFilePath = filePath;

	// Set face info
	mFaceNumber = mesh->mNumFaces;
	mFaceInfo = mesh->mFaces;

	// Set vertex info
	mVertexNumber = mesh->mNumVertices;
	mVertexInfo = mesh->mVertices;

	// Set normal info
	mNormalNumber = mesh->mNumVertices;	// Assimp uses per-vertex normals
	mNormalInfo = mesh->mNormals;
}

// PROTECTED METHODS

// Write face information into meshName.fi file
bool Mesh::writeFaces()
{
	// Write face info
	string facePath = mFilePath + "\\" + mMeshName + ".fi";
	FILE* faceFile = fopen(facePath.c_str(), "wb");

	if (faceFile)
	{
		// I assume, faces are triangle. So number of columns are always 3.
		fwrite(&mFaceNumber, sizeof(int), 1, faceFile);

		for (int fi = 0; fi < mFaceNumber; fi++)
		{
			for (int ind = 0; ind < mFaceInfo[fi].mNumIndices; ind++)
			{
				int faceIndex = mFaceInfo[fi].mIndices[ind];
				fwrite(&faceIndex, sizeof(int), 1, faceFile);
			}
		}

		fclose(faceFile);
	}
	else
	{
		cout << "\n\nERROR!!! CANNOT OPEN FACE FILE: " << facePath << " FOR MESH " << mMeshName << std::endl << std::endl;
		return false;
	}

	return true;
}

// Write vertex information into meshName.vi file
bool Mesh::writeVertices()
{
	string vertexPath = mFilePath + "\\" + mMeshName + ".vi";
	FILE* vertexFile = fopen(vertexPath.c_str(), "wb");

	if (vertexFile)
	{
		fwrite(&mVertexNumber, sizeof(int), 1, vertexFile);

		for (int vi = 0; vi < mVertexNumber; vi++)
		{
			// Vertices
			//TODO THIS PART IS FOR DEBUGING !!!!!!
			aiVector3D vPos = mVertexInfo[vi];
			float x = vPos.x;
			float y = vPos.y;
			float z = vPos.z;
			//TODO THIS PART IS FOR DEBUGING !!!!!!

			fwrite(&x, sizeof(float), 1, vertexFile);
			fwrite(&y, sizeof(float), 1, vertexFile);
			fwrite(&z, sizeof(float), 1, vertexFile);
		}

		fclose(vertexFile);
	}
	else
	{
		cout << "\n\nERROR!!! CANNOT OPEN FACE FILE: " << vertexPath << " FOR MESH " << mMeshName << std::endl << std::endl;
		return false;
	}

	return true;
}

// Write vertex information into meshName.ni file
bool Mesh::writePerVerNormals()
{
	// Write vertex position and normal information
	string normalPath = mFilePath + "\\" + mMeshName + ".ni";
	FILE* normalsFile = fopen(normalPath.c_str(), "wb");

	if (normalsFile)
	{
		fwrite(&mNormalNumber, sizeof(int), 1, normalsFile);

		for (int vi = 0; vi < mNormalNumber; vi++)
		{
			// Normals
			//TODO THIS PART IS FOR DEBUGING !!!!!!
			aiVector3D vNor = mNormalInfo[vi];
			float norX = vNor.x;
			float norY = vNor.y;
			float norZ = vNor.z;
			//TODO THIS PART IS FOR DEBUGING !!!!!!

			fwrite(&norX, sizeof(float), 1, normalsFile);
			fwrite(&norY, sizeof(float), 1, normalsFile);
			fwrite(&norZ, sizeof(float), 1, normalsFile);
		}


		fclose(normalsFile);
	}
	else
	{
		cout << "\n\nERROR!!! CANNOT OPEN FACE FILE: " << normalPath << " FOR MESH " << mMeshName << std::endl << std::endl;
		return false;
	}

	return true;
}