#pragma once
#include <string>
#include <vector>
#include <FreeImage.h>
#include "Object.h"
#include "PMXLoader.h"
#include "TexLoader.h"

class Object;
class TexLoader;

class Header
{
public:
	enum class TextEncoding
	{
		UTF16LE,
		UTF8
	};

	float version;
	TextEncoding textEncodingType;
	int appendixUV;
	size_t vertexIndexSize;
	size_t textureIndexSize;
	size_t materialIndexSize;
	size_t boneIndexSize;
	size_t morphIndexSize;
	size_t rigidBodyIndexSize;

	void Output()
	{
		std::cout << "------ Header Info ------" << std::endl;
		std::cout << "Version : " << version << std::endl;
		std::cout << "Text Encoding Type : ";
		switch (textEncodingType)
		{
		case TextEncoding::UTF16LE:
			std::cout << "UTF-16LE" << std::endl;
			break;
		case TextEncoding::UTF8:
			std::cout << "UTF-8" << std::endl;
			break;
		default:
			std::cerr << "This encoding type is not defined." << std::endl;
			break;
		}
		std::cout << "Appendix UV : " << appendixUV << std::endl;
		std::cout << "Vertex Index Size : " << vertexIndexSize << " byte" << std::endl;
		std::cout << "Texture Index Size : " << textureIndexSize << " byte" << std::endl;
		std::cout << "Material Index Size : " << materialIndexSize << " byte" << std::endl;
		std::cout << "Bone Index Size : " << boneIndexSize << " byte" << std::endl;
		std::cout << "Morph Index Size : " << morphIndexSize << " byte" << std::endl;
		std::cout << "Rigid Body Index Size : " << rigidBodyIndexSize << " byte" << std::endl;
		std::cout << "-------------------------" << std::endl;
	}
};

enum class WeightType
{
	BDEF1,
	BDEF2,
	BDEF4,
	SDEF
};

struct Weight
{
	std::vector<int> bones;
	std::vector <float> weights;
	std::vector<std::array<float, 3>> sdef;
};

class VertexData
{
public:
	float position[3];
	float normal[3];
	float uv[2];

	// std::vector<std::array<float,4>> appendixUV;

	// WeightType weightType;
	// Weight weight;

	float edgeScale;
};

class MaterialData
{
public:
	float diffuseColor[4];
	float specularColor[3];
	float specularity;
	float ambientColor[3];
	char drawingModeflags;
	float edgeColor[4];
	float edgeSize;
	int textureIndex;
	int sphereTextureIndex;
	int sphereMode;
	bool sharedToonFlag;
	int toonTextureIndex;
	int surfaceCount;

	enum class DrawingMode
	{
		DoubleSided,
		Shadow,
		SelfShadowMap,
		SelfShadow,
		DrawEdges
	};

	bool DrawingModeFlag(DrawingMode mode)
	{
		return (drawingModeflags &(1 << static_cast<int>(mode))) != 0;
	}
};

class Model
{
public:
	Header header;

	int vertexCount;
	std::vector<VertexData> vertexData;

	std::vector<Object::Vertex> vertices;


	int faceCount;
	std::vector<GLuint> vertexIndex;

	int texCount;
	std::vector<std::wstring> texPath;
	// std::vector<TexLoader::Texture> textureData;
	std::vector<FIBITMAP*> textureData;

	int materialCount;
	std::vector<MaterialData> materialData;

	PMXLoader pmxLoader;
	TexLoader texLoader;

	Model(const std::wstring& path);

};

// 六面体の頂点の位置
constexpr Object::Vertex cubeVertex[] =
{
	{ -1.0f, -1.0f, -1.0f }, // (0)
{ -1.0f, -1.0f, 1.0f }, // (1)
{ -1.0f, 1.0f, 1.0f }, // (2)
{ -1.0f, 1.0f, -1.0f }, // (3)
{ 1.0f, 1.0f, -1.0f }, // (4)
{ 1.0f, -1.0f, -1.0f }, // (5)
{ 1.0f, -1.0f, 1.0f }, // (6)
{ 1.0f, 1.0f, 1.0f } // (7)
};
// 六面体の稜線の両端点のインデックス
constexpr GLuint wireCubeIndex[] =
{
	1, 0, // (a)
	2, 7, // (b)
	3, 0, // (c)
	4, 7, // (d)
	5, 0, // (e)
	6, 7, // (f)
	1, 2, // (g)
	2, 3, // (h)
	3, 4, // (i)
	4, 5, // (j)
	5, 6, // (k)
	6, 1 // (l)
};

constexpr GLuint solidCubeIndex[] =
{
	0, 1, 2, 0, 2, 3, // 左
	0, 3, 4, 0, 4, 5, // 裏
	0, 5, 6, 0, 6, 1, // 下
	7, 6, 5, 7, 5, 4, // 右
	7, 4, 3, 7, 3, 2, // 上
	7, 2, 1, 7, 1, 6 // 前
};

// 矩形の頂点の位置
constexpr Object::Vertex vertices[] =
{
	{ 0.0f, 1.0f, 0.0f },
{ -1.0f, 0.0f, 0.0f },
{ 0.0f, -1.0f, 0.0f },
{ 1.0f, 0.0f, 0.0f },
{ 0.0f, 1.0f, 0.0f },
{ 0.0f, 0.0f, 1.0f },
{ 0.0f, -1.0f, 0.0f },
{ 0.0f, 0.0f, -1.0f },
{ -1.0f, 0.0f, 0.0f },
{ 0.0f, 0.0f, 1.0f },
{ 1.0f, 0.0f, 0.0f },
{ 0.0f, 0.0f, -1.0f }
};


