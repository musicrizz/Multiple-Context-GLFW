//============================================================================
// Author      : Giovanni Grandinetti
// Version     : 0.1
// Copyright   : Giovanni Grandinetti
//============================================================================
/*
 * TempoMap.h
 *
 *  Created on: Nov 29, 2020
 *      Author: gio
 */
#pragma once
#ifndef OPENGL_CONTEXT_UTIL_TEMPOMAP_H_
#define OPENGL_CONTEXT_UTIL_TEMPOMAP_H_

#include <string>
#include <unordered_map>

#include "Tempo.h"

/*
 * Map of time point Tempo
 */
class TempoMap {
private:

	static std::unordered_map<std::string, Tempo*> map;

	TempoMap();
	TempoMap(const TempoMap &other) = delete;
	TempoMap(TempoMap &&other) = delete;
	TempoMap& operator=(const TempoMap &other) = delete;
	TempoMap& operator=(TempoMap &&other) = delete;

public:

	virtual ~TempoMap();

	static void create(std::string);

	static Tempo* get(std::string);

	static void erase(std::string name);

	static void updateStart(std::string);

	static void updateCurrent(std::string);

	static long long int getElapsedNano(std::string);

	static long long int getElapsedMicro(std::string);

	static long long int getElapsedMill(std::string);

	static long long int getElapsedSeconds(std::string);

	static double getElapsedMillToSec(std::string);
};

#endif /* OPENGL_CONTEXT_UTIL_TEMPOMAP_H_ */
