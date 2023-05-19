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

	// シェーダーオブジェクトのコンパイル結果を表示する
	//		shader: シェーダーオブジェクト名
	//		str: コンパイルエラーが発生した場所を表す文字列
	GLboolean printShaderInfoLog(GLuint shader, const char *str)
	{
		// コンパイル結果を取得する
		GLint status;
		// 第2引数 GL_COMPLIE_STATUS: コンパイルに成功していればGL_TRUEを返し, 失敗していればGL_FALSEを返す
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
			std::cerr << "Complie Error in " << str << std::endl;

		// シェーダーのコンパイル時のログの長さを取得する
		GLsizei bufsize;
		// 第2引数 GL_INFO_LOG_LENGTH: コンパイル時に生成されたログの長さを*busizeに格納する
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufsize);

		if (bufsize > 1)
		{
			// シェーダーのコンパイル時のログの内容を取得する
			std::vector<GLchar> infoLog(bufsize);
			GLsizei length;
			glGetShaderInfoLog(shader, bufsize, &length, &infoLog[0]);
			std::cerr << &infoLog[0] << std::endl;
		}

		return static_cast<GLboolean>(status);
	}

	// プログラムオブジェクトにリンク結果を表示する
	//		program: プログラムオブジェクト名
	GLboolean printProgramInfoLog(GLuint program)
	{
		// リンク結果を取得する
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
			std::cerr << "Link Error." << std::endl;

		// シェーダーのリンク時のログの長さを取得する
		GLsizei bufsize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufsize);

		if (bufsize > 1)
		{
			// シェーダーのリンク時のログの内容を取得する
			std::vector<GLchar> infoLog(bufsize);
			GLsizei length;
			glGetProgramInfoLog(program, bufsize, &length, &infoLog[0]);
			std::cerr << &infoLog[0] << std::endl;
		}

		return static_cast<GLboolean>(status);
	}

	// プログラムオブジェクトの作成
	//  	vsrc: バーテックスシェーダーのソースプログラムの文字列
	//		fsrc: フラグメントシェーダーのソースプログラムの文字列
	GLuint createProgram(const char *vsrc, const char *fsrc)
	{
		// 空のプログラムオブジェクトの作成
		const GLuint program(glCreateProgram());

		if (vsrc != NULL)
		{
			// バーテックスシェーダーのシェーダーオブジェクトの作成
			// glCreateShader()の戻り値は作成されたシェーダーオブジェクト名(番号), 作成に失敗した場合は0を返す
			const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
			// シェーダーのソースプログラムの読み込み
			//		第1引数: シェーダーオブジェクト
			//		第2引数: 第3引数に指定した配列の要素数
			//		第3引数: シェーダーのソースプログラム
			glShaderSource(vobj, 1, &vsrc, NULL);
			// シェーダーオブジェクトに読み込まれたソースファイルをコンパイルする
			glCompileShader(vobj);

			// バーテックスシェーダーのシェーダーオブジェクトをプログラムオブジェクトに組み込む
			if (printShaderInfoLog(vobj, "vertex shader"))
				glAttachShader(program, vobj);
			// シェーダーオブジェクトはプログラムオブジェクトに組み込んだ後は不要になるため削除マークをつける
			// 削除マークが付けられたシェーダーオブジェクトは, それを組み込んだプログラムオブジェクトが削除された時点で削除される
			glDeleteShader(vobj);
		}

		if (fsrc != NULL)
		{
			// フラグメントシェーダーのシェーダーオブジェクトの作成
			const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
			glShaderSource(fobj, 1, &fsrc, NULL);
			glCompileShader(fobj);

			// フラグメントシェーダーのシェーダーオブジェクトをプログラムオブジェクトに組み込む
			if (printShaderInfoLog(fobj, "fragment shader"))
				glAttachShader(program, fobj);
			glDeleteShader(fobj);
		}

		// バーテックスシェーダーのattribute変数の場所(番号)を指定する
		// attribute変数はバーテックスシェーダーのソースプログラムでin変数として宣言される
		glBindAttribLocation(program, 0, "position");
		glBindAttribLocation(program, 1, "normal");
		glBindAttribLocation(program, 2, "uv");
		glBindAttribLocation(program, 3, "edgeScale");
		// フラグメントシェーダーのソースプログラム中のout変数にカラーバッファを割り当てる
		glBindFragDataLocation(program, 0, "fragment");
		// プログラムオブジェクトをリンクする
		glLinkProgram(program);

		// 作成したプログラムオブジェクトを返す
		if (printProgramInfoLog(program))
			return program;

		// プログラムオブジェクトが作成出来なければ0を返す
		glDeleteProgram(program);
		return 0;
	}

	// シェーダーのソースファイルを読み込んだメモリを返す
	//		name: シェーダーのソースファイル名
	//		buffer: 読み込んだソースファイルのテキスト
	bool readShaderSource(const char *name, std::vector<GLchar> &buffer)
	{
		// ファイル名がNULLのとき
		if (name == NULL)
			return false;

		// ソースファイルを開く
		std::ifstream file(name, std::ios::binary);
		if (file.fail())
		{
			// 開けなかった場合
			std::cerr << "Error: Can't open source flie: " << name << std::endl;
			return false;
		}

		// ファイルの末尾に移動しファイルサイズを得る
		// ファイルの末尾へ移動
		file.seekg(0L, std::ios::end);
		// 現在位置を取得することでファイルサイズを得る
		GLsizei length = static_cast<GLsizei>(file.tellg());

		// ファイルサイズのメモリを確保
		buffer.resize(length + 1);

		// ファイルを先頭から読み込む
		file.seekg(0L, std::ios::beg);
		file.read(buffer.data(), length);
		buffer[length] = '\0';

		if (file.fail())
		{
			// 上手く読み込めなかった
			std::cerr << "Error: Could not read source file: " << name << std::endl;
			file.close();
			return false;
		}

		// 読み込み成功
		file.close();
		return true;
	}

	// シェーダーのソースファイルを読み込んでプログラムオブジェクトを作成する
	//		vert: バーテックスシェーダーのソースファイル名
	//		frag: フラグメントシェーダーのソースファイル名
	GLuint loadProgram(const char *vert, const char *frag)
	{
		// シェーダーのソースファイルを読み込む
		std::vector<GLchar> vsrc;
		const bool vstat(readShaderSource(vert, vsrc));
		std::vector<GLchar> fsrc;
		const bool fstat(readShaderSource(frag, fsrc));

		//	プログラムオブジェクトの作成
		return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
	}

};