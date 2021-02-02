/*
 * Character.h
 *
 *  Created on: Dec 13, 2020
 *      Author: gio
 */

#ifndef TEXT_CHARACTER_H_
#define TEXT_CHARACTER_H_

#include "../../context_util/util.h"

class Character {
private:

	char ID;
	glm::ivec2 dimension;
	glm::ivec2 bearing;
	unsigned int advance;
	unsigned char* bitmapBuffer;
	size_t texture_offset;

	Character(const Character &other) = delete;
	Character(Character &&other) = delete;
	Character& operator=(const Character &other) = delete;
	Character& operator=(Character &&other) = delete;

public:
	Character(char id) {
		this->ID = id;
		this->dimension = glm::vec2(0.0f,0.0f);
		this->bearing = glm::vec2(0.0f,0.0f);
		this->advance = 0;
		this->bitmapBuffer = 0;
		this->texture_offset = 0;
	}

	virtual ~Character() {}

	char getChar() {
		return ID;
	}

	glm::ivec2 getDimension() {
		return this->dimension;
	}
	void setDimesion(glm::ivec2 dimension){
		this->dimension = dimension;
	}
	size_t getSize()  {
		return size_t(this->dimension.x * this->dimension.y);
	}
	int getW() {
		return this->dimension.x;
	}
	int getH()  {
		return this->dimension.y;
	}

	glm::ivec2 getBearing() {
		return this->bearing;
	}
	void setBearing(glm::ivec2 bearing) {
		this->bearing = bearing;
	}

	int getOriginOffset()  {
		return this->getH() - this->getBearing().y;
	}

	unsigned int getAdvance() {
		return this->advance;
	}
	void setAdvance(unsigned int advance) {
		this->advance = advance;
	}

	size_t getTextureOffset()  {
		return this->texture_offset;
	}
	void setTextureOffset(size_t offset)   {
		this->texture_offset = offset;
	}

	void copyBitmapBuffer(unsigned char* buffer, size_t size) {
	    bitmapBuffer = (unsigned char*)malloc(size);
	    memcpy(bitmapBuffer, buffer, size);
	}
	unsigned char* getBitmapBuffer() {
	    return bitmapBuffer;
	}
	void freeBitmapBuffer() {
	    if(bitmapBuffer) free(bitmapBuffer);
	}
};

#endif /* TEXT_CHARACTER_H_ */
