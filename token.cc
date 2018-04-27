#include "token.hpp"

namespace token {

void tokenize(toklst_t &toks, std::string_view src){
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
			push(toks, src, tsiz);
			src.remove_prefix(skip);
			break;
		case '/':
			push(toks, src, tsiz);
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
			push(toks, src, tsiz, type::opr);
			break;
		// 文字列
		case '\'':
		case '\"':
			push(toks, src, tsiz);
			{
				auto comsiz = src.find(src[tsiz], 1);
				if(comsiz == std::string_view::npos){ /*TODO: 文字列が終わらないエラー*/ }
				comsiz++;
				push(toks, src, comsiz);
			}
			break;
		// 色々な演算子
		case '-':
		case '=':
			push(toks, src, tsiz);
			tsiz++;
			if(src[tsiz]=='>' ||		// ->,=>
				src[tsiz-1]==src[tsiz])	// --,==
				tsiz++;
			push(toks, src, tsiz, type::opr);
			break;
		case '+':
		case '&':
		case '|':
			push(toks, src, tsiz);
			tsiz++;
			if(src[tsiz]==src[tsiz+1] || // ++,&&,||
				src[tsiz]=='=') // +=,&=,|=
				tsiz++;
			push(toks, src, tsiz, type::opr);
			break;
		case '*':
		case '%':
		case '!':
		case '^':
			push(toks, src, tsiz);
			tsiz++;
			if(src[tsiz]=='=') tsiz++; // *=,%=,!=,^=
			push(toks, src, tsiz, type::opr);
			break;
		case ':':
			push(toks, src, tsiz);
			tsiz++;
			if(src[tsiz]==':') tsiz++; // ::
			push(toks, src, tsiz, type::opr);
			break;
		// シフトとか
		case '<':
		case '>':
			push(toks, src, tsiz);
			tsiz++;
			if(src[tsiz-1]==src[tsiz]) tsiz++; // <<,>>
			if(src[tsiz]=='=') tsiz++; // <=,>=,<<=,>>=
			push(toks, src, tsiz, type::opr);
			break;
		// 確実に1文字で区切れるやつ
		case '~':
		case '?':
		case ';':
		case ',':
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
			push(toks, src, tsiz);
			tsiz++;
			push(toks, src, tsiz);
			break;
		default:
			tsiz++;
			break;
		}
	}
}

void push(toklst_t &toks, std::string_view &src, size_t &tsiz, type t){
	token_t token;
	auto str = src.substr(0,tsiz);
	if(str.empty()) return;
	token.s = str;
	token.t = t;
	toks.push_back(token);
	src.remove_prefix(tsiz);
	tsiz = 0;
}

} // namespace
