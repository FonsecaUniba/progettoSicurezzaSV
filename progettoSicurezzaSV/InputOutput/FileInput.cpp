#include "stdafx.h"
#include "FileInput.h"

const int FIRST_CHARACTER = 0;

const int X_POSITION = 0;
const int Y_POSITION = 1;
const int TIMESTAMP = 2;
const int PRESSURE = 3;

const std::string GENUINE = "_g_";
const std::string FORGERY = "_f_";
const std::string EXTENSION = ".sig";

const std::string TRAINING_PATH = "Signatures/TrainingSubset";
const std::string TESTING_PATH = "Signatures/TestingSubset";

enum Columns : int {USER_ID, SIGNATURE_ID, GENUINE_FORGERY, ACCEPTED_REJECTED, OK_FR_FA};

using namespace std;

std::vector<std::string> split_string(std::string str) {
	std::istringstream buf(str);
	std::istream_iterator<std::string> beg(buf), end;

	std::vector<std::string> tokens(beg, end);
	return tokens;
}

Signature read_signature(std::string file_to_read, bool is_genuine)
{
	//Creates object to store result Signature
	Signature current_signature;
	current_signature.is_genuine = is_genuine;
	current_signature.is_known = true;

	//Opens file buffer
	std::ifstream file(file_to_read);
	
	//String to store file line
	std::string str;
	//Vector to store file lines
	std::vector<std::string> file_lines;
	
	int counter = 0;
	while (std::getline(file, str))
	{
		//Skip first two lines
		if(counter > 1 && !str.empty()) {
			//Appends Read Line to vector
			file_lines.push_back(str);
		}
		else if (counter == 1) {
			std::string data_size = split_string(str).at(1);
			file_lines.reserve(std::stoi(data_size));
		}
		counter++;
	}
	file.close();


	for (int i = 0; i < file_lines.size(); i++) {
		//Instant data at row i
		std::vector<std::string> i_t0;
		//Instant data at row i+1
		std::vector<std::string> i_t1;

		//If last element, take previous
		if (i == (file_lines.size()-1)) {
			i_t0 = split_string(file_lines.at(i) );
			i_t1 = split_string(file_lines.at(i - 1) );
		}
		else {
			i_t0 = split_string(file_lines.at(i));
			i_t1 = split_string(file_lines.at(i + 1));
		}

		int x_t0 = std::stoi( i_t0.at(X_POSITION) );
		int y_t0 = std::stoi( i_t0.at(Y_POSITION) );
		int t0 = std::stoi( i_t0.at(TIMESTAMP) );
		int p_t0 = std::stoi( i_t0.at(PRESSURE) );
		int x_t1 = std::stoi( i_t1.at(X_POSITION) );
		int y_t1 = std::stoi( i_t1.at(Y_POSITION) );
		int t1 = std::stoi( i_t1.at(TIMESTAMP) );

		//Instant(int x0, int y0, int t0, int p0, int x1, int y1, int t1);
		Instant current_instant(x_t0, y_t0, t0, p_t0, x_t1, y_t1, t1);

		current_signature.time_sequence.push_back(current_instant);
	}

	return current_signature;
}

std::vector<Signature> read_all_signatures(std::string path, std::string user) {
	std::vector<Signature> all_signatures;
	Signature current_signature;
	bool is_genuine;

	std::string path_to_file;

	//Reads all Genuine Signatures
	is_genuine = true;
	for (int i = 1; i <= 4; i++) {
		path_to_file = path + user + GENUINE + std::to_string(i) + EXTENSION;
		all_signatures.push_back(read_signature(path_to_file, is_genuine));
	}
	
	//Reads all Forgery Signatures
	is_genuine = false;
	for (int i = 1; i <= 4; i++) {
		path_to_file = path + user + FORGERY + std::to_string(i) + EXTENSION;
		all_signatures.push_back(read_signature(path_to_file, is_genuine));
	}

	return all_signatures;
}

std::vector<User> read_all_users(bool isTraining) {
	const int USER_SIZE = 100;

	//Determines if training or testing phase, sets path accordingly
	std::string path = (isTraining) ? TRAINING_PATH : TESTING_PATH ;

	//Creates a vector of Users
	std::vector<User> all_users;
	all_users.reserve(USER_SIZE);

	//Iterates USER_SIZE times
	for (int i = 1; i <= USER_SIZE; i++) {
		//Creates new user
		User current_user(i);
		//Generates a string which will contain the userID
		std::string user;

		//Pads UserID to 3 characters
		if (i < 10) {
			user = "00" + std::to_string(i);
		}
		else if (100) {
			user = "0" + std::to_string(i);
		}
		else {
			user = std::to_string(i);
		}

		//Reads all signatures for an user and adds the user to the vector
		current_user.user_signatures = read_all_signatures(path, user);
		all_users.push_back(current_user);
	}

	return all_users;
}


