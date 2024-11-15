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

	// アンチエイリアシング
	glfwWindowHint(GLFW_SAMPLES, 4);

	// ウィンドウを作成する
	Window window;

	// 背景色を指定する
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 背面カリングを有効にする
	glFrontFace(GL_CCW);
	// glCullFace(GL_BACK);
	glCullFace(GL_FRONT);

	// glFrontFace(GL_CW);
	// glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

	// デプスバッファを有効にする
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// プログラムオブジェクトを作成する
	Shader shader("../Shaders/vertex.vert", "../Shaders/textured.frag");

	// uniform変数の場所を取得する
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
	// std::wstring path = L"../Models/Tda式初音ミク・アペンドVer1.10/Tda式初音ミク・アペンドVer1.10/Tda式初音ミク・アペンド_Ver1.10.pmx";
	// std::wstring path = L"../Models/Alicia/MMD/Alicia_solid.pmx";
	// std::wstring path = L"../Models/つみ式ミクさんv4/つみ式ミクさんv4.pmx";
	// std::wstring path = L"../Models/コロン式++初音ミクV3_Re_rev.1.2/コロン式  初音ミクV3_Re_rev.1.2/コロン式  初音ミクV3_Re_rev.1.2(ノーマル).pmx";
	std::wstring path = L"../Models/ゆかりver7/ゆかりver7/ゆかりver7.pmx";
	// std::wstring path = L"../Models/koharu_rikka_mmd_20230521/小春六花3Dモデル_20230521_半袖追加/小春六花.pmx";
	// std::wstring path = L"../Models/dot_v3/どっと式初音ミク_V3_ver.2.02/どっと式初音ミク_V3.pmx";
	// std::wstring path = L"../Models/dot_honeywhip/どっと式初音ミク_ハニーウィップ_ver.2.01/どっと式初音ミク_ハニーウィップ.pmx";
	// std::wstring path = L"../Models/dot_breathyou/どっと式初音ミク_ブレス・ユー_ver.2.01/どっと式初音ミク_ブレス・ユー.pmx";
	// std::wstring path = L"../Models/038_かに/かに.pmx";
	Model model(path);

	// 図形データの作成
	// uniqu_ptrはポインタが削除されたとき、それが指すインスタンスも削除される
	// また、ポインタのコピーができない
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

		// 法線の変換行列
		glm::mat4 normalMatrix = shape->transform.normalMatrix();

		// 光源の位置を変換
		Lpos = camera.viewMatrix() * Lpos;
 
		// Uniform変数に値を設定
		glUniform1f(aspectLoc, window.getAspect());
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE,&modelViewMatrix[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projectionMatrix[0][0]);
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
		glUniform4fv(lightPositonLoc, 1, &Lpos[0]);

		// 図形を描画
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