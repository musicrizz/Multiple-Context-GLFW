//============================================================================
// Author      : Giovanni Grandinetti
// Version     : 0.1
// Copyright   : Giovanni Grandinetti
//============================================================================
/*
 * ShaderMap.h
 *
 *  Created on: Nov 21, 2020
 *      Author: gio
 */
#pragma once
#ifndef SHADERMAP_H_
#define SHADERMAP_H_

#include <unordered_map>

#include "CommonHeaders.h"
#include "ShaderException.h"
#include "Program.h"

class ShaderMap {
private:
	static unsigned int current;
	static std::unordered_map<std::string, unsigned int> map;

	ShaderMap();
	ShaderMap(const ShaderMap &other) = delete;
	ShaderMap(ShaderMap &&other) = delete;
	ShaderMap& operator=(const ShaderMap &other) = delete;
	ShaderMap& operator=(ShaderMap &&other) = delete;
public:

	virtual ~ShaderMap();

	bool static createProgram(std::string name, const char* vertexPath, const char* fragmentPath);

	void static useProgram(std::string name);
	void static useProgram(int p);

	unsigned int static getProgram(std::string name);

	unsigned int static getCurrentProgram();

	int static getCountPrograms();

	int static getUniformLocation(std::string program, std::string var_name);
	int static getUniformLocation(std::string var_name);

	int static getUniformBlockIndex(std::string program, std::string var_name);
	int static getUniformBlockIndex(std::string var_name);

	void static bindingUniformBlocks(std::string var_name, unsigned int biding_point);

	int static getAttributeLocation(std::string program, std::string var_name);
	int static getAttributeLocation(std::string var_name);

};

#endif /* SHADERMAP_H_ */
