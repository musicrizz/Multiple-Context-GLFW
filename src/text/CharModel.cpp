/*
 * CharModel.cpp
 *
 *  Created on: Jan 7, 2021
 *      Author: gio
 */

#include "CharModel.h"

CharModel::~CharModel() {

}

CharModel::CharModel(Character *c) {
	this->C = c;

	this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->model_mtx = glm::mat4(1.0);
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotationX = 0.0f;
	this->rotationY = 0.0f;
	this->rotationZ = 0.0f;
	this->scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	this->model_mtx_flag = false;

	this->tail = false;

	this->massa = 20;
	this->gravity = (rand() % 6 + 1);
	this->velocity = 0;

	this->alpha_fade_in = 3.0f;
	this->alpha_fade_out = 4.0f;

	this->pause_time = 0;

	this->render = false;
}

CharModel::CharModel(const CharModel &other) { // @suppress("Class members should be properly initialized")
	this->C = other.C;
	this->color = other.color;
	this->model_mtx = other.model_mtx;
	this->position = other.position;
	this->tail_model.reserve(TAIL_LENGHT);
//		this->rotationX = other.rotationX;
//		this->rotationY = other.rotationY;
//		this->rotationZ = other.rotationZ;
	this->scaling = other.scaling;
}

CharModel& CharModel::operator=(const CharModel &other) {
	if (this != &other) {
		this->C = other.C;
		this->color = other.color;
		this->model_mtx = other.model_mtx;
		this->position = other.position;
//		this->rotationX = other.rotationX;
//		this->rotationY = other.rotationY;
//		this->rotationZ = other.rotationZ;
		this->scaling = other.scaling;
	}
	return *this;
}

void CharModel::update(double deltaTime) {
	float factor = deltaTime * gravity;
	velocity += massa * factor;

	if (color.a < 1.0f) {
		color.a += (alpha_fade_in * factor);
		color.a > 1.0f ? color.a = 1.0f : NULL;
	}

	if (tail && this->tail_model.back()->position.y > -5.0f) {
		addPositionY(-velocity);
		for (auto T : tail_model) {
			T->addPositionY(-velocity);
		}
	} else if (position.y > -5.0f) {
		addPositionY(-velocity);
	} else{
		this->render = false;
	}
}

bool CharModel::checkLastTailYposition(float value)  {
	return this->tail && (this->tail_model.back()->position.y > value);
}

bool CharModel::isRender() {
	return this->render;
}

void CharModel::setRender(bool r) {
	this->render = r;
}

bool CharModel::isTail() {
	return this->tail;
}

void CharModel::setTail(Character **&c) {
	this->tail = true;

	float factor = 0.9f;

	CharModel tail_current = *this;

	for (short i = 0; i < TAIL_LENGHT; i++) {
		CharModel *tail_new = new CharModel(c[i]);
		tail_new->setPosition(
				glm::vec3(
						(tail_current.position.x - float(tail_current.C->getBearing().x)), //X
						(tail_current.position.y + float(tail_current.C->getH() * 1.8f)),        //Y
						0.0f));                                                          //Z
		tail_new->addPositionX(float(c[i]->getBearing().x) + ( float(c[i]->getBearing().x) * (1.0f - factor) ));
		//tail_new->addPositionY(float(-c[i]->getOriginOffset()));
		tail_new->setScaling(glm::vec3(factor, factor, 1.0f));
		tail_new->setColor(glm::vec4(0.0f, 1.0f, 0.0f, factor));
		tail_new->setRender(true);

		factor-=0.1f;

		tail_model.push_back(tail_new);

		tail_current = *tail_new;
	}
}
std::vector<CharModel*>& CharModel::getTail() {
	return this->tail_model;
}

void CharModel::clearTail() {
	for (auto t : tail_model) {
		if (t)
			delete t;
	}
	this->tail_model.clear();
}

