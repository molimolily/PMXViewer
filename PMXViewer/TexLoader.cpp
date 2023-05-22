#include "TexLoader.h"
#include "Model.h"

TexLoader::TexLoader(Model* model) : model(model)
{

}

void TexLoader::Load()
{
	for (int i = 0; i < model->texCount; i++)
	{
		std::wstring path = model->texPath[i];

		std::ifstream file(path, std::ios::binary);

		if (!file)
		{
			std::wcerr << "Failed to open " << path << std::endl;
		}

		std::wstring extension = path.substr(path.rfind(L".") + 1);
		if (extension == L"tga")
		{
			// std::cout << "tga" << std::endl;
		}
		else if (extension == L"bmp")
		{
			// std::cout << LoadBMP(file) << std::endl;;
		}

		file.close();
	}
}

char* TexLoader::LoadTGA(std::ifstream& file)
{
	char header[18];
	file.read(header, 18);
	int width = header[12] + (header[13] << 8);
	int height = header[14] + (header[15] << 8);
	int bitsPerPixel = header[16];

	// ピクセルデータのバッファを作成
	int dataSize = width * height * (bitsPerPixel / 8);
	char* pixelData = new char[dataSize];

	file.read(pixelData, dataSize);

	// std::cout << "w : " << width << ", h : " << height << ", bitsPerPixel : " << bitsPerPixel << std::endl;

	return pixelData;
}

char* TexLoader::LoadBMP(std::ifstream& file)
{
	char header[54];
	file.read(header, 54);
	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitsPerPixel = *(short*)&header[28];

	// ピクセルデータのバッファを作成
	int dataSize = width * height * (bitsPerPixel / 8);
	char* pixelData = new char[dataSize];

	file.read(pixelData, dataSize);

	return pixelData;
}