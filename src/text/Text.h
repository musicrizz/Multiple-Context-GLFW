/*
 * Text.h
 *
 *  Created on: Dec 13, 2020
 *      Author: gio
 */

#ifndef TEXT_TEXT_H_
#define TEXT_TEXT_H_

#include "../Constants.h"
#include <unordered_map>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Character.h"
#include "CharModel.h"

class Text {
private:

	static std::once_flag instance_flag;
	static Text* instance;

	int viewport_w, viewport_h;
	bool flag_viewpot = true;

	enum VAO {V, VAOS_NUM};
	unsigned int vaos[VAOS_NUM];

	enum BUFFERS {B_VERTEX, B_PIXEL_UNPAK, BUFFERS_NUM};
	unsigned int buffers[BUFFERS_NUM];
	size_t pixel_unpack_bff_size;
	size_t buffer_offset = (sizeof(glm::vec2)*4);

	enum TEXTURES {T_NULL, T_TEXT,  TEXTURES_NUM};
	unsigned int textures[TEXTURES_NUM];

	int loc_mtx_projection = -1,
		loc_mtx_model = -1,
		loc_color = -1;
	bool flag_projection = true;

	std::unordered_map<char, Character*> character_map;
	int character_advance_max;

	std::vector<CharModel*> char_models;
	Character** random_tail = new Character*[TAIL_LENGHT];

	Text();

	Text(const Text &other) = delete;
	Text(Text &&other) = delete;
	Text& operator=(const Text &other) = delete;
	Text& operator=(Text &&other) = delete;

	void render(double deltaTime);
	void render(Character* C);
	void updateViewPort();
	void updateProjectionMtx();

	//standard ASCII from 33 to 126
	short randomChar() {
		return rand()%93+33;
	}

	void initCharModelLine();
	void updateCharModelLine();

	inline bool casuality()  {
		return !((rand()%25) % 2 );
	}

public:

	static Text* getInstance();

	virtual ~Text();

	void display(double deltaTime);

};

#endif /* TEXT_TEXT_H_ */
