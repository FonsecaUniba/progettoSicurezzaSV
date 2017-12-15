#pragma once

using namespace std;

class Instant
{

	/*
		Signature's Instant representation as parameters
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
	*/
	Instant(int x0, int y0, int t0, int p0, int x1, int y1, int t1);
	//	Deconstructor for a Signature's Instant
	~Instant();

	// Returns the Instantaneous Displacement
	float get_displacement();
	// Returns the Instantaneous Velocity
	float get_velocity();
	// Returns the Instantaneous Acceleration
	float get_acceleration();
	// Returns the Instantaneous Pressure
	int get_pressure();

private:
	// Instantaneous Displacement
	float displacement;
	// Instantaneous Velocity
	float velocity;
	// Instantaneous Acceleration
	float acceleration;
	// Instantaneous pressure
	int pressure;
};

