
#ifndef __BORC_MODEL_COMMAND_COMMAND_HPP__
#define __BORC_MODEL_COMMAND_COMMAND_HPP__

#include <string>


namespace borc {
	class Command {
	public:
		virtual ~Command();

		virtual void execute() = 0;

		[[deprecated]]
		virtual void addOption(const std::string &option) = 0;

		template<typename Iterator>
		[[deprecated]]
		void addOptionRange(Iterator begin, Iterator end) {
			for (Iterator i=begin; i!=end; i++) {
				this->addOption(*i);
			}
		}
	};
}

#endif
