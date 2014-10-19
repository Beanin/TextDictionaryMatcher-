#include "Base.h"
#include <map>
using std::string;
using std::map;
using std::shared_ptr;

class TTrieNode {
private:
	TStringID terminal_;
	map<char,shared_ptr<TTrieNode>> edges;
	shared_ptr<TTrieNode> parentnode_;
	char parentsymbol_;
	size_t height_;
public:
	TTrieNode();
	TTrieNode(shared_ptr<TTrieNode> parentnode, char parentsymbol, size_t height);
	shared_ptr<TTrieNode> next(char nextsymbol);
	void setEdge(char nextsymbol, shared_ptr<TTrieNode> nextnode);
	void maketerminal(TStringID id);
	TStringID terminal();
	bool isEdge(char nextsymbol);
	size_t height();
	char parentsymbol();
	shared_ptr<TTrieNode> parent();
	
};
class TTrie{
private:
	shared_ptr<TTrieNode> root_;
public:
	TTrie();
	shared_ptr<TTrieNode> getroot();
	shared_ptr<TTrieNode> addstring(const string &newstring, TStringID id);
	shared_ptr<TTrieNode> addReverseString(const string &newstring, TStringID id);
};
