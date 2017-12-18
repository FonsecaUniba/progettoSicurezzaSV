#include "stdafx.h"
#include "FileInput.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

#include "../Signature/Signature.h"
#include "../Signature/Instant.h"

std::vector<std::string> split_string(std::string str) {
	std::istringstream buf(str);
	std::istream_iterator<std::string> beg(buf), end;

	std::vector<std::string> tokens(beg, end); // done!
	return tokens;
}

Signature readSignature(std::string file_to_read, bool is_genuine) //TODO CHANGE_TYPE
{
	//Creates object to store result Signature
	Signature* current_signature = new Signature();
	current_signature->is_genuine = is_genuine;

	//Opens file buffer
	std::ifstream file(file_to_read);
	
	//String to store file line
	std::string str;
	//String to store whole file to reverse
	std::vector<std::string> file_lines;

	while (std::getline(file, str))
	{
		file_lines.insert(file_lines.begin(), str);
	}

	//TODO Generate Instants
	//TODO Add Instants to Signature

	return *current_signature; //TODO CHANGE
}

/*
	Train
		AllSignatures = new Array of Signature
			Signature = readSignature();
				for every line (
					New Instant = readLine;
					Signature.timesequence.add(Instant)
				)
				return Signature;
			AllSignatures.add(Signature)
			return AllSignatures
*/