#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>

class Shader
{
	const GLuint program;

public:

	Shader(const char *vert, const char *frag) : program(loadProgram(vert, frag))
	{
		
	}

	void useShaderProgram()
	{
		glUseProgram(program);
	}

	GLint getUniformLoc(const char *uni)
	{
		return glGetUniformLocation(program, uni);
	}

private:

	// �V�F�[�_�[�I�u�W�F�N�g�̃R���p�C�����ʂ�\������
	//		shader: �V�F�[�_�[�I�u�W�F�N�g��
	//		str: �R���p�C���G���[�����������ꏊ��\��������
	GLboolean printShaderInfoLog(GLuint shader, const char *str)
	{
		// �R���p�C�����ʂ��擾����
		GLint status;
		// ��2���� GL_COMPLIE_STATUS: �R���p�C���ɐ������Ă����GL_TRUE��Ԃ�, ���s���Ă����GL_FALSE��Ԃ�
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
			std::cerr << "Complie Error in " << str << std::endl;

		// �V�F�[�_�[�̃R���p�C�����̃��O�̒������擾����
		GLsizei bufsize;
		// ��2���� GL_INFO_LOG_LENGTH: �R���p�C�����ɐ������ꂽ���O�̒�����*busize�Ɋi�[����
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufsize);

		if (bufsize > 1)
		{
			// �V�F�[�_�[�̃R���p�C�����̃��O�̓��e���擾����
			std::vector<GLchar> infoLog(bufsize);
			GLsizei length;
			glGetShaderInfoLog(shader, bufsize, &length, &infoLog[0]);
			std::cerr << &infoLog[0] << std::endl;
		}

