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


	// �E�B���h�E���쐬����
	Window window;

	// �w�i�F���w�肷��
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// �w�ʃJ�����O��L���ɂ���
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// �f�v�X�o�b�t�@��L���ɂ���
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// �v���O�����I�u�W�F�N�g���쐬����
	Shader shader("..\\Shaders\\point.vert", "..\\Shaders\\point.frag");

	// uniform�ϐ��̏ꏊ���擾����
	const GLint aspectLoc(shader.getUniformLoc("aspect"));
	const GLint modelViewLoc(shader.getUniformLoc("modelView"));
	const GLint projectionLoc(shader.getUniformLoc("projection"));

	// std::string path = "..\\Models\\test.pmx";
	// std::string path = "..\\Models\\Sphere.pmx";
	// std::string path = "..\\Models\\miku.pmx";
	// std::string path = "..\\Models\\Tda�������~�N�E�A�y���hVer1.10\\Tda�������~�N�E�A�y���hVer1.10\\Tda�������~�N�E�A�y���h_Ver1.10.pmx";
	std::string path = "..\\Models\\Alicia\\MMD\\Alicia_solid.pmx";
	// std::string path = "..\\Models\\�ݎ��~�N����v4\\�ݎ��~�N����v4.pmx";
	// std::string path = "..\\Models\\�R������++�����~�NV3_Re_rev.1.2\\�R������  �����~�NV3_Re_rev.1.2\\�R������  �����~�NV3_Re_rev.1.2(�m�[�}��).pmx";
	// std::string path = "..\\Models\\�䂩��ver7\\�䂩��ver7\\�䂩��ver7.pmx";
	// std::string path = "..\\Models\\038_����\\����.pmx";
	Model model(path);

	// �}�`�f�[�^�̍쐬
	// uniqu_ptr�̓|�C���^���폜���ꂽ�Ƃ��A���ꂪ�w���C���X�^���X���폜�����
	// �܂��A�|�C���^�̃R�s�[���ł��Ȃ�
	// std::unique_ptr<Shape> shape(new Shape(3, 12, vertices));
	// std::unique_ptr<Shape> shape(new ShapeIndex(3, 8, cubeVertex, 24, wireCubeIndex));
	// std::unique_ptr<Shape> shape(new SolidShapeIndex(3, 8, cubeVertex, 36, solidCubeIndex));
	// std::unique_ptr<Shape> shape(new Shape(3, model.vertexCount, model.vertices.data()));
	std::unique_ptr<Shape> shape(new SolidShapeIndex(3, model.vertexCount, model.vertices.data(), model.faceCount, model.vertexIndex.data()));

	Camara camera;
	glm::vec3 initCameraPos = glm::vec3(0.0f, 10.0f, -20.0f);
	shape->transform.setModelPosition(0.0f, 0.0f, 0.0f);
	camera.transform.setCameraPosition(initCameraPos);
	camera.transform.setCameraTarget(glm::vec3(0.0f,10.0f,0.0f));

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

		// Uniform�ϐ��ɒl��ݒ�
		glUniform1f(aspectLoc, window.getAspect());
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE,&modelViewMatrix[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projectionMatrix[0][0]);

		// �}�`��`��
		shape->draw();

		window.swapBuffers();
	}
}