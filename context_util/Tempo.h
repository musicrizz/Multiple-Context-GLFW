//============================================================================
// Author      : Giovanni Grandinetti
// Version     : 0.1
// Copyright   : Giovanni Grandinetti
//============================================================================
/*
 * Tempo.h
 *
 *  Created on: Nov 29, 2020
 *      Author: gio
 */
#pragma once
#ifndef OPENGL_CONTEXT_UTIL_TEMPO_H_
#define OPENGL_CONTEXT_UTIL_TEMPO_H_

#include <chrono>

/*Simple Class to create, start, update a time point
 * and get the elapsed time
 */
class Tempo {
private:

	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point current;

	Tempo(Tempo &&other) = delete;
	Tempo& operator=(const Tempo &other) = delete;
	Tempo& operator=(Tempo &&other) = delete;

public:
	Tempo() {
		start = std::chrono::steady_clock::now();
		current = std::chrono::steady_clock::now();
	}
	virtual ~Tempo() {};

	Tempo(const Tempo &other) {
		this->start = other.start;
		this->current = other.current;
	}

	std::chrono::steady_clock::time_point getStart() {
		return this->start;
	}
	std::chrono::steady_clock::time_point getCurrent() {
		return this->current;
	}

	void updateStart() {
		this->start = std::chrono::steady_clock::now();
	}

	void updateCurrent()  {
		this->current = std::chrono::steady_clock::now();;
	}

	long long int getElapsedNano()   {
		updateCurrent();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(this->current - this->start).count();
	}

	long long int getElapsedMicro() {
		updateCurrent();
		return std::chrono::duration_cast<std::chrono::microseconds>(this->current - this->start).count();
	}
	long long int getElapsedMill() {
		updateCurrent();
		return std::chrono::duration_cast<std::chrono::milliseconds>(this->current - this->start).count();
	}
	long long int getElapsedSeconds() {
		updateCurrent();
		return std::chrono::duration_cast<std::chrono::seconds>(this->current - this->start).count();
	}

};

#endif /* OPENGL_CONTEXT_UTIL_TEMPO_H_ */
