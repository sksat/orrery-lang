#include "token.hpp"
#include <iostream>

namespace token {

void skip_space(std::string_view &src){
	size_t skip;
	for(skip=0;skip<=src.size();skip++){
		const char &c = src[skip];
		if(c != ' ' && c!= '\t' && c!='\n') break;
	}
	src.remove_prefix(skip);
}

bool skip_comment(std::string_view &src){
	size_t skip;
	if(src[0] != '/') return false;
	if(src[1] == '/'){ // 1行コメント
		for(skip=2;skip<=src.size();skip++){
			if(src[skip] == '\n') break;
		}
	}else if(src[1] == '*'){ // 複数行コメント
		for(skip=2;skip<=src.size();skip++){
			const char &c = src[skip];
			if(src[skip-1]=='*' && c == '/') break;
		}
	}else return false;
	src.remove_prefix(skip+1);
	return true;
}

token_t get_delimiter_token(std::string_view &src){
	token_t t;
	size_t siz;
	switch(src[0]){
		// 文字列
		case '\'':
		case '\"':
			siz = src.find(src[0], 1);
			if(siz == std::string_view::npos){ /*TODO: error*/ }
			siz++;
			break;
		case ':': // ':' or '::'
			siz = 1;
			if(src[1] == ':') siz++;
			break;
		case '<': // <,<<,<=,<<=
		case '>': // >,>>,>=,>>=
			siz = 1;
			if(src[1] == src[0]) siz++; // <<,>>
			if(src[siz] == '=') siz++; // <=,>=,<<=,>>=
			break;
		case '-': // -,-=,->
		case '=': // =,==,=>
			siz = 1;
			if(src[1]=='=' || src[1]=='>') siz++;
			break;
		case '+': // +,++,+=
		case '&': // &,&&,&=
		case '|': // |,||,|=
			siz = 1;
			if(src[0] == src[1] || src[1] == '=') siz++;
			break;
		case '*': // *,*=
		case '/': // /,/=
		case '%': // %,%=
		case '!': // !,!=
		case '^': // ^,^=
			siz = 1;
			if(src[1] == '=') siz++;
			break;
		// 確実に1字で区切れるやつ
		case '[': case ']':
		case '{': case '}':
		case '(': case ')':
		case ',': case ';':
		case '~': case '?':
			siz = 1;
			break;
		default:
			return {};
	}

	t.s = src.substr(0, siz);
	src.remove_prefix(siz);

	std::cout<<"delim: ["<<t.s<<"]"<<std::endl;

	return t;
}

token_t get_token(std::string_view &src){
	token_t t;
	size_t siz = 0;

	while(true){
		skip_space(src);
		if(!skip_comment(src)) break;
	}
	if(src.empty())
		return {};

	while(true){
		switch(src[siz]){
			case ' ': case '\t': case '\n':
			// 区切り文字
			case '\'':
			case '\"':
			case ':':
			case '<': case '>':
			case '-': case '=':
			case '+': case '&': case '|':
			case '*': case '/': case '%': case '!': case '^':
			case '[': case ']':
			case '{': case '}':
			case '(': case ')':
			case ',': case ';':
			case '~': case '?':
				if(siz==0) t = get_delimiter_token(src);
				else{
					t.s = src.substr(0, siz); // 直前まで
					std::cout<< "unknown:["<<t.s<<"]"<<std::endl;
					src.remove_prefix(siz);
				}
				return t;
			default:
				if(siz == src.size()){
					std::cout<<"last:["<<src<<"]"<<std::endl;
					t.s = src;
					src.remove_prefix(src.size());
					return t;
				}
				siz++;
				break;
		}
	}
}

void tokenize(std::vector<token_t> &toks, std::string_view src){
	while(true){
		if(src.empty()) break;
		auto t = get_token(src);
		if(t.s.empty()) break;
		toks.push_back(t);
	}
}

}
