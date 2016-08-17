//#include <iostream>
//using std::cout;
//
//#include <fstream>
//using std::ifstream;
//
//#include <string>
//using std::string;
//
//#include <assimp\Importer.hpp>
//Assimp::Importer importer;
//
//#include <assimp\scene.h>
//const aiScene* scene = NULL;
//
//#include <assimp\postprocess.h>
//
//#include <vector>
//#include <utility>
//using std::pair;
//
//// Helper class for keeping mesh anme-mesh path pairs
//class MeshInfo
//{
//public:
//	MeshInfo(const string& name, const string& path)
//	{
//		meshName = name;
//		meshPath = path;
//	}
//
//	string meshName;
//	string meshPath;
//};
//
//// Helper function for traversing aiNode of Assimp
//void nodetraversal(const aiNode* node, string& lines)
//{
//	cout << lines << " " << node->mName.C_Str() << "   Number of meshes: " << node->mNumMeshes << "   Number of Children: " << node->mNumChildren << std::endl;
//
//	for (int ci = 0; ci < node->mNumChildren; ci++)
//	{
//		string newLine = lines + "-";
//		nodetraversal(node->mChildren[ci], newLine);
//	}
//}
//
//bool checkIfInBones(const aiMesh* mesh, const string& nodeName)
//{
//	for (int si = 0; si < mesh->mNumBones; si++)
//	{
//		if (nodeName.compare(mesh->mBones[si]->mName.C_Str()) == 0)
//		{
//			return true;
//		}
//	}
//
//	return false;
//}
//
//// Helper function for traversing aiBone of Assimp
//void bonetraversal(const aiNode* node, const aiMesh* mesh, string& lines)
//{
//	cout << lines << " " << node->mName.C_Str() << "   Number of meshes: " << node->mNumMeshes << "   Number of Children: " << node->mNumChildren << std::endl;
//
//	for (int ci = 0; ci < node->mNumChildren; ci++)
//	{
//		// Check whether the children in the bones
//		if (checkIfInBones(mesh, node->mChildren[ci]->mName.C_Str()))
//		{
//			string newLine = lines + "-";
//			bonetraversal(node->mChildren[ci], mesh, newLine);
//		}
//	}
//}
//
//// Check whether mesh has bones, faces, normals, and positions and print warnings.
//void meshCheck(const aiMesh* mesh, const string& udMeshName)
//{
//	//TODO GOTTA DO SOMETHING BETTER THAN JUST PRINT A WARNING!
//
//	if (!mesh->HasBones())
//	{
//		cout << "WARNING!!! THE MESH " << udMeshName << " HAS NO BONES!!!\n";
//	}
//	else if (!mesh->HasFaces())
//	{
//		cout << "WARNING!!! THE MESH " << udMeshName << " HAS NO FACES!!!\n";
//	}
//	else if (!mesh->HasNormals())
//	{
//		cout << "WARNING!!! THE MESH " << udMeshName << " HAS NO NORMALS!!!\n";
//	}
//	else if (!mesh->HasPositions())
//	{
//		cout << "WARNING!!! THE MESH " << udMeshName << " HAS NO VERTEX POSITIONS!!!\n";
//	}
//}
//
//// Write face information
//bool writeFaces(const int meshIndex, const string& udMeshName, const aiMesh* mesh)
//{
//	// Write face info
//	string facePath = udMeshName + mesh->mName.C_Str() + "_" + std::to_string(meshIndex) +"_" + "faceInfo.fi";
//	FILE* faceFile = fopen(facePath.c_str(), "wb");
//
//	if (faceFile)
//	{
//		fwrite(&mesh->mNumFaces, sizeof(int), 1, faceFile);
//
//		for (int fi = 0; fi < mesh->mNumFaces; fi++)
//		{
//		//	cout << "F-" << fi << ": ";
//
//			for (int ind = 0; ind < mesh->mFaces[fi].mNumIndices; ind++)
//			{
//				int faceIndex = mesh->mFaces[fi].mIndices[ind];
//			//	cout << faceIndex << " - ";
//				fwrite(&faceIndex, sizeof(int), 1, faceFile);
//			}
//
//		//	cout << std::endl;
//		}
//
//		fclose(faceFile);
//	}
//	else
//	{
//		cout << "\n\nERROR!!! CANNOT OPEN FACE FILE: " << facePath << " FOR MESH " << udMeshName << std::endl << std::endl;
//		return false;
//	}
//
//	return true;
//}
//
//// Write vertex positions
//bool writeVertices(const int meshIndex, const string& udMeshName, const aiMesh* mesh)
//{
//	string vPath = udMeshName + mesh->mName.C_Str() + "_" + std::to_string(meshIndex) + "_" + "vertexInfo.vi";
//	FILE* vertexFile = fopen(vPath.c_str(), "wb");
//
//	if (vertexFile)
//	{
//		fwrite(&mesh->mNumVertices, sizeof(int), 1, vertexFile);
//
//		for (int vi = 0; vi < mesh->mNumVertices; vi++)
//		{
//			// Vertices
//			//TODO THIS PART IS FOR DEBUGING !!!!!!
//			aiVector3D vPos = mesh->mVertices[vi];
//			float x = vPos.x;
//			float y = vPos.y;
//			float z = vPos.z;
//			//TODO THIS PART IS FOR DEBUGING !!!!!!
//
//			fwrite(&x, sizeof(float), 1, vertexFile);
//			fwrite(&y, sizeof(float), 1, vertexFile);
//			fwrite(&z, sizeof(float), 1, vertexFile);
//		}
//
//		fclose(vertexFile);
//	}
//	else
//	{
//		cout << "\n\nERROR!!! CANNOT OPEN FACE FILE: " << vPath << " FOR MESH " << udMeshName << std::endl << std::endl;
//		return false;
//	}
//
//	return true;
//}
//
//// Write per-vertex normals
//bool writePerVerNormals(const int meshIndex, const string& udMeshName, const aiMesh* mesh)
//{
//	// Write vertex position and normal information
//	
//	string nPath = udMeshName + mesh->mName.C_Str() + "_" + std::to_string(meshIndex) + "_" + "normalInfo.ni";
//	FILE* normalsFile = fopen(nPath.c_str(), "wb");
//
//	if ( normalsFile)
//	{
//		fwrite(&mesh->mNumVertices, sizeof(int), 1, normalsFile);
//
//		for (int vi = 0; vi < mesh->mNumVertices; vi++)
//		{
//			// Normals
//			//TODO THIS PART IS FOR DEBUGING !!!!!!
//			aiVector3D vNor = mesh->mNormals[vi];
//			float norX = vNor.x;
//			float norY = vNor.y;
//			float norZ = vNor.z;
//			//TODO THIS PART IS FOR DEBUGING !!!!!!
//
//			fwrite(&norX, sizeof(float), 1, normalsFile);
//			fwrite(&norY, sizeof(float), 1, normalsFile);
//			fwrite(&norZ, sizeof(float), 1, normalsFile);
//		}
//
//		
//		fclose(normalsFile);
//	}
//	else
//	{
//		cout << "\n\nERROR!!! CANNOT OPEN FACE FILE: " << nPath << " FOR MESH " << udMeshName << std::endl << std::endl;
//		return false;
//	}
//
//	return true;
//}
//
//// Write bone structure and bone positions
//bool writeBoneInfo(const int meshIndex, const string& udMeshName, const aiMesh* mesh)
//{
//	//TODO !!!!!! NEEEEED LOTS OF LOTS OF REFACTORING !!!!!!!!!!!!
//
//	std::vector<pair<int, int>> boneStructure;
//
//	std::vector<bool> isBoneIn;
//	isBoneIn.resize(mesh->mNumBones);
//	std::fill(isBoneIn.begin(), isBoneIn.end(), false);
//
//	std::vector<std::vector <float>> vertexWeights(mesh->mNumVertices, std::vector<float>(mesh->mNumBones));
//
//	// Find bone structure
//	for (int bi = 0; bi < mesh->mNumBones; bi++)
//	{
//		aiBone* nextBone = mesh->mBones[bi];
//		aiNode* node = scene->mRootNode->FindNode(nextBone->mName.C_Str());
//
//		//bonetraversal(node, mesh, string(""));
//
//		// Set weights
//		for (int wi = 0; wi < nextBone->mNumWeights; wi++)
//		{
//			int nextV = nextBone->mWeights[wi].mVertexId;
//			float nextW = nextBone->mWeights[wi].mWeight;
//
//			vertexWeights[nextBone->mWeights[wi].mVertexId].at(bi) = nextBone->mWeights[wi].mWeight;
//		}
//
//		// SALAKCA KISIM:
//		// ASSIMP NODE VE BONE AYRI AYRI TUTUYOR. KIMI NODE LAR BONE OLARAK TUTULMUYOR
//		// SENIN NODE U BULUP NODE UN HER COCUGU ICIN BONE LARI GEZIP ILGILI BONE ILE NODE UN COCUGU OLAN BONE ARASINDA BAGLANTI KURMAN LAZIM
//
//		
//
//		//cout << bi << " - Node: " << node->mName.C_Str() << std::endl;
//
//		for (int ci = 0; ci < node->mNumChildren; ci++)
//		{
//			string nextName = node->mChildren[ci]->mName.C_Str();
//
//			for (int si = 0; si < mesh->mNumBones; si++)
//			{
//				if (nextName.compare(mesh->mBones[si]->mName.C_Str()) == 0)
//				{
//					//cout << "   " << "#" << ci + 1 << " Child of " << nextBone->mName.C_Str() << " (" << bi << ")" << ": " << nextName << " (" << si << ")" << std::endl;
//
//					boneStructure.push_back(std::make_pair(bi, si));
//					break;
//				}
//			}
//		}
//
//	}	// End of each bones
//
//	// Write weight info
//	string weightPath = udMeshName + mesh->mName.C_Str() + "_" + std::to_string(meshIndex) + "_" + "boneWeights.bw";
//	FILE* weightFile = fopen(weightPath.c_str(), "wb");
//
//	if (weightFile)
//	{
//		// Write file size
//		fwrite(&mesh->mNumVertices, sizeof(int), 1, weightFile);
//		fwrite(&mesh->mNumBones, sizeof(int), 1, weightFile);
//
//		for (int vi = 0; vi < vertexWeights.size(); vi++)
//		{
//			for (int bi = 0; bi < vertexWeights[vi].size(); bi++)
//			{
//				fwrite(&vertexWeights[vi].at(bi), sizeof(float), 1, weightFile);
//			}
//		}
//
//		fclose(weightFile);
//	}
//	else
//	{
//		cout << "\n\nERROR!!! CANNOT OPEN BONE STRUCTURE FILE: " << weightPath << " FOR MESH " << udMeshName << std::endl << std::endl;
//		return false;
//	}
//
//	// Write bone structure info
//	string structurePath = udMeshName + mesh->mName.C_Str() + "_" + std::to_string(meshIndex) + "_" + "boneStructure.bs";
//	FILE* structureFile = fopen(structurePath.c_str(), "wb");
//
//	if (structureFile)
//	{
//		int rows = boneStructure.size();
//		int cols = 2;
//
//		fwrite(&rows, sizeof(int), 1, structureFile);
//		fwrite(&cols, sizeof(int), 1, structureFile);
//
//		for (int bs = 0; bs < rows; bs++)
//		{
//			int first = boneStructure[bs].first;
//			int sec = boneStructure[bs].second;
//
//			fwrite(&first, sizeof(int), 1, structureFile);
//			fwrite(&sec, sizeof(int), 1, structureFile);
//		}
//
//		fclose(structureFile);
//	}
//	else
//	{
//		cout << "\n\nERROR!!! CANNOT OPEN BONE STRUCTURE FILE: " << structurePath << " FOR MESH " << udMeshName << std::endl << std::endl;
//		return false;
//	}
//
//	// Decide which bones are needed
//	int boneCounter = 0;
//	for (int bi = 0; bi < boneStructure.size(); bi++)
//	{
//		int first = boneStructure[bi].first, second = boneStructure[bi].second;
//
//		if (!isBoneIn[first])
//		{
//			isBoneIn[first] = true;
//			boneCounter++;
//		}
//
//		if (!isBoneIn[second])
//		{
//			isBoneIn[second] = true;
//			boneCounter++;
//		}
//	}
//
//	// Write bone positions
//	string bonePos = udMeshName + mesh->mName.C_Str() + "_" + std::to_string(meshIndex) + "_" + "bonePos.bp";
//	FILE* bonePosFile = fopen(bonePos.c_str(), "wb");
//
//	if (bonePosFile)
//	{
//		fwrite(&boneCounter, sizeof(int), 1, bonePosFile);
//
//		aiMatrix4x4 globalInverseMat = scene->mRootNode->mTransformation;
//		globalInverseMat.Inverse();
//
//		for (int bi = 0; bi < mesh->mNumBones; bi++)
//		{
//			if (isBoneIn[bi])
//			{
//
//				//aiMatrix4x4 boneMatrix = mesh->mBones[bi]->mOffsetMatrix;
//				//aiNode* currentNode = scene->mRootNode->FindNode(mesh->mBones[bi]->mName);
//				//aiMatrix4x4 globalBoneMatrix = currentNode->mTransformation;
//
//				//// Traverse all parents to get bone matrix
//				//const aiNode* explorerNode = currentNode;
//				//while (explorerNode->mParent)
//				//{
//				//	globalBoneMatrix = explorerNode->mParent->mTransformation * globalBoneMatrix;
//				//	//boneMatrix = boneMatrix * explorerNode->mTransformation;
//				//	explorerNode = explorerNode->mParent;
//				//}
//
//				////aiMatrix4x4 mat = mesh->mBones[bi]->mOffsetMatrix * scene->mRootNode->FindNode(mesh->mBones[bi]->mName)->mTransformation;
//
//				//aiMatrix4x4 finalMatrix = globalInverseMat * globalBoneMatrix * boneMatrix;
//
//				aiVector3D scale, vPos;
//				aiQuaternion rot;
//
//				aiMatrix4x4 boneMatrix = mesh->mBones[bi]->mOffsetMatrix.Inverse();
//				//finalMatrix.Decompose(scale, rot, vPos);
//				
//				boneMatrix.Decompose(scale, rot, vPos);
//
//				// Vertices
//				//TODO THIS PART IS FOR DEBUGING !!!!!!
//				float x = vPos.x;
//				float y = vPos.y;
//				float z = vPos.z;
//				//TODO THIS PART IS FOR DEBUGING !!!!!!
//
//				fwrite(&x, sizeof(float), 1, bonePosFile);
//				fwrite(&y, sizeof(float), 1, bonePosFile);
//				fwrite(&z, sizeof(float), 1, bonePosFile);
//			}
//		}
//
//		fclose(bonePosFile);
//	}
//	else
//	{
//		cout << "\n\nERROR!!! CANNOT OPEN FACE FILE: " << bonePos << " FOR MESH " << udMeshName << std::endl << std::endl;
//		return false;
//	}
//
//	return true;
//}
//
//// Get mesh info from the scene and write files
//bool getMeshData(const aiScene* scene, const string& udMeshName)
//{
//	//TODO ASSIMP SOMETIMES DEVIDE ONE MESH INTO FEW SUBMESHES! HANDLE THIS SITUATION!
//	// Currently robot arm does not need to handle this. That's why I'll only consider scene=>mMesh[0]
//
//	if (scene->HasMeshes())
//	{
//		//TODO !!!!!!!
//		aiMesh* mesh = scene->mMeshes[0];	//TODO !!!!!!!
//		//TODO !!!!!!!
//
//		meshCheck(mesh, udMeshName);
//
//		// Write vertex positions and per-vertex normal info
//
//		// Get and write face information
//		if (!writeFaces(0, udMeshName, mesh))	// TODO !!!!!! FIRST PARAMETERE SHOULDNT BE 0!!!!
//		{
//			// TODO SHOUD I RETURN FALSE OR SOMETHING?
//		}
//
//		if (!writeVertices(0, udMeshName, mesh))	// TODO !!!!!! FIRST PARAMETERE SHOULDNT BE 0!!!!
//		{
//			// TODO SHOUD I RETURN FALSE OR SOMETHING?
//		}
//
//		if (!writePerVerNormals(0, udMeshName, mesh))// TODO !!!!!! FIRST PARAMETERE SHOULDNT BE 0!!!!
//		{
//			// TODO SHOUD I RETURN FALSE OR SOMETHING?
//		}
//
//		// Write Bone Positions + Bone Connection info
//		if (!writeBoneInfo(0, udMeshName, mesh))// TODO !!!!!! FIRST PARAMETERE SHOULDNT BE 0!!!!
//		{
//			// TODO SHOUD I RETURN FALSE OR SOMETHING?
//		}
//
//		// Write Bone Weights info
//		
//
//	}
//	else
//	{
//		return false;
//	}
//
//	return true;
//	
//}
//
//// Read mesh file using Assimp and generates related files
//bool readMesh(const MeshInfo* meshInfo)
//{
//	// Check whether the file exists
//	std::ifstream meshFile(meshInfo->meshPath.c_str());
//	if (meshFile)
//	{
//		meshFile.close();	// File exists. Start mesh reading.
//
//		// Read mesh file using Assimp
//		scene = importer.ReadFile(meshInfo->meshPath.c_str(), aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);
//
//		if (scene)
//		{
//			if (!getMeshData(scene, meshInfo->meshName))
//			{
//				// Scene does not have any meshes!
//				cout << "ERROR! ASSIMP SCENE OBJECT DOES NOT CONTAIN ANY MESHES!\n\n";
//			}
//		}
//		else
//		{
//			// Mesh reading failed!
//			cout << "ERROR! ASSIMP CANNOT IMPORT MESH!\n";
//			cout << importer.GetErrorString() << std::endl << std::endl;
//		}
//	}
//	else
//	{
//		return false;
//	}
//
//	return true;
//}
//
//// Generate bone structure + positions, bone weights, face, vertex, and normal files for given meshes
//void generateMeshFiles(const std::vector<MeshInfo*>& meshList)
//{
//	for (int mi = 0; mi < meshList.size(); mi++)
//	{
//		if (!readMesh(meshList[mi]))
//		{
//			cout << "ERROR! A PROBLEM ACCORD WHILE READING " << meshList[mi]->meshName << "MESH AT " << meshList[mi]->meshPath << "MESH PATH!\n\n";
//		}
//	}
//}

#include "MeshReader.h"

void main()
{
	// Read options.txt to get mesh list.
	//std::vector<MeshInfo*> meshList;
	//ifstream optionsFile("options.txt");
	//if (optionsFile)
	//{
	//	string line;

	//	// Read mesh name - mesh file path
	//	while (getline(optionsFile, line))
	//	{
	//		// You have mesh name, now get mesh path
	//		string path;
	//		getline(optionsFile, path);
	//		MeshInfo* newInfo = new MeshInfo(line, path);
	//		meshList.push_back(newInfo);
	//	}

	//	optionsFile.close();

	//	// Start reading and writing process
	//	generateMeshFiles(meshList);

	//	cout << "DONE!";
	//}
	//else
	//{
	//	cout << "ERROR! CANNOT OPEN options.txt FILE to read mesh name-mesh path pairs!\n\n";
	//}

	MeshReader mr;
	mr.start();
}