#include "stdafx.h"
#include "DBConnector.h"

/*
	This data is plain for academic purposes
	It will obviously be different in an actual system
*/
const std::string user = "root";
const std::string passwd = "alpine";
const std::string database = "signaturesvm";
const std::string dbhost = "tcp://localhost:3306";

void saveModel(int user_id, std::string model_path) {
	sql::Driver* driver = NULL;
	sql::Connection* connection = NULL;
	sql::Statement* stmt = NULL;
	sql::ResultSet* res = NULL;
	sql::PreparedStatement* pstmt = NULL;
	
	try {
		// Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);
		
		//Prepare insert command
		pstmt = connection->prepareStatement("INSERT INTO svmodels(UserID, ModelPATH) VALUES (?,?)");

		//Add variables to command
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
	delete res;
	delete stmt;
	delete connection;
}

std::string loadModel(int user_id) {
	sql::Driver* driver = NULL;
	sql::Connection* connection = NULL;
	sql::Statement* stmt = NULL;
	sql::ResultSet* res = NULL;

	//Path to Model
	std::string filepath;

	//Prepare Query
	std::string query = "SELECT ModelPATH FROM svmodels WHERE UserID=" + std::to_string(user_id);

	try {
		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);
		
		//Execute Query
		stmt = connection->createStatement();
		res = stmt->executeQuery(query);

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
	delete stmt;
	delete connection;

	//Return Model Path
	return filepath;
}

double get_threshold(int user_id) {
	sql::Driver* driver = NULL;
	sql::Connection* connection = NULL;
	sql::Statement* stmt = NULL;
	sql::ResultSet* res = NULL;

	//Prepare Query
	std::string query = "SELECT Threshold FROM svmodels WHERE UserID=" + std::to_string(user_id);

	double result;

	try {
		//Connect to Database
		driver = get_driver_instance();
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);
		
		//Execute Query
		stmt = connection->createStatement();
		res = stmt->executeQuery(query);

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
	delete stmt;
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
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);

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