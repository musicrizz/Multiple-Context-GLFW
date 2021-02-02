/*
 * Animation.h
 *
 *  Created on: Jan 11, 2021
 *      Author: gio
 */

#ifndef CAMERATEXTURE_ANIMATION_H_
#define CAMERATEXTURE_ANIMATION_H_

#include "QuadModel.h"

// VERY VERY CHEAP ANIMATION CLASS !!! It IS NOT THE PROER WAY TO DO !!!

class Animation {

private:
	QuadModel *camera, *canny;

	enum DRAW { CAM = 0, CANNY };

	int fase_loc;

	enum FASE{ STOP = -1, IDLE = 0, SPLIT, FUSE, RUN };

	struct STATUS {
		//operation for step animation
		int fase = IDLE;
		bool flag_fase = true;

		//Strings for  Timer creation
		std::string TIMER_BASE = "TIMER_BASE_ANIMATION";
		//temp variables
		float time, time_factor_pos, time_factor_rotation;

		//keep position for trasformation
		glm::vec3 camera_pos, camera_rotat, canny_pos, canny_rotat;

	}status;

	Animation(const Animation &other) = delete;
	Animation(Animation &&other) = delete;
	Animation& operator=(const Animation &other) = delete;
	Animation& operator=(Animation &&other) = delete;

public:

	virtual ~Animation();

	Animation(int model_mtx_loc, int draw_op_loc, int fase_loc) { // @suppress("Class members should be properly initialized")

		QuadModel::draw_op_loc = draw_op_loc;
		QuadModel::model_mtx_loc = model_mtx_loc;
		camera = new QuadModel(CAM);
		canny = new QuadModel(CANNY);

		this->fase_loc = fase_loc;

		status.camera_pos = camera->getPosition();
		status.camera_rotat = camera->getRotation();

		status.canny_pos = canny->getPosition();
		status.canny_rotat = canny->getRotation();

	}

	void animation(double deltaTime);

	QuadModel* getCamera() {
		return this->camera;
	}

	QuadModel* getCanny() {
		return this->canny;
	}

	void sendStatus() {
		if (status.flag_fase) {
			glUniform1i(fase_loc, status.fase);
			status.flag_fase = false;
		}
	}

	void setStatus(int op)  {
		this->status.fase = op;
		this->status.flag_fase = true;
	}

	int getStatus()  {
		return this->status.fase;
	}

	bool isStatus(FASE f)  {
		return status.fase == f;
	}
};

#endif /* CAMERATEXTURE_ANIMATION_H_ */
