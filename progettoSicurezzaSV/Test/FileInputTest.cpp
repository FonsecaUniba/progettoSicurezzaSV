#include "stdafx.h"
#include "FileInputTest.h"

/*
	std::vector<std::string> split_string(std::string str);

	Signature read_signature(std::string file_to_read, bool is_genuine);

	std::vector<Signature> read_all_signatures(std::string path, std::string user);

	std::vector<User> read_all_users(bool isTraining);

	void print_results(std::string path, std::vector<std::string> results);

	std::vector<std::string> split_by_comma(std::string str);

	std::string calculate_averages(double threshold, std::vector<std::vector<std::string>> file_lines);

	void print_optimization(std::string threshold_path, std::string results_path, double first, double step, double stop);

	double read_user_optimal(std::string path, double max_far);
*/

void test_split_string() {
	std::string to_test = "Str1nga di prova";
	std::vector<std::string> result = split_string(to_test);

	const int EXPECTED_SIZE = 3;
	int returned_size = result.size();

	bool size_check = (EXPECTED_SIZE == returned_size);

	if (size_check) {
		std::cout << "FileInputTest::test_split_string Everything OK" << std::endl << std::endl;
	}
	else {
		std::cout << "FileInputTest::test_split_string Size Mismatch" << std::endl;
		std::cout << "Expected " << EXPECTED_SIZE << " but got " << returned_size << std::endl << std::endl;
	}
}

void test_read_signature() {
	/*
		FIRST SIGNATURE
	*/
	std::string path = "C:/Signatures/TestingSubset/001_f_1.sig";
	bool is_genuine = false;
	int EXPECTED_SIZE = 698;

	Signature returned_signature = read_signature(path, is_genuine);
	int returned_size = returned_signature.time_sequence.size();

	/* Signature file name contains "_f_" so it's forgery
		genuine_check==true when is_genuine==false */
	bool genuine_check = !returned_signature.is_genuine;
	// Signature is known, so known_check==true when is_known==true
	bool known_check = returned_signature.is_known;
	bool size_check = EXPECTED_SIZE == returned_size;

	if (genuine_check && known_check && size_check) {
		std::cout << "FileInputTest::test_read_signature Forgery Test Passed" << std::endl << std::endl;
	}
	else {
		if (!genuine_check) {
			std::cout << "FileInputTest::test_read_signature Forgery Test Genuine Mismatch" << std::endl;
			std::cout << "Expected " << false << " but got " << returned_signature.is_genuine << std::endl << std::endl;
		}
		if (!known_check) {
			std::cout << "FileInputTest::test_read_signature Forgery Test Known Mismatch" << std::endl;
			std::cout << "Expected " << EXPECTED_SIZE << " but got " << returned_signature.is_known << std::endl << std::endl;
		}
		if (!size_check) {
			std::cout << "FileInputTest::test_read_signature Forgery Test Size Mismatch" << std::endl;
			std::cout << "Expected " << EXPECTED_SIZE << " but got " << returned_size << std::endl << std::endl;
		}
	}

	/*
		SECOND SIGNATURE
	*/
	path = "C:/Signatures/TestingSubset/001_g_1.sig";
	is_genuine = true;
	EXPECTED_SIZE = 202;

	returned_signature = read_signature(path, is_genuine);
	returned_size = returned_signature.time_sequence.size();

	/* Signature file name contains "_f_" so it's forgery
	genuine_check==true when is_genuine==false */
	genuine_check = returned_signature.is_genuine;
	// Signature is known, so known_check==true when is_known==true
	known_check = returned_signature.is_known;
	size_check = EXPECTED_SIZE == returned_size;

	if (genuine_check && known_check && size_check) {
		std::cout << "FileInputTest::test_read_signature Genuine Test Passed" << std::endl << std::endl;
	}
	else {
		if (!genuine_check) {
			std::cout << "FileInputTest::test_read_signature Genuine Test Genuine Mismatch" << std::endl;
			std::cout << "Expected " << true << " but got " << returned_signature.is_genuine << std::endl << std::endl;
		}
		if (!known_check) {
			std::cout << "FileInputTest::test_read_signature Genuine Test Known Mismatch" << std::endl;
			std::cout << "Expected " << EXPECTED_SIZE << " but got " << returned_signature.is_known << std::endl << std::endl;
		}
		if (!size_check) {
			std::cout << "FileInputTest::test_read_signature Genuine Test Size Mismatch" << std::endl;
			std::cout << "Expected " << EXPECTED_SIZE << " but got " << returned_size << std::endl << std::endl;
		}
	}
}

