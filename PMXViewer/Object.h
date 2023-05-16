#pragma once
#include<GL/glew.h>

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
	Object(GLint size, GLsizei vertexcount, const Vertex *vertex,GLsizei indexcount = 0, const GLuint *index = NULL)
	{
		// 頂点配列オブジェクトの作成
		glGenVertexArrays(1, &vao);
		// 頂点配列オブジェクトのバインド
		glBindVertexArray(vao);

		// 頂点バッファオブジェクトの作成
		glGenBuffers(1, &vbo);
		// 頂点バッファオブジェクトのバインド
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// 頂点バッファオブジェクトにデータを転送
		glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex ,GL_STATIC_DRAW);

		// 頂点バッファオブジェクトをattribute変数に関連付ける
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
		// attribute変数の有効化
		glEnableVertexAttribArray(0);

		// インデックスの頂点バッファオブジェクト
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}

	// デストラクタ
	virtual ~Object()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}

	// 頂点配列オブジェクトの結合
	// 描画前に描画する図形データの保持した頂点配列オブジェクトをバインドする
	void bind() const
	{
		glBindVertexArray(vao);
	}

private:
	// インスタンスのコピーが複数作成されることを防ぐ
	// コピーコンストラクタによる代入禁止
	Object(const Object &o);
	// 代入によるコピー禁止
	Object &operator = (const Object &o);
};