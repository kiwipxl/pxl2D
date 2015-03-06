#include "PXL_Debug.h"
#include <string>

#if defined(PXL_SYSTEM_WINDOWS)
	#include <Windows.h>
	#if defined(_DEBUG)
		#include <DbgHelp.h>
	#endif

	const std::string PXL_stack_trace(int num_traces) {
		#if defined(_DEBUG)
			PXL_uint i;
			DWORD* stacks = new DWORD[num_traces];
			PXL_ushort frames_captured;
			SYMBOL_INFO* symbol;
			HANDLE process;

			process = GetCurrentProcess();
			SymInitialize(process, NULL, TRUE);
			frames_captured = CaptureStackBackTrace(0, num_traces, (PVOID*)stacks, NULL);

			//needs to be allocated on heap
			symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
			symbol->MaxNameLen = 192;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

			IMAGEHLP_LINE line;
			DWORD disp;

			std::string output = "";
			char* char_output = new char[255];
			for (i = 0; i < frames_captured; i++) {
				SymFromAddr(process, stacks[i], NULL, symbol);
				SymGetLineFromAddr(process, stacks[i], &disp, &line);

				//convert file name to string and remove everything in the string before src or include
				std::string file_name = line.FileName;
				int id;
				if ((id = file_name.rfind("src")) != -1) {
					file_name = file_name.substr(id + 4, file_name.length());
				}else if ((id = file_name.rfind("include")) != -1) {
					file_name = file_name.substr(id, file_name.length());
				}

				if (id != -1) {
					sprintf(char_output, "%i: %s (%s, line %i)\n", frames_captured - i - 1, symbol->Name, file_name.c_str(), line.LineNumber);
					output += char_output;
				}
			}

			delete symbol;
			delete[] stacks;

			return output;
		#endif

		return "";
	}

	/**
	Returns the last error reported by the os in a string. Returns an empty string if there is no error
	**/
	std::string PXL_get_last_error() {
		#if defined(PXL_SYSTEM_WINDOWS)
			//gets the last error message from windows
			DWORD error_msg_id = GetLastError();
			if (error_msg_id == 0) { return "No error message has been recorded"; }

			LPSTR msg_buffer = nullptr;
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, error_msg_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg_buffer, 0, NULL);

			std::string message = msg_buffer;
			LocalFree(msg_buffer);

			return message;
		#endif
		return "";
	}

#elif defined(PXL_SYSTEM_ANDROID)
	
	const std::string PXL_stack_trace(int num_traces) {
		return "";
	}

	/**
	Returns the last error reported by the os in a string. Returns an empty string if there is no error
	**/
	std::string PXL_get_last_error() {
		return "";
	}

#elif defined(PXL_SYSTEM_LINUX)

#elif defined(PXL_SYSTEM_MAC_OS)

#endif
