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
	// GLFW を初期化する
	if (glfwInit() == GL_FALSE)
	{
		// 初期化に失敗した
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	// OpenGLのバージョン指定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	// 前方互換プロファイルを使用するかどうか, 使用する場合は古い機能(非推奨の機能)が削除される
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// 使用するプロファイルの指定
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// ウィンドウを作成する
	Window window;

	// 背景色を指定する
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 背面カリングを有効にする
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// デプスバッファを有効にする
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// プログラムオブジェクトを作成する
	Shader shader("..\\Shaders\\point.vert", "..\\Shaders\\point.frag");

	// uniform変数の場所を取得する
	const GLint aspectLoc(shader.getUniformLoc("aspect"));
	const GLint modelViewLoc(shader.getUniformLoc("modelView"));
	const GLint projectionLoc(shader.getUniformLoc("projection"));

	// std::string path = "..\\Models\\test.pmx";
	// std::string path = "..\\Models\\Sphere.pmx";
	// std::string path = "..\\Models\\miku.pmx";
	// std::string path = "..\\Models\\Tda式初音ミク・アペンドVer1.10\\Tda式初音ミク・アペンドVer1.10\\Tda式初音ミク・アペンド_Ver1.10.pmx";
	std::string path = "..\\Models\\Alicia\\MMD\\Alicia_solid.pmx";
	// std::string path = "..\\Models\\つみ式ミクさんv4\\つみ式ミクさんv4.pmx";
	// std::string path = "..\\Models\\コロン式++初音ミクV3_Re_rev.1.2\\コロン式  初音ミクV3_Re_rev.1.2\\コロン式  初音ミクV3_Re_rev.1.2(ノーマル).pmx";
	// std::string path = "..\\Models\\ゆかりver7\\ゆかりver7\\ゆかりver7.pmx";
	// std::string path = "..\\Models\\038_かに\\かに.pmx";
	Model model(path);

	// 図形データの作成
	// uniqu_ptrはポインタが削除されたとき、それが指すインスタンスも削除される
	// また、ポインタのコピーができない
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
		// カラーバッファを背景色で初期化
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// シェーダーを適用
		shader.useShaderProgram();

		shape->transform.translateModelPosition(window.getMouseVelocity(false)[0], window.getMouseVelocity(false)[1], 0.0f);
		shape->transform.rotateModelPosition(window.getMouseVelocity(true)[0], window.getMouseVelocity(true)[1]);
		camera.transform.setCameraPosition(initCameraPos + glm::vec3(0.0f, 0.0f, window.getMouseWheelOffset()));

		// 変換行列の設定
		glm::mat4 modelViewMatrix = camera.viewMatrix() * shape->transform.modelMatrix();
		glm::mat4 projectionMatrix = camera.perspective(45.0f, window.getAspect(), 0.1f, 200.0f);

		// Uniform変数に値を設定
		glUniform1f(aspectLoc, window.getAspect());
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE,&modelViewMatrix[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projectionMatrix[0][0]);

		// 図形を描画
		shape->draw();

		window.swapBuffers();
	}
}