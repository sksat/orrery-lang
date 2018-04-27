#ifndef ORRERY_TOKEN_HPP_
#define ORRERY_TOKEN_HPP_

#include <vector>
#include <string_view>

namespace token {
	enum class type {
		digit,
		opr,
		unknown,
	};

	struct token_t {
		std::string_view s;
		type t;
	};

	using toklst_t = std::vector<token_t>;

	void tokenize(toklst_t &toks, std::string_view src);
	void push(toklst_t &tokens, std::string_view &src, size_t &tsiz, type=type::unknown);
}

#endif
