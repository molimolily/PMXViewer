#pragma once
#include <memory>
#include "Object.h"
#include "Transform.h"

class Shape
{
	// shared_ptrを使うことで同一のインスタンスを指すShapeクラスのインスタンスが全て削除されたとき, ポインタが削除される
	// インスタンスのコピーが可能になる
	std::shared_ptr<const Object> object;

protected:
	// 描画に使う頂点の数
	const GLsizei vertexcount;

public:
	// モデルのTransform
	Transform transform;

	// コンストラクタ
	Shape(GLint size,GLsizei vertexcount,const Object::Vertex* vertex, GLsizei indexcount = 0, const GLuint *index = NULL)
		:object(new Object(size,vertexcount,vertex,indexcount,index)),vertexcount(vertexcount)
	{

	}

	Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex, Transform transform, GLsizei indexcount = 0, const GLuint *index = NULL)
		:object(new Object(size, vertexcount, vertex, indexcount, index)), vertexcount(vertexcount)
	{
		
	}

	// 描画
	void draw() const
	{
		// 頂点配列オブジェクトのバインド
		object->bind();

		// 描画を実行
		execute();
	}

	virtual void execute() const
	{
		// バインドされた頂点配列オブジェクトを用いて描画
		// glDrawArrays(GL_TRIANGLES, 0, vertexcount);
		// glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
		glDrawArrays(GL_POINTS, 0, vertexcount);
	}
};