#pragma once

#ifndef INSTANT_CLASS
#define INSTANT_CLASS

#include <string>
#include <vector>

class Instant
{

	/*
		Signature's Instant representation as parameters

		Author: Nicola Oreste Fonseca
		Created on: 15 December 2017
	*/

public:
	/*
		Contructor for a Signature's Instant
		It converts the Signature's Data in a series of parameters

		x0 Position x at t0
		y0 Position y at t0
		t0 Current Timestamp
		p0 Pressure at t0

		x1 Position x at t1 > t0
		y1 Position y at t1 > t0
		t1 Next Timestamp



		If last instant of time sequence use previous instant as t1
		
		Example:
		Time sequence:
			x = 1; y = 1; t = 10; p = 20;
			x = 4; y = 3; t = 20; p = 50;
			x = 8; y = 1; t = 30; p = 10;

		Declarations:
		Instant i0 = Instant(1, 1, 10, 20, 4, 3, 20);
		Instant i1 = Instant(4, 3, 20, 50, 8, 1, 30);
		Instant i2 = Instant(8, 1, 30, 10, 4, 3, 20);
	*/
	Instant(int x0, int y0, int t0, int p0, int x1, int y1, int t1);
	/*
		Construct an empty Instant for zero padding
	*/
	Instant();
	//	Deconstructor for a Signature's Instant
	~Instant();

	/*
		Returns a string containing displacement,
		velocity, acceleration and pressure
		separated by comma

		Example "1, 1, 1, 5"
	*/
	std::string to_string();

	// Returns Position X
	int get_x();
	// Returns Position Y
	int get_y();
	// Returns the Instantaneous Displacement
	float get_displacement();
	// Returns the Instantaneous Velocity
	float get_velocity();
	// Returns the Instantaneous Acceleration
	float get_acceleration();
	// Returns the Instantaneous Pressure
	float get_pressure();
	// Returns timestamp
	int get_timestamp();
	/*
		Normalizes the values

		Array order is {DISPLACEMENT, VELOCITY, ACCELERATION, PRESSURE}

		min - Vector containg the min value of each parameter
		max - Vector containg the max value of each parameter
	*/
	void normalize_values(std::vector<float> min, std::vector<float> max);

private:
	// Position X of Point, used for plotting
	int x;
	// Position Y of Point, used for plotting
	int y;
	// Instantaneous Displacement
	float displacement;
	// Instantaneous Velocity
	float velocity;
	// Instantaneous Acceleration
	float acceleration;
	// Instantaneous pressure
	float pressure;
	// Instant Time
	int timestamp;
};

#endif //INSTANT_CLASS