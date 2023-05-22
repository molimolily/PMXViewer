#pragma once
#include <fstream>
#include <string>

class Model;

class TexLoader
{
	Model *model;
public:
	TexLoader(Model *model);
	void Load();
	char* LoadTGA(std::ifstream& file);
	char* LoadBMP(std::ifstream& file);
};