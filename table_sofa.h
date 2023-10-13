#ifndef table_sofa_h
#define table_sofa_h

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Table_Sofa {

public:
	std::vector<glm::mat4> modelMatrices;
	float tox, toy, toz;
	Table_Sofa(float x = 0, float y = 0, float z = 0) {
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


	Shader ret_shader(Shader ourShader, unsigned int VAO, unsigned int VAO2, unsigned int VAOC, unsigned int VAO4, unsigned int VAO5) {
		glm::mat4 model;
		float rotateAngle_X = 0;
		float rotateAngle_Y = 0;
		float rotateAngle_Z = 0;
		//table top
		model = transforamtion(0, 0, 0.2, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 5.5, 0.2, 1.75);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//Leg side
		model = transforamtion(0, 0, .57, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 5.5, -1.0, .2);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//base for legside
		model = transforamtion(0, -0.75, 0.52, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 5.5, .5, .4);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//left side outer chair
		//chair_Top
		model = transforamtion(0.25, -.35, .8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.5, 0.1, 1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair Leg
		model = transforamtion(0.25, -.35, .8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, -.8, 1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair Leg
		model = transforamtion(2.45, -.35, .8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, -.8, 1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//chair side left one
		model = transforamtion(0.25, -.3, .8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, .3, 1.0);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair side right one
		model = transforamtion(2.45, -.3, .8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, .3, 1.0);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair back
		model = transforamtion(0.25, .15, 1.2, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.42, -1.0, 0.2);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);




		//another chair: left side inner side
		//chair_Top
		model = transforamtion(0.25, -.35, -.075, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.5, 0.1, 1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//chair Leg
		model = transforamtion(2.45, -.35, -0.075, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, -.8, 1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair Leg
		model = transforamtion(0.25, -.35, -0.075, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, -.8, 1);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//chair side left one
		model = transforamtion(0.25, -.3, -.075, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, .3, 1.0);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair side right one
		model = transforamtion(2.45, -.3, -.075, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, .3, 1.0);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//chair back
		model = transforamtion(0.25, .15, -.075, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.42, -1.0, 0.2);
		modelMatrices.push_back(model);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		return ourShader;
	}
};


#endif
#pragma once
