#ifndef ORRERY_TOKEN_HPP_
#define ORRERY_TOKEN_HPP_

#include <vector>
#include <string_view>

namespace token {
	enum class type {
		Unknown,
		String,
		Digit,
		SubStart,
		SubEnd,
		Delim,
		ExprEnd,
		BlkStart,
		BlkEnd,
		Operator,
	};

	struct token_t {
		token_t() : t(type::Unknown) {}
		std::string_view s;
		type t;
	};

	token_t get_token(std::string_view &src);
	void tokenize(std::vector<token_t> &toks, std::string_view src);
}

#endif
