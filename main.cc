#include <iostream>
#include <fstream>
#include <string>
#include "token.hpp"
#include "parse.hpp"

int main(int argc, char **argv){
	using namespace orrery;
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

	std::vector<token_t> tokens;
	tokenize(tokens, src_str);

	std::cout << "tokens:" << std::endl;
	{
		size_t scope = 0;
		for(size_t i=0;i<tokens.size();i++){
			auto &t = tokens[i];
			bool flg = false;

			if(t.t == type::Operator)	std::cout << t.s << " ";
			else				std::cout << "[" << t.s << "] ";

			if(t.t==type::BlkStart || t.t==type::BlkEnd || t.t==type::ExprEnd){
				const auto &next = tokens[i+1];
				if(t.t == type::BlkStart) scope++;
				if(next.t == type::BlkEnd && scope) scope--;
				std::cout<<std::endl;
				for(size_t i=0;i<scope;i++) std::cout<<"\t";
			}
		}
	}

	unit_t global;
	global.begin	= tokens.begin();
	global.end	= tokens.end();
	parse(global);
}
