#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <array>
#include <string>

class Model;

class PMXLoader
{
	std::string path;
	Model* model;

public:
	PMXLoader(const std::string& path, Model* model);

	void Load();
	std::wstring decodeUTF16LE(const std::vector<char>& binaryData);
	void writeStringToFile(const std::wstring& str, const std::string& filename);
};