void CharModel::resetBaseParameter() {
	this->velocity = 0;
	this->gravity = (rand() % 6 + 1);
	this->color.a = 0.0f;
	if (this->tail) {
		this->tail = false;
		this->clearTail();
	}
}

Character* CharModel::getC() {
	return this->C;
}

void CharModel::setC(Character *C) {
	this->C = C;
}

void CharModel::setModelMtx(glm::mat4x4 m) {
	this->model_mtx = m;
	this->model_mtx_flag = true;
}

void CharModel::calculateModelMtx() {
	if (model_mtx_flag) { //in this example I don't use a rotation and scaling
		this->model_mtx = glm::translate(glm::vec3(position.x, position.y, 0.0f)) *
//			glm::rotate(rotationX, glm::vec3(1.0f, 0.0f, 0.0f)) *
// 			glm::rotate(rotationY, glm::vec3(0.0f, 1.0f, 0.0f)) *
//			glm::rotate(rotationZ, glm::vec3(0.0f, 0.0f, 1.0f)) *
//			glm::translate( glm::vec3( 0.0f - glm::ceil(C->getDimension().x/2), 0.0f - glm::ceil(C->getDimension().y/2) , 0.0f) ) //for center rotation ;)
						  glm::scale(scaling);
		model_mtx_flag = false;
	}
}

	glm::mat4x4 CharModel::getModelMtx() {
	if (model_mtx_flag)
		calculateModelMtx();
	return this->model_mtx;
}

void CharModel::setPosition(glm::vec3 position) {
	this->position = position;
	this->model_mtx_flag = true;
}

void CharModel::addPositionX(float x) {
	position.x += x;
	this->model_mtx_flag = true;
}
void CharModel::addPositionY(float y) {
	position.y += y;
	this->model_mtx_flag = true;
}
void CharModel::setPositionY(float y) {
	this->position.y = y;
}
void CharModel::addPositionZ(float z) {
	position.z += z;
	this->model_mtx_flag = true;
}
glm::vec3 CharModel::getPosition() {
	return this->position;
}

void CharModel::setRotation(glm::vec3 r) {
	rotationX = glm::radians(r.x);
	rotationY = glm::radians(r.y);
	rotationZ = glm::radians(r.z);
	model_mtx_flag = true;
}
glm::vec3 CharModel::getRotation() {
	return glm::vec3(rotationX, rotationY, rotationZ);
}

void CharModel::setRotationX(float x) {
	rotationX = glm::radians(x);
	model_mtx_flag = true;
}
void CharModel::addRatotionX(float x) {
	rotationX += glm::radians(x);
	model_mtx_flag = true;
}
float CharModel::getRotationX() {
	return this->rotationX;
}

void CharModel::setRotationY(float y) {
	rotationY = glm::radians(y);
	model_mtx_flag = true;
}
void CharModel::addRatotionY(float y) {
	rotationY += glm::radians(y);
	model_mtx_flag = true;
}
float CharModel::getRotationY() {
	return this->rotationY;
}

void CharModel::setRotationZ(float z) {
	rotationZ = glm::radians(z);
	model_mtx_flag = true;
}
void CharModel::addRatotionZ(float z) {
	rotationZ += glm::radians(z);
	model_mtx_flag = true;
}
float CharModel::getRotationZ() {
	return this->rotationZ;
}

void CharModel::setScaling(glm::vec3 s) {
	scaling = s;
	model_mtx_flag = true;
}
void CharModel::addScalingX(float x) {
	scaling.x += x;
	model_mtx_flag = true;
}
void CharModel::addScalingY(float y) {
	scaling.y += y;
	model_mtx_flag = true;
}
void CharModel::addScalingZ(float z) {
	scaling.z += z;
	model_mtx_flag = true;
}
glm::vec3 CharModel::getScaling() {
	return this->scaling;
}

glm::vec4 CharModel::getColor() {
	return this->color;
}
void CharModel::setColor(glm::vec4 c) {
	this->color = c;
}

