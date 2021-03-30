/*
 * SimpleTriangle.cpp
 *
 *  Created on: Dec 5, 2020
 *      Author: gio
 */

#include "SimpleTriangle.h"
#include "../../lib/logger/easylogging++.h"

std::once_flag SimpleTriangle::instance_flag;
SimpleTriangle* SimpleTriangle::instance = nullptr;

SimpleTriangle::SimpleTriangle() {
	if(OpenGLContext::getContext(SIMPLE_TRIANGLE) != nullptr) return;

	OpenGLContext::createContext(SIMPLE_TRIANGLE, 300, 300, NULL, true, false, true);

	OpenGLContext::makecurrent(SIMPLE_TRIANGLE);

	ShaderMap::createProgram(SIMPLE_TRIANGLE, "resources/shaders/simpleTriangle_vrtx.glsl", "resources/shaders/simpleTriangle_frag.glsl");

	LOG(INFO)<<OpenGLerror::check("Simple triangle creation simple program");

	glGenBuffers(BUFFERS_NUM, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*6, NULL, GL_STATIC_DRAW);
	glm::vec4* v = (glm::vec4*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
		*v++ = glm::vec4(-0.8f, -0.8f, 0.0f, 1.0f);
		*v++ = glm::vec4( 0.8f, -0.8f, 0.0f, 1.0f);
		*v++ = glm::vec4( 0.0f,  0.8f, 0.0f, 1.0f);
		*v++ = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		*v++ = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		*v   = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LOG(INFO)<<OpenGLerror::check("Simple triangle creation buffer vertex");

	glGenVertexArrays(VAOS_NUM, vaos);
	unsigned int vertex_loc = ShaderMap::getAttributeLocation(SIMPLE_TRIANGLE, "position");
	unsigned int color_loc = ShaderMap::getAttributeLocation(SIMPLE_TRIANGLE, "color");
	glBindVertexArray(vaos[V]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
		glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), reinterpret_cast<void*>(0));
		glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), reinterpret_cast<void*>(sizeof(glm::vec4)*3));
		glEnableVertexAttribArray(vertex_loc);
		glEnableVertexAttribArray(color_loc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	LOG(INFO)<<OpenGLerror::check("Simple triangle creation buffer VAO");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);
	glDepthMask(true);
	glClearDepth(1.0f);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColorMask(true, true, true, true);

	glfwGetFramebufferSize(OpenGLContext::getCurrent(), &w, &h);
	aspect = float(h)/float(w);

	OpenGLContext::setUserPointer(this);
	glfwSwapInterval(1);

	ShaderMap::useProgram(0);
	OpenGLContext::releaseContext();

}

SimpleTriangle* SimpleTriangle::getInstance()   {
	std::call_once(instance_flag, []()-> SimpleTriangle* {return instance = new SimpleTriangle();});
	return instance;
}

void SimpleTriangle::display()  {
	OpenGLContext::makecurrent(SIMPLE_TRIANGLE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w, h);

	ShaderMap::useProgram(SIMPLE_TRIANGLE);

	glBindVertexArray(vaos[V]);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);

	ShaderMap::useProgram(0);

	OpenGLContext::swapBuffers();
	OpenGLContext::releaseContext();
}


SimpleTriangle::~SimpleTriangle() {
	OpenGLContext::destroy(SIMPLE_TRIANGLE);
}
