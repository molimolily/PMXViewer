#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
	// レンダリングコンテキストの宣言
	GLFWwindow* const window;

	// ウィンドウサイズ
	// GLfloat size[2];

	// アスペクト比
	GLfloat aspect;

	// マウスカーソルの速度
	GLfloat mouseVelocity[2];

	// マウスカーソルの位置
	double mousePos[2] = { 0.0, 0.0 };
	double mousePosOld[2] = { 0.0, 0.0 };

	bool isClickLeftMouse = false;

public:

	Window(int width = 640, int height = 480, const char *title = "New Window", GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL)
		: window(glfwCreateWindow(width, height, title, monitor, share)),mouseVelocity{0.0f,0.0f}
	{
		if (window == NULL)
		{
			// ウィンドウが作成できなかった
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		// ウィンドウをカレントにする
		glfwMakeContextCurrent(window);

		// GLEWの初期化
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "Can't initialize GLEW" << std::endl;
			exit(1);
		}

		// 垂直同期のタイミングを待つ
		glfwSwapInterval(1);

		// このインスタンスのthisポインタを記録する
		// コールバックに設定する関数はstaticな関数でなければならないが、staticなメンバ関数はメンバ変数を参照できない
		// staticなメンバ関数内からメンバ変数を間接的に参照するためにthisポインタを記録する
		glfwSetWindowUserPointer(window, this);

		// ウィンドウサイズ変更時のコールバック
		glfwSetWindowSizeCallback(window, resize);

		// 開いたウィンドウの初期設定
		resize(window, width, height);
	}

	// ポリモーフィズムを利用する場合はvirtualにすべき
	// 子クラスのポインタを親クラスのポインタにキャストした場合、デストラクタ呼び出し時に子クラスのデストラクタが呼び出されない
	~Window()
	{
		glfwDestroyWindow(window);
	}

	// bool演算子のオーバーロード
	explicit operator bool()
	{
		// イベントを取り出す
		glfwPollEvents();

		// マウスの左ボタンの状態を調べる
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

		// ウィンドウを閉じる必要がなければtrueを返す
		return !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE);
	}

	// ダブルバッファリング
	void swapBuffers() const
	{
		// カラーバッファを入れ替える
		glfwSwapBuffers(window);
	}

	// ウィンドウのサイズ変更時の処理
	// コールバック関数として登録するために静的メンバ関数にする
	static void resize(GLFWwindow* const window, int width, int height)
	{
		// フレームバッファのサイズを調べる(実際のピクセル数)
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// フレームバッファ全体をビューポートに設定する
		// glViewport(Glint x, GLint y, GLsizei w, GLsizei h)
		//		x,y: ビューポートの左下隅の位置を指定する
		//		w,h: ビューポートの幅と高さを指定する
		glViewport(0, 0, fbWidth, fbHeight);

		// このインスタンスのthisポインタを得る
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

		if (instance != NULL)
		{
			// このインスタンスが保持する縦横比を更新する
			instance->aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
			// std::cout << "aspect : " << instance->aspect << std::endl;

			// 開いたウィンドウのサイズを保存する
			// instance->size[0] = static_cast<GLfloat>(width);
			// instance->size[1] = static_cast<GLfloat>(height);
		}
	}

	// ウィンドウのサイズを取り出す
	// const GLfloat* getSize() const { return size; }

	// アスペクト比を取り出す
	GLfloat getAspect() const { return aspect; }

	// マウスの速度を取り出す
	const GLfloat* getMouseVelocity() { return mouseVelocity; }
};