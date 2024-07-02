#include "basicerror.h"

BasicError::BasicError(int line, const char* file) noexcept : line(line), file(file) {}

const char* BasicError::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* BasicError::GetType() const noexcept {
	return "Basic Error";
}
int BasicError::GetLine() const noexcept { return line; }
const std::string& BasicError::GetFile() const noexcept { return file; }
std::string BasicError::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "FILE: " << file << std::endl;
	oss << "LINE: " << line;
	return oss.str();
}