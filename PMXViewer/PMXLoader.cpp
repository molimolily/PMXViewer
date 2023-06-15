#include "Model.h"
#include "PMXLoader.h"

PMXLoader::PMXLoader(const std::wstring& path, Model* model) :
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
		std::wcerr << "Failed to open " << path << std::endl;
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

	// model->header.Output();

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
		float vertexPosition[3];
		file.read(reinterpret_cast<char*>(vertexPosition), 3 * sizeof(float));
		vertexPosition[0] *= -1;
		vertexData.position[0] = vertexPosition[0];
		vertexData.position[1] = vertexPosition[1];
		vertexData.position[2] = vertexPosition[2];
		// file.read(reinterpret_cast<char*>(vertexData.position), 3 * sizeof(float));
		file.read(reinterpret_cast<char*>(vertexData.normal), 3 * sizeof(float));

		// uv座標を右手系に修正する
		float uv[2];
		file.read(reinterpret_cast<char*>(uv), 2 * sizeof(float));
		// uv[0]*= 1.0f - uv[0];
		// uv[1] = 1.0f - uv[1];
		vertexData.uv[0] = uv[0];
		vertexData.uv[1] = uv[1];
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
			// vertexData.appendixUV.push_back(appendixUV);
		}

		char wiType;
		file.read(reinterpret_cast<char*>(&wiType), 1);
		int _witype = static_cast<int>(wiType);
		// vertexData.weightType = static_cast<WeightType>(wiType);
		

		switch (static_cast<WeightType>(_witype))
		{
		case WeightType::BDEF1:
		{
			std::vector<char> bone(model->header.boneIndexSize);
			file.read(bone.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone[0]));
			break;
		}
		case WeightType::BDEF2:
		{
			std::vector<char> bone1(model->header.boneIndexSize);
			file.read(bone1.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone1[0]));

			std::vector<char> bone2(model->header.boneIndexSize);
			file.read(bone2.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone2[0]));

			float weight1;
			file.read(reinterpret_cast<char*>(&weight1), sizeof(float));
			// vertexData.weight.weights.push_back(weight1);

			// vertexData.weight.weights.push_back(1.0f - weight1);
			break;
		}
		case WeightType::BDEF4:
		{
			
			std::vector<char> bone1(model->header.boneIndexSize);
			file.read(bone1.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone1[0]));

			std::vector<char> bone2(model->header.boneIndexSize);
			file.read(bone2.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone2[0]));

			std::vector<char> bone3(model->header.boneIndexSize);
			file.read(bone3.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone3[0]));

			std::vector<char> bone4(model->header.boneIndexSize);
			file.read(bone4.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone4[0]));

			float weights[4];
			file.read(reinterpret_cast<char*>(weights), 4 * sizeof(float));
			for (float weight : weights)
			{
				// vertexData.weight.weights.push_back(weight);
			}
			break;
		}
		case WeightType::SDEF:
		{
			std::vector<char> bone1(model->header.boneIndexSize);
			file.read(bone1.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone1[0]));

			std::vector<char> bone2(model->header.boneIndexSize);
			file.read(bone2.data(), model->header.boneIndexSize);
			// vertexData.weight.bones.push_back(static_cast<int>(bone2[0]));

			float weight1;
			file.read(reinterpret_cast<char*>(&weight1), sizeof(float));
			// vertexData.weight.weights.push_back(weight1);

			// vertexData.weight.weights.push_back(1.0f - weight1);

			std::array<float, 3> sdef_c;
			file.read(reinterpret_cast<char*>(sdef_c.data()), 3 * sizeof(float));
			// vertexData.weight.sdef.push_back(sdef_c);

			std::array<float, 3> sdef_r0;
			file.read(reinterpret_cast<char*>(sdef_r0.data()), 3 * sizeof(float));
			// vertexData.weight.sdef.push_back(sdef_r0);

			std::array<float, 3> sdef_r1;
			file.read(reinterpret_cast<char*>(sdef_r1.data()), 3 * sizeof(float));
			// vertexData.weight.sdef.push_back(sdef_r1);

			break;
		}
		default:
			// std::cout << i << " : Weight Type : " << static_cast<int>(vertexData.weightType);
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

	// int textureCount;
	file.read(reinterpret_cast<char*>(&model->texCount), sizeof(int));
	// std::cout << "Texture Count : " << textureCount << std::endl;
	for (int i = 0; i < model->texCount; i++)
	{
		getPMXStringUTF16(file, text);
		std::wstring dirPath = path;
		dirPath.erase(path.rfind(L"/")+1);

		text = dirPath + text;
		
		model->texPath.push_back(text);
		// std::wcout << "Texture" << i << " : " << text << std::endl;
	}

	file.read(reinterpret_cast<char*>(&model->materialCount), sizeof(int));
	// std::cout << "Material Count : " << model->materialCount << std::endl;
	
	for (int i = 0; i < model->materialCount; i++)
	{
		MaterialData materialData;

		// マテリアル名(日)
		getPMXStringUTF16(file, text);
		std::wcout << "Material" << i << " : " << text << std::endl;

		// マテリアル名(英)
		getPMXStringUTF16(file, text);
		// std::wcout << "English Material" << 1 << " : " << text << std::endl;

		file.read(reinterpret_cast<char*>(materialData.diffuseColor), 4 * sizeof(float));
		// std::cout << "Diffuse : (" << materialData.diffuseColor[0] << ", " << materialData.diffuseColor[1] << ", " << materialData.diffuseColor[2] << ", " << materialData.diffuseColor[3] << ")" << std::endl;

		file.read(reinterpret_cast<char*>(materialData.specularColor), 3 * sizeof(float));
		// std::cout << "Specular : (" << materialData.specularColor[0] << ", " << materialData.specularColor[1] << ", " << materialData.specularColor[2] << ")" << std::endl;

		file.read(reinterpret_cast<char*>(&materialData.specularity), sizeof(float));
		// std::cout << "Specularity : " << materialData.specularity << std::endl;

		file.read(reinterpret_cast<char*>(materialData.ambientColor), 3 * sizeof(float));
		// std::cout << "Ambient Color : (" << materialData.ambientColor[0] << ", " << materialData.ambientColor[1] << ", " << materialData.ambientColor[2] << ")" << std::endl;

		file.read(&materialData.drawingModeflags, sizeof(char));
		std::bitset<8> flagbits(materialData.drawingModeflags);
		// std::cout << "Toon Flag       : " << flagbits << std::endl;
		// std::cout << "Double-Sided    : " << materialData.DrawingModeFlag(MaterialData::DrawingMode::DoubleSided) << std::endl;
		// std::cout << "Shadow          : " << materialData.DrawingModeFlag(MaterialData::DrawingMode::Shadow) << std::endl;
		// std::cout << "Self Shadow Map : " << materialData.DrawingModeFlag(MaterialData::DrawingMode::SelfShadowMap) << std::endl;
		// std::cout << "Self Shadow     : " << materialData.DrawingModeFlag(MaterialData::DrawingMode::SelfShadow) << std::endl;
		// std::cout << "Draw Edges      : " << materialData.DrawingModeFlag(MaterialData::DrawingMode::DrawEdges) << std::endl;

		file.read(reinterpret_cast<char*>(materialData.edgeColor), 4 * sizeof(float));
		// std::cout << "Edge Color : (" << materialData.edgeColor[0] << ", " << materialData.edgeColor[1] << ", " << materialData.edgeColor[2] << ", " << materialData.edgeColor[3] << ")" << std::endl;

		file.read(reinterpret_cast<char*>(&materialData.edgeSize), sizeof(float));
		// std::cout << "Edge Size : " << materialData.edgeSize << std::endl;

		switch (model->header.textureIndexSize)
		{
		case sizeof(char) :
		{
			char buf;
			file.read(&buf, sizeof(char));
			materialData.textureIndex = static_cast<int>(buf);
			file.read(&buf, sizeof(char));
			materialData.sphereTextureIndex = static_cast<int>(buf);
			if (materialData.textureIndex != -1)
			{
				// std::wcout << "Texture Index : " << materialData.textureIndex << ", Texture : " << model->texPath[materialData.textureIndex] << std::endl;
			}
			else
			{
				std::wcout << "Texture Index : " << materialData.textureIndex << std::endl;
			}
			// std::cout << "Sphere Texture Index : " << materialData.sphereTextureIndex << std::endl;
			
			break;
		}
		case sizeof(short) :
		{
			short textureIndex;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(short));
			// std::cout << "Texture Index : " << static_cast<int>(textureIndex) << std::endl;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(short));
			// std::cout << "Texture Index : " << static_cast<int>(textureIndex) << std::endl;
			break;
		}
		case sizeof(int) :
		{
			int textureIndex;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(int));
			// std::cout << "Texture Index : " << static_cast<int>(textureIndex) << std::endl;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(int));
			// std::cout << "Texture Index : " << static_cast<int>(textureIndex) << std::endl;
			break;
		}
		default:
			break;
		}

		char sphereMode;
		file.read(&sphereMode, sizeof(char));
		materialData.sphereMode = static_cast<int>(sphereMode);
		// std::cout << "Sphere Mode : " << materialData.sphereMode << std::endl;

		char toonFlag;
		file.read(&toonFlag, sizeof(char));
		materialData.sharedToonFlag = static_cast<int>(toonFlag);
		// std::cout << "Shared Toon Flag : " << materialData.sharedToonFlag << std::endl;

		if (materialData.sharedToonFlag)
		{
			char toonIndex;
			file.read(reinterpret_cast<char*>(&toonIndex), sizeof(char));
			materialData.toonTextureIndex = static_cast<int>(toonIndex);
			// std::cout << "Shared Toon Index : " << materialData.toonTextureIndex << std::endl;
		}
		else
		{
			switch (model->header.textureIndexSize)
			{
			case sizeof(char) :
			{
				char toonIndex;
				file.read(reinterpret_cast<char*>(&toonIndex), sizeof(char));
				materialData.toonTextureIndex = static_cast<int>(toonIndex);
				// std::cout << "Toon Index : " << materialData.toonTextureIndex << std::endl;
				break;
			}
			case sizeof(short) :
			{
				short toonIndex;
				file.read(reinterpret_cast<char*>(&toonIndex), sizeof(short));
				materialData.toonTextureIndex = static_cast<int>(toonIndex);
				// std::cout << "Toon Index : " << materialData.toonTextureIndex << std::endl;
				break;
			}
			case sizeof(int) :
			{
				int toonIndex;
				file.read(reinterpret_cast<char*>(&toonIndex), sizeof(int));
				materialData.toonTextureIndex = static_cast<int>(toonIndex);
				// std::cout << "Toon Index : " << materialData.toonTextureIndex << std::endl;
				break;
			}
			default:
				std::cerr << "error" << std::endl;
				break;
			}
		}


		getPMXStringUTF16(file, text);
		// std::wcout << "Memo : " << text << std::endl;

		file.read(reinterpret_cast<char*>(&materialData.surfaceCount), sizeof(int));
		// std::cout << "Surface Count : " << materialData.surfaceCount << std::endl;

		model->materialData.push_back(materialData);
	}
}