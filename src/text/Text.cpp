/*
 * Text.cpp
 *
 *  Created on: Dec 13, 2020
 *      Author: gio
 */

#include "Text.h"
#include "../../lib/logger/easylogging++.h"

Text* Text::instance = nullptr;
std::once_flag Text::instance_flag;

Text* Text::getInstance()  {
	std::call_once(instance_flag, []()-> Text* {return instance = new Text();});
	return instance;
}

Text::Text() { // @suppress("Class members should be properly initialized")

	//Initialize maps of Character
	FT_Library library;
	FT_Face face;

	if (FT_Init_FreeType(&library))  {
		LOG(ERROR) << "ERROR::FREETYPE: Could not init FreeType Library";
	}else{
		LOG(DEBUG) <<"FREE TYPE INITIALIZED";
	}

	if (FT_New_Face(library, "resources/font/UbuntuMono-R.ttf", 0, &face))   {
		LOG(ERROR) << "ERROR::FREETYPE: Failed to load font";
	}else{
		LOG(DEBUG) <<"FACES LOADED";
	}

	LOG(DEBUG)<<"face->family_name : "<<face->family_name;
	LOG(DEBUG)<<"face->num_glyphs : "<<face->num_glyphs;

	FT_Set_Pixel_Sizes(face, 0, 48);

	this->pixel_unpack_bff_size = 0;
	this->character_advance_max = 0; // in this case we use monospace the advance is the same for all
	for(unsigned char c = 33; c < 126; c++)  {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			LOG(ERROR) << "ERROR::FREETYTPE: Failed to load Glyph "<< c << "\n";
		    continue;
		}

		Character* C = new Character(c);
		C->setDimesion(glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows));
		C->setBearing(glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top));
		C->setAdvance(face->glyph->advance.x);

		if(C->getDimension().x == 0 || C->getDimension().y == 0)continue;

		C->copyBitmapBuffer(face->glyph->bitmap.buffer, C->getSize());

		this->pixel_unpack_bff_size += C->getSize();
		(C->getAdvance() > this->character_advance_max) ? this->character_advance_max = C->getAdvance() : NULL;

		character_map.insert(std::pair<char, Character*>(c, C));
		//LOG(DEBUG)<<"Caratter loaded -> :"<<c<<" , w:"<<C->getDimension().x<<", h:"<<C->getDimension().y<<", adv:"<<C->getAdvance()/64;
	}

	LOG(DEBUG)<<OpenGLerror::check("Caricamento mappa caratteri ")<<"tot size = "<<this->pixel_unpack_bff_size;

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	//---------------------------------------------
	//---------------------------------------------

	OpenGLContext::createContext(TEXT_RENDER, 900, 400, NULL, true, false, true);
	OpenGLContext::makecurrent(TEXT_RENDER);

	OpenGLContext::setReshape([](GLFWwindow* window, int width, int height){
		Text* p = (Text*)OpenGLContext::getUserPointer();
		p->viewport_w = width;
		p->viewport_h = height;
		p->flag_viewpot = true;
		p->flag_projection = true;
		p->initCharModelLine();
	});

	OpenGLContext::setKeyboard([](GLFWwindow* window, int key, int scancode, int action, int mods ){
		switch (key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS) {
				OpenGLContext::setWindowShouldClose(GL_TRUE);
			}
			break;
		case GLFW_KEY_SPACE:
			if (action == GLFW_PRESS) {
				GLFWmonitor *primary = glfwGetPrimaryMonitor();
				const GLFWvidmode *mode = glfwGetVideoMode(primary);
				glfwSetWindowMonitor(window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
			}
			break;
		}
	});

	ShaderMap::createProgram(TEXT_RENDER, "resources/shaders/TextRendering_vertx.glsl", "resources/shaders/TextRendering_frag.glsl");

	glGenVertexArrays(VAOS_NUM, vaos);
	glGenBuffers(BUFFERS_NUM, buffers);
	glGenTextures(TEXTURES_NUM, textures);

	//load Characters bitmap into  PIXEL_UNPACK_BUFFER
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffers[B_PIXEL_UNPAK]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, pixel_unpack_bff_size, NULL, GL_STATIC_DRAW);
	int offset = 0;
	for(auto c : character_map)  {
		glBufferSubData(GL_PIXEL_UNPACK_BUFFER, offset, c.second->getSize(), c.second->getBitmapBuffer());
		c.second->setTextureOffset(offset);
		c.second->freeBitmapBuffer();
		offset+=c.second->getSize();
	}
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	LOG(DEBUG)<<OpenGLerror::check("GL_PIXEL_UNPACK_BUFFER loaded");


	//load Texture Coordinate
	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 8, NULL, GL_DYNAMIC_DRAW);
	glm::vec2 *v = (glm::vec2*) glMapBufferRange(GL_ARRAY_BUFFER, buffer_offset, buffer_offset, GL_MAP_WRITE_BIT);
	// UV COORD
		*v++ = glm::vec2(0.0f, 1.0f);
		*v++ = glm::vec2(1.0f, 1.0f);
		*v++ = glm::vec2(1.0f, 0.0f);
		*v =   glm::vec2(0.0f, 0.0f);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int vertex_loc = ShaderMap::getAttributeLocation(TEXT_RENDER, "position");
	unsigned int uv_loc = ShaderMap::getAttributeLocation(TEXT_RENDER, "uv");
	glBindVertexArray(vaos[V]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
		glVertexAttribPointer(vertex_loc, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(0));
		glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(buffer_offset));
		glEnableVertexAttribArray(vertex_loc);
		glEnableVertexAttribArray(uv_loc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	LOG(INFO)<<OpenGLerror::check("Text Rendering creation vertex buffer ");

	ShaderMap::useProgram(TEXT_RENDER);

	glActiveTexture(GL_TEXTURE1);
	unsigned int sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, textures[T_TEXT]);
	glBindSampler(1, sampler);
	glUniform1i(ShaderMap::getUniformLocation("char_texture"), 1);

	loc_color = ShaderMap::getUniformLocation("color");
	loc_mtx_model = ShaderMap::getUniformLocation("mtx_model");
	loc_mtx_projection = ShaderMap::getUniformLocation("mtx_projection");

	//glUniformMatrix4fv(loc_mtx_projection, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.f, 400.f, 0.f, 400.f, -1.0f, 10.0f)));
	flag_projection = true;

	ShaderMap::useProgram(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);
	glDepthMask(true);
	glClearDepth(1.0f);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColorMask(true, true, true, true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwGetFramebufferSize(OpenGLContext::getCurrent(), &viewport_w, &viewport_h);
	flag_viewpot = true;

	OpenGLContext::setUserPointer(this);
	glfwSwapInterval(1);

	ShaderMap::useProgram(0);
	OpenGLContext::releaseContext();

	initCharModelLine();

}

