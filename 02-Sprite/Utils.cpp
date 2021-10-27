#include "Utils.h"

vector<string> Split(string content, string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> tokens;

	while ((pos_end = content.find(delimiter, pos_start)) != string::npos) {
		token = content.substr(pos_start, pos_end - pos_start);
		tokens.push_back(token);

		pos_start = pos_end + delim_len;	// Cộng thêm length của delim để trỏ pos_start sang vị trí kí tự đầu tiên của token tiếp theo
	}

	tokens.push_back(content.substr(pos_start));
	return tokens;
}

LPCWSTR ToLPCWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring* w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}