#include "StaticMatcher.h"
#include <set>
TStaticTemplateMatcher::TStaticTemplateMatcher():TTrie(),template_lengths(),sink(new TTrieNode),used(false),currentnode(getroot()) {
	getroot()->suf_link_ = sink;
	getroot()->hard_suf_link_ = sink;
	sink->suf_link_ = sink;
	sink->hard_suf_link_ = getroot();
}


shared_ptr<TTrieNode> TStaticTemplateMatcher::moveto(shared_ptr<TTrieNode> node, char symbol) {
	if (node == sink)
		return getroot();
	if (node->isEdge(symbol))
		return node->next(symbol);
	return moveto(suf_link_move(node),symbol);
}
shared_ptr<TTrieNode> TStaticTemplateMatcher::suf_link_move(shared_ptr<TTrieNode> node) {
	if (node->suf_link_)
		return node->suf_link_;
	shared_ptr<TTrieNode> prev = node->parentnode_;
	while (prev != getroot() && !suf_link_move(prev)->isEdge(node->parentsymbol_))
		prev = suf_link_move(prev);
	node->suf_link_ = moveto(suf_link_move(prev),node->parentsymbol_);
	return node->suf_link_;
}

shared_ptr<TTrieNode> TStaticTemplateMatcher::hard_suf_link_move(shared_ptr<TTrieNode> node){
	if (node->hard_suf_link_)
		return node->hard_suf_link_;
	if (!suf_link_move(node)->terminal_.empty())	{
		node->hard_suf_link_ = suf_link_move(node);
		return node->hard_suf_link_;
	}
	node->hard_suf_link_= hard_suf_link_move(suf_link_move(node)); 
	return node->hard_suf_link_; 
}

TStringID TStaticTemplateMatcher::AddTemplate(const std::string &templatestr) {
	if (used) 
		throw std::logic_error("AddTemplate after MatchStream");
	addstring(templatestr,template_lengths.size());
	template_lengths.push_back(templatestr.length());
	return template_lengths.size() - 1;
}
void TStaticTemplateMatcher::Handlesymbol(char ch, TMatchResults& ans, size_t &pos) {
	currentnode = moveto(currentnode, ch);
		shared_ptr<TTrieNode> ansiterator = currentnode;
		if (ansiterator->terminal_.empty())
			ansiterator = hard_suf_link_move(ansiterator);
		while (!ansiterator->terminal_.empty()) {
			for(TStringID &templateid: ansiterator->terminal_) {
				ans.push_back(std::make_pair(pos, templateid));
			}
			ansiterator = hard_suf_link_move(ansiterator);
		}
}

TMatchResults TStaticTemplateMatcher::MatchStream(ICharStream &stream) {
	used = true;
	std::vector<TOccurance> ans;
	size_t pos = 0;
	currentnode = getroot();
	if (!getroot()->terminal_.empty()) 
		for (TStringID &templateid: getroot()->terminal_)
			ans.push_back(std::make_pair(0,templateid));
	while (!stream.IsEmpty()) {
		char newchar=stream.GetChar();
		if (static_cast<int>(newchar) < 32 || static_cast<int>(newchar) > 255) 
			throw std::logic_error("Character out of range");
		Handlesymbol(newchar, ans, pos);
		pos++;
	}
	return ans;
}
TDynamicTemplateMatcher::TDynamicTemplateMatcher():Matchers(), templates(), CheckOnce() {
	template_groups.push_back(std::vector<size_t>());
	Matchers.push_back(std::vector<TStaticTemplateMatcher>());
}

TStringID TDynamicTemplateMatcher::AddTemplate(const std::string &templatestr) {
	std::vector<size_t> powerof2(1,1); 
	CheckOnce.addstring(templatestr, 0);
	size_t currentpos = 0; 
	templates.push_back(templatestr);
	template_groups[0].push_back(templates.size() - 1);
	Matchers[0].push_back(TStaticTemplateMatcher());
	Matchers[0].back().AddTemplate(templatestr);
	while (Matchers[currentpos].size() > 1) {
		TStaticTemplateMatcher tmp;
		if (template_groups.size() - 1 == currentpos) {
			template_groups.push_back(std::vector<size_t>());
			Matchers.push_back(std::vector<TStaticTemplateMatcher>());
			
		}
		if (powerof2.size() - 1 == currentpos)
			powerof2.push_back(powerof2.back()*2);
		while (Matchers[currentpos].size() > 0)
			Matchers[currentpos].pop_back();
		while (!template_groups[currentpos].empty()) {
			template_groups[currentpos + 1].push_back(template_groups[currentpos].back());
			tmp.AddTemplate(templates[template_groups[currentpos].back()]);
			template_groups[currentpos].pop_back();
		}
		Matchers[currentpos + 1].push_back(std::move(tmp));
		currentpos++;
	}
	return templates.size() - 1;	
}

