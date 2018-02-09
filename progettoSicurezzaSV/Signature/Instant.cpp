#include "stdafx.h"
#include "Instant.h"
#include <math.h>

Instant::Instant(int x0, int y0, int t0, int p, int x1, int y1, int t1)
{
	// Stores 2D representation
	this->x = x0;
	this->y = y0;

	/*
		Stores the timestamp
	*/
	this->timestamp = t0;

	/*
		Computes Displacement between point A(x0,y0) and B(x1,y1)
		using the Pythagorean theorem 
	*/
	this->displacement = sqrt( pow( (x1-x0), 2) + pow( (y1-y0), 2) );
	
	/*
		Computes Velocity as Displacement/Time
	*/
	this->velocity = this->displacement/abs(t1-t0);
	
	/*
		Computes Acceleration as Velocity/Time
	*/
	this->acceleration = this->velocity/abs(t1-t0);

	/*
		Stores pressure without applying changes
	*/
	this->pressure = p;
}

Instant::~Instant()
{
	// Does Nothing
}

std::string Instant::to_string() {
	std::string result = "";
	result += std::to_string(this->displacement) + ",";
	result += std::to_string(this->velocity) + ",";
	result += std::to_string(this->acceleration) + ",";
	result += std::to_string(this->pressure);

	return result;
}

/*
	Getters Section
*/

int Instant::get_x() {
	return this->x;
}

int Instant::get_y() {
	return this->y;
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

int Instant::get_timestamp() {
	return this->timestamp;
}

void Instant::normalize_values(float min[], float max[]) {
	enum parameters : int {DISPLACEMENT, VELOCITY, ACCELERATION, PRESSURE};
	
	/*
		Normalizes parameters by substracting the min value of the parameter
		and diving the value by the difference by min and max values of the parameter
	*/
	this->displacement = (this->displacement - min[DISPLACEMENT]) / (max[DISPLACEMENT] - min[DISPLACEMENT]);
	this->velocity = (this->velocity - min[VELOCITY]) / (max[VELOCITY] - min[VELOCITY]);
	this->acceleration = (this->acceleration - min[ACCELERATION]) / (max[ACCELERATION] - min[ACCELERATION]);
	this->pressure = (this->pressure - min[PRESSURE]) / (max[PRESSURE] - min[PRESSURE]);
}