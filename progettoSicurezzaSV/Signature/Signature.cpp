#include "stdafx.h"
#include "Signature.h"


Signature::Signature()
{
	// Does Nothing
}


Signature::~Signature()
{
	this->time_sequence.clear();
}

void Signature::normalize() {
	enum parameters : int { DISPLACEMENT, VELOCITY, ACCELERATION, PRESSURE };
	const int PARAM_COUNT = 4;

	float min[PARAM_COUNT];
	float max[PARAM_COUNT];

	min[DISPLACEMENT] = this->time_sequence.at(0).get_displacement();
	min[VELOCITY] = this->time_sequence.at(0).get_velocity();
	min[ACCELERATION] = this->time_sequence.at(0).get_acceleration();
	min[PRESSURE] = this->time_sequence.at(0).get_pressure();

	max[DISPLACEMENT] = this->time_sequence.at(0).get_displacement();
	max[VELOCITY] = this->time_sequence.at(0).get_velocity();
	max[ACCELERATION] = this->time_sequence.at(0).get_acceleration();
	max[PRESSURE] = this->time_sequence.at(0).get_pressure();

	//For each Instant
	for (int i = 1; i < this->time_sequence.size(); i++) {
		//Alias instant for better readability
		Instant& instant = this->time_sequence.at(i);

		//Check if current instant value is lower than current minimum
		if (min[DISPLACEMENT] > instant.get_displacement()) {
			min[DISPLACEMENT] = instant.get_displacement();
		}
		if (min[VELOCITY] > instant.get_velocity()) {
			min[VELOCITY] = instant.get_velocity();
		}
		if (min[ACCELERATION] > instant.get_acceleration()) {
			min[ACCELERATION] = instant.get_acceleration();
		}
		if (min[PRESSURE] > instant.get_pressure()) {
			min[PRESSURE] = instant.get_pressure();
		}

		//Check if current instant value is lower than current maximum
		if (max[DISPLACEMENT] < instant.get_displacement()) {
			max[DISPLACEMENT] = instant.get_displacement();
		}
		if (max[VELOCITY] < instant.get_velocity()) {
			max[VELOCITY] = instant.get_velocity();
		}
		if (max[ACCELERATION] < instant.get_acceleration()) {
			max[ACCELERATION] = instant.get_acceleration();
		}
		if (max[PRESSURE] < instant.get_pressure()) {
			max[PRESSURE] = instant.get_pressure();
		}
	}

	//Normalize values
	for (int i = 1; i < this->time_sequence.size(); i++) {
		this->time_sequence.at(i).normalize_values(min, max);
	}
}
