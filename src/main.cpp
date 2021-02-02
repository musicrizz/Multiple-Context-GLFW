//============================================================================
// Name        : Texture_1_OpenCV.cpp
// Author      : Giovanni Grandinetti
// Version     :
// Copyright   : Giovanni Grandinetti
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../context_util/util.h"
#include "../lib/logger/easylogging++.h"
#include "../lib/image/stb_image.h"
#include "Constants.h"
#include "simpleTriangle/SimpleTriangle.h"
#include "cameraTexture/CameraTexture.h"
#include "pointSprite/PointSprite.h"
#include "text/Text.h"

#define ELPP_THREAD_SAFE
INITIALIZE_EASYLOGGINGPP

using namespace std;

void glfw_error_callback(int error, const char *description);

int main(int argc, char **argv) {

	START_EASYLOGGINGPP(argc, argv);
	el::Configurations log_conf("conf/logger.conf");
	el::Loggers::reconfigureLogger("default", log_conf);
	el::Loggers::reconfigureAllLoggers(log_conf);

	if (!glfwInit()) {
		fprintf(stderr, "GLEW INTI ERROR");
		exit(EXIT_FAILURE);
	}

	srand (time(NULL));

	LOG(INFO)<<"GLFW VERSION : "<<glfwGetVersionString();

	glfwSetErrorCallback(glfw_error_callback);

	GLFWmonitor *primary = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(primary);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 0);

	//To enable set DEVELOPEMENT 1 in contex_util/CommonHeaders
#if DEVELOPEMENT
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	SimpleTriangle* simpleTriangle = SimpleTriangle::getInstance();
	PointSprite* pointSprite = PointSprite::getInstance();
	CameraTexture* cameraTexture = CameraTexture::getInstance();
	Text* textRender = Text::getInstance();

	OpenGLContext::setWindowPosition(SIMPLE_TRIANGLE, 50, 50);
	OpenGLContext::setWindowPosition(POINT_SPRITE, 0, 880);
	OpenGLContext::setWindowPosition(TEXTURE_CAM, 510, 50);
	OpenGLContext::setWindowPosition(TEXT_RENDER, 510, 750);

	int frames = 0;
	int fps_time = 5;
	double delta = .0f;

	while (!glfwWindowShouldClose(OpenGLContext::getCurrent())) {

		if (TempoMap::getElapsedMill(FPS_MAX) >= fps_time) {

			TempoMap::updateStart(DELTA_TIME);

			simpleTriangle->display();

			pointSprite->display();

			cameraTexture->display(delta);

			textRender->display(delta);

			/*Make Current the contex you want to capture callbacks events
			 * like keyboard, mouse ecc...
			 */
			OpenGLContext::makecurrent(TEXT_RENDER);

			frames++;

			TempoMap::updateStart(FPS_MAX);

			delta = TempoMap::getElapsedMillToSec(DELTA_TIME);
		}

		glfwPollEvents(); //  It MUST be in the main thread

		if (TempoMap::getElapsedSeconds("MoveSpriteWindow") >= 1) {
			float fac = float(TempoMap::getElapsedMill("Tween"))/15000.f * 1320.f;
			OpenGLContext::setWindowPosition(POINT_SPRITE,
					int(std::min(1320.0f, fac)) ,
					880);
		}

//		if (TempoMap::getElapsedSeconds(DEBUG_TIME) >= 1) {
//			LOG(INFO) << "Frame per second : " << frames<< ", fps_time :"<<fps_time <<" , Delta time : "<<delta;
//			frames = 0;
//			TempoMap::updateStart(DEBUG_TIME);
//		}

		this_thread::sleep_for(chrono::milliseconds(1));
	}
	cameraTexture->stop();
	cameraTexture->join();

	OpenGLContext::destroyAll();

	glfwTerminate();

	fprintf(stdout, " MAIN FINISH .");
	exit(EXIT_SUCCESS);
}

void glfw_error_callback(int error, const char *description) {
	fprintf(stderr, "GLFW_ERROR: %u :  %s ", error, description);
}
