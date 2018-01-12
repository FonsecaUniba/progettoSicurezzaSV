#include "stdafx.h"
#include "InstantTest.h"

/*
x0 Position x at t0
y0 Position y at t0
t0 Current Timestamp
p0 Pressure at t0

x1 Position x at t1 > t0
y1 Position y at t1 > t0
t1 Next Timestamp
*/

using namespace std;

void test_instant() {
	// Function to test
	Instant test_i = Instant(1, 1, 10, 20, 2, 2, 20);

	const double EXPECTED_DISPLACEMENT = sqrt(2);
	const double EXPECTED_VELOCITY = sqrt(0.02);
	const double EXPECTED_ACCELERATION = sqrt(0.0002);
	const int EXPECTED_PRESSURE = 20;
	const int EXPECTED_TIMESTAMP = 10;

	double returned_displacement = test_i.get_displacement();
	double returned_velocity = test_i.get_velocity();
	double returned_acceleration = test_i.get_acceleration();
	int returned_pressure = test_i.get_pressure();
	int returned_timestamp = test_i.get_timestamp();

	// Checks expected values
	bool displacement_check = ( abs( EXPECTED_DISPLACEMENT - returned_displacement ) <= 1e-6 );
	bool velocity_check = ( abs( EXPECTED_VELOCITY - returned_velocity ) <= 1e-6 );
	bool acceleration_check = ( abs( EXPECTED_ACCELERATION - returned_acceleration ) <= 1e-6 );
	bool pressure_check( EXPECTED_PRESSURE == returned_pressure );
	bool timestamp_check( EXPECTED_TIMESTAMP == returned_timestamp );

	// If all values are as expected
	if (displacement_check && velocity_check && acceleration_check && pressure_check && timestamp_check) {
		cout << "InstantTest::test_instant Everything OK" << endl << endl;
	}
	else {
		// Prints which values where different
		if (!displacement_check) {
			cout << "InstantTest::test_instant Displacement Mismatch" << endl;
			cout << "Expected " << EXPECTED_DISPLACEMENT << " but got " << returned_displacement << endl << endl;
		}
		if (!velocity_check) {
			cout << "InstantTest::test_instant Velocity Mismatch" << endl;
			cout << "Expected " << EXPECTED_VELOCITY << " but got " << returned_velocity << endl << endl;
		}
		if (!acceleration_check) {
			cout << "InstantTest::test_instant Acceleration Mismatch" << endl;
			cout << "Expected " << EXPECTED_ACCELERATION << " but got " << returned_acceleration << endl << endl;
		}
		if (!pressure_check) {
			cout << "InstantTest::test_instant Pressure Mismatch" << endl;
			cout << "Expected " << EXPECTED_PRESSURE << " but got " << returned_pressure << endl << endl;
		}
		if (!timestamp_check) {
			cout << "InstantTest::test_instant Timestamp Mismatch" << endl;
			cout << "Expected " << EXPECTED_TIMESTAMP << " but got " << returned_timestamp << endl << endl;
		}
	}
}

void test_instant_module() {
	test_instant();
}