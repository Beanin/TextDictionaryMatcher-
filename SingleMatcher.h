#include "Base.h"
using std::string;
using std::vector;

class TSingleTemplateMatcher:public IMetaTemplateMatcher {
private:
	bool initialized;
	string substr_;
	ptrdiff_t currentvalue; 
	vector<size_t> values;
public:
	TSingleTemplateMatcher();
	size_t AddChar(char newchar);
	TStringID AddTemplate(const string &template_) override;
	TMatchResults MatchStream(ICharStream &stream) override;
};
