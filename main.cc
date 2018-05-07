#include <iostream>
#include <fstream>
#include <string>
#include "token.hpp"

int main(int argc, char **argv){
	std::string src_str;

	if(argc == 1){
		std::cerr<<"usage> "<<argv[0]<<" srcfile"<<std::endl;
		return -1;
	}

	auto src_file = std::ifstream(argv[1]);

	if(!src_file){
		std::cerr<<"cannot open file: \""<<argv[1]<<"\""<<std::endl;
	}

	std::string line;
	while(true){
		std::getline(src_file, line);
		if(!src_file) break;
		src_str += line + '\n';
	}

	std::cout << "src:" << std::endl
		<< src_str
		<< std::endl;

	std::vector<token::token_t> tokens;
	token::tokenize(tokens, src_str);

	for(auto &t : tokens){
		std::cout << "[" << t.s << "] ";
	}
}
