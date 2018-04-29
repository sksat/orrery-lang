#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include "token.hpp"

struct node_t {
	using tokitr = token::toklst_t::const_iterator;

	tokitr begin, end;
	std::vector<std::shared_ptr<node_t>> child;

	virtual std::string str(size_t scope=0){
		std::string s;
		for(auto it=begin;it<end;it++){
			s += it->s;
			s += " ";
		}
		return s;
	}
};

struct expr_t : node_t {
	std::string str(size_t scope=0){
		std::string s;
		s += "expr(" + node_t::str() + ");";
		return s;
	}
};

struct block_t : node_t {
	enum type_enum {
		Unknown,
		Func,
		If,
		For,
	};

	type_enum type;

	node_t head;

	std::string str(size_t scope=0){
		std::string s;
		for(auto i=0;i<scope;i++) s+="  ";
		s += "block{";
		s += "type:";
		switch(type){
			case Unknown:	s +="unknown";	break;
			case Func:	s +="function";	break;
			case If:	s +="if";	break;
			case For:	s +="for";	break;
		}
		s += ", ";
		s += "header: " + head.str();
		s += "}";
		s += "\n";
		for(auto& c : child){
			for(auto i=0;i<=scope;i++) s+="  ";
			s += c->str(scope+1);
			s += "\n";
		}
		return s;
	}
};

void print_tokens(const token::toklst_t &tokens);
void parse_block(block_t &parent);

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

	block_t global;
	global.begin = tokens.cbegin();
	global.end   = tokens.cend();
	parse_block(global);
	std::cout<<global.str();
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

void parse_block(block_t &parent){
	auto it = parent.begin;
	const auto end = parent.end;

	if(it == end) return;

	while(true){
		if(it->s == "fn" || it->s == "if" || it->s == "for"){
			auto blk = std::make_shared<block_t>();
			std::cout<<"statement "<<it->s<<":"<<std::endl;
			if(it->s == "fn") blk->type = block_t::Func;
			if(it->s == "if") blk->type = block_t::If;
			if(it->s == "for")blk->type = block_t::For;
			it++;
			blk->head.begin = it;
			while(true){
				if(it->s == "{") break;
				it++;
			}
			blk->head.end = it;
			it++;
			blk->begin = it;

			size_t block_scope = 0;
			while(true){
				if(it->s == "{") block_scope++;
				if(it->s == "}"){
					if(block_scope == 0) break;
					block_scope--;
				}
				it++;
			}
			blk->end = it;
			if(it == end) break;
			it++;

			parse_block(*(blk.get()));
			parent.child.push_back(blk);
		}else{
			auto expr = std::make_shared<expr_t>();
			expr->begin = it;
			while(true){
				if(it == end) continue;
				if(it->s == ";") break;
				else it++;
			}
			expr->end = it;
			if(it == end) break;
			it++;
			if(expr->begin == expr->end) continue;
			parent.child.push_back(expr);
		}
		if(it == parent.end || it == end) break;
	}
}
