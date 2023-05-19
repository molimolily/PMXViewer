#pragma once
#include<iostream>
#include<GL/glew.h>
// #include "Object.h"

class VertexData;
class Model;

class Object
{
	// ���_�z��I�u�W�F�N�g��
	GLuint vao;

	// ���_�o�b�t�@�I�u�W�F�N�g��
	GLuint vbo;

	// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
	GLuint ibo;

public:
	// ���_����
	struct Vertex
	{
		GLfloat position[3];
	};
	

	// �R���X�g���N�^
	Object(GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount, const GLuint *index);

	Object(Model model);

	// �f�X�g���N�^
	~Object();

	// ���_�z��I�u�W�F�N�g�̌���
	// �`��O�ɕ`�悷��}�`�f�[�^�̕ێ��������_�z��I�u�W�F�N�g���o�C���h����
	void bind() const;

private:
	// �C���X�^���X�̃R�s�[�������쐬����邱�Ƃ�h��
	// �R�s�[�R���X�g���N�^�ɂ�����֎~
	Object(const Object &o);
	// ����ɂ��R�s�[�֎~
	Object &operator = (const Object &o);
};