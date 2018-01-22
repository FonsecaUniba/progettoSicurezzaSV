#include "stdafx.h"
#include "DBConnector.h"

/*
	This data is plain for academic purposes
	It will obviously be different in an actual system
*/
#define USER "root"
#define PASSWD "alpine"
#define DATABASE "signaturesvm"
#define DBHOST "tcp://127.0.0.1:3306"

void save_model(int user_id, std::string model_path) {
	sql::Driver* driver = NULL;
	sql::Connection* connection = NULL;
	sql::PreparedStatement* pstmt = NULL;
	
	try {
		// Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(DBHOST, USER, PASSWD);
		connection->setSchema(DATABASE);

		//Prepare insert command
		pstmt = connection->prepareStatement("INSERT INTO svmodels VALUES (?, ?, 0.70)");
		pstmt->setInt(1, user_id);
		pstmt->setString(2, model_path);
		
		//Execute command
		pstmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
	}

	//Delete Pointers
	delete pstmt;
	delete connection;
}

std::string load_model(int user_id) {
	sql::Driver* driver = NULL;
	sql::Connection* connection = NULL;
	sql::PreparedStatement* pstmt = NULL;
	sql::ResultSet* res = NULL;

	//Path to Model
	std::string filepath;

	try {
		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(DBHOST, USER, PASSWD);
		connection->setSchema(DATABASE);
		
		//Execute Query
		pstmt = connection->prepareStatement("SELECT ModelPATH FROM svmodels WHERE UserID= ?");
		pstmt->setInt(1, user_id);
		res = pstmt->executeQuery();

		//Save Model Path
		while (res->next()){
			filepath = res->getString("ModelPATH");
		}
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
	}

	//Delete Pointers
	delete res;
	delete pstmt;
	delete connection;

	//Return Model Path
	return filepath;
}

double get_threshold(int user_id) {
	sql::Driver* driver = NULL;
	sql::Connection* connection = NULL;
	sql::PreparedStatement* pstmt = NULL;
	sql::ResultSet* res = NULL;

	double result;

	try {
		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(DBHOST, USER, PASSWD);
		connection->setSchema(DATABASE);
		
		//Execute Query
		pstmt = connection->prepareStatement("SELECT Threshold FROM svmodels WHERE UserID = ?");
		pstmt->setInt(1, user_id);
		res = pstmt->executeQuery();

		//Save Threshold
		while (res->next()) {
			result = res->getDouble("Threshold");
		}
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
	}

	//Delete Pointers
	delete res;
	delete pstmt;
	delete connection;

	//Return Treshold
	return result;
}

void update_threshold(int user_id, double threshold) {
	sql::Driver* driver = NULL;
	sql::Connection* connection = NULL;
	sql::Statement* stmt = NULL;
	sql::ResultSet* res = NULL;
	sql::PreparedStatement* pstmt = NULL;

	try {
		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(DBHOST, USER, PASSWD);
		connection->setSchema(DATABASE);

		//Prepare update command
		pstmt = connection->prepareStatement("UPDATE svmodels SET Threshold = ? WHERE UserID = ?");

		//Add variables to command
		pstmt->setDouble(1, threshold);
		pstmt->setInt(2, user_id);

		//Execute command
		pstmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
	}

	//Delete Pointers
	delete pstmt;
	delete res;
	delete stmt;
	delete connection;

}