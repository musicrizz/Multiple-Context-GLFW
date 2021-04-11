/*
 * CameraTexture.cpp
 *
 *  Created on: Dec 6, 2020
 *      Author: gio
 */

#include "CameraTexture.h"
#include "../../lib/logger/easylogging++.h"

CameraTexture* CameraTexture::instance = nullptr;

CameraTexture::~CameraTexture() {
	stop_capture = true;
	OpenGLContext::destroy(TEXTURE_CAM);
	cam_capture.release();
}

CameraTexture* CameraTexture::getInstance() {
	if(instance) return instance;
	instance = new CameraTexture();
	return instance;

}

CameraTexture::CameraTexture() { // @suppress("Class members should be properly initialized")

	if (cam_capture.open(0)) {
		LOG(INFO) << "Init Opencv Camera Success";
	} else {
		LOG(ERROR) << "File To Open Camera";
		cam_capture.release();
		exit(33);
	}

	cam_capture.read(cam_buffer);
	if (cam_buffer.empty()) {
		LOG(ERROR) << "Error retrieve Frame from camera";
		cam_capture.release();
		exit(33);
	}

	frame_ch = cam_buffer.channels();
	frame_w = cam_buffer.cols;
	frame_h = cam_buffer.rows;
	depth = cam_buffer.depth();//Return a valid type identifier for the individial channels of m0 (e.g., CV_32F )
	type = cam_buffer.type();//Return a valid type identifier for the elements of m0 (e.g., CV_ 32FC3 )
	LOG(INFO) << "Frame Channels : "<<frame_ch<<", Colons = "<<frame_w<<", Rows : "<<frame_h<<" type: "<<type<<", depth: "<<depth;
	cv::cvtColor(cam_buffer, canny_buffer, cv::COLOR_BGR2GRAY);

	stop_capture = false;
	dataReady = false;

	// CREATE OPENGL CONTEXT

	OpenGLContext::createContext(TEXTURE_CAM, 900, 700, NULL, true, false, true);
	//OpenGLContext::setReshape(TEXTURE_CAM, reshape);
	//OpenGLContext::setKeyboard(TEXTURE_CAM,keyboard);
	//OpenGLContext::setUserPointer(this);
	OpenGLContext::makecurrent(TEXTURE_CAM);


//	glfwSetWindowCloseCallback(OpenGLContext::getCurrent(), [](GLFWwindow* window){
//		CameraTexture* p = (CameraTexture*) OpenGLContext::getUserPointer(TEXTURE_CAM);
//		if (p) p->stop();
//	});


	ShaderMap::createProgram(TEXTURE_CAM, "resources/shaders/CameraTexture_vertx.glsl", "resources/shaders/CameraTexture_frag.glsl");

	LOG(INFO)<<OpenGLerror::check("Texture Cam creation program");

	float x = float(frame_w) / float(std::max(frame_w, frame_w));
	float y = float(frame_h) / float(std::max(frame_w, frame_w));
	LOG(INFO)<<"X: "<<x<<" , Y: "<<y;

	glGenBuffers(BUFFERS_NUM, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, ( sizeof(glm::vec4)*4 + sizeof(glm::vec2)*4) , NULL, GL_STATIC_DRAW);
	glm::vec4* v = (glm::vec4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, (sizeof(glm::vec4)*4), GL_MAP_WRITE_BIT);
		*v++ = glm::vec4(-x, -y, 0.0f, 1.0f);
		*v++ = glm::vec4( x, -y, 0.0f, 1.0f);
		*v++ = glm::vec4( x,  y, 0.0f, 1.0f);
		*v   = glm::vec4(-x,  y, 0.0f, 1.0f);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glm::vec2* uv = (glm::vec2*)glMapBufferRange(GL_ARRAY_BUFFER, (sizeof(glm::vec4)*4), (sizeof(glm::vec2)*4), GL_MAP_WRITE_BIT);
		*uv++ = glm::vec2(0.0f, 1.0f);
		*uv++ = glm::vec2(1.0f, 1.0f);
		*uv++ = glm::vec2(1.0f, 0.0f);
		*uv++ = glm::vec2(0.0f, 0.0f);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LOG(INFO)<<OpenGLerror::check("Texture Cam creation buffer vertex");

	glGenVertexArrays(VAOS_NUM, vaos);
	unsigned int vertex_loc = ShaderMap::getAttributeLocation(TEXTURE_CAM, "position");
	unsigned int uvcord_loc = ShaderMap::getAttributeLocation(TEXTURE_CAM, "uv_coord");
	glBindVertexArray(vaos[V]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
		glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), reinterpret_cast<void*>(0));
		glVertexAttribPointer(uvcord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(sizeof(glm::vec4)*4));
		glEnableVertexAttribArray(vertex_loc);
		glEnableVertexAttribArray(uvcord_loc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	LOG(INFO)<<OpenGLerror::check("Texture Cam creation buffer VAO");

	 unsigned int sampler;
	 glGenSamplers(1, &sampler);
	 glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	 glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	 glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(TEXTURES_NUM, textures);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	 int lev = mipmapsLevels(frame_w, frame_h, 160);
	glBindTexture(GL_TEXTURE_2D, textures[T_CAM]);
	    glTexStorage2D(GL_TEXTURE_2D, lev, GL_RGB8, frame_w, frame_h);
	    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_w, frame_h, GL_BGR, GL_UNSIGNED_BYTE, cam_buffer.data);
	    glGenerateMipmap(GL_TEXTURE_2D);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	 glBindTexture(GL_TEXTURE_2D, T_NULL);

	 LOG(INFO)<<OpenGLerror::check("Creazione Texture  T_CAM")<<std::endl;

	glBindTexture(GL_TEXTURE_2D, textures[T_CANNY]);
	 	glTexStorage2D(GL_TEXTURE_2D, lev, GL_R8, frame_w, frame_h);
	 	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_w, frame_h, GL_RED, GL_UNSIGNED_BYTE, canny_buffer.data);
	 	glGenerateMipmap(GL_TEXTURE_2D);
	 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	 glBindTexture(GL_TEXTURE_2D, T_NULL);

	 LOG(INFO)<<OpenGLerror::check("Creazione Texture T_CANNY ")<<std::endl;

	 //Init uniform variable in the shader
	 glfwGetFramebufferSize(OpenGLContext::getCurrent(), &viewport_w, &viewport_h);
	 viewport_aspect = float(viewport_h) / float(viewport_w);

	 ShaderMap::useProgram(TEXTURE_CAM);

	 project_mtx_loc = ShaderMap::getUniformLocation("projection_mtx");
	 project_mtx = glm::frustum(-1.0f, 1.0f, -viewport_aspect, viewport_aspect, 1.0f, 500.0f);
	 glUniformMatrix4fv(project_mtx_loc, 1, GL_FALSE, glm::value_ptr(project_mtx));

	 view_mtx_loc = ShaderMap::getUniformLocation("view_mtx");
	 view_mtx = glm::translate(glm::vec3(0.0f, 0.0f, -1.0f));
	 glUniformMatrix4fv(view_mtx_loc, 1, GL_FALSE, glm::value_ptr(view_mtx));

	 glActiveTexture(GL_TEXTURE1);
	 glUniform1i(ShaderMap::getUniformLocation("camera_image"), T_CAM);
	 	 glBindTexture(GL_TEXTURE_2D, textures[T_CAM]);
	 	 glBindSampler(T_CAM, sampler);

	 glActiveTexture(GL_TEXTURE2);
	 glUniform1i(ShaderMap::getUniformLocation("canny_image"), T_CANNY);
	 	 glBindTexture(GL_TEXTURE_2D, textures[T_CANNY]);
	 	 glBindSampler(T_CANNY, sampler);

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, T_NULL);

	 animation = new Animation(
			 ShaderMap::getUniformLocation("model_mtx"),
			 ShaderMap::getUniformLocation("draw_op"),
			 ShaderMap::getUniformLocation("fase")
			 );


	 glEnable(GL_DEPTH_TEST);
	 glDepthFunc(GL_LEQUAL);
	 glDepthRange(0.0, 1.0);
	 glDepthMask(true);
	 glClearDepth(1.0f);
	 glEnable(GL_LINE_SMOOTH);
	 glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	 glColorMask(true, true, true, true);

	 glViewport(0, 0, viewport_w, viewport_h);
	 glUniform2iv(ShaderMap::getUniformLocation("viewport"),
			 	 	 1,
					 glm::value_ptr(glm::ivec2(viewport_w, viewport_h)));

	 glfwSwapInterval(1);

 	ShaderMap::program_null();
	OpenGLContext::releaseContext();

	thread_cam = std::thread(&CameraTexture::camCapture, this);

}

