
#include "ProcessRedirector.hpp"
#include <sstream>

namespace borc {
	static bool isWinNT() {
		OSVERSIONINFOW osv = { 0 };
		osv.dwOSVersionInfoSize = sizeof(osv);
		GetVersionExW(&osv);
		return (osv.dwPlatformId == VER_PLATFORM_WIN32_NT);
	}

	ProcessRedirector::ProcessRedirector(STARTUPINFO *si, PROCESS_INFORMATION *pi) {
		SECURITY_ATTRIBUTES sa = { 0 };
		SECURITY_DESCRIPTOR sd = { 0 };

		// initialize SECURITY_ATTRIBUTES
		if (isWinNT()) {
			InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
			SetSecurityDescriptorDacl(&sd, true, NULL, false);
			sa.lpSecurityDescriptor = &sd;
		}
		else {
			sa.lpSecurityDescriptor = NULL;
		}

		// enable process output redirection
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = &sd;
		sa.bInheritHandle = TRUE;

		HANDLE hStdIn = 0, hStdOut = 0;
		HANDLE hReadStdOut = 0, hWriteStdIn = 0;

		BOOL result;
		result = ::CreatePipe(&hStdIn, &hWriteStdIn, &sa, 0);
		result = ::CreatePipe(&hReadStdOut, &hStdOut, &sa, 0);

		si->dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si->wShowWindow = SW_HIDE;
		si->hStdOutput = hStdOut;
		si->hStdError = hStdOut;
		si->hStdInput = hStdIn;

		this->hStdIn = hStdIn;
		this->hStdOut = hStdOut;
		this->hReadStdOut = hReadStdOut;
		this->hWriteStdIn = hWriteStdIn;

		this->pi = pi;
	}

	ProcessRedirector::~ProcessRedirector() {
		::CloseHandle(hStdIn);
		::CloseHandle(hStdOut);
		::CloseHandle(hReadStdOut);
		::CloseHandle(hWriteStdIn);
	}

	std::string ProcessRedirector::getOutput() {
		PROCESS_INFORMATION pi = *this->pi;

		std::stringstream ss;

		while (true) {
			// check for pending reads from stdout
			const int BUFFER_SIZE = 1024;
			char buff[BUFFER_SIZE] = { 0 };
			DWORD readed = 0;
			DWORD avail = 0;

			::PeekNamedPipe(this->hReadStdOut, buff, BUFFER_SIZE - 1, &readed, &avail, NULL);

			if (readed > 0) {
				::ZeroMemory(buff, sizeof(buff));

				if (avail > BUFFER_SIZE - 1) {
					while (readed >= BUFFER_SIZE - 1) {
						::ReadFile(this->hReadStdOut, buff, BUFFER_SIZE - 1, &readed, NULL);
						ss << buff;
						::ZeroMemory(buff, sizeof(buff));
					}
				}
				else {
					::ReadFile(this->hReadStdOut, buff, BUFFER_SIZE - 1, &readed, NULL);
					ss << buff;
				}
			}

			// check if process is running
			unsigned long exitCode = 0;
			::GetExitCodeProcess(pi.hProcess, &exitCode);

			if (exitCode != STILL_ACTIVE) {
				break;
			}
		}

		return ss.str();
	}
}
