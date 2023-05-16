#pragma once
#include<GL/glew.h>

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
	Object(GLint size, GLsizei vertexcount, const Vertex *vertex,GLsizei indexcount = 0, const GLuint *index = NULL)
	{
		// ���_�z��I�u�W�F�N�g�̍쐬
		glGenVertexArrays(1, &vao);
		// ���_�z��I�u�W�F�N�g�̃o�C���h
		glBindVertexArray(vao);

		// ���_�o�b�t�@�I�u�W�F�N�g�̍쐬
		glGenBuffers(1, &vbo);
		// ���_�o�b�t�@�I�u�W�F�N�g�̃o�C���h
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// ���_�o�b�t�@�I�u�W�F�N�g�Ƀf�[�^��]��
		glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex ,GL_STATIC_DRAW);

		// ���_�o�b�t�@�I�u�W�F�N�g��attribute�ϐ��Ɋ֘A�t����
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
		// attribute�ϐ��̗L����
		glEnableVertexAttribArray(0);

		// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}

	// �f�X�g���N�^
	virtual ~Object()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}

	// ���_�z��I�u�W�F�N�g�̌���
	// �`��O�ɕ`�悷��}�`�f�[�^�̕ێ��������_�z��I�u�W�F�N�g���o�C���h����
	void bind() const
	{
		glBindVertexArray(vao);
	}

private:
	// �C���X�^���X�̃R�s�[�������쐬����邱�Ƃ�h��
	// �R�s�[�R���X�g���N�^�ɂ�����֎~
	Object(const Object &o);
	// ����ɂ��R�s�[�֎~
	Object &operator = (const Object &o);
};