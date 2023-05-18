#include "Model.h"
#include "PMXLoader.h"

PMXLoader::PMXLoader(const std::string& path, Model* model) :
	path(path), model(model)
{

}

bool PMXLoader::getPMXStringUTF16(std::ifstream& _file, std::wstring& output)
{
	std::array<wchar_t, 512> wBuffer{};
	int textSize;

	_file.read(reinterpret_cast<char*>(&textSize), 4);

	_file.read(reinterpret_cast<char*>(&wBuffer), textSize);
	output = std::wstring(&wBuffer[0], &wBuffer[0] + textSize / 2);

	return true;
}

void PMXLoader::Load()
{
	std::ifstream file(path, std::ios::binary);
	if (!file)
	{
		std::cerr << "Failed to open " << path << std::endl;
		// std::exit(1);
	}

	char signature[4];
	char pmxsign[4] = { 0x50,0x4d,0x58,0x20 };
	if (file.read(signature, 4))
	{
		for (int i = 0; i < 4; i++)
		{
			if (signature[i] != pmxsign[i])
			{
				std::cerr << "This file is not PMX." << std::endl;
				// std::exit(1);
			}
		}
	}

	float version;
	if (file.read(reinterpret_cast<char*>(&version), sizeof(float)))
	{
		if (version != 2.0f)
		{
			std::cout << "The version of this PMX file is " << std::fixed << std::setprecision(1) << version << std::endl;
			std::cerr << "This version is invalid." << std::endl;
			std::exit(1);
		}

		model->header.version = version;
	}

	// ヘッダーのバイト列の長さが1バイトで記述されている。Ver2.0のモデルは長さ8で固定
	file.seekg(1, std::ios::cur);

	char headerInfo[8];
	// ヘッダーのバイト列を読み込み
	/*
		0 : Text Encoding Type
		1 : Appendix UV
		2 : Vertex Index Size
		3 : Texture Index Size
		4 : Material Index Size
		5 : Bone Index Size
		6 : Morph Index Size
		7 : Rigid Body Index Size
	*/
	if (file.read(headerInfo, 8))
	{
		model->header.textEncodingType = static_cast<Header::TextEncoding>(headerInfo[0]);
		model->header.appendixUV = static_cast<int>(headerInfo[1]);
		model->header.vertexIndexSize = static_cast<size_t>(headerInfo[2]);
		model->header.textureIndexSize = static_cast<size_t>(headerInfo[3]);
		model->header.materialIndexSize = static_cast<size_t>(headerInfo[4]);
		model->header.boneIndexSize = static_cast<size_t>(headerInfo[5]);
		model->header.morphIndexSize = static_cast<size_t>(headerInfo[6]);
		model->header.rigidBodyIndexSize = static_cast<size_t>(headerInfo[7]);
	}

	model->header.Output();

	// コンソールにテキストを表示するためにロケールを設定
	setlocale(LC_ALL, "Japanese");

	std::wstring text{};

	getPMXStringUTF16(file, text);
	std::cout << std::endl;
	std::wcout << "Character Name : " << text << std::endl;

	getPMXStringUTF16(file, text);
	std::wcout << "English Character Name : " << text << std::endl;

	getPMXStringUTF16(file, text);
	std::cout << std::endl << "--------------- Comment ---------------" << std::endl;
	std::wcout << text << std::endl;
	std::cout << "---------------------------------------" << std::endl;

	getPMXStringUTF16(file, text);
	std::cout << "----------- English Comment -----------" << std::endl;
	std::wcout << text << std::endl;
	std::cout << "---------------------------------------" << std::endl << std::endl;

	// 頂点数の読み込み
	int vertexCount;
	if (file.read(reinterpret_cast<char*>(&vertexCount), sizeof(int)))
	{
		model->vertexCount = vertexCount;
		std::cout << "Vertex Count : " << vertexCount << std::endl;
	}

	
	for (int i = 0; i < vertexCount; i++)
	{
		VertexData vertexData;
		file.read(reinterpret_cast<char*>(vertexData.position), 3 * sizeof(float));
		file.read(reinterpret_cast<char*>(vertexData.normal), 3 * sizeof(float));
		file.read(reinterpret_cast<char*>(vertexData.uv), 2 * sizeof(float));
		/* std::cout << "Position : (" << vertexData.position[0] << ", " << vertexData.position[1] << ", " << vertexData.position[2] << ")";
		std::cout << ", Normal : (" << vertexData.normal[0] << ", " << vertexData.normal[1] << ", " << vertexData.normal[2] << ")";
		std::cout << ", UV : (" << vertexData.uv[0] << ", " << vertexData.uv[1] << ")" << std::endl; */

		Object::Vertex v = { vertexData.position[0],vertexData.position[1] ,vertexData.position[2] };
		model->vertices.push_back(v);

		// AppendixUVの読み込み, AppendixUVの数はヘッダーを参照
		for (int i = 0; i < model->header.appendixUV; i++)
		{
			std::array<float, 4> appendixUV;
			file.read(reinterpret_cast<char*>(appendixUV.data()), 4 * sizeof(float));
			vertexData.appendixUV.push_back(appendixUV);
		}

		char wiType;
		file.read(reinterpret_cast<char*>(&wiType), 1);
		int _witype = static_cast<int>(wiType);
		vertexData.weightType = static_cast<WeightType>(wiType);
		

		switch (vertexData.weightType)
		{
		case WeightType::BDEF1:
		{
			std::vector<char> bone(model->header.boneIndexSize);
			file.read(bone.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone[0]));
			break;
		}
		case WeightType::BDEF2:
		{
			std::vector<char> bone1(model->header.boneIndexSize);
			file.read(bone1.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone1[0]));

			std::vector<char> bone2(model->header.boneIndexSize);
			file.read(bone2.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone2[0]));

			float weight1;
			file.read(reinterpret_cast<char*>(&weight1), sizeof(float));
			vertexData.weight.weights.push_back(weight1);

			vertexData.weight.weights.push_back(1.0f - weight1);
			break;
		}
		case WeightType::BDEF4:
		{
			
			std::vector<char> bone1(model->header.boneIndexSize);
			file.read(bone1.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone1[0]));

			std::vector<char> bone2(model->header.boneIndexSize);
			file.read(bone2.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone2[0]));

			std::vector<char> bone3(model->header.boneIndexSize);
			file.read(bone3.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone3[0]));

			std::vector<char> bone4(model->header.boneIndexSize);
			file.read(bone4.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone4[0]));

			float weights[4];
			file.read(reinterpret_cast<char*>(weights), 4 * sizeof(float));
			for (float weight : weights)
			{
				vertexData.weight.weights.push_back(weight);
			}
			break;
		}
		case WeightType::SDEF:
		{
			std::vector<char> bone1(model->header.boneIndexSize);
			file.read(bone1.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone1[0]));

			std::vector<char> bone2(model->header.boneIndexSize);
			file.read(bone2.data(), model->header.boneIndexSize);
			vertexData.weight.bones.push_back(static_cast<int>(bone2[0]));

			float weight1;
			file.read(reinterpret_cast<char*>(&weight1), sizeof(float));
			vertexData.weight.weights.push_back(weight1);

			vertexData.weight.weights.push_back(1.0f - weight1);

			std::array<float, 3> sdef_c;
			file.read(reinterpret_cast<char*>(sdef_c.data()), 3 * sizeof(float));
			vertexData.weight.sdef.push_back(sdef_c);

			std::array<float, 3> sdef_r0;
			file.read(reinterpret_cast<char*>(sdef_r0.data()), 3 * sizeof(float));
			vertexData.weight.sdef.push_back(sdef_r0);

			std::array<float, 3> sdef_r1;
			file.read(reinterpret_cast<char*>(sdef_r1.data()), 3 * sizeof(float));
			vertexData.weight.sdef.push_back(sdef_r1);

			break;
		}
		default:
			std::cout << i << " : Weight Type : " << static_cast<int>(vertexData.weightType);
			std::cout << " This weight Type is invalid." << std::endl;
			break;
		}

		file.read(reinterpret_cast<char*>(&vertexData.edgeScale), sizeof(float));

		model->vertexData.push_back(vertexData);
	}

	// std::cout << "Vertex data was loaded. " << std::endl;

	int faceCount;
	if (file.read(reinterpret_cast<char*>(&faceCount), sizeof(int)))
	{
		model->faceCount = faceCount;
		std::cout << "Face Count : " << faceCount << std::endl;
	}
	// file.read(reinterpret_cast<char*>(&model->faceCount), sizeof(float));

	switch (model->header.vertexIndexSize)
	{
	case 1:
	{
		char vertexIndex;
		for (int i = 0; i < model->faceCount; i++)
		{
			file.read(&vertexIndex, sizeof(char));
			model->vertexIndex.push_back(static_cast<GLuint>(vertexIndex));
		}
		break;
	}
	case 2:
		short vertexIndex;
		{
			for (int i = 0; i < model->faceCount; i++)
			{
				file.read(reinterpret_cast<char*>(&vertexIndex), sizeof(short));
				model->vertexIndex.push_back(static_cast<GLuint>(vertexIndex));
			}
			break;
		}
	case 4:
	{
		int vertexIndex;
		for (int i = 0; i < model->faceCount; i++)
		{
			file.read(reinterpret_cast<char*>(&vertexIndex), sizeof(int));
			model->vertexIndex.push_back(static_cast<GLuint>(vertexIndex));
		}
		break;
	}
	default:
		std::cout << "This vertex index size is invalid." << std::endl;
		break;
	}

	// std::cout << "Vertex index was loaded." << std::endl;

	int textureCount;
	file.read(reinterpret_cast<char*>(&textureCount), sizeof(int));
	std::cout << "Texture Count : " << textureCount << std::endl;
	for (int i = 0; i < textureCount; i++)
	{
		getPMXStringUTF16(file, text);
		std::wcout << "Texture" << i << " : " << text << std::endl;
	}

	int materialCount;
	file.read(reinterpret_cast<char*>(&materialCount), sizeof(int));
	std::cout << "Material Count : " << materialCount << std::endl;

	
	
	
	for (int i = 0; i < materialCount; i++)
	{
		// マテリアル名(日)
		getPMXStringUTF16(file, text);
		std::wcout << "Material" << 1 << " : " << text << std::endl;

		// マテリアル名(英)
		getPMXStringUTF16(file, text);
		std::wcout << "English Material" << 1 << " : " << text << std::endl;

		float diffuseColor[4];
		file.read(reinterpret_cast<char*>(diffuseColor), 4 * sizeof(float));
		std::cout << "Diffuse : (" << diffuseColor[0] << ", " << diffuseColor[1] << ", " << diffuseColor[2] << ", " << diffuseColor[3] << ")" << std::endl;

		float specularColor[3];
		file.read(reinterpret_cast<char*>(specularColor), 3 * sizeof(float));
		std::cout << "Specular : (" << specularColor[0] << ", " << specularColor[1] << ", " << specularColor[2] << ")" << std::endl;

		float specularity;
		file.read(reinterpret_cast<char*>(&specularity), sizeof(float));
		std::cout << "Specularity : " << specularity << std::endl;

		float ambientColor[3];
		file.read(reinterpret_cast<char*>(ambientColor), 3 * sizeof(float));
		std::cout << "Ambient Color : (" << ambientColor[0] << ", " << ambientColor[1] << ", " << ambientColor[2] << ")" << std::endl;

		char drawingModeFlag;
		file.read(&drawingModeFlag, sizeof(char));
		std::bitset<8> flagbits(drawingModeFlag);
		std::cout << "Toon Flag : " << flagbits << std::endl;
		/*std::cout << "Drawing Mode Flag : " << static_cast<int>(drawingModeFlag);
		switch (drawingModeFlag)
		{
		case 0x00:
			std::cout << " : Off" << std::endl;
			break;
		case 0x01:
			std::cout << " : Double-Sided" << std::endl;
			break;
		case 0x02:
			std::cout << " : Shadow" << std::endl;
			break;
		case 0x04:
			std::cout << " : Self shadow map" << std::endl;
			break;
		case 0x08:
			std::cout << " : Self shadow" << std::endl;
			break;
		case 0x10:
			std::cout << " : Draw edges" << std::endl;
			break;
		default:
			std::cout << std::endl;
			break;
		}*/

		float edgeColor[3];
		file.read(reinterpret_cast<char*>(edgeColor), 3 * sizeof(float));
		std::cout << "Edge Color : (" << edgeColor[0] << ", " << edgeColor[1] << ", " << edgeColor[2] << ")" << std::endl;

		float edgeSize;
		file.read(reinterpret_cast<char*>(&edgeSize), sizeof(float));
		std::cout << "Edge Size : " << edgeSize << std::endl;

		// ????????
		// file.get();
		// file.get();
		// file.get();
		// file.get();
		switch (model->header.textureIndexSize)
		{
		case sizeof(char) :
		{

			char textureIndex[3];
			file.read(textureIndex, 3);
			// std::cout << "TextureIndex : " << static_cast<std::bitset<8>>(textureIndex[0]) << std::endl;
			// std::cout << "TextureIndex : " << static_cast<std::bitset<8>>(textureIndex[1]) << std::endl;
			// std::cout << "TextureIndex : " << static_cast<std::bitset<8>>(textureIndex[2]) << std::endl;

			file.read(textureIndex, 3);
			// std::cout << "TextureIndex : " << static_cast<std::bitset<8>>(textureIndex[0]) << std::endl;
			// std::cout << "TextureIndex : " << static_cast<std::bitset<8>>(textureIndex[1]) << std::endl;
			// std::cout << "TextureIndex : " << static_cast<std::bitset<8>>(textureIndex[2]) << std::endl;
			std::cout << "Texture Index : " << static_cast<int>(textureIndex[1]) << std::endl;
			std::cout << "Sphere Texture Index : " << static_cast<int>(textureIndex[2]) << std::endl;
			// std::cout << "TextureIndex : " << reinterpret_cast<int>(textureIndex) << std::endl;
			break;
		}
		case sizeof(short) :
		{
			short textureIndex;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(short));
			std::cout << "Texture Index : " << static_cast<int>(textureIndex) << std::endl;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(short));
			std::cout << "Texture Index : " << static_cast<int>(textureIndex) << std::endl;
			break;
		}
		case sizeof(int) :
		{
			int textureIndex;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(int));
			std::cout << "Texture Index : " << static_cast<int>(textureIndex) << std::endl;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(int));
			std::cout << "Texture Index : " << static_cast<int>(textureIndex) << std::endl;
			break;
		}
		default:
			break;
		}

		unsigned char sphereMode;
		file.read(reinterpret_cast<char*>(&sphereMode), sizeof(unsigned char));
		if (sphereMode == 255)
		{
			std::cout << "Error" << std::endl;
		}
		else
		{
			std::cout << "Sphere Mode : " << static_cast<int>(sphereMode) << std::endl;
		}

		char toonFlag;
		file.read(&toonFlag, sizeof(char));
		std::cout << "Toon Flag : " << static_cast<int>(toonFlag) << std::endl;

		switch (toonFlag)
		{
		case 0:
		{
			switch (model->header.textureIndexSize)
			{
			case sizeof(char) :
			{
				unsigned char toonIndex;
				file.read(reinterpret_cast<char*>(&toonIndex), sizeof(unsigned char));
				std::cout << "ToonIndex : " << static_cast<int>(toonIndex) << std::endl;
				break;
			}
			case sizeof(short) :
			{
				unsigned short toonIndex;
				file.read(reinterpret_cast<char*>(&toonIndex), sizeof(unsigned short));
				std::cout << "ToonIndex : " << toonIndex << std::endl;
				break;
			}
			case sizeof(int) :
			{
				unsigned int toonIndex;
				file.read(reinterpret_cast<char*>(&toonIndex), sizeof(unsigned int));
				std::cout << "ToonIndex : " << toonIndex << std::endl;
				break;
			}
			default:
				std::cerr << "error" << std::endl;
				break;
			}
			break;
		}
		case 1:
		{
			unsigned char toonIndex;
			file.read(reinterpret_cast<char*>(&toonIndex), sizeof(unsigned char));
			std::cout << "ToonIndex : " << static_cast<int>(toonIndex) << std::endl;
			break;
		}
		default:
			std::cout << "This toonFlag is invalid" << std::endl;
			break;
		}


		getPMXStringUTF16(file, text);
		std::wcout << "Memo : " << text << std::endl;

		int fromFaceNumber;
		file.read(reinterpret_cast<char*>(&fromFaceNumber), sizeof(int));
		std::cout << "FromFaceNumber : " << fromFaceNumber << std::endl;

		// if (i > 0) break;
	}
}