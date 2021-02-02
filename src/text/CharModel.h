/*
 * Wrapper.h
 *
 *  Created on: Dec 20, 2020
 *      Author: gio
 */

#ifndef TEXT_CHARMODEL_H_
#define TEXT_CHARMODEL_H_

#include "Character.h"
#include "ConstParameter.h"
#include <vector>

class CharModel {

private:

	Character* C;

	glm::vec4 color;

	glm::mat4 model_mtx;
	glm::vec3 position;
	float rotationX, rotationY, rotationZ;
	glm::vec3 scaling;
	bool model_mtx_flag;

	bool render;
	float massa, velocity, gravity;

	float alpha_fade_in , alpha_fade_out;

	int pause_time;

	bool tail;
	std::vector<CharModel*> tail_model;

	void calculateModelMtx();
	bool checkLastTailYposition(float value);

	//CharModel(const CharModel &other) = delete;
	CharModel(CharModel &&other) = delete;
	//CharModel& operator=(const CharModel &other) = delete;
	CharModel& operator=(CharModel &&other) = delete;

public:
	virtual ~CharModel();

	CharModel(Character* c);

	CharModel(const CharModel &other);

	CharModel& operator=(const CharModel &other);

	void update(double deltaTime);

	bool isRender();

	void setRender(bool r);

	bool isTail();

	void setTail(Character** &c);

	std::vector<CharModel*>&  getTail();

	void clearTail();

	void resetBaseParameter();

	Character* getC();

	void setC(Character* C);

	void setModelMtx(glm::mat4x4 m);

	glm::mat4x4 getModelMtx();

	void setPosition(glm::vec3 position);

	void addPositionX(float x);
	void addPositionY(float y);
	void setPositionY(float y);
	void addPositionZ(float z);
	glm::vec3 getPosition();

	void setRotation(glm::vec3 r);
	glm::vec3 getRotation();

	void setRotationX(float x);
	void addRatotionX(float x);
	float getRotationX();

	void setRotationY(float y);
	void addRatotionY(float y);
	float getRotationY();

	void setRotationZ(float z);
	void addRatotionZ(float z);
	float getRotationZ();
	void setScaling(glm::vec3 s);
	void addScalingX(float x);
	void addScalingY(float y);
	void addScalingZ(float z);
	glm::vec3 getScaling();

	glm::vec4 getColor();
	void setColor(glm::vec4 c);
};


#endif /* TEXT_CHARMODEL_H_ */
