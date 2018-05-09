#ifndef ORRERY_PARSE_HPP_
#define ORRERY_PARSE_HPP_

#include "token.hpp"

namespace orrery {
	using tokitr = std::vector<token_t>::const_iterator;
	struct unit_t {
		tokitr begin, end;
		std::vector<unit_t> child;
	};

	void parse(unit_t &unit);
}

#endif
