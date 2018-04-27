#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

enum class token_type {
	digit,
	unknown,
};

struct token_t {
	std::string_view s;
	token_type t;
};

void tokenize(std::string_view src);

int main(int argc, char **argv){
	std::ifstream file;
	std::string line;

	if(argc>1) file = std::ifstream(argv[1]);

	// とりあえず1行
	if(argc < 2){
		std::cout << "> ";
		std::getline(std::cin, line);
	}else{
		std::getline(file, line);
	}

	tokenize(line);
}

void push_token(std::string_view &src, size_t &tsiz){
	auto token = src.substr(0,tsiz);
	if(token.empty()) return;
	std::cout<<"["<<token<<"] ";
	src.remove_prefix(tsiz);
	tsiz = 0;
}

void tokenize(std::string_view src){
	std::cout << "[" << src << "]" << std::endl;

	size_t tsiz = 0; // トークンの文字数
	size_t skip;
	while(true){
		if(src.empty()) break;
		if(src.size() < tsiz) break;
		if(src.size() == tsiz){
			std::cout<<"["<<src<<"]"<<std::endl;
			break;
		}

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
			//std::cout<<"["<<src.substr(0, tsiz)<<"] ";
			//src.remove_prefix(tsiz+skip);
			push_token(src, tsiz);
			src.remove_prefix(skip);
			break;
		case '/':
			push_token(src, tsiz);
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
			push_token(src, tsiz);
			break;
		// 文字列
		case '\'':
		case '\"':
			push_token(src, tsiz);
			{
				auto comsiz = src.find(src[tsiz], 1);
				if(comsiz == std::string_view::npos) abort();
				comsiz++;
				push_token(src, comsiz);
			}
			break;
		// 色々な演算子
		case '-':
		case '=':
			push_token(src, tsiz);
			tsiz++;
			if(src[tsiz]=='>' ||		// ->,=>
				src[tsiz-1]==src[tsiz])	// --,==
				tsiz++;
			push_token(src, tsiz);
			break;
		case '+':
		case '&':
		case '|':
			push_token(src, tsiz);
			tsiz++;
			if(src[tsiz]==src[tsiz+1] || // ++,&&,||
				src[tsiz]=='=') // +=,&=,|=
				tsiz++;
			push_token(src, tsiz);
			break;
		case '*':
		case '%':
		case '!':
		case '^':
			push_token(src, tsiz);
			tsiz++;
			if(src[tsiz]=='=') tsiz++; // *=,%=,!=,^=
			push_token(src, tsiz);
			break;
		case ':':
			push_token(src, tsiz);
			tsiz++;
			if(src[tsiz]==':') tsiz++; // ::
			push_token(src, tsiz);
			break;
		// シフトとか
		case '<':
		case '>':
			push_token(src, tsiz);
			tsiz++;
			if(src[tsiz-1]==src[tsiz]) tsiz++; // <<,>>
			if(src[tsiz]=='=') tsiz++; // <=,>=,<<=,>>=
			push_token(src, tsiz);
			break;
		// 確実に1文字で区切れるやつ
		case '~':
		case '?':
		case ';':
		case ',':
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
			push_token(src, tsiz);
			tsiz++;
			push_token(src, tsiz);
			break;
		default:
			tsiz++;
			break;
		}
	}
}
