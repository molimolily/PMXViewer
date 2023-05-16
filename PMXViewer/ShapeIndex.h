#pragma once
#include "Shape.h"

// インデックスを使った図形の描画
class ShapeIndex : public Shape
{
protected:
	// 描画に使う頂点数
	const GLsizei indexcount;

public:
	// コンストラクタ
	// size: 頂点の位置の次元
	// vertexcount: 頂点の数
	// vertex: 頂点属性を格納した配列
	// indexcount: 頂点のインデックスの要素数
	// index: 頂点のインデックスを格納した配列
	ShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index)
		: Shape(size, vertexcount, vertex, indexcount, index), indexcount(indexcount)
	{

	}

	// 描画の実行
	virtual void execute() const
	{
		glDrawElements(GL_LINES, indexcount, GL_UNSIGNED_INT, 0);
	}
};

class SolidShapeIndex : public ShapeIndex
{
public:

	SolidShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index)
		: ShapeIndex(size, vertexcount, vertex, indexcount, index)
	{

	}

	// 描画の実行
	virtual void execute() const
	{
		glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
	}
};