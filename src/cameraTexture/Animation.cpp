/*
 * Animation.cpp
 *
 *  Created on: Jan 12, 2021
 *      Author: gio
 */



#include "Animation.h"
#include "../../lib/logger/easylogging++.h"

int QuadModel::draw_op_loc = -1;
int QuadModel::model_mtx_loc = -1;

Animation::~Animation() {}

// VERY VERY CHEAP ANIMATION CLASS !!! IT IS ONLY FOR FUN .

void Animation::animation(double deltaTime) {
	if(isStatus(RUN))return;

	if( isStatus(IDLE) && (TempoMap::getElapsedSeconds(status.TIMER_BASE) < 5) ) {
		return;
	}
	if(isStatus(IDLE)) {
		TempoMap::updateStart(status.TIMER_BASE);
		setStatus(SPLIT);
		LOG(INFO)<<"Fase Idle finish";
	}

	if( isStatus(SPLIT) && (TempoMap::getElapsedSeconds(status.TIMER_BASE) < 5) ) {
		status.time = float(TempoMap::getElapsedMill(status.TIMER_BASE));
		status.time_factor_pos = status.time / 3000.0f;
		status.time_factor_rotation = status.time / 4000.0f *28.0f;

		camera->setRotationY(status.camera_rotat.y + std::min(28.0f, status.time_factor_rotation) );
		camera->setPositionZ(status.camera_pos.z - std::min(.8f, .8f * status.time_factor_pos) );
		camera->setPositionX(status.camera_pos.x - std::min(1.0f, status.time_factor_pos) );

		canny->setRotationY(status.canny_rotat.y - std::min(28.0f, status.time_factor_rotation) );
		canny->setPositionZ(status.canny_pos.z - std::min(.79f, .79f * status.time_factor_pos) );
		canny->setPositionX(status.canny_pos.x + std::min(1.0f, status.time_factor_pos) );
		return;
	}
	if(isStatus(SPLIT)) {
		status.camera_pos = camera->getPosition();
		status.camera_rotat = camera->getRotation();
		status.canny_pos = canny->getPosition();
		status.canny_rotat = canny->getRotation();
		TempoMap::updateStart(status.TIMER_BASE);
		setStatus(FUSE);
		LOG(INFO)<<"Fase 1 finish , Z pos camera = "<<camera->getPosition().z<<", rotation : "<< camera->getRotation().y;
	}

	if( isStatus(FUSE) && (TempoMap::getElapsedSeconds(status.TIMER_BASE) < 4) ) {
		status.time = float(TempoMap::getElapsedMill(status.TIMER_BASE));
		status.time_factor_pos = status.time / 3000.0f;
		status.time_factor_rotation = status.time / 2800.0f * 28.0f;

		camera->setRotationY(status.camera_rotat.y - std::min(28.0f, status.time_factor_rotation) );
		camera->setPositionX(status.camera_pos.x + std::min(1.0f, status.time_factor_pos));

		canny->setRotationY(status.canny_rotat.y + std::min(28.0f, status.time_factor_rotation)  );
		canny->setPositionX(status.canny_pos.x - std::min(1.0f,  status.time_factor_pos) );
		return;
	}
	if(isStatus(FUSE)) {
		TempoMap::erase(status.TIMER_BASE);
		camera->setPositionZ(-0.01f);
		canny->setPositionZ(0.0f);
		setStatus(RUN);
	}


}


