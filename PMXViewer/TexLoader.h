#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <FreeImage.h>

class Model;

class TexLoader
{
	Model *model;
public:
	struct Texture
	{
		GLsizei width;
		GLsizei height;
		GLenum format;
		GLenum type;

		// FIBITMAP *bitmap;
	};
	TexLoader(Model *model);
	void Load();
	// Texture LoadTGA(std::wstring path);
	// Texture LoadBMP(std::wstring path);
};