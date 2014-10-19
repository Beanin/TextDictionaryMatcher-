#include "SingleMatcher.h"

TSingleTemplateMatcher::TSingleTemplateMatcher():initialized(false), substr_(), currentvalue(0), values() {
}

TStringID TSingleTemplateMatcher::AddTemplate(const string &template_) {
	if (initialized) 
		throw std::logic_error("more than one templates adding");
	//substr_ = "$";
	substr_+=template_; 
	substr_.push_back('$');
	values.reserve(substr_.length());
	values.push_back(0);
	for (size_t prefix_number = 1; prefix_number < substr_.length(); ++prefix_number) 
		values.push_back(AddChar(substr_[prefix_number]));
	initialized = true;
	return 0;
}

size_t TSingleTemplateMatcher::AddChar(char new_char) {
	while (currentvalue > 0 && substr_[currentvalue]!=new_char) {
		currentvalue = values[currentvalue - 1];
	}
	if (substr_[currentvalue]==new_char)
		return ++currentvalue;
	else
		return currentvalue;
}

TMatchResults TSingleTemplateMatcher::MatchStream(ICharStream &stream) {
	size_t shift = 0;
	TMatchResults Matches;
	while (!stream.IsEmpty()) {
		if (AddChar(stream.GetChar()) == substr_.length() - 1) 
			Matches.push_back(std::make_pair(2 + shift-substr_.length(), 0));
		++shift;
	}
	return Matches;
}




