
#include "ProcessCommand.hpp"
#include "Common.hpp"

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "win32/ProcessRedirector.hpp"
#include <Windows.h>

namespace borc {
	ProcessCommand::ProcessCommand(const std::string &base)
		: _base(base) {}

	ProcessCommand::ProcessCommand(const std::string &base, const std::vector<std::string> &options)
		: _base(base), _options(options) {}

	void ProcessCommand::execute() {
		const std::string commandLine = _base + " " + join(_options, " ");

		std::cout << commandLine << std::endl;

		STARTUPINFO si = { 0 };
		si.cb = sizeof(STARTUPINFO);

		PROCESS_INFORMATION pi = { 0 };

		// invoke process
		BOOL result = ::CreateProcess(
			nullptr, const_cast<char*>(commandLine.c_str()),
			NULL, NULL, TRUE,
			CREATE_SUSPENDED, NULL, NULL,
			&si, &pi
		);

		if (result == FALSE) {
			const DWORD dwError = ::GetLastError();
			throw std::runtime_error(std::to_string(::GetLastError()));
		}

		::ResumeThread(pi.hThread);
		::WaitForSingleObject(pi.hThread, INFINITE);

		DWORD exitCode = 0;

		::GetExitCodeProcess(pi.hProcess, &exitCode);

		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);

		if (int(exitCode) != 0) {
			throw std::runtime_error("The command returned the following error: " + std::to_string(exitCode));
		}
	}

	void ProcessCommand::addOption(const std::string &option) {
		_options.push_back(option);
	}
}
