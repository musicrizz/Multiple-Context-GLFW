/*
 * Mesh.h
 *
 *  Created on: Jan 10, 2021
 *      Author: gio
 */

#ifndef CAMERATEXTURE_QUADMODEL_H_
#define CAMERATEXTURE_QUADMODEL_H_

#include "../../context_util/util.h"

class QuadModel {

private:

	int draw_op;

	glm::mat4 model_mtx;

	glm::vec3 position;
	float rotationX, rotationY, rotationZ;
	glm::vec3 scaling;
	bool model_mtx_flag;

	bool render;

	//QuadModel(const QuadModel &other) = delete;
	QuadModel(QuadModel &&other) = delete;
	QuadModel& operator=(const QuadModel &other) = delete;
	QuadModel& operator=(QuadModel &&other) = delete;

public:

	static int draw_op_loc, model_mtx_loc;

	virtual ~QuadModel() {}

	QuadModel(int draw_op) {
		this->draw_op = draw_op;
		model_mtx = glm::mat4(1.0f);
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		rotationX = 0.0f;
		rotationY = 0.0f;
		rotationZ = 0.0f;
		scaling = glm::vec3(1.0f, 1.0f, 1.0f);
		model_mtx_flag = false;
		render = false;
	}

	QuadModel(const QuadModel &other) {
		draw_op = other.draw_op;
		model_mtx = other.model_mtx;
		position  = other.position;
		rotationX = other.rotationX;
		rotationY = other.rotationY;
		rotationZ = other.rotationZ;
		scaling   = other.scaling;
		model_mtx_flag = other.model_mtx_flag;
		render    = other.render;
	}

	void update(double deltaTime);

	bool isRender();

	void setRender(bool r);

	void setModelMtx(glm::mat4x4 m) {
		this->model_mtx = m;
		this->model_mtx_flag = true;
	}

	void setModelMatx_loc(int loc)  {
		this->model_mtx_loc = loc;
	}

	void calculateModelMtx() {
		this->model_mtx =
			glm::translate(glm::vec3(position.x, position.y, position.z))
			* glm::rotate(rotationX, glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(rotationY, glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(rotationZ, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(scaling);
		model_mtx_flag = false;
	}

	void sendModelMtx()  {
		if(model_mtx_flag) calculateModelMtx();
		glUniformMatrix4fv(model_mtx_loc, 1, GL_FALSE, glm::value_ptr(model_mtx));
	}

	void draw()  {
		sendModelMtx();
		glUniform1i(draw_op_loc, draw_op);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	glm::mat4x4 getModelMtx() {
		if (model_mtx_flag)
			calculateModelMtx();
		return this->model_mtx;
	}

	void setPosition(glm::vec3 position) {
		this->position = position;
		this->model_mtx_flag = true;
	}

	void addPositionX(float x) {
		position.x += x;
		this->model_mtx_flag = true;
	}
	void setPositionX(float x) {
		this->position.x = x;
		this->model_mtx_flag = true;
	}
	void addPositionY(float y) {
		position.y += y;
		this->model_mtx_flag = true;
	}
	void setPositionY(float y) {
		this->position.y = y;
		this->model_mtx_flag = true;
	}
	void addPositionZ(float z) {
		position.z += z;
		this->model_mtx_flag = true;
	}
	void setPositionZ(float z) {
		this->position.z = z;
		this->model_mtx_flag = true;
	}
	glm::vec3 getPosition() {
		return this->position;
	}

	void setRotation(glm::vec3 r) {
		rotationX = glm::radians(r.x);
		rotationY = glm::radians(r.y);
		rotationZ = glm::radians(r.z);
		model_mtx_flag = true;
	}
	glm::vec3 getRotation() {
		return glm::vec3( glm::degrees(rotationX), glm::degrees(rotationY), glm::degrees(rotationZ) );
	}

	void setRotationX(float x) {
		rotationX = glm::radians(x);
		model_mtx_flag = true;
	}
	void addRatotionX(float x) {
		rotationX += glm::radians(x);
		model_mtx_flag = true;
	}
	float getRotationX() {
		return this->rotationX;
	}

	void setRotationY(float y) {
		rotationY = glm::radians(y);
		model_mtx_flag = true;
	}
	void addRatotionY(float y) {
		rotationY += glm::radians(y);
		model_mtx_flag = true;
	}
	float getRotationY() {
		return this->rotationY;
	}

	void setRotationZ(float z) {
		rotationZ = glm::radians(z);
		model_mtx_flag = true;
	}
	void addRatotionZ(float z) {
		rotationZ += glm::radians(z);
		model_mtx_flag = true;
	}
	float getRotationZ() {
		return this->rotationZ;
	}

	void setScaling(glm::vec3 s) {
		scaling = s;
		model_mtx_flag = true;
	}
	void addScalingX(float x) {
		scaling.x += x;
		model_mtx_flag = true;
	}
	void addScalingY(float y) {
		scaling.y += y;
		model_mtx_flag = true;
	}
	void addScalingZ(float z) {
		scaling.z += z;
		model_mtx_flag = true;
	}
	glm::vec3 getScaling() {
		return this->scaling;
	}

};

#endif /* CAMERATEXTURE_QUADMODEL_H_ */