Text::~Text() {
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glDeleteBuffers(BUFFERS_NUM, buffers);
	OpenGLContext::destroy(TEXT_RENDER);

	//TO DO empty maps
}

void Text::display(double deltaTime) {
	OpenGLContext::makecurrent(TEXT_RENDER);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	updateViewPort();

	ShaderMap::useProgram(TEXT_RENDER);

	updateProjectionMtx();

	render(deltaTime);

	updateCharModelLine();

	ShaderMap::useProgram(0);

	OpenGLContext::swapBuffers();
	OpenGLContext::releaseContext();

}

void Text::render(double deltaTime)  {
	glBindVertexArray(vaos[V]);

	for(auto M : char_models) {
		if (M->isRender()) {
			glUniformMatrix4fv(loc_mtx_model,1, GL_FALSE, glm::value_ptr(M->getModelMtx()));
			glUniform4fv(loc_color,1,glm::value_ptr(M->getColor()));
			render(M->getC());
			if(M->isTail())  {
				for(auto T : M->getTail()) {
					glUniformMatrix4fv(loc_mtx_model,1, GL_FALSE, glm::value_ptr(T->getModelMtx()));
					glUniform4fv(loc_color,1,glm::value_ptr(T->getColor()));
					render(T->getC());
				}
			}
			M->update(deltaTime);
		}
	}

	glBindVertexArray(0);
}

void Text::render(Character* C) {

	glBindBuffer(GL_ARRAY_BUFFER, buffers[B_VERTEX]);
	//             VERTEX
	glm::vec2 *v = (glm::vec2*) glMapBufferRange(GL_ARRAY_BUFFER, 0, buffer_offset, GL_MAP_WRITE_BIT);
		*v++ = glm::vec2(0.0f, 0.0f);
		*v++ = glm::vec2(float(C->getW()), 0.0f);
		*v++ = glm::vec2(float(C->getW()), float(C->getH()));
		*v   = glm::vec2(0.0f,  float(C->getH()));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffers[B_PIXEL_UNPAK]);
	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[T_TEXT]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,C->getW(), C->getH(),0, GL_RED, GL_UNSIGNED_BYTE,
								reinterpret_cast<void*>(C->getTextureOffset()));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

}

void Text::updateViewPort()  {
	if(flag_viewpot)  {
		glViewport(0, 0, viewport_w, viewport_h);
		flag_viewpot = false;
	}
}

void Text::updateProjectionMtx() {
	if(flag_projection)  {
		glUniformMatrix4fv(loc_mtx_projection, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.f, float(viewport_w), 0.f, float(viewport_h), -1.0f, 10.0f)));
		flag_projection = false;
	}
}

void Text::initCharModelLine() {

	char_models.clear();

	float X = (character_advance_max/64) / 2.0f;
	int num_colon = (viewport_w / (character_advance_max/64)) - 1;
	LOG(INFO)<<"tot num_colon : "<<num_colon;

	Character* C;
	CharModel* M;
	for (int i = 0; i < num_colon; i++) {
		C = character_map.at(randomChar());
		M = new CharModel(C);
		M->setPosition(glm::vec3(X, float(viewport_h), 0.0f));
		M->addPositionX(C->getBearing().x);
		M->addPositionY(float(-C->getOriginOffset()));
		M->setRender(casuality());
		X += float(character_advance_max) / 64.0f;
		char_models.push_back(M);
	}

}

void Text::updateCharModelLine()   {
	if (TempoMap::getElapsedSeconds("CHAR_MODEL_LINE") > 1) {

		Character* C;
		for(auto M : char_models) {
			if( !M->isRender() && casuality() ) {
				C = character_map.at(randomChar());
				M->addPositionX(-M->getC()->getBearing().x);
				M->setC(C);
				M->addPositionX(C->getBearing().x);
				M->setPositionY(float(viewport_h));
				M->addPositionY(float(-C->getOriginOffset()));
				M->resetBaseParameter();

				if(casuality())  {
					for(short i=0; i < TAIL_LENGHT; i++)  {
						random_tail[i] = character_map.at(randomChar());
					}
					M->setTail(random_tail);
				}
				M->setRender(true);
			}
		}

		TempoMap::updateStart("CHAR_MODEL_LINE");
	}

}






