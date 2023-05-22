#include "Model.h"
#include "PMXLoader.h"
#include "TexLoader.h"
// #include "Object.h"

Model::Model(const std::wstring& path) : pmxLoader(path,this), texLoader(this)
{
	pmxLoader.Load();
	texLoader.Load();
	// header.Output();
	// std::cout << "Vertex Count : " << vertexCount << std::endl;
	// std::cout << "Face Count : " << faceCount << std::endl;

	/*std::cout << "------ Verticies ------" << std::endl;
	for (int i = 0; i < vertexCount; i++)
	{
		std::cout << "(" <<vertices[i].position[0] << ", " << vertices[i].position[1] << ", " << vertices[i].position[2] << ")" << std::endl;
	}
	std::cout << "-------------------------" << std::endl;*/
}

