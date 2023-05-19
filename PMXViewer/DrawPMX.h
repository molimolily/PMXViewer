#pragma once
#include <vector>
#include<GL/glew.h>
#include "Model.h"
#include "Transform.h"
// class Model;

class DrawPMX
{
	// 頂点属性のインデックスを定義
	const GLuint POSITION_INDEX = 0;
	const GLuint NORMAL_INDEX = 1;
	const GLuint UV_INDEX = 2;
	const GLuint EDGE_SCALE_INDEX = 3;

	// マテリアルの数
	int materialCount;

	// 頂点バッファオブジェクト名
	GLuint vbo;

	// 頂点配列オブジェクト名
	std::vector<GLuint> vao;

	// インデックスの頂点バッファオブジェクト
	std::vector<GLuint> ibo;

	// ユニフォームバッファオブジェクト
	// GLuint ubo;

	void setAndEnableVertexAttrib();
public:
	Model model;
	Transform transform;
	DrawPMX(Model model);

	~DrawPMX();

	void bind(int n) const;
	void draw(int n);
	void execute();

	int getMaterialCount();
};