void CameraTexture::stop()   {
	this->stop_capture = true;
}

void CameraTexture::join()    {
	this->thread_cam.join();
}

void CameraTexture::camCapture() {
	while (!stop_capture) {
		cam_capture >> cam_buffer_tmp;
		//if (cam_buffer.empty())continue;
		std::lock_guard<std::mutex> lk (mutex_cam_buffer);
		cam_buffer_tmp.copyTo(cam_buffer);
		if(animation->getStatus() > 0) {
			cv::cvtColor(cam_buffer_tmp, cam_buffer_tmp, cv::COLOR_BGR2GRAY);
			cv::GaussianBlur(cam_buffer_tmp, cam_buffer_tmp,cv::Size(5, 5),0);
			cv::Canny(cam_buffer_tmp, canny_buffer, 5, 25);
		}
		dataReady = true;
	}
}

void CameraTexture::updateTexture()  {
	std::unique_lock<std::mutex> lck(mutex_cam_buffer,std::defer_lock);
	if(dataReady  && lck.try_lock()) {
		glBindTexture(GL_TEXTURE_2D, textures[T_CAM]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_w, frame_h, GL_BGR, GL_UNSIGNED_BYTE, cam_buffer.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if(animation->getStatus() > 0) {
			glBindTexture(GL_TEXTURE_2D, textures[T_CANNY]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_w, frame_h, GL_RED, GL_UNSIGNED_BYTE, canny_buffer.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		dataReady = false;
	}
}

void CameraTexture::display(double deltaTime)  {

	OpenGLContext::makecurrent(TEXTURE_CAM);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderMap::useProgram(TEXTURE_CAM);

	updateTexture();

	animation->animation(deltaTime);
	animation->sendStatus();

	glBindVertexArray(vaos[V]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[T_CAM]);

	animation->getCamera()->draw();

	if(animation->getStatus() > 0) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textures[T_CANNY]);
		animation->getCanny()->draw();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, T_NULL);

	glBindVertexArray(0);

	ShaderMap::program_null();

	OpenGLContext::swapBuffers();

	OpenGLContext::releaseContext();
}



