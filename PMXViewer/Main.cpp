#pragma comment (lib,"opengl32")
#include <iostream>
#include <string>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Window.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "DrawPMX.h"

int main(int argc, char *argv[])
{
	// GLFW ������������
	if (glfwInit() == GL_FALSE)
	{
		// �������Ɏ��s����
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	// OpenGL�̃o�[�W�����w��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	// �O���݊��v���t�@�C�����g�p���邩�ǂ���, �g�p����ꍇ�͌Â��@�\(�񐄏��̋@�\)���폜�����
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// �g�p����v���t�@�C���̎w��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// �A���`�G�C���A�V���O
	glfwWindowHint(GLFW_SAMPLES, 4);

	// �E�B���h�E���쐬����
	Window window;

	// �w�i�F���w�肷��
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// �w�ʃJ�����O��L���ɂ���
	glFrontFace(GL_CCW);
	// glCullFace(GL_BACK);
	glCullFace(GL_FRONT);

	// glFrontFace(GL_CW);
	// glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

	// �f�v�X�o�b�t�@��L���ɂ���
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// �v���O�����I�u�W�F�N�g���쐬����
	Shader shader("../Shaders/vertex.vert", "../Shaders/textured.frag");

	// uniform�ϐ��̏ꏊ���擾����
	const GLint aspectLoc(shader.getUniformLoc("aspect"));
	const GLint modelViewLoc(shader.getUniformLoc("modelView"));
	const GLint projectionLoc(shader.getUniformLoc("projection"));
	const GLint normalMatrixLoc(shader.getUniformLoc("normalMatrix"));

	const GLint lightPositonLoc(shader.getUniformLoc("Lpos"));
	const GLint diffuseColorLoc(shader.getUniformLoc("diffuseColor"));
	const GLint specularColorLoc(shader.getUniformLoc("specularColor"));
	const GLint specularityLoc(shader.getUniformLoc("specularity"));
	const GLint ambientColorLoc(shader.getUniformLoc("ambientColor"));
	const GLint textureLoc(shader.getUniformLoc("tex"));

	// std::wstring path = L"../Models/test.pmx";
	// std::wstring path = L"../Models/Sphere.pmx";
	// std::wstring path = L"../Models/Quad/Quad.pmx";
	// std::wstring path = L"../Models/miku.pmx";
	// std::wstring path = L"../Models/Tda�������~�N�E�A�y���hVer1.10/Tda�������~�N�E�A�y���hVer1.10/Tda�������~�N�E�A�y���h_Ver1.10.pmx";
	// std::wstring path = L"../Models/Alicia/MMD/Alicia_solid.pmx";
	// std::wstring path = L"../Models/�ݎ��~�N����v4/�ݎ��~�N����v4.pmx";
	// std::wstring path = L"../Models/�R������++�����~�NV3_Re_rev.1.2/�R������  �����~�NV3_Re_rev.1.2/�R������  �����~�NV3_Re_rev.1.2(�m�[�}��).pmx";
	std::wstring path = L"../Models/�䂩��ver7/�䂩��ver7/�䂩��ver7.pmx";
	// std::wstring path = L"../Models/koharu_rikka_mmd_20230521/���t�Z��3D���f��_20230521_�����ǉ�/���t�Z��.pmx";
	// std::wstring path = L"../Models/dot_v3/�ǂ��Ǝ������~�N_V3_ver.2.02/�ǂ��Ǝ������~�N_V3.pmx";
	// std::wstring path = L"../Models/dot_honeywhip/�ǂ��Ǝ������~�N_�n�j�[�E�B�b�v_ver.2.01/�ǂ��Ǝ������~�N_�n�j�[�E�B�b�v.pmx";
	// std::wstring path = L"../Models/dot_breathyou/�ǂ��Ǝ������~�N_�u���X�E���[_ver.2.01/�ǂ��Ǝ������~�N_�u���X�E���[.pmx";
	// std::wstring path = L"../Models/038_����/����.pmx";
	Model model(path);

	// �}�`�f�[�^�̍쐬
	// uniqu_ptr�̓|�C���^���폜���ꂽ�Ƃ��A���ꂪ�w���C���X�^���X���폜�����
	// �܂��A�|�C���^�̃R�s�[���ł��Ȃ�
	// std::unique_ptr<Shape> shape(new Shape(3, 12, vertices));
	// std::unique_ptr<Shape> shape(new ShapeIndex(3, 8, cubeVertex, 24, wireCubeIndex));
	// std::unique_ptr<Shape> shape(new SolidShapeIndex(3, 8, cubeVertex, 36, solidCubeIndex));
	// std::unique_ptr<Shape> shape(new Shape(3, model.vertexCount, model.vertices.data()));
	// std::unique_ptr<Shape> shape(new SolidShapeIndex(3, model.vertexCount, model.vertices.data(), model.faceCount, model.vertexIndex.data()));

	std::unique_ptr<DrawPMX> shape(new DrawPMX(model));

	Camara camera;
	glm::vec3 initCameraPos = glm::vec3(0.0f, 10.0f, -20.0f);
	shape->transform.setModelPosition(0.0f, 0.0f, 0.0f);
	camera.transform.setCameraPosition(initCameraPos);
	camera.transform.setCameraTarget(glm::vec3(0.0f,10.0f,0.0f));

	glm::vec4 Lpos = glm::vec4(0.0, 10.0, -20.0, 1.0);

	// std::cout << "texture : " << static_cast<unsigned int>(shape->model.textureData[0].data[1]) << std::endl;

	int materialCount = shape->getMaterialCount();
	std::vector<bool> meshFlag(materialCount, true);

	// meshFlag[19] = false;
	// meshFlag[3] = false;
	// meshFlag[8] = false;
	// meshFlag[9] = false;
	// meshFlag[13] = false;
	while (window)
	{
		// �J���[�o�b�t�@��w�i�F�ŏ�����
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// �V�F�[�_�[��K�p
		shader.useShaderProgram();

		shape->transform.translateModelPosition(window.getMouseVelocity(false)[0], window.getMouseVelocity(false)[1], 0.0f);
		shape->transform.rotateModelPosition(window.getMouseVelocity(true)[0], window.getMouseVelocity(true)[1]);
		camera.transform.setCameraPosition(initCameraPos + glm::vec3(0.0f, 0.0f, window.getMouseWheelOffset()));

		// �ϊ��s��̐ݒ�
		glm::mat4 modelViewMatrix = camera.viewMatrix() * shape->transform.modelMatrix();
		glm::mat4 projectionMatrix = camera.perspective(45.0f, window.getAspect(), 0.1f, 200.0f);

		// �@���̕ϊ��s��
		glm::mat4 normalMatrix = shape->transform.normalMatrix();

		// �����̈ʒu��ϊ�
		Lpos = camera.viewMatrix() * Lpos;
 
		// Uniform�ϐ��ɒl��ݒ�
		glUniform1f(aspectLoc, window.getAspect());
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE,&modelViewMatrix[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projectionMatrix[0][0]);
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
		glUniform4fv(lightPositonLoc, 1, &Lpos[0]);

		// �}�`��`��
		for (int i = 0; i < shape->getMaterialCount(); i++)
		{
			if (meshFlag[i])
			{
				glUniform4fv(diffuseColorLoc, 1, shape->model.materialData[i].diffuseColor);
				glUniform3fv(specularColorLoc, 1, shape->model.materialData[i].specularColor);
				glUniform1f(specularityLoc, shape->model.materialData[i].specularity);
				glUniform3fv(ambientColorLoc, 1, shape->model.materialData[i].ambientColor);


				if (shape->model.materialData[i].textureIndex != -1)
				{
					glActiveTexture(GL_TEXTURE0 + shape->model.materialData[i].textureIndex);
					glBindTexture(GL_TEXTURE_2D, shape->texID[shape->model.materialData[i].textureIndex]);
					glUniform1i(textureLoc, shape->model.materialData[i].textureIndex);
					// glUniform1i(textureLoc, shape->texID[shape->model.materialData[i].textureIndex]);
				}
				shape->draw(i);
			}
		}

		window.swapBuffers();
	}
}