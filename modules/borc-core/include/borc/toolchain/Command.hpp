
#ifndef __BORC_MODEL_COMMAND_HPP__
#define __BORC_MODEL_COMMAND_HPP__

#include <string>
#include "Predef.h"

namespace borc {
	class Command {
	public:
		virtual ~Command();

		virtual void execute() = 0;

		virtual void addOption(const std::string &option) = 0;

		template<typename Iterator>
		void addOptionRange(Iterator begin, Iterator end) {
			for (Iterator i=begin; i!=end; i++) {
				this->addOption(*i);
			}
		}
	};
}

#endif
