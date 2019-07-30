
#include "Common.hpp"

namespace borc {
	std::string join(const std::vector<std::string> &strings, const std::string &separator) {
		std::string str;

		for (std::size_t i = 0; i < strings.size(); i++) {
			str += strings[i];

			if (i < strings.size() - 1) {
				str += separator;
			}
		}

		return str;
	}
}
