/*
 * CameraTexture.h
 *
 *  Created on: Dec 6, 2020
 *      Author: gio
 */

#ifndef CAMERATEXTURE_H_
#define CAMERATEXTURE_H_

#include "Animation.h"
#include <atomic>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include "../Constants.h"


class CameraTexture {
private:

	cv::VideoCapture cam_capture;
	int frame_w, frame_h, frame_ch, depth, type ;

	std::thread thread_cam;
	std::mutex mutex_cam_buffer;
	bool stop_capture;

	cv::Mat cam_buffer, canny_buffer;
	cv::Mat cam_buffer_tmp;
	bool dataReady; // better atomic

	static CameraTexture* instance;

	int viewport_w, viewport_h;
	float viewport_aspect;

	glm::mat4 project_mtx, view_mtx;
	int project_mtx_loc, view_mtx_loc;

	enum VAO {V, VAOS_NUM};
	unsigned int vaos[VAOS_NUM];

	enum BUFFERS {B_VERTEX, BUFFERS_NUM};
	unsigned int buffers[BUFFERS_NUM];

	enum TEXTURES {T_NULL, T_CAM, T_CANNY, TEXTURES_NUM};
	unsigned int textures[TEXTURES_NUM];

	Animation* animation;

	void updateTexture();

	CameraTexture();
	CameraTexture(const CameraTexture &other)=delete;
	CameraTexture(CameraTexture &&other)=delete;
	CameraTexture& operator=(const CameraTexture &other)=delete;
	CameraTexture& operator=(CameraTexture &&other)=delete;

public:

	virtual ~CameraTexture();

	static CameraTexture* getInstance();

	void stop();

	void display(double deltaTime);

	void join();

	void camCapture();
};

#endif /* CAMERATEXTURE_H_ */