void test_read_all_signatures() {
	std::string path = "Signatures/TestingSubset/";
	std::string user = "001";
	const int EXPECTED_SIZE = 8;

	std::vector<Signature> returned_signatures = read_all_signatures(path, user);
	int returned_size = returned_signatures.size();

	bool size_check = EXPECTED_SIZE == returned_size;

	if (size_check) {
		std::cout << "FileInputTest::test_read_all_signatures Everything OK" << std::endl << std::endl;
	}
	else {
		std::cout << "FileInputTest::test_read_all_signatures Size Mismatch" << std::endl;
		std::cout << "Expected " << EXPECTED_SIZE << " but got " << returned_size << std::endl << std::endl;
	}
}

void test_read_all_users() {
	bool isTraining = true;
	const int EXPECTED_SIZE = 100;

	std::vector<User> returned_users = read_all_users(isTraining);
	int returned_size = returned_users.size();

	bool size_check = EXPECTED_SIZE == returned_size;

	if (size_check) {
		std::cout << "FileInputTest::test_read_all_users Everything OK" << std::endl << std::endl;
	}
	else {
		std::cout << "FileInputTest::test_read_all_users Size Mismatch" << std::endl;
		std::cout << "Expected " << EXPECTED_SIZE << " but got " << returned_size << std::endl << std::endl;
	}
}

void test_print_results() {
	std::string path = "Results/UnitTest/test_print_results.csv";
	std::vector<std::string> test_values;
	
	std::string str = "";

	for (int i = 0; i <= 100; i++) {
		// Use i as UserID
		str += (std::to_string(i) + ",");
		// Use i as SignatureID
		str += (std::to_string(i) + ",");
		// Genuine if i%2==0, false otherwise
		str += ( ( (i % 2) == 0 ) ? "Genuine," : "Forgery," );
		// Accepted if i%2==0, Rejected otherwise
		str += (((i % 2) == 0) ? "Accepted," : "Rejected,");
		str += "OK";

		test_values.push_back(str);
		str = "";
	}

	print_results(path, test_values);

	std::cout << "FileInputTest::test_print_results Cannot output automated result" << std::endl;
	std::cout << "Open Results and check if file was correctly created" << std::endl << std::endl;
}

void test_split_by_comma() {
	std::string to_test = "Str1nga,di,prova";
	std::vector<std::string> result = split_by_comma(to_test);

	const int EXPECTED_SIZE = 3;
	int returned_size = result.size();

	bool size_check = EXPECTED_SIZE == returned_size;

	if (size_check) {
		std::cout << "FileInputTest::test_split_by_comma Everything OK" << std::endl << std::endl;
	}
	else {
		std::cout << "FileInputTest::test_split_by_comma Size Mismatch" << std::endl;
		std::cout << "Expected " << EXPECTED_SIZE << " but got " << returned_size << std::endl << std::endl;
	}
}

void test_calculate_averages() {
	std::vector<std::string> test_values;
	std::vector<std::vector<std::string>> file_lines;
	double threshold = 0.10;
	const std::string EXPECTED_STRING= std::to_string(0.10) + "," + std::to_string(0.0) + "," + std::to_string(0.0) + "\n";

	for (int i = 0; i <= 100; i++) {
		// Use i as UserID
		test_values.push_back(std::to_string(i));
		// Use i as SignatureID
		test_values.push_back(std::to_string(i));
		// Genuine if i%2==0, false otherwise
		test_values.push_back(((i % 2) == 0) ? "Genuine" : "Forgery");
		// Accepted if i%2==0, Rejected otherwise
		test_values.push_back(((i % 2) == 0) ? "Accepted" : "Rejected");
		test_values.push_back("OK");

		file_lines.push_back(test_values);
		test_values.clear();
	}

	std::string returned_string = calculate_averages(threshold, file_lines);

	bool check_string = EXPECTED_STRING == returned_string;

	if (check_string) {
		std::cout << "FileInputTest::test_calculate_averages Everything OK" << std::endl << std::endl;
	}
	else {
		std::cout << "FileInputTest::test_calculate_averages Value Mismatch" << std::endl;
		std::cout << "Expected " << EXPECTED_STRING << " but got " << returned_string << std::endl << std::endl;
	}
}

