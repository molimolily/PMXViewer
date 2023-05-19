#include "Object.h"
#include "Model.h"

// コンストラクタ
Object::Object(GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount = 0, const GLuint *index = NULL)
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
	glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

	// 頂点バッファオブジェクトをattribute変数に関連付ける
	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute変数の有効化
	glEnableVertexAttribArray(0);

	// インデックスの頂点バッファオブジェクト
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
}

Object::Object(Model model)
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
	glBufferData(GL_ARRAY_BUFFER, model.vertexCount * sizeof(VertexData), model.vertexData.data(), GL_STATIC_DRAW);

	// 頂点属性のインデックスを定義します
	const GLuint POSITION_INDEX = 0;
	const GLuint NORMAL_INDEX = 1;
	const GLuint UV_INDEX = 2;
	const GLuint EDGE_SCALE_INDEX = 3;
	// 頂点バッファオブジェクトをattrib変数に関連付ける
	glVertexAttribPointer(POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, position)));
	glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, normal)));
	glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, uv)));
	glVertexAttribPointer(EDGE_SCALE_INDEX, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, edgeScale)));

	// attribute変数の有効化
	glEnableVertexAttribArray(0);

	// インデックスの頂点バッファオブジェクト
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.faceCount * sizeof(GLuint), model.vertexIndex.data(), GL_STATIC_DRAW);
}

// デストラクタ
Object::~Object()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

// 頂点配列オブジェクトの結合
// 描画前に描画する図形データの保持した頂点配列オブジェクトをバインドする
void Object::bind() const
{
	glBindVertexArray(vao);
}