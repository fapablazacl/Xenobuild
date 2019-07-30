
#ifndef __BORC_CORE_WIN32_PROCESSREDIRECTOR_HPP__
#define __BORC_CORE_WIN32_PROCESSREDIRECTOR_HPP__

#include <string>
#include <Windows.h>

namespace borc {
	class ProcessRedirector {
	public:
		explicit ProcessRedirector(STARTUPINFO *si, PROCESS_INFORMATION *pi);

		~ProcessRedirector();

		std::string getOutput();

	private:
		HANDLE hStdIn = 0, hStdOut = 0;
		HANDLE hReadStdOut = 0, hWriteStdIn = 0;
		PROCESS_INFORMATION *pi = nullptr;
	};
}

#endif
