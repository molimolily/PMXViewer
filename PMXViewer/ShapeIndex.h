#pragma once
#include "Shape.h"

// �C���f�b�N�X���g�����}�`�̕`��
class ShapeIndex : public Shape
{
protected:
	// �`��Ɏg�����_��
	const GLsizei indexcount;

public:
	// �R���X�g���N�^
	// size: ���_�̈ʒu�̎���
	// vertexcount: ���_�̐�
	// vertex: ���_�������i�[�����z��
	// indexcount: ���_�̃C���f�b�N�X�̗v�f��
	// index: ���_�̃C���f�b�N�X���i�[�����z��
	ShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index)
		: Shape(size, vertexcount, vertex, indexcount, index), indexcount(indexcount)
	{

	}

	// �`��̎��s
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

	// �`��̎��s
	virtual void execute() const
	{
		glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
	}
};