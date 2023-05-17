#include "Model.h"
#include "PMXLoader.h"

Model::Model(const std::string& path) : pmxLoader(path,this)
{
	pmxLoader.Load();
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

