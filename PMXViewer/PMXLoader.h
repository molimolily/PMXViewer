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
};


