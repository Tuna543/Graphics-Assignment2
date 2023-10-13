#ifndef tool_h
#define tool_h

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Tool {

public:
	std::vector<glm::mat4> modelMatrices;
	float tox, toy, toz;
	Tool(float x = 0, float y = 0, float z = 0) {
		tox = x;
		toy = y;
		toz = z;
	}
	glm::mat4 transforamtion(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
		tx += tox;
		ty += toy;
		tz += toz;
		glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
		translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
		rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
		scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
		model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
		return model;
	}


	Shader ret_shader(Shader ourShader, unsigned int VAO_CIRC, unsigned int VAO2, unsigned int VAO3) {



		glm::mat4 model;
		float rotateAngle_X = 0;
		float rotateAngle_Y = 0;
		float rotateAngle_Z = 0;
		//Tool_Top
		//lower portion
		model = transforamtion(0.625, -.15, .8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .5, 0.1, .5);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO_CIRC);
		glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);
		//Tool_Top
		//upper portion
		model = transforamtion(0.625, -.05, .8, rotateAngle_X, rotateAngle_Y, 180.0f, .5, 0.01, .5);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO_CIRC);
		glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);

		//chair Leg
		model = transforamtion(0.425, -.2, .5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, -1.1, 0.1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair Leg
		model = transforamtion(.875, -.2, .5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, -1.1, 0.1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//chair Leg
		model = transforamtion(.875, -.2, 1.035, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, -1.1, 0.1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair Leg
		model = transforamtion(0.425, -.2, 1.035, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, -1.1, 0.1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		return ourShader;
	}
};


#endif
#pragma once
