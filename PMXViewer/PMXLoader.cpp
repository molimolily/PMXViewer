#include "Model.h"
#include "PMXLoader.h"

PMXLoader::PMXLoader(const std::string& path, Model* model) :
	path(path), model(model)
{

}

void PMXLoader::Load()
{
	std::ifstream file(path, std::ios::binary);
	if (!file)
	{
		std::cerr << "Failed to open " << path << std::endl;
		std::exit(1);
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
				std::exit(1);
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

	char textSizeBuf[4];
	int textSize;

	// キャラクターネーム(日)のサイズを読み込み
	if (file.read(textSizeBuf, 4))
	{
		textSize = *reinterpret_cast<int*>(textSizeBuf);
	}

	// テキストのエンコードが面倒なのでサイズ分を読み飛ばす
	file.seekg(textSize, std::ios::cur);

	// キャラクターネーム(英)のサイズを読み込み
	if (file.read(textSizeBuf, 4))
	{
		textSize = *reinterpret_cast<int*>(textSizeBuf);
	}
	
	// テキストサイズ分読み飛ばす
	file.seekg(textSize, std::ios::cur);

	// コメント(日)のサイズを読み込み
	if (file.read(textSizeBuf, 4))
	{
		textSize = *reinterpret_cast<int*>(textSizeBuf);
	}

	// テキストサイズ分読み飛ばす
	file.seekg(textSize, std::ios::cur);

	// コメント(英)のサイズを読み込み
	if (file.read(textSizeBuf, 4))
	{
		textSize = *reinterpret_cast<int*>(textSizeBuf);
	}

	// テキストサイズ分読み飛ばす
	file.seekg(textSize, std::ios::cur);

	// 頂点数の読み込み
	int vertexCount;
	if (file.read(reinterpret_cast<char*>(&vertexCount), sizeof(int)))
	{
		model->vertexCount = vertexCount;
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

	std::cout << "Vertex data was loaded. " << std::endl;

	int faceCount;
	if (file.read(reinterpret_cast<char*>(&faceCount), sizeof(int)))
	{
		model->faceCount = faceCount;
	}
	// file.read(reinterpret_cast<char*>(&model->faceCount), sizeof(float));

	std::cout << "Face count : " << model->faceCount << std::endl;

	for (int i = 0; i < model->faceCount; i++)
	{
		std::vector<char> vi(model->header.vertexIndexSize);
		file.read(vi.data(), model->header.vertexIndexSize);
		model->vertexIndex.push_back(static_cast<GLuint>(vi[0]));

		// std::cout << "Vertex Index : " << static_cast<int>(vi[0]) << std::endl;
	}

	std::cout << "Vertex index was loaded." << std::endl;

	
	file.read(textSizeBuf, 4);
	int textureCount = *reinterpret_cast<int*>(textSizeBuf);
	std::cout << "Texture Count : " << textureCount << std::endl;
	for (int i = 0; i < textureCount; i++)
	{
		file.read(textSizeBuf, 4);
		textSize = *reinterpret_cast<int*>(textSizeBuf);
		std::cout << "Text Size : " << textSize << std::endl;
		file.seekg(textSize, std::ios::cur);
	}


	file.read(textSizeBuf, 4);
	int materialCount = *reinterpret_cast<int*>(textSizeBuf);
	std::cout << "Material Count : " << materialCount << std::endl;
}