TMatchResults TDynamicTemplateMatcher::MatchStream(ICharStream& stream){ 
	std::vector<TOccurance> ans;
	size_t pos = 0;
	for (size_t groupnumber = 0; groupnumber < Matchers.size(); ++groupnumber) 
		if (!Matchers[groupnumber].empty() && !Matchers[groupnumber][0].getroot()->terminal_.empty()) 
			ans.push_back(std::make_pair(0,Matchers[groupnumber][0].getroot()->terminal_[0]));
	while (!stream.IsEmpty()) {
		char newchar=stream.GetChar();
		if (static_cast<int>(newchar) < 32 || static_cast<int>(newchar) > 255) 
			throw std::logic_error("Character out of range");
		for (size_t groupnumber = 0; groupnumber < Matchers.size(); ++groupnumber) {
			if (!Matchers[groupnumber].empty()) {
				TMatchResults tmp;
				Matchers[groupnumber][0].Handlesymbol(newchar, tmp, pos);
				while (!tmp.empty()) {
					tmp.back().second = template_groups[groupnumber][tmp.back().second];
					ans.push_back(tmp.back());
					tmp.pop_back();
				}
			}
		}
		pos++;
	}
	return ans;
}



void T2DSingleTemplateMatcher::AddTemplateTable(const std::vector<string> &templatetable) {
	templateheight = templatetable.size(); 
	Matcher.allowsametemplates = true;
	if (!templatetable.empty())
		templatelength = templatetable.back().size();
	for (size_t row_number = 0; row_number < templatetable.size(); ++row_number) {
		if (templatetable[row_number].size()!=templatelength)
			throw std::logic_error("row lengths don't match");
		Matcher.AddTemplate(templatetable[row_number]);
	}
}

void T2DSingleTemplateMatcher::FindRowMatchSequence(const std::vector<size_t>& Matches, size_t columnnumber,
													TMatchResults &Result, const std::vector<size_t>& equivalences) {
	std::vector<size_t> prefix_function(Matches.size(),0);
	if (!Matches.empty()) 
		if (Matches[0] == 0) {
			prefix_function[0]=1;
			if (templateheight == 1)
				Result.push_back(std::make_pair(0,columnnumber));
		}
	for (size_t rownumber = 1; rownumber < textheight; ++rownumber) {
		size_t prev = prefix_function[rownumber - 1];
		if (Matches[rownumber] != equivalences[prev])
			prev = 0;
		prefix_function[rownumber] = prev;
		if (Matches[rownumber]==equivalences[prev])
			++prefix_function[rownumber];
		if (prefix_function[rownumber] == templateheight)
			Result.push_back(std::make_pair(rownumber, columnnumber));
	}
}

TMatchResults T2DSingleTemplateMatcher::MatchTable(const std::vector<string> &texttable){
	string text;
	std::vector<size_t> equivalence_class(templateheight, -1);
	equivalence_class.push_back(-2);
	textheight = texttable.size();
	if (!texttable.empty())
		textlength = texttable.back().size();
	for (size_t row_number = 0; row_number < texttable.size(); ++row_number) {
		if (texttable[row_number].size()!=textlength)
			throw std::logic_error("row lengths don't match");
		text+=texttable[row_number];
	}
	TStringStream stream(text);
	TMatchResults RowsMatch = Matcher.MatchStream(stream);
	std::vector<std::vector<size_t>> Matches(textlength, std::vector<size_t>(textheight,-1));

	for (size_t Match_number = 0; Match_number < RowsMatch.size(); ++Match_number) {
		size_t x = RowsMatch[Match_number].first / textlength;
		size_t y = RowsMatch[Match_number].first % textlength;
		if (Matches[y][x] + 1 == 0) 
			Matches[y][x]= RowsMatch[Match_number].second;
		if (equivalence_class[RowsMatch[Match_number].second] + 1 == 0) 
				equivalence_class[RowsMatch[Match_number].second] = Matches[y][x];
	}
	TMatchResults Res;
	for (size_t columntocheck = templatelength - 1;columntocheck < textlength ;++columntocheck) {
		FindRowMatchSequence(Matches[columntocheck], columntocheck, Res, equivalence_class);
	}	
	return Res;
}



