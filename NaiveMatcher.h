#include "Base.h"
#include "trie.h"
using std::shared_ptr;

class TNaiveTemplateMatcher:public IMetaTemplateMatcher {
private:
	TTrie templates;
	std::vector<shared_ptr<TTrieNode>> template_ends;
	size_t maxtemplatelength;
public:
	TStringID AddTemplate(const std::string &templatestr) override;
	TMatchResults MatchStream(ICharStream &stream) override;
};