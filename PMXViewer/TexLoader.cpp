#include "TexLoader.h"
#include "Model.h"

TexLoader::TexLoader(Model* model) : model(model)
{
	
	std::wstring path = L"../Models/Tda�������~�N�E�A�y���hVer1.10/Tda�������~�N�E�A�y���hVer1.10/toon_skin.bmp";
	FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeU(path.data());
	FIBITMAP* img = FreeImage_LoadU(format, path.data());
	if (img == nullptr)
	{
		std::cerr << "Load Error" << std::endl;
	}
	else
	{
		FIBITMAP* img32 = FreeImage_ConvertTo32Bits(img);
		std::cout << "Load Success" << std::endl;
		std::cout << "width : " << FreeImage_GetWidth(img) << ", height : " << FreeImage_GetHeight(img) << std::endl;
		std::cout << "BPP : " << FreeImage_GetBPP(img) << std::endl;
		std::cout << "filetype : " << FIF_TARGA << ", " << FreeImage_GetFileTypeU(path.data()) << std::endl;
		std::cout << "imageType : " << FreeImage_GetImageType(img) << std::endl;
		BYTE* bits = FreeImage_GetBits(img32);
		std::cout << "Bits: ";
		int length = FreeImage_GetWidth(img32) * FreeImage_GetHeight(img32) * FreeImage_GetBPP(img32);
		for (size_t i = 0; i < length/2; i++) {
			// std::cout << static_cast<int>(bits[i]) << " ";
		}
		std::cout << std::endl;
	}
	
}

void TexLoader::Load()
{
	model->textureData.resize(model->texCount);
	for (int i = 0; i < model->texCount; i++)
	{
		std::wstring path = model->texPath[i];

		FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeU(path.c_str(),0);
		FIBITMAP* img = FreeImage_LoadU(format, path.c_str());

		if (img != nullptr)
		{
			FIBITMAP* img32 = FreeImage_ConvertTo32Bits(img);
			model->textureData[i] = img32;
			std::cout << "width : " << FreeImage_GetWidth(img32) << ", height : " << FreeImage_GetHeight(img32) << std::endl;
			FreeImage_Unload(img);
		}
		else
		{
			std::wcerr << "Failed to load texture: " << path << std::endl;
		}
	}
}
/*
TexLoader::Texture TexLoader::LoadTGA(std::wstring path)
{
	Texture tex;

	tex.format = GL_BGR;
	tex.type = GL_UNSIGNED_BYTE;

	char header[18];
	file.read(header, 18);
	tex.width = header[12] + (header[13] << 8);
	tex.height = header[14] + (header[15] << 8);
	int bitsPerPixel = header[16];

	// �s�N�Z���f�[�^�̃o�b�t�@���쐬
	int dataSize = tex.width * tex.height * (bitsPerPixel / 8);

	tex.data.resize(dataSize);

	file.read(tex.data.data(), dataSize);

	return tex;
}

TexLoader::Texture TexLoader::LoadBMP(std::wstring )
{
	Texture tex;

	tex.format = GL_BGR;
	tex.type = GL_UNSIGNED_BYTE;

	char header[54];
	file.read(header, 54);
	tex.width = *(int*)&header[18];
	tex.height = *(int*)&header[22];
	int bitsPerPixel = *(short*)&header[28];

	// �s�N�Z���f�[�^�̃o�b�t�@���쐬
	int dataSize = tex.width * tex.height * (bitsPerPixel / 8);

	tex.data.resize(dataSize);

	file.read(tex.data.data(), dataSize);

	return tex;
}
*/