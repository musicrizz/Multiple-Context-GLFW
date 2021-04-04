/*
 * PointSprite.cpp
 *
 *  Created on: Dec 11, 2020
 *      Author: gio
 */

#include "PointSprite.h"
#include "../../lib/logger/easylogging++.h"
#include "../../lib/image/stb_image.h"

PointSprite* PointSprite::instance = nullptr;

PointSprite::~PointSprite() {
	OpenGLContext::destroy(POINT_SPRITE);
}


PointSprite* PointSprite::getInstance() {
	if(instance) return instance;
	instance = new PointSprite();
	return instance;
}

PointSprite::PointSprite() { // @suppress("Class members should be properly initialized")
	OpenGLContext::createContext(POINT_SPRITE, 600, 400, NULL, true, false, true);
	OpenGLContext::makecurrent(POINT_SPRITE);

	ShaderMap::createProgram(POINT_SPRITE, "resources/shaders/PointSprite_vertx.glsl", "resources/shaders/PointSprite_frag.glsl");

	//useless, the window is not decorated
	/*OpenGLContext::setReshape(POINT_SPRITE, [](GLFWwindow* window, int width, int height){
		PointSprite* p = (PointSprite*) glfwGetWindowUserPointer(window);
		if (p) {
			p->w = width;
			p->h = height;
			p->aspect = float(height)/float(width);
			OpenGLContext::makecurrent(POINT_SPRITE);
			ShaderMap::useProgram(POINT_SPRITE);
			glUniform2iv(ShaderMap::getUniformLocation(POINT_SPRITE,"viewport"), 1, glm::value_ptr(glm::ivec2(p->w , p->h)));
		}
	});*/

	LOG(INFO)<<OpenGLerror::check("Poit Sprite creation program");

	glGenBuffers(BUFFERS_NUM, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0)), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LOG(INFO)<<OpenGLerror::check("Poit Sprite creation vertex buffer ");

	glGenVertexArrays(VAOS_NUM, vaos);
	unsigned int vertex_loc = ShaderMap::getAttributeLocation(POINT_SPRITE, "position");
	glBindVertexArray(vaos[V]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
		glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(vertex_loc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	LOG(INFO)<<OpenGLerror::check("Poit Sprite creation  VAO");

	stbi_set_flip_vertically_on_load(true);
	const char* img = "resources/images/cat.png";
	int img_w, img_h, img_nrChannels;
	unsigned char *img_data = stbi_load(img, &img_w, &img_h, &img_nrChannels, 0);
	unsigned int img_size = sizeof(unsigned char) * img_w * img_h * img_nrChannels;

	LOG(INFO)<<"Poit Sprite image texture loaded : w: "<<img_w<<", h: "<<img_h<<", ch: "<<img_nrChannels;

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffers[B_PIXEL_UNPAK]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, img_size, img_data, GL_STATIC_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	stbi_image_free(img_data);

	LOG(INFO)<<OpenGLerror::check("Creazione Point Sprite GL_PIXEL_UNPACK_BUFFER ");

	glGenTextures(TEXTURES_NUM, textures);

	switch(img_nrChannels)  {
		case 1: internalFormat = GL_R8; format = GL_RED;break;
		case 3: internalFormat = GL_RGB32F; format = GL_RGB;break;
		case 4: internalFormat = GL_RGBA32F; format = GL_RGBA;break;
	}

	frame_w = 512;
	frame_h = 256;
	frame_size = frame_w * frame_h * img_nrChannels;
	current_frame = 7;
	num_frames = 7;
	frames_interval = 50;//milliseconsd
	TempoMap::create(TEMPO_ANIMATION_SPRITE);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffers[B_PIXEL_UNPAK]);
	glBindTexture(GL_TEXTURE_2D, textures[T_SPRITE]);
	glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, frame_w, frame_h);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_w, frame_h, format, GL_UNSIGNED_BYTE,  reinterpret_cast<void*>(frame_size*current_frame));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glBindTexture(GL_TEXTURE_2D, T_NULL);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	unsigned int sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	LOG(INFO)<<OpenGLerror::check("Creazione Point Sprite TEXTURE");

	ShaderMap::useProgram(POINT_SPRITE);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(ShaderMap::getUniformLocation("sprite_texture"), 1);
	glBindTexture(GL_TEXTURE_2D, textures[T_SPRITE]);

	glfwGetFramebufferSize(OpenGLContext::getCurrent(), &w, &h);
	aspect = float(h)/float(w);
	glUniform2iv(ShaderMap::getUniformLocation("viewport"), 1, glm::value_ptr(glm::ivec2(w,h)));

	ShaderMap::program_null();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);
	glDepthMask(true);
	glClearDepth(1.0f);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColorMask(true, true, true, true);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
	//GL_UPPER_LEFT default altrimenti texture capa sotto
	//glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, ??);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OpenGLContext::setUserPointer(this);
	glfwSwapInterval(1);

	//ShaderMap::program_null();
	//OpenGLContext::releaseContex();
}

void PointSprite::display() {
	OpenGLContext::makecurrent(POINT_SPRITE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w, h);

	ShaderMap::useProgram(POINT_SPRITE);

	animation();

	glBindVertexArray(vaos[V]);
	glDrawArrays(GL_POINTS, 0, 1);

	glBindVertexArray(0);

	ShaderMap::program_null();

	OpenGLContext::swapBuffers();
	OpenGLContext::releaseContext();
}

void PointSprite::animation()  {
	if(TempoMap::getElapsedMill(TEMPO_ANIMATION_SPRITE) >= frames_interval) {
		if(--current_frame < 0) current_frame = num_frames;
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffers[B_PIXEL_UNPAK]);
			glBindTexture(GL_TEXTURE_2D, textures[T_SPRITE]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_w, frame_h, format, GL_UNSIGNED_BYTE, reinterpret_cast<void*>(frame_size*current_frame));
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

		TempoMap::updateStart(TEMPO_ANIMATION_SPRITE);
	}

}


