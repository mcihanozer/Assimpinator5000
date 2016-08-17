// M. Cihan Ozer - August 2016, Montreal
//				^-_-^

// Decleration of RegularMesh class for serialization of mesh containing 
// only vertex position, per-vertex normal, and face information.

#ifndef REGULARMESH_H
#define REGULARMESH_H

#include "Mesh.h"

class RegularMesh : Mesh
{
public:
	RegularMesh(const aiMesh* baseMesh, const string& meshName, const string& filePath, bool isLibigl = false)
		: Mesh(baseMesh, meshName, filePath, isLibigl)
	{}

	bool save()
	{
		writeFaces();
		writeVertices();
		writePerVerNormals();

		return true;
	}
};

#endif