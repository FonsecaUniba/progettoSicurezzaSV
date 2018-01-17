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
	std::string to_test = "Str1nga. \\di\\ pr-ova";
	std::vector<std::string> result = split_string(to_test);

	const int EXPECTED_SIZE = 3;
	int returned_size = result.size();

	bool size_check = EXPECTED_SIZE == returned_size;

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
	std::string path = "Signatures/TestingSubset/001_f_1.sig";
	bool is_genuine = false;
	int EXPECTED_SIZE = 699;

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
	path = "Signatures/TestingSubset/001_g_1.sig";
	is_genuine = true;
	EXPECTED_SIZE = 203;

	returned_signature = read_signature(path, is_genuine);
	returned_size = returned_signature.time_sequence.size();

	/* Signature file name contains "_f_" so it's forgery
	genuine_check==true when is_genuine==false */
	genuine_check = !returned_signature.is_genuine;
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
	std::string path = "Results/test_print_results.csv";
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
	std::string to_test = "Str1nga.,\\di\\,pr-ova";
	std::vector<std::string> result = split_string(to_test);

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
	const std::string EXPECTED_STRING= "0.10,0,0\n";

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
		std::cout << "FileInputTest::test_calculate_averages String Mismatch" << std::endl;
		std::cout << "Expected " << EXPECTED_STRING << " but got " << returned_string << std::endl << std::endl;
	}
}

void test_print_optimization() {
	//TODO write this
}

void test_read_user_optimal() {
	//TODO write this
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