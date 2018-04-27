#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>

enum class token_type {
	digit,
	opr,
	unknown,
};

struct token_t {
	std::string_view s;
	token_type t;
};

void tokenize(std::vector<token_t> &toks, std::string_view src);

int main(int argc, char **argv){
	std::ifstream file;
	std::string src;
	std::vector<token_t> tokens;

	if(argc>1) file = std::ifstream(argv[1]);

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

	tokenize(tokens, src);

	{
		size_t scope = 0;
		bool newline = false;
		for(auto &t : tokens){
			if(t.s == ";") newline=true;
			else if(t.s == "{"){newline=true; scope++;}
			else if(t.s == "}"){newline=true; scope--;}
			else{
				if(t.s == "(" || t.s ==")") std::cout<<t.s;
				else if(t.t != token_type::opr)
					std::cout<<"["<<t.s<<"] ";
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
}

void push_token(std::vector<token_t> &tokens, std::string_view &src, size_t &tsiz, token_type t=token_type::unknown){
	auto str = src.substr(0,tsiz);
	if(str.empty()) return;
	token_t token;
	token.s = str;
	token.t = t;
	tokens.push_back(token);
	src.remove_prefix(tsiz);
	tsiz = 0;
}

void tokenize(std::vector<token_t> &toks, std::string_view src){
	size_t tsiz = 0; // トークンの文字数
	size_t skip;
	while(true){
		if(src.empty()) break;
		if(src.size() < tsiz) break;

		switch(src[tsiz]){
		// 空白
		case ' ':
		case '\t':
		case '\n':
			for(skip=1; (tsiz+skip) < src.size(); skip++){
				if(src[tsiz+skip] != ' ' &&
					src[tsiz+skip] != '\t' &&
					src[tsiz+skip] != '\n') break;
			}
			push_token(toks, src, tsiz);
			src.remove_prefix(skip);
			break;
		case '/':
			push_token(toks, src, tsiz);
			if(src[tsiz+1] == '/'){ // 1行コメント
				for(skip=2; (tsiz+skip) < src.size(); skip++){
					if(src[tsiz+skip] == '\n') break;
				}
				tsiz += skip;
				src.remove_prefix(tsiz);
				tsiz=0;
				break;
			}else if(src[tsiz+1] == '*'){ // 複数行コメント
				for(skip=2; (tsiz+skip) < src.size(); skip++){
					if(src[tsiz+skip-1] == '*'){
						if(src[tsiz+skip] == '/') break;
					}
				}
				tsiz += skip + 1;
				src.remove_prefix(tsiz);
				tsiz=0;
				break;
			}
			tsiz++;
			push_token(toks, src, tsiz, token_type::opr);
			break;
		// 文字列
		case '\'':
		case '\"':
			push_token(toks, src, tsiz);
			{
				auto comsiz = src.find(src[tsiz], 1);
				if(comsiz == std::string_view::npos) abort();
				comsiz++;
				push_token(toks, src, comsiz);
			}
			break;
		// 色々な演算子
		case '-':
		case '=':
			push_token(toks, src, tsiz);
			tsiz++;
			if(src[tsiz]=='>' ||		// ->,=>
				src[tsiz-1]==src[tsiz])	// --,==
				tsiz++;
			push_token(toks, src, tsiz, token_type::opr);
			break;
		case '+':
		case '&':
		case '|':
			push_token(toks, src, tsiz);
			tsiz++;
			if(src[tsiz]==src[tsiz+1] || // ++,&&,||
				src[tsiz]=='=') // +=,&=,|=
				tsiz++;
			push_token(toks, src, tsiz, token_type::opr);
			break;
		case '*':
		case '%':
		case '!':
		case '^':
			push_token(toks, src, tsiz);
			tsiz++;
			if(src[tsiz]=='=') tsiz++; // *=,%=,!=,^=
			push_token(toks, src, tsiz, token_type::opr);
			break;
		case ':':
			push_token(toks, src, tsiz);
			tsiz++;
			if(src[tsiz]==':') tsiz++; // ::
			push_token(toks, src, tsiz, token_type::opr);
			break;
		// シフトとか
		case '<':
		case '>':
			push_token(toks, src, tsiz);
			tsiz++;
			if(src[tsiz-1]==src[tsiz]) tsiz++; // <<,>>
			if(src[tsiz]=='=') tsiz++; // <=,>=,<<=,>>=
			push_token(toks, src, tsiz, token_type::opr);
			break;
		// 確実に1文字で区切れるやつ
		case '~':
		case '?':
		case ';':
		case ',':
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
			push_token(toks, src, tsiz);
			tsiz++;
			push_token(toks, src, tsiz);
			break;
		default:
			tsiz++;
			break;
		}
	}
}
