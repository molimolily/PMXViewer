#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
	glm::vec3 modelPosition;
	glm::vec3 modelRotation;
	glm::vec3 modelScale;
public:
	Transform() : modelPosition(glm::vec3(0.0f, 0.0f, 0.0f)), modelRotation(glm::vec3(0.0f, 0.0f, 0.0f)), modelScale(glm::vec3(1.0f, 1.0f, 1.0f))
	{
	}

	Transform(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float xScal, float yScal, float zScal)
		: modelPosition(glm::vec3(xPos,yPos,zPos)), modelRotation(glm::vec3(xRot,yRot,zRot)), modelScale(glm::vec3(xScal,yScal,zScal))
	{

	}
	void setModelPosition(float x, float y, float z)
	{
		modelPosition = glm::vec3(x, y, z);
	}

	void setModelRotation(float x, float y, float z)
	{
		modelRotation = glm::vec3(x, y, z);
	}

	void setModelScale(float x, float y, float z)
	{
		modelScale = glm::vec3(x, y, z);
	}

	void translateModelPosition(float x, float y, float z)
	{
		x *= 0.02f;
		y *= 0.02f;
		z *= 0.02f;

		modelPosition.x -= x;
		modelPosition.y -= y;
		modelPosition.z += z;
	}
	void rotateModelPosition(float x, float y)
	{
		x *= 0.1f;
		y *= 0.1f;
		
		modelRotation.x -= y;
		modelRotation.y += x;
	}

	glm::vec3 getModelPosition()
	{
		return modelPosition;
	}

	glm::mat4 modelMatrix()
	{
		// モデル行列を作成
		glm::mat4 modelMatrix = glm::mat4(1.0f); // 単位行列で初期化
		modelMatrix = glm::translate(modelMatrix, modelPosition); // 平行移動
		modelMatrix = glm::rotate(modelMatrix, glm::radians(modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // X軸を中心に回転
		modelMatrix = glm::rotate(modelMatrix, glm::radians(modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Y軸を中心に回転
		modelMatrix = glm::rotate(modelMatrix, glm::radians(modelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z軸を中心に回転
		modelMatrix = glm::scale(modelMatrix, modelScale); // スケール変換

		return modelMatrix;
	}
};