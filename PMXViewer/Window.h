#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
	// �����_�����O�R���e�L�X�g�̐錾
	GLFWwindow* const window;

	// �E�B���h�E�T�C�Y
	// GLfloat size[2];

	// �A�X�y�N�g��
	GLfloat aspect;

	// �}�E�X�J�[�\���̑��x
	GLfloat mouseVelocity[2];

	// �}�E�X�J�[�\���̈ʒu
	double mousePos[2] = { 0.0, 0.0 };
	double mousePosOld[2] = { 0.0, 0.0 };

	bool isClickLeftMouse = false;

public:

	Window(int width = 640, int height = 480, const char *title = "New Window", GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL)
		: window(glfwCreateWindow(width, height, title, monitor, share)),mouseVelocity{0.0f,0.0f}
	{
		if (window == NULL)
		{
			// �E�B���h�E���쐬�ł��Ȃ�����
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		// �E�B���h�E���J�����g�ɂ���
		glfwMakeContextCurrent(window);

		// GLEW�̏�����
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "Can't initialize GLEW" << std::endl;
			exit(1);
		}

		// ���������̃^�C�~���O��҂�
		glfwSwapInterval(1);

		// ���̃C���X�^���X��this�|�C���^���L�^����
		// �R�[���o�b�N�ɐݒ肷��֐���static�Ȋ֐��łȂ���΂Ȃ�Ȃ����Astatic�ȃ����o�֐��̓����o�ϐ����Q�Ƃł��Ȃ�
		// static�ȃ����o�֐������烁���o�ϐ����ԐړI�ɎQ�Ƃ��邽�߂�this�|�C���^���L�^����
		glfwSetWindowUserPointer(window, this);

		// �E�B���h�E�T�C�Y�ύX���̃R�[���o�b�N
		glfwSetWindowSizeCallback(window, resize);

		// �J�����E�B���h�E�̏����ݒ�
		resize(window, width, height);
	}

	// �|�����[�t�B�Y���𗘗p����ꍇ��virtual�ɂ��ׂ�
	// �q�N���X�̃|�C���^��e�N���X�̃|�C���^�ɃL���X�g�����ꍇ�A�f�X�g���N�^�Ăяo�����Ɏq�N���X�̃f�X�g���N�^���Ăяo����Ȃ�
	~Window()
	{
		glfwDestroyWindow(window);
	}

	// bool���Z�q�̃I�[�o�[���[�h
	explicit operator bool()
	{
		// �C�x���g�����o��
		glfwPollEvents();

		// �}�E�X�̍��{�^���̏�Ԃ𒲂ׂ�
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
		{
			mousePosOld[0] = mousePos[0];
			mousePosOld[1] = mousePos[1];

			glfwGetCursorPos(window, &mousePos[0], &mousePos[1]);

			if (isClickLeftMouse)
			{
				mouseVelocity[0] = static_cast<GLfloat>(mousePos[0] - mousePosOld[0]);
				mouseVelocity[1] = static_cast<GLfloat>(mousePos[1] - mousePosOld[1]);

				// std::cout << "x : " << mouseVelocity[0] << " y : " << mouseVelocity[1] << std::endl;
			}

			isClickLeftMouse = true;
		}
		else if(isClickLeftMouse)
		{
			isClickLeftMouse = false;
			mouseVelocity[0] = 0.0f;
			mouseVelocity[1] = 0.0f;
		}

		// �E�B���h�E�����K�v���Ȃ����true��Ԃ�
		return !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE);
	}

	// �_�u���o�b�t�@�����O
	void swapBuffers() const
	{
		// �J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(window);
	}

	// �E�B���h�E�̃T�C�Y�ύX���̏���
	// �R�[���o�b�N�֐��Ƃ��ēo�^���邽�߂ɐÓI�����o�֐��ɂ���
	static void resize(GLFWwindow* const window, int width, int height)
	{
		// �t���[���o�b�t�@�̃T�C�Y�𒲂ׂ�(���ۂ̃s�N�Z����)
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// �t���[���o�b�t�@�S�̂��r���[�|�[�g�ɐݒ肷��
		// glViewport(Glint x, GLint y, GLsizei w, GLsizei h)
		//		x,y: �r���[�|�[�g�̍������̈ʒu���w�肷��
		//		w,h: �r���[�|�[�g�̕��ƍ������w�肷��
		glViewport(0, 0, fbWidth, fbHeight);

		// ���̃C���X�^���X��this�|�C���^�𓾂�
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

		if (instance != NULL)
		{
			// ���̃C���X�^���X���ێ�����c������X�V����
			instance->aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
			// std::cout << "aspect : " << instance->aspect << std::endl;

			// �J�����E�B���h�E�̃T�C�Y��ۑ�����
			// instance->size[0] = static_cast<GLfloat>(width);
			// instance->size[1] = static_cast<GLfloat>(height);
		}
	}

	// �E�B���h�E�̃T�C�Y�����o��
	// const GLfloat* getSize() const { return size; }

	// �A�X�y�N�g������o��
	GLfloat getAspect() const { return aspect; }

	// �}�E�X�̑��x�����o��
	const GLfloat* getMouseVelocity() { return mouseVelocity; }
};