#pragma once

#include <unordered_map>
#include <string>
#include "mainwin.h"

class WinMessageMap {
public:
	WinMessageMap() noexcept;
	std::string operator() (DWORD, LPARAM, WPARAM) const noexcept;
private:
	std::unordered_map<DWORD, std::string> map;
};