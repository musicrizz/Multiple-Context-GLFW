//============================================================================
// Author      : Giovanni Grandinetti
// Version     : 0.1
// Copyright   : Giovanni Grandinetti
//============================================================================
/*
 * ShaderMap.cpp
 *
 *  Created on: Nov 21, 2020
 *      Author: gio
 */

#include "ShaderMap.h"

unsigned int ShaderMap::current { 0 };
std::unordered_map<std::string, unsigned int> ShaderMap::map;
ShaderMap::ShaderMap() {
}

ShaderMap::~ShaderMap() {
	current = 0;
	for(auto i : map)  {
		glDeleteProgram(i.second);
	}
	map.clear();
}

bool ShaderMap::createProgram(std::string name, const char *vertexPath, const char *fragmentPath) {
	try {
		Program p(name, vertexPath, fragmentPath);
		if (p.getID() > 0) {
			map.insert(std::pair<std::string, unsigned int>(name, p.getID()));
			return true;
		}
	} catch (ShaderException &e) {
		fprintf(stderr,"Create Program %s ShaderException: \n %s \n", name.c_str(), e.what());
		// throw
	}

	return false;
}

void ShaderMap::useProgram(std::string name)  {
	unsigned int pgr = getProgram(name);
	if(pgr > 0) {
		glUseProgram(pgr);
		current = pgr;
	}
}

unsigned int ShaderMap::getProgram(std::string name)  {
	try {
		return map.at(name);
	} catch (const std::out_of_range &oor) {}
	return 0;

	/*if(map.empty())return 0;
	std::unordered_map<std::string, unsigned int>::const_iterator it = map.find(name);
	if(it != map.end())   {
		return it->second;
	}
	return 0;*/
}

void ShaderMap::useProgram(int p)  {
	glUseProgram(p);
}

unsigned int ShaderMap::getCurrentProgram()  {
	return current;
}

int ShaderMap::getCountPrograms()  {
	return map.size();
}

int ShaderMap::getUniformLocation(std::string program, std::string var_name)   {
	int p = getProgram(program);
	if(p)  {
		return glGetUniformLocation(p, var_name.c_str());
	}
	return -1;
}

int ShaderMap::getUniformLocation(std::string var_name)   {
	if(current)  {
		return glGetUniformLocation(current, var_name.c_str());
	}
	return -1;
}

int ShaderMap::getUniformBlockIndex(std::string program, std::string var_name) {
	int p = getProgram(program);
	if (p) {
		return glGetUniformBlockIndex(p, var_name.c_str());
	}
	return -1;
}

int ShaderMap::getUniformBlockIndex(std::string var_name) {
	if (current) {
		return glGetUniformBlockIndex(current, var_name.c_str());
	}
	return -1;
}

void ShaderMap::bindingUniformBlocks(std::string var_name, unsigned int biding_point)   {
	for(auto p : map)  {
		glUniformBlockBinding(p.second, getUniformBlockIndex(p.first, var_name), biding_point);
	}
}


int ShaderMap::getAttributeLocation(std::string program, std::string var_name) {
	int p = getProgram(program);
	if (p) {
		return glGetAttribLocation(p, var_name.c_str());
	}
	return -1;
}

int ShaderMap::getAttributeLocation(std::string var_name) {
	if (current) {
		return glGetAttribLocation(current, var_name.c_str());
	}
	return -1;
}


