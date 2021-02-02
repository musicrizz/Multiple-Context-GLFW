//============================================================================
// Author      : Giovanni Grandinetti
// Version     : 0.1
// Copyright   : Giovanni Grandinetti
//============================================================================
/*
 * TempoMap.cpp
 *
 *  Created on: Nov 29, 2020
 *      Author: gio
 */

#include "TempoMap.h"

std::unordered_map<std::string, Tempo*> TempoMap::map;

TempoMap::TempoMap() {
}

TempoMap::~TempoMap() {
	for(auto t : map)  {
		if(t.second)delete(t.second);
	}
	map.clear();
}


void TempoMap::create(std::string name)  {
	map.insert(std::pair<std::string, Tempo*>(name, new Tempo()));
}

/*
 * if @name does not exist , a new Tempo is created associated with @name
 */
inline Tempo* TempoMap::get(std::string name) {
	std::unordered_map<std::string, Tempo*>::const_iterator it = map.find(name);
	if (it != map.end()) {
		return it->second;
	}else{
		Tempo* tmp = new Tempo();
		map.insert(std::pair<std::string, Tempo*>(name, tmp));
		return tmp;
	}
}

void TempoMap::updateStart(std::string name)   {
	get(name)->updateStart();
}

void TempoMap::updateCurrent(std::string name)   {
	return get(name)->updateCurrent();
}

long long int TempoMap::getElapsedNano(std::string name) {
	return get(name)->getElapsedNano();
}

long long int TempoMap::getElapsedMicro(std::string name) {
	return get(name)->getElapsedMicro();
}

long long int TempoMap::getElapsedMill(std::string name) {
	return get(name)->getElapsedMill();
}

long long int TempoMap::getElapsedSeconds(std::string name) {
	return get(name)->getElapsedSeconds();
}

double TempoMap::getElapsedMillToSec(std::string name) {
	return double(getElapsedMill(name)) / 3600.0;
}

void TempoMap::erase(std::string name) {
	std::unordered_map<std::string, Tempo*>::const_iterator it = map.find(name);
	if (it != map.end()) {
		if(it->second)delete it->second;
		map.erase(it);
	}
}



