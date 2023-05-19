#pragma once
#include<iostream>
#include<GL/glew.h>
// #include "Object.h"

class VertexData;
class Model;

class Object
{
	// 頂点配列オブジェクト名
	GLuint vao;

	// 頂点バッファオブジェクト名
	GLuint vbo;

	// インデックスの頂点バッファオブジェクト
	GLuint ibo;

public:
	// 頂点属性
	struct Vertex
	{
		GLfloat position[3];
	};
	

	// コンストラクタ
	Object(GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount, const GLuint *index);

	Object(Model model);

	// デストラクタ
	~Object();

	// 頂点配列オブジェクトの結合
	// 描画前に描画する図形データの保持した頂点配列オブジェクトをバインドする
	void bind() const;

private:
	// インスタンスのコピーが複数作成されることを防ぐ
	// コピーコンストラクタによる代入禁止
	Object(const Object &o);
	// 代入によるコピー禁止
	Object &operator = (const Object &o);
};