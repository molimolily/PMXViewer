#include "Object.h"
#include "Model.h"

// �R���X�g���N�^
Object::Object(GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount = 0, const GLuint *index = NULL)
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
	glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

	// ���_�o�b�t�@�I�u�W�F�N�g��attribute�ϐ��Ɋ֘A�t����
	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute�ϐ��̗L����
	glEnableVertexAttribArray(0);

	// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
}

Object::Object(Model model)
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
	glBufferData(GL_ARRAY_BUFFER, model.vertexCount * sizeof(VertexData), model.vertexData.data(), GL_STATIC_DRAW);

	// ���_�����̃C���f�b�N�X���`���܂�
	const GLuint POSITION_INDEX = 0;
	const GLuint NORMAL_INDEX = 1;
	const GLuint UV_INDEX = 2;
	const GLuint EDGE_SCALE_INDEX = 3;
	// ���_�o�b�t�@�I�u�W�F�N�g��attrib�ϐ��Ɋ֘A�t����
	glVertexAttribPointer(POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, position)));
	glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, normal)));
	glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, uv)));
	glVertexAttribPointer(EDGE_SCALE_INDEX, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, edgeScale)));

	// attribute�ϐ��̗L����
	glEnableVertexAttribArray(0);

	// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.faceCount * sizeof(GLuint), model.vertexIndex.data(), GL_STATIC_DRAW);
}

// �f�X�g���N�^
Object::~Object()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

// ���_�z��I�u�W�F�N�g�̌���
// �`��O�ɕ`�悷��}�`�f�[�^�̕ێ��������_�z��I�u�W�F�N�g���o�C���h����
void Object::bind() const
{
	glBindVertexArray(vao);
}