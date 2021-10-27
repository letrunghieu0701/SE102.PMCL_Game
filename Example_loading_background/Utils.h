#pragma once

#include <string>
#include <vector>
#include "tinyxml.h"
#include <Windows.h>

using namespace std;

vector<string> Split(string content, string delimiter);
LPCWSTR ToLPCWSTR(string st);

