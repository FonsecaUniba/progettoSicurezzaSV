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
	this->displacement = (float) sqrt( pow( (x1-x0), 2) + pow( (y1-y0), 2) );
	
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
	this->pressure = (float) p;
}

Instant::Instant() {
	// Stores 2D representation
	this->x = 0;
	this->y = 0;

	/*
	Stores the timestamp
	*/
	this->timestamp = 0;

	/*
	Computes Displacement between point A(x0,y0) and B(x1,y1)
	using the Pythagorean theorem
	*/
	this->displacement = 0.0;

	/*
	Computes Velocity as Displacement/Time
	*/
	this->velocity = 0.0;

	/*
	Computes Acceleration as Velocity/Time
	*/
	this->acceleration = 0.0;

	/*
	Stores pressure without applying changes
	*/
	this->pressure = 0.0;
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

float Instant::get_pressure() {
	return this->pressure;
}

int Instant::get_timestamp() {
	return this->timestamp;
}

void Instant::normalize_values(std::vector<float> min, std::vector<float> max) {
	enum parameters : int {DISPLACEMENT, VELOCITY, ACCELERATION, PRESSURE};
	
	/*
		Normalizes parameters by substracting the min value of the parameter
		and diving the value by the difference by min and max values of the parameter
	*/
	this->displacement = (this->displacement - min.at(DISPLACEMENT)) / (max.at(DISPLACEMENT) - min.at(DISPLACEMENT));
	this->velocity = (this->velocity - min.at(VELOCITY)) / (max.at(VELOCITY) - min.at(VELOCITY));
	this->acceleration = (this->acceleration - min.at(ACCELERATION)) / (max.at(ACCELERATION) - min.at(ACCELERATION));
	this->pressure = (this->pressure - min.at(PRESSURE)) / (max.at(PRESSURE) - min.at(PRESSURE));
}