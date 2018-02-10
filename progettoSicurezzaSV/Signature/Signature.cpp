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

/*
	Checks if min values has to be updated

	min - Vector containing current minimum
	instant - Instant to check
*/
std::vector<float> check_min(std::vector<float> min, Instant instant) {
	enum parameters : int { DISPLACEMENT, VELOCITY, ACCELERATION, PRESSURE };

	if (min.at(DISPLACEMENT) > instant.get_displacement()) {
		min.at(DISPLACEMENT) = instant.get_displacement();
	}
	if (min.at(VELOCITY) > instant.get_velocity()) {
		min.at(VELOCITY) = instant.get_velocity();
	}
	if (min.at(ACCELERATION) > instant.get_acceleration()) {
		min.at(ACCELERATION) = instant.get_acceleration();
	}
	if (min.at(PRESSURE) > instant.get_pressure()) {
		min.at(PRESSURE) = instant.get_pressure();
	}

	return min;
}

/*
	Checks if max values has to be updated

	max - Vector containing current maximums
	instant - Instant to check
*/
std::vector<float> check_max(std::vector<float> max, Instant instant) {
	enum parameters : int { DISPLACEMENT, VELOCITY, ACCELERATION, PRESSURE };

	if (max.at(DISPLACEMENT) < instant.get_displacement()) {
		max.at(DISPLACEMENT) = instant.get_displacement();
	}
	if (max.at(VELOCITY) < instant.get_velocity()) {
		max.at(VELOCITY) = instant.get_velocity();
	}
	if (max.at(ACCELERATION) < instant.get_acceleration()) {
		max.at(ACCELERATION) = instant.get_acceleration();
	}
	if (max.at(PRESSURE) < instant.get_pressure()) {
		max.at(PRESSURE) = instant.get_pressure();
	}

	return max;
}

void Signature::normalize() {
	const int PARAM_COUNT = 4;
	const int FIRST = 0;

	std::vector<float> min;
	std::vector<float> max;

	//Reserve space for PARAM_COUNT parameters
	min.reserve(PARAM_COUNT);
	max.reserve(PARAM_COUNT);

	//Set FIRST element as first minimum
	min.push_back( this->time_sequence.at(FIRST).get_displacement() );
	min.push_back( this->time_sequence.at(FIRST).get_velocity() );
	min.push_back( this->time_sequence.at(FIRST).get_acceleration() );
	min.push_back( this->time_sequence.at(FIRST).get_pressure() );
	//Set FIRST element as first maximum
	max.push_back(this->time_sequence.at(FIRST).get_displacement());
	max.push_back(this->time_sequence.at(FIRST).get_velocity());
	max.push_back(this->time_sequence.at(FIRST).get_acceleration());
	max.push_back(this->time_sequence.at(FIRST).get_pressure());

	//For each Instant
	for (int i = 1; i < this->time_sequence.size(); i++) {
		//Alias instant for better readability
		Instant& instant = this->time_sequence.at(i);

		//Check if current instant value is lower than current minimum
		min = check_min(min, instant);
		//Check if current instant value is lower than current maximum
		max = check_max(max, instant);
	}

	//Normalize values
	for (int i = 1; i < this->time_sequence.size(); i++) {
		this->time_sequence.at(i).normalize_values(min, max);
	}
}
