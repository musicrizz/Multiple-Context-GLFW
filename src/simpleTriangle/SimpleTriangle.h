/*
 * SimpleTriangle.h
 *
 *  Created on: Dec 5, 2020
 *      Author: gio
 */

#ifndef SIMPLETRIANGLE_H_
#define SIMPLETRIANGLE_H_

#include "../../context_util/util.h"
#include "../IDs.h"

class SimpleTriangle {
private:

	static std::once_flag instance_flag;
	static SimpleTriangle* instance;

	int w,h;
	float aspect;

	enum VAO {V, VAOS_NUM};
	unsigned int vaos[VAOS_NUM];

	enum BUFFERS {B_VERTEX, BUFFERS_NUM};
	unsigned int buffers[BUFFERS_NUM];

	SimpleTriangle();

	void rotate();

	SimpleTriangle(const SimpleTriangle &other)=delete;
	SimpleTriangle(SimpleTriangle &&other)=delete;
	SimpleTriangle& operator=(const SimpleTriangle &other)=delete;
	SimpleTriangle& operator=(SimpleTriangle &&other)=delete;

public:

	virtual ~SimpleTriangle();

	static SimpleTriangle* getInstance();

	void display();

};

#endif /* SIMPLETRIANGLE_H_ */