void test_print_optimization() {
	double start = 0.10;
	double end = 1.00;

	while (abs(start - end) > 1e-6) {
		std::string file_path = "Results/UnitTest/Thresholds/" + std::to_string(start) + ".csv";

		std::ofstream file;
		file.open(file_path);

		file << "UserID,SignatureID,Genuine\\Forgery,Accepted\\Rejected,OK\\FR\\FA\n";

		for (int j = 0; j <= 10; j++) {
			std::string line = "";

			//Append data to file
			line += std::to_string(j) + ",";
			line += std::to_string(j) + ",";
			line += ((j % 2) == 0) ? "Genuine," : "Forgery,";
			line += ((j % 2) == 0) ? "Accepted," : "Rejected,";
			line += "OK\n";

			file << line;
		}

		file.close();

		std::string result_path = "Results/UnitTest/001.csv";
		std::string threshold_path = "Results/UnitTest/Thresholds/" + std::to_string(start) + ".csv";
		print_optimization(threshold_path, result_path, start);

		start += 0.10;
	}

	std::cout << "FileInputTest::test_print_results Cannot output automated result" << std::endl;
	std::cout << "Open Results and check if file was correctly created" << std::endl << std::endl;
}

void test_read_user_optimal() {
	/*
		REALLY unacceptable value, but
		since the testing graphs are
		linear it's the only way to check
		past the first values
	*/
	const double MAX_FAR = 0.60;

	// First File
	std::string file_path = "Results/UnitTest/001.csv";
	std::ofstream file;
	file.open(file_path);

	file << "Threshold,FAR,FRR\n";

	double index = 0.10;
	double end = 1.00;
	std::string to_print;

	while ( abs(index - end) > 1e-6 ) {
		to_print = std::to_string(index) + "," + std::to_string(index) + "," + std::to_string(index) + "\n";
		file << to_print;

		index += 0.10;
	}

	file.close();

	double EXPECTED_RESULT = 0.10;
	double returned_result = read_user_optimal(file_path, MAX_FAR);

	bool check_optimal = abs(EXPECTED_RESULT - returned_result) < 1e-6;

	if (check_optimal) {
		std::cout << "FileInputTest::read_user_optimal Everything OK" << std::endl << std::endl;
	}
	else {
		std::cout << "FileInputTest::read_user_optimal Result Mismatch" << std::endl;
		std::cout << "Expected " << EXPECTED_RESULT << " but got " << returned_result << std::endl << std::endl;
	}


	// Second File
	file_path = "Results/UnitTest/002.csv";
	file;
	file.open(file_path);

	file << "Threshold,FAR,FRR\n";

	index = 0.10;
	end = 1.00;
	to_print;

	while (abs(index - end) > 1e-6) {
		to_print = std::to_string(index) + "," + std::to_string(index) + "," + std::to_string(1.0 - index) + "\n";
		file << to_print;

		index += 0.10;
	}

	file.close();

	EXPECTED_RESULT = 0.50;
	returned_result = read_user_optimal(file_path, MAX_FAR);

	check_optimal = abs(EXPECTED_RESULT - returned_result) < 1e-6;

	if (check_optimal) {
		std::cout << "FileInputTest::read_user_optimal Everything OK" << std::endl << std::endl;
	}
	else {
		std::cout << "FileInputTest::read_user_optimal Result Mismatch" << std::endl;
		std::cout << "Expected " << EXPECTED_RESULT << " but got " << returned_result << std::endl << std::endl;
	}
}
void test_fileinput_module() {
	test_split_string();

	test_read_signature();

	test_read_all_signatures();
	
	test_read_all_users();

	test_print_results();

	test_split_by_comma();

	test_calculate_averages();
	
	test_print_optimization();

	test_read_user_optimal();
}