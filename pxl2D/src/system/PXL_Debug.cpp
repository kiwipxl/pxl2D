#include "PXL_Debug.h"
#include <string>

const std::string PXL_stack_trace(int num_traces) {
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
}