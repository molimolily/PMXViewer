#pragma once
#include <memory>
#include "Object.h"
#include "Transform.h"

class Shape
{
	// shared_ptr���g�����Ƃœ���̃C���X�^���X���w��Shape�N���X�̃C���X�^���X���S�č폜���ꂽ�Ƃ�, �|�C���^���폜�����
	// �C���X�^���X�̃R�s�[���\�ɂȂ�
	std::shared_ptr<const Object> object;

protected:
	// �`��Ɏg�����_�̐�
	const GLsizei vertexcount;

public:
	// ���f����Transform
	Transform transform;

	// �R���X�g���N�^
	Shape(GLint size,GLsizei vertexcount,const Object::Vertex* vertex, GLsizei indexcount = 0, const GLuint *index = NULL)
		:object(new Object(size,vertexcount,vertex,indexcount,index)),vertexcount(vertexcount)
	{

	}

	Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex, Transform transform, GLsizei indexcount = 0, const GLuint *index = NULL)
		:object(new Object(size, vertexcount, vertex, indexcount, index)), vertexcount(vertexcount)
	{
		
	}

	// �`��
	void draw() const
	{
		// ���_�z��I�u�W�F�N�g�̃o�C���h
		object->bind();

		// �`������s
		execute();
	}

	virtual void execute() const
	{
		// �o�C���h���ꂽ���_�z��I�u�W�F�N�g��p���ĕ`��
		// glDrawArrays(GL_TRIANGLES, 0, vertexcount);
		// glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
		glDrawArrays(GL_POINTS, 0, vertexcount);
	}
};