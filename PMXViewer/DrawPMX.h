#pragma once
#include <vector>
#include<GL/glew.h>
#include "Model.h"
#include "Transform.h"
// class Model;

class DrawPMX
{
	// ���_�����̃C���f�b�N�X���`
	const GLuint POSITION_INDEX = 0;
	const GLuint NORMAL_INDEX = 1;
	const GLuint UV_INDEX = 2;
	const GLuint EDGE_SCALE_INDEX = 3;

	// �}�e���A���̐�
	int materialCount;

	// ���_�o�b�t�@�I�u�W�F�N�g��
	GLuint vbo;

	// ���_�z��I�u�W�F�N�g��
	std::vector<GLuint> vao;

	// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
	std::vector<GLuint> ibo;

	// ���j�t�H�[���o�b�t�@�I�u�W�F�N�g
	// GLuint ubo;

	void setAndEnableVertexAttrib();
public:
	Model model;
	Transform transform;
	DrawPMX(Model model);

	~DrawPMX();

	void bind(int n) const;
	void draw(int n);
	void execute();

	int getMaterialCount();
};