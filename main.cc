#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include "token.hpp"

void print_tokens(const token::toklst_t &tokens);

void parse(const token::toklst_t &tokens, size_t pos=0);

int main(int argc, char **argv){
	std::ifstream file;
	std::string src;
	token::toklst_t tokens;

	if(argc>1) file = std::ifstream(argv[1]);

	if(!file){
		std::cerr<<"cannot open file: \""<<argv[1]<<"\""<<std::endl;
		return -1;
	}

	while(true){ // 流れてきた文字列ぜんぶ読む
		std::string line;
		if(argc < 2){
			std::cout << "> ";
			std::getline(std::cin, line);
			if(line == "quit") break;
		}else{
			std::getline(file, line);
			if(!file) break;
		}
		src += line + '\n';
	}

	std::cout << "src:"<<std::endl
		<< src
		<< std::endl;

	token::tokenize(tokens, src);
	print_tokens(tokens);

	parse(tokens);
}

void print_tokens(const token::toklst_t &tokens){
	size_t scope = 0;
	bool newline = false;
	for(auto &t : tokens){
		if(t.s == ";") newline=true;
		else if(t.s == "{"){newline=true; scope++;}
		else if(t.s == "}"){newline=true; scope--;}
		else{
			if(t.t != token::type::opr)
				std::cout<<"["<<t.s<<"] ";
			else
				std::cout<<t.s<<" ";
		}
		if(newline){
			if(t.s=="}") std::cout<<"\b\b\b\b";
			std::cout<<t.s<<std::endl;
			for(size_t i=0;i<scope;i++) std::cout<<"    ";
			newline=false;
		}
	}
	std::cout<<std::endl;
}

struct node_t {
	using tokitr = token::toklst_t::const_iterator;
	std::vector<node_t> child;
};

struct expr_t : node_t {
	tokitr begin, end;
};

struct block_t : node_t {
	struct header_t {
		tokitr begin, end;
	};
	header_t head;

	// ブロックの中身
	tokitr begin, end;
};

void print_block(const block_t &blk){
	std::cout<<"block:"<<std::endl;
	std::cout<<"\theader:\n\t";
	for(auto it=blk.head.begin;it<=blk.head.end;it++)
		std::cout<<"["<<it->s<<"] ";
	std::cout<<std::endl;
	std::cout<<"\tmain:\n\t";
	for(auto it=blk.begin;it<=blk.end;it++)
		std::cout<<"["<<it->s<<"] ";
	std::cout<<std::endl;
}

void parse(const token::toklst_t &tokens, size_t pos){
	auto read = tokens.begin() + pos;
	auto it = read;

	if(pos == 0) std::cout<<"parse:"<<std::endl;

	while(true){
		if(it->s == ";"){
			std::cout<<"expr(";
			while(read<it){
				std::cout<<"["<<read->s<<"] ";
				read++;
			}
			read++;
			std::cout<<");"<<std::endl;
		}else if(it->s == "{"){
			block_t blk;

			// [ここの部分] {}
			// if(true)とか
			blk.head.begin	= read;
			blk.head.end	= it-1;
			it++; // ブロックの中身の最初
			blk.begin = read = it;

			size_t block_scope=0;
			while(true){
				if(it->s=="{"){
					block_scope++;
				}else if(it->s=="}"){
					if(block_scope==0) break;
					block_scope--;
				}
			//	std::cout<<"["<<it->s<<"] ";
				it++;
			}
			blk.end = it-1;

			print_block(blk);

			it++;
			read = it;
			std::cout<<std::endl;
		}
		if(it != tokens.end()) it++;
		else break;
	}
}
