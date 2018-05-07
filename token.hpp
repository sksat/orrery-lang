#ifndef ORRERY_TOKEN_HPP_
#define ORRERY_TOKEN_HPP_

#include <vector>
#include <string_view>

namespace token {
	enum class type {
		string,
		digit,
		unknown,
	};

	struct token_t {
		std::string_view s;
	};

	token_t get_token(std::string_view &src);
	void tokenize(std::vector<token_t> &toks, std::string_view src);
}

#endif