		return static_cast<GLboolean>(status);
	}

	// �v���O�����I�u�W�F�N�g�Ƀ����N���ʂ�\������
	//		program: �v���O�����I�u�W�F�N�g��
	GLboolean printProgramInfoLog(GLuint program)
	{
		// �����N���ʂ��擾����
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
			std::cerr << "Link Error." << std::endl;

		// �V�F�[�_�[�̃����N���̃��O�̒������擾����
		GLsizei bufsize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufsize);

		if (bufsize > 1)
		{
			// �V�F�[�_�[�̃����N���̃��O�̓��e���擾����
			std::vector<GLchar> infoLog(bufsize);
			GLsizei length;
			glGetProgramInfoLog(program, bufsize, &length, &infoLog[0]);
			std::cerr << &infoLog[0] << std::endl;
		}

		return static_cast<GLboolean>(status);
	}

	// �v���O�����I�u�W�F�N�g�̍쐬
	//  	vsrc: �o�[�e�b�N�X�V�F�[�_�[�̃\�[�X�v���O�����̕�����
	//		fsrc: �t���O�����g�V�F�[�_�[�̃\�[�X�v���O�����̕�����
	GLuint createProgram(const char *vsrc, const char *fsrc)
	{
		// ��̃v���O�����I�u�W�F�N�g�̍쐬
		const GLuint program(glCreateProgram());

		if (vsrc != NULL)
		{
			// �o�[�e�b�N�X�V�F�[�_�[�̃V�F�[�_�[�I�u�W�F�N�g�̍쐬
			// glCreateShader()�̖߂�l�͍쐬���ꂽ�V�F�[�_�[�I�u�W�F�N�g��(�ԍ�), �쐬�Ɏ��s�����ꍇ��0��Ԃ�
			const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
			// �V�F�[�_�[�̃\�[�X�v���O�����̓ǂݍ���
			//		��1����: �V�F�[�_�[�I�u�W�F�N�g
			//		��2����: ��3�����Ɏw�肵���z��̗v�f��
			//		��3����: �V�F�[�_�[�̃\�[�X�v���O����
			glShaderSource(vobj, 1, &vsrc, NULL);
			// �V�F�[�_�[�I�u�W�F�N�g�ɓǂݍ��܂ꂽ�\�[�X�t�@�C�����R���p�C������
			glCompileShader(vobj);

			// �o�[�e�b�N�X�V�F�[�_�[�̃V�F�[�_�[�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
			if (printShaderInfoLog(vobj, "vertex shader"))
				glAttachShader(program, vobj);
			// �V�F�[�_�[�I�u�W�F�N�g�̓v���O�����I�u�W�F�N�g�ɑg�ݍ��񂾌�͕s�v�ɂȂ邽�ߍ폜�}�[�N������
			// �폜�}�[�N���t����ꂽ�V�F�[�_�[�I�u�W�F�N�g��, �����g�ݍ��񂾃v���O�����I�u�W�F�N�g���폜���ꂽ���_�ō폜�����
			glDeleteShader(vobj);
		}

		if (fsrc != NULL)
		{
			// �t���O�����g�V�F�[�_�[�̃V�F�[�_�[�I�u�W�F�N�g�̍쐬
			const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
			glShaderSource(fobj, 1, &fsrc, NULL);
			glCompileShader(fobj);

			// �t���O�����g�V�F�[�_�[�̃V�F�[�_�[�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
			if (printShaderInfoLog(fobj, "fragment shader"))
				glAttachShader(program, fobj);
			glDeleteShader(fobj);
		}

		// �o�[�e�b�N�X�V�F�[�_�[��attribute�ϐ��̏ꏊ(�ԍ�)���w�肷��
		// attribute�ϐ��̓o�[�e�b�N�X�V�F�[�_�[�̃\�[�X�v���O������in�ϐ��Ƃ��Đ錾�����
		glBindAttribLocation(program, 0, "position");
		glBindAttribLocation(program, 1, "normal");
		glBindAttribLocation(program, 2, "uv");
		glBindAttribLocation(program, 3, "edgeScale");
		// �t���O�����g�V�F�[�_�[�̃\�[�X�v���O��������out�ϐ��ɃJ���[�o�b�t�@�����蓖�Ă�
		glBindFragDataLocation(program, 0, "fragment");
		// �v���O�����I�u�W�F�N�g�������N����
		glLinkProgram(program);

		// �쐬�����v���O�����I�u�W�F�N�g��Ԃ�
		if (printProgramInfoLog(program))
			return program;

		// �v���O�����I�u�W�F�N�g���쐬�o���Ȃ����0��Ԃ�
		glDeleteProgram(program);
		return 0;
	}

	// �V�F�[�_�[�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ�
	//		name: �V�F�[�_�[�̃\�[�X�t�@�C����
	//		buffer: �ǂݍ��񂾃\�[�X�t�@�C���̃e�L�X�g
	bool readShaderSource(const char *name, std::vector<GLchar> &buffer)
	{
		// �t�@�C������NULL�̂Ƃ�
		if (name == NULL)
			return false;

		// �\�[�X�t�@�C�����J��
		std::ifstream file(name, std::ios::binary);
		if (file.fail())
		{
			// �J���Ȃ������ꍇ
			std::cerr << "Error: Can't open source flie: " << name << std::endl;
			return false;
		}

		// �t�@�C���̖����Ɉړ����t�@�C���T�C�Y�𓾂�
		// �t�@�C���̖����ֈړ�
		file.seekg(0L, std::ios::end);
		// ���݈ʒu���擾���邱�ƂŃt�@�C���T�C�Y�𓾂�
		GLsizei length = static_cast<GLsizei>(file.tellg());

		// �t�@�C���T�C�Y�̃��������m��
		buffer.resize(length + 1);

		// �t�@�C����擪����ǂݍ���
		file.seekg(0L, std::ios::beg);
		file.read(buffer.data(), length);
		buffer[length] = '\0';

		if (file.fail())
		{
			// ��肭�ǂݍ��߂Ȃ�����
			std::cerr << "Error: Could not read source file: " << name << std::endl;
			file.close();
			return false;
		}

		// �ǂݍ��ݐ���
		file.close();
		return true;
	}

	// �V�F�[�_�[�̃\�[�X�t�@�C����ǂݍ���Ńv���O�����I�u�W�F�N�g���쐬����
	//		vert: �o�[�e�b�N�X�V�F�[�_�[�̃\�[�X�t�@�C����
	//		frag: �t���O�����g�V�F�[�_�[�̃\�[�X�t�@�C����
	GLuint loadProgram(const char *vert, const char *frag)
	{
		// �V�F�[�_�[�̃\�[�X�t�@�C����ǂݍ���
		std::vector<GLchar> vsrc;
		const bool vstat(readShaderSource(vert, vsrc));
		std::vector<GLchar> fsrc;
		const bool fstat(readShaderSource(frag, fsrc));

		//	�v���O�����I�u�W�F�N�g�̍쐬
		return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
	}

};