#include "stdafx.h"
#include "DBConnectorTest.h"

/*
This data is plain for academic purposes
It will obviously be different in an actual system
*/
#define user "root"
#define passwd "alpine"
#define database "signaturesvm"
#define dbhost "tcp://localhost:3306"

using namespace std;

void test_save_model() {
	const int EXPECTED_USER_ID = 1;
	const std::string EXPECTED_PATH = "C:/Expected";
	const double EXPECTED_THRESHOLD = 0.70;
	
	save_model(EXPECTED_USER_ID, EXPECTED_PATH);
	
	
	int returned_user_id;
	std::string returned_model_path;
	double returned_threshold;
	
	try {
		sql::Driver* driver = NULL;
		sql::Connection* connection = NULL;
		sql::PreparedStatement* pstmt = NULL;
		sql::ResultSet* res = NULL;

		//Prepare Query
		std::string query = "SELECT * FROM svmodels WHERE UserID=?" + std::to_string(EXPECTED_USER_ID);

		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);

		//Execute Query
		pstmt = connection->prepareStatement("SELECT * FROM svmodels WHERE UserID=?");
		pstmt->setInt(1, EXPECTED_USER_ID);
		res = pstmt->executeQuery();

		//Save Threshold
		while (res->next()) {
			returned_user_id = res->getInt("Threshold");
			returned_model_path = res->getString("Threshold");
			returned_threshold = res->getDouble("Threshold");
		}
		
		//Prepare Query
		query = "DELETE FROM svmodels WHERE UserID=" + std::to_string(EXPECTED_USER_ID);

		//Execute Query
		pstmt = connection->prepareStatement("DELETE FROM svmodels WHERE UserID=?");
		pstmt->setInt(1, EXPECTED_USER_ID);
		pstmt->executeUpdate();

		//Delete Pointers
		delete res;
		delete pstmt;
		delete connection;
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
	}

	bool user_id_check = (EXPECTED_USER_ID == returned_user_id);
	bool model_path_check = (EXPECTED_PATH == returned_model_path);
	bool threshold_check = abs(EXPECTED_THRESHOLD - returned_threshold) <= 1e-6;

	if (user_id_check && model_path_check && threshold_check) {
		cout << "DBConnectorTest::test_save_model Everything OK" << endl << endl;
	}
	else {
		if (!user_id_check) {
			cout << "DBConnectorTest::test_save_model ID Mismatch" << endl;
			cout << "Expected " << EXPECTED_USER_ID << " but got " << returned_user_id << endl << endl;
		}

		if (!model_path_check) {
			cout << "DBConnectorTest::test_save_model Model path Mismatch" << endl;
			cout << "Expected " << EXPECTED_PATH << " but got " << returned_model_path << endl << endl;
		}

		if (!threshold_check) {
			cout << "DBConnectorTest::test_save_model Threshold Mismatch" << endl;
			cout << "Expected " << EXPECTED_THRESHOLD << " but got " << returned_threshold << endl << endl;
		}
	}
}

void test_load_model() {
	const int EXPECTED_USER_ID = 1;
	const std::string EXPECTED_PATH = "C:/Expected";

	save_model(EXPECTED_USER_ID, EXPECTED_PATH);
	std::string returned_path = load_model(EXPECTED_USER_ID);

	
	try {
		sql::Driver* driver = NULL;
		sql::Connection* connection = NULL;
		sql::PreparedStatement* pstmt = NULL;

		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);

		//Execute Query
		pstmt = connection->prepareStatement("DELETE FROM svmodels WHERE UserID=?");
		pstmt->setInt(1, EXPECTED_USER_ID);
		pstmt->executeUpdate();

		//Delete Pointers
		delete pstmt;
		delete connection;
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
	}


	bool path_check = EXPECTED_PATH == returned_path;

	if (path_check) {
		cout << "DBConnectorTest::test_load_model Everything OK" << endl << endl;
	}
	else {
		cout << "DBConnectorTest::test_load_model Model path Mismatch" << endl;
		cout << "Expected " << EXPECTED_PATH << " but got " << returned_path << endl << endl;
	}
}

void test_get_threshold() {
	const int EXPECTED_USER_ID = 1;
	const std::string EXPECTED_PATH = "C:/Expected";
	const double EXPECTED_THRESHOLD = 0.70;

	save_model(EXPECTED_USER_ID, EXPECTED_PATH);
	double returned_threshold = get_threshold(EXPECTED_USER_ID);


	try {
		sql::Driver* driver = NULL;
		sql::Connection* connection = NULL;
		sql::PreparedStatement* pstmt = NULL;

		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);

		//Execute Query
		pstmt = connection->prepareStatement("DELETE FROM svmodels WHERE UserID=?");
		pstmt->setInt(1, EXPECTED_USER_ID);
		pstmt->executeUpdate();

		//Delete Pointers
		delete pstmt;
		delete connection;
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
	}


	bool threshold_check = EXPECTED_THRESHOLD == returned_threshold;

	if (threshold_check) {
		cout << "DBConnectorTest::test_get_threshold Everything OK" << endl << endl;
	}
	else {
		cout << "DBConnectorTest::test_get_threshold Threshold Mismatch" << endl;
		cout << "Expected " << EXPECTED_THRESHOLD << " but got " << returned_threshold << endl << endl;
	}
}

void test_update_threshold() {
	const int EXPECTED_USER_ID = 1;
	const std::string EXPECTED_PATH = "C:/Expected";
	const double EXPECTED_THRESHOLD = 0.50;

	save_model(EXPECTED_USER_ID, EXPECTED_PATH);
	update_threshold(EXPECTED_USER_ID, EXPECTED_THRESHOLD);
	double returned_threshold = get_threshold(EXPECTED_USER_ID);
	

	try {
		sql::Driver* driver = NULL;
		sql::Connection* connection = NULL;
		sql::PreparedStatement* pstmt = NULL;

		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);

		//Execute Query
		pstmt = connection->prepareStatement("DELETE FROM svmodels WHERE UserID=?");
		pstmt->setInt(1, EXPECTED_USER_ID);
		pstmt->executeUpdate();

		//Delete Pointers
		delete pstmt;
		delete connection;
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
	}


	bool threshold_check = EXPECTED_THRESHOLD == returned_threshold;

	if (threshold_check) {
		cout << "DBConnectorTest::test_update_threshold Everything OK" << endl << endl;
	}
	else {
		cout << "DBConnectorTest::test_update_threshold Threshold Mismatch" << endl;
		cout << "Expected " << EXPECTED_THRESHOLD << " but got " << returned_threshold << endl << endl;
	}
}

void test_dbconnector_module() {
	test_save_model();

	test_load_model();

	test_get_threshold();

	test_update_threshold();
}