#include "stdafx.h"
#include "Instant.h"
#include <math.h>

Instant::Instant(int x0, int y0, int t0, int p, int x1, int y1, int t1)
{
	/*
		Computes Displacement between point A(x0,y0) and B(x1,y1)
		using the Pythagorean theorem 
	*/
	this->displacement = sqrt( pow( (x1-x0), 2) + pow( (y1-y0), 2) );
	
	this->velocity = this->displacement/(t1-t0);
	
	this->acceleration = this->velocity/(t1-t0);

	this->pressure = p;

}


Instant::~Instant()
{

}

float Instant::get_displacement(){
	return this->displacement;
}

float Instant::get_velocity() {
	return this->velocity;
}

float Instant::get_acceleration() {
	return this->acceleration;
}

int Instant::get_pressure() {
	return this->pressure;
}