#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Transform.h"

class CameraTransform
{
	glm::vec3 cameraPosition;
	glm::vec3 cameraTarget;
	glm::vec3 upVector;

public:
	CameraTransform()
		: cameraPosition(glm::vec3(0.0f, 0.0f, 0.0f)), cameraTarget(glm::vec3(0.0f, 0.0f, 0.0f)), upVector(glm::vec3(0.0f, 1.0f, 0.0f))
	{

	}

	void setCameraPosition(float x,float y,float z)
	{
		cameraPosition = glm::vec3(x, y, z);
	}

	void setCameraPosition(glm::vec3 pos)
	{
		cameraPosition = pos;
	}

	void setCameraTarget(float x, float y, float z)
	{
		cameraTarget = glm::vec3(x, y, z);
	}

	void setCameraTarget(glm::vec3 target)
	{
		cameraTarget = target;
	}

	void setUpVector(float x, float y, float z)
	{
		upVector = glm::vec3(x, y, z);
	}

	glm::vec3 getCameraPosition()
	{
		return cameraPosition;
	}

	glm::vec3 getCameraTarget()
	{
		return cameraTarget;
	}

	glm::vec3 getUpVector()
	{
		return upVector;
	}
};
class Camara
{
public:
	CameraTransform transform;

	glm::mat4 viewMatrix()
	{
		return glm::lookAt(transform.getCameraPosition(), transform.getCameraTarget(), transform.getUpVector());
	}
	glm::mat4 perspective(float fovy,float aspect,float near,float far)
	{
		return glm::perspective(glm::radians(fovy), aspect, near, far);
	}
};