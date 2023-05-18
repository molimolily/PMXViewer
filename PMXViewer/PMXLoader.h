#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <array>
#include <string>
#include <cstring>
#include <bitset>

class Model;

class PMXLoader
{
	std::string path;
	Model* model;

public:
	PMXLoader(const std::string& path, Model* model);

	void Load();
	bool getPMXStringUTF16(std::ifstream& _file, std::wstring& output);
};