/*
- UserID
- SignatureID
- Genuine/Forgery
- Accepted/Rejected
- OK/FR/FA
*/
void print_results(std::string path, std::vector<std::string> results) {
	// Declares Output Stream
	ofstream output_stream;
	
	// Opens Stream
	output_stream.open(path);
	// Sets header
	output_stream << "UserID,SignatureID,Genuine\\Forgery,Accepted\\Rejected,OK\\FR\\FA\n";
	
	// Prints Results
	for (int i = 0; i < results.size(); i++) {
		output_stream << results[i] << "\n";
	}

	//Closes Stream
	output_stream.close();
}

std::vector<std::string> split_by_comma(std::string str) {
	//Result vector
	std::vector<std::string> result;
	//String to store temporary data
	std::string tmp = "";

	//Cycle through string to split
	for (int i = 0; i < str.size(); i++) {
		//If ',' or string end
		if (str.at(i) == ',' || (i == str.size() - 1)) {
			//Add data to result vector
			result.push_back(tmp);
			//Reset string
			tmp = "";
		}
		else {
			//Add char to string
			tmp += str.at(i);
		}
	}

	return result;
}

std::string calculate_averages(double threshold, std::vector<std::vector<std::string>>& file_lines) {
	int total = file_lines.size();
	int countFAR = 0;
	int countFRR = 0;

	for (int i = 0; i < file_lines.size(); i++) {
		std::string value = file_lines.at(i).at(OK_FR_FA);
		if (value == "FR") {
			countFRR++;
		}
		else if (value == "FA") {
			countFAR++;
		}
	}

	double FAR = (countFAR / total);
	double FRR = (countFRR / total);

	return std::to_string(threshold) + "," + std::to_string(FAR) + "," + std::to_string(FRR) + "\n";
}

/*
	- Threshold
	- FAR
	- FRR
*/
void print_optimization(std::string threshold_path, std::string results_path, double first, double step, double stop) {
	// Declares output_stream
	ofstream output_stream;
	// Opens output stream
	output_stream.open(results_path);
	// Sets Header
	output_stream << "Threshold,FAR,FRR\n";
	
	double index = first;
	while ( abs(index - stop) > 1e-6 ) {
		// String to store lines
		std::string str;
		// Vector to store file lines
		std::vector<std::vector<std::string>> file_lines;

		// Determines current_file_path
		std::string current_path = threshold_path + std::to_string(index) + ".csv";
		// Opens File
		std::ifstream current_file;
		current_file.open(current_path);

		// Read Line by Line
		while (std::getline(current_file, str)) {
			// Splits lines by comma
			std::vector<std::string> data = split_by_comma(str);
			// Adds line to file vector
			file_lines.push_back(data);
		}

		// Closes File Stream
		current_file.close();

		output_stream << calculate_averages(index, file_lines);

		// Clears Memory
		file_lines.clear();

		index += step;
	}

	//Closes Output Stream
	output_stream.close();
}

double read_user_optimal(std::string path, double max_far) {
	// Defines the ROC graph as a struct
	struct roc_graph {
		// False Acceptance Rate
		double far;
		// True Acceptance Rate
		double tar;
		// Threshold
		double threshold;
		// Distance from FAR = 0.0 and TAR = 1.0
		double distance_from_optimal;
	};

	//Optimal False Acceptance Rate Value
	const double OPTIMAL_FAR = 0.0;
	//Optimal True Acceptance Rate Value
	const double OPTIMAL_TAR = 1.0;

	// Vector to store graph
	std::vector<roc_graph> user_data;
	// Variable to temporarely store points
	roc_graph point;

	// File Columns
	enum file_header { THRESHOLD, FAR, FRR };
	// String to store file data
	std::string str;
	// Input Stream
	std::ifstream file;
	file.open(path);
	
	//Is first line?
	bool header = true;
	// Read Line by Line
	while (std::getline(file, str)) {
		
		if(!header) {
			// Splits line by comma
			std::vector<std::string> line = split_by_comma(str);

			// Stores line info
			point.threshold = std::stod( line.at(THRESHOLD) );
			point.far = std::stod( line.at(FAR) );
			point.tar = 1.0 - std::stod( line.at(FRR) );

			// Calculates euclidean distance to optimal point
			double delta_x = point.far - OPTIMAL_FAR;
			double delta_y = point.tar - OPTIMAL_TAR;

			point.distance_from_optimal = sqrt( pow(delta_x, 2) + pow(delta_y, 2) );

			// Stores point data
			user_data.push_back(point);
		} else {
			//Skips first line
			header = false;
		}
	}

	// Closes File Stream
	file.close();

	// The optimal distance
	int result_index = 0;

	// Cycle user_data to find minimum distance allowed
	for (int i = 1; i < user_data.size(); i++) {
		if (user_data.at(i).far >= max_far) {
			break;
		}

		//If current point has a lower distance than previous optimal
		if (user_data.at(i).distance_from_optimal < user_data.at( result_index ).distance_from_optimal) {
			// Store new index
			result_index = i;
		}
	}

	// Return threshold corresponding to result_index point
	return user_data.at(result_index).threshold;
}