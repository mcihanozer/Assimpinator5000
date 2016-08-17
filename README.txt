M. Cihan Ozer - Aug. 2016, Montreal
	mco@mcihanozer.com
	^-_-^
Quote of the Day: "No! No, see, this is the fundamental flaw with the check check-plus check-minus system!
The only reason Token was able to do all that is because his family is rich!"


NAME:

As always, I did not care and could not come up about/with a name when I start to right this program.
So, I am naming it as Assimpinator5000.

PURPOSE:

This program reads meshes using Assimp library and then serialize them into bunch of different files.
Thus, instead of writing/adding bunch of mesh reader functions for
different mesh extensions or handling Assimp library on your own, you can use this program, and write
a C++ code that reads binary files to load the meshes.

FLOW:

Program reads mesh name + mesh source + destination path pairs(?) in options.txt, and generates
.fi (face information), .vi (vertex information), .ni (normal information),
.bp (bone position information), .bs (bone structure information), and .bw (bone weights information)
files.

STRUCTURE:

options.txt:
-------------------------------------------------------------------------------------------------------
MESH_NAME_1
FILE_PATH_FOR_MESH_SOURCE_1
DESTINATION_PATH_FOR_GENERATED_FILES_1
MESH_NAME_2
FILE_PATH_FOR_MESH_SOURCE_2
DESTINATION_PATH_FOR_GENERATED_FILES_2
.
.
.
MESH_NAME_N
FILE_PATH_FOR_MESH_SOURCE_N
DESTINATION_PATH_FOR_GENERATED_FILES_N

Output Files:
-------------------------------------------------------------------------------------------------------
.fi: First, number of faces (int), rest face information (int int int). When you init your matrix, std::vector,
etc. you should create it as (number of faces, 3).
This file assumes all faces are triangulated and does not bother itself by writing a constant 3 value following
number of faces.

.vi: First, number of vertices (int), rest vertex position information (float float float). When you init your
matrix, std::vector, etc. you should create it as (number of vertices, 3).
This file assumes all vertices only have (x, y, z) positions.

.ni: First, number of normals (int), rest normal information (float float float). When you init your matrix,
std::vector, etc. you should create it as (number of normals, 3).
This files keeps per-vertex normals.

.bp: First, number of bones (int), rest bone positions (float float float). When you init your matrix, std::vector,
etc. you should create it as (number of bones, 3).
This file assumes all bones only have (x, y, z) positions.

.bs: This file may change in the future but, right now I focus on ligigl .tgf file. So, it has (int, int) pairs where
the ints are index of the bones. This file contains first (int, int) number of rows
and cols ( (numberofbones, 2) ), and structure information in (int, int) pairs.

.bw: First, number of vertices (int) and number of bones (int). Rest, weight information (float). This file keeps whole
weight information for each bone even if the weight is 0 -for now...

WARNING:

There might be some bugs, because I did not consider every possible scenario for now. For exmaple, I do not care whether
Assimp divides a single mesh into bunch of meshes or not. Because, currently
all the meshes I am using does not create this problem.

FUTURE:

Right now, this program is like this because, this is enough for  me. But, when I need more, I will add more. In the and,
it will become a complete program that considers everything and generates
a good file -ideally single or mesh, bone, animation etc. .