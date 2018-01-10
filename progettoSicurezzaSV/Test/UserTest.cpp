#include "stdafx.h"
#include "UserTest.h"

void test_user() {
	const int EXPECTED_ID = 1;
	const double EXPECTED_THRESHOLD = 0.70;

	User* to_test = new User(1);
	int returned_id = to_test->get_id();
	double returned_threshold = to_test->get_threshold();

	delete to_test;

	bool id_check = EXPECTED_ID == returned_id;
	bool threshold_check = abs(EXPECTED_THRESHOLD - returned_threshold) <= 1e-6;

	if (id_check && threshold_check) {
		cout << "UserTest::test_user Everything OK" << endl << endl;
	}
	else {
		if (!id_check) {
			cout << "UserTest::test_user ID Mismatch" << endl;
			cout << "Expected " << EXPECTED_ID << " but got " << returned_id << endl << endl;
		}
		if (!threshold_check) {
			cout << "UserTest::test_user Threshold Mismatch" << endl;
			cout << "Expected " << EXPECTED_THRESHOLD << " but got " << returned_threshold << endl << endl;
		}
	}
}

void test_set_threshold() {
	const int RANDOM_ID = 1;
	const double EXPECTED_THRESHOLD = 0.20;

	User* to_test = new User(1);
	to_test->set_threshold(EXPECTED_THRESHOLD);
	double returned_threshold = to_test->get_threshold();
	
	delete to_test;

	bool threshold_check = abs(EXPECTED_THRESHOLD - returned_threshold) <= 1e-6;

	if (threshold_check) {
		cout << "UserTest::test_set_threshold Everything OK" << endl << endl;
	}
	else {
		cout << "UserTest::test_set_threshold Threshold Mismatch" << endl;
		cout << "Expected " << EXPECTED_THRESHOLD << " but got " << returned_threshold << endl << endl;
	}
}