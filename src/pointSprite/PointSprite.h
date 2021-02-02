/*
 * PointSprite.h
 *
 *  Created on: Dec 11, 2020
 *      Author: gio
 */

#ifndef POINTSPRITE_H_
#define POINTSPRITE_H_

#include "../Constants.h"
#include "../../context_util/util.h"

class PointSprite {
private:

	static PointSprite* instance;

	int w,h;
	float aspect;

	enum VAO {V, VAOS_NUM};
	unsigned int vaos[VAOS_NUM];

	enum BUFFERS {B_VERTEX, B_PIXEL_UNPAK, BUFFERS_NUM};
	unsigned int buffers[BUFFERS_NUM];

	enum TEXTURES {T_NULL, T_SPRITE,  TEXTURES_NUM};
	unsigned int textures[TEXTURES_NUM];

	GLenum internalFormat, format;
	short current_frame, num_frames;
	int frame_w, frame_h, frame_size, frames_interval;


	void animation();

	PointSprite();

	PointSprite(const PointSprite &other) = delete;
	PointSprite(PointSprite &&other) = delete;
	PointSprite& operator=(const PointSprite &other) = delete;
	PointSprite& operator=(PointSprite &&other) = delete;
public:

	virtual ~PointSprite();

	static PointSprite* getInstance();

	void display();
};

#endif /* POINTSPRITE_H_ */
