#include "Model.h"
#include "PMXLoader.h"

PMXLoader::PMXLoader(const std::string& path, Model* model) :
	path(path), model(model)
{

}

std::wstring PMXLoader::decodeUTF16LE(const std::vector<char>& binaryData) {
	std::wstring decodedString;
	for (size_t i = 0; i < binaryData.size(); i += 2) {
		unsigned char lowByte = binaryData[i + 1];  // �o�C�g�̏������t�ɉ���
		unsigned char highByte = binaryData[i];     // �o�C�g�̏������t�ɉ���
		wchar_t utf16Char = (highByte << 8) | lowByte;
		decodedString += utf16Char;
	}
	return decodedString;
}

void PMXLoader::writeStringToFile(const std::wstring& str, const std::string& filename) {
	std::ofstream outputFile(filename, std::ios::binary);
	if (outputFile) {
		// BOM��t��
		unsigned char bom[2] = { 0xFF, 0xFE };
		outputFile.write(reinterpret_cast<const char*>(bom), sizeof(unsigned char) * 2);
		// UTF-16LE��������o�C�i���`���ŏ�������
		outputFile.write(reinterpret_cast<const char*>(str.data()), sizeof(wchar_t) * str.size());
		outputFile.close();
	}
	else
	{
		std::cerr << "error" << std::endl;
	}
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

	// �w�b�_�[�̃o�C�g��̒�����1�o�C�g�ŋL�q����Ă���BVer2.0�̃��f���͒���8�ŌŒ�
	file.seekg(1, std::ios::cur);

	char headerInfo[8];
	// �w�b�_�[�̃o�C�g���ǂݍ���
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

	// �L�����N�^�[�l�[��(��)�̃T�C�Y��ǂݍ���
	if (file.read(textSizeBuf, 4))
	{
		textSize = *reinterpret_cast<int*>(textSizeBuf);
	}

	// �o�C�i���f�[�^�̗�Ƃ��āA������ "Hello, ���E!" ��UTF-16LE�G���R�[�h���g�p���܂�
	std::vector<char> binaryData = { 0x48, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x6F, 0x00, 0x2C, 0x00, 0x20, 0x00,
		0x30, 0x4C, 0x4E, 0x00, 0x21, 0x00 };

	// UTF-16LE�`���̃o�C�i���f�[�^���f�R�[�h���ăt�@�C���ɏo�͂���
	std::wstring decodedString = decodeUTF16LE(binaryData);
	// writeStringToFile(decodedString, "..\\Output\\output.txt");

	std::vector<char> name(textSize);
	file.read(&name[0], textSize);
	writeStringToFile(decodeUTF16LE(name), "..\\Output\\output.txt");

	// �e�L�X�g�̃G���R�[�h���ʓ|�Ȃ̂ŃT�C�Y����ǂݔ�΂�
	// file.seekg(textSize, std::ios::cur);

	// �L�����N�^�[�l�[��(�p)�̃T�C�Y��ǂݍ���
	if (file.read(textSizeBuf, 4))
	{
		textSize = *reinterpret_cast<int*>(textSizeBuf);
	}
	
	// �e�L�X�g�T�C�Y���ǂݔ�΂�
	file.seekg(textSize, std::ios::cur);

	// �R�����g(��)�̃T�C�Y��ǂݍ���
	if (file.read(textSizeBuf, 4))
	{
		textSize = *reinterpret_cast<int*>(textSizeBuf);
	}

	// �e�L�X�g�T�C�Y���ǂݔ�΂�
	file.seekg(textSize, std::ios::cur);

	// �R�����g(�p)�̃T�C�Y��ǂݍ���
	if (file.read(textSizeBuf, 4))
	{
		textSize = *reinterpret_cast<int*>(textSizeBuf);
	}

	// �e�L�X�g�T�C�Y���ǂݔ�΂�
	file.seekg(textSize, std::ios::cur);

	// ���_���̓ǂݍ���
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

		// AppendixUV�̓ǂݍ���, AppendixUV�̐��̓w�b�_�[���Q��
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

	
	file.read(textSizeBuf, 4);
	int textureCount = *reinterpret_cast<int*>(textSizeBuf);
	std::cout << "Texture Count : " << textureCount << std::endl;
	for (int i = 0; i < textureCount; i++)
	{
		file.read(textSizeBuf, 4);
		textSize = *reinterpret_cast<int*>(textSizeBuf);
		// std::cout << "Text Size : " << textSize << std::endl;
		file.seekg(textSize, std::ios::cur);
	}


	file.read(textSizeBuf, 4);
	int materialCount = *reinterpret_cast<int*>(textSizeBuf);
	std::cout << "Material Count : " << materialCount << std::endl;

	/*for (int i = 0; i < materialCount; i++)
	{
		// �}�e���A����(��)
		file.read(textSizeBuf, sizeof(int));
		textSize = *reinterpret_cast<int*>(textSizeBuf);
		std::cout << "Text Size : " << textSize << std::endl;
		file.seekg(textSize, std::ios::cur);

		// �}�e���A����(�p)
		file.read(textSizeBuf, sizeof(int));
		textSize = *reinterpret_cast<int*>(textSizeBuf);
		std::cout << "Text Size : " << textSize << std::endl;
		file.seekg(textSize, std::ios::cur);

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
		std::cout << "Drawing Mode Flag : " << static_cast<int>(drawingModeFlag);
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
		}

		float edgeColor[3];
		file.read(reinterpret_cast<char*>(edgeColor), 3 * sizeof(float));
		std::cout << "Edge Color : (" << edgeColor[0] << ", " << edgeColor[1] << ", " << edgeColor[2] << ")" << std::endl;

		float edgeSize;
		file.read(reinterpret_cast<char*>(&edgeSize), sizeof(float));
		std::cout << "Edge Size : " << edgeSize << std::endl;

		switch (model->header.textureIndexSize)
		{
		case sizeof(char) :
		{
			char textureIndex;
			file.read(&textureIndex, sizeof(char));
			std::cout << "Texture Index : " << static_cast<GLuint>(textureIndex) << std::endl;
			break;
		}
		case sizeof(short) :
		{
			short textureIndex;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(short));
			std::cout << "Texture Index : " << static_cast<GLuint>(textureIndex) << std::endl;
			break;
		}
		case sizeof(int) :
		{
			int textureIndex;
			file.read(reinterpret_cast<char*>(&textureIndex), sizeof(int));
			std::cout << "Texture Index : " << static_cast<GLuint>(textureIndex) << std::endl;
			break;
		}
		default:
			break;
		}
	}*/
	
}