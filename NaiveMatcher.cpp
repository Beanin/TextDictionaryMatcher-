#include "NaiveMatcher.h"

TNaiveTemplateMatcher::TNaiveTemplateMatcher():templates(), template_ends(), maxtemplatelength(0) {
}

TStringID TNaiveTemplateMatcher::AddTemplate(const std::string &templatestr) {
	template_ends.push_back(templates.addReverseString(templatestr,template_ends.size()));
	maxtemplatelength = std::max(maxtemplatelength, template_ends.back()->height());
	return template_ends.size() - 1;
}

TMatchResults TNaiveTemplateMatcher::MatchStream(ICharStream &stream){
	string buffer;
	TMatchResults Matches;
	size_t substringstart = 0; 
	while (!stream.IsEmpty() || buffer.length()) {
		for (size_t template_id = 0; template_id < template_ends.size(); ++template_id) {
			while (buffer.length() < template_ends[template_id]->height() && !stream.IsEmpty()) {
				char newch=stream.GetChar();
				if (static_cast<int>(newch) < 32 || static_cast<int>(newch) > 255) 
				throw std::logic_error("Character out of range");
				buffer.push_back(newch);
			}
			if (buffer.length() < template_ends[template_id]->height())
				continue;
			bool match = true;
			shared_ptr<TTrieNode> current_template_position = template_ends[template_id];
			for (size_t shift = 0; shift < template_ends[template_id]->height(); ++shift) {
				if (current_template_position->parentsymbol()!=buffer[shift])
					match = false;
				current_template_position = current_template_position->parent();
			}
			if (match) {
				Matches.push_back(std::make_pair(substringstart+template_ends[template_id]->height() - 1,template_id));
			}
		}
		buffer.erase(0,1);
		substringstart++;
	}
	return Matches;
}


