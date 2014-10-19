#include "Base.h"

TStringStream::TStringStream(std::string s):str(s), pos(){
}

char TStringStream::GetChar() {
	if (IsEmpty()) 
		throw std::logic_error("Reading from empty stream");
	return str[pos++]; 
}

bool TStringStream::IsEmpty() const {
	return (pos == str.length());
}

void TStringStream::reset() {
	pos = 0;
}