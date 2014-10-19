#include "trie.h"

TTrieNode::TTrieNode():terminal_(-1),edges(),parentnode_(nullptr),parentsymbol_(),height_() {
}
TTrieNode::TTrieNode(shared_ptr<TTrieNode> parentnode, char parentsymbol, size_t height):terminal_(-1),edges(),parentnode_(parentnode),parentsymbol_(parentsymbol),height_(height) {
}

shared_ptr<TTrieNode> TTrieNode::next(char nextsymbol) {
	return edges[nextsymbol];
}
void TTrieNode::setEdge(char nextsymbol, shared_ptr<TTrieNode> nextnode) {
	if (edges.count(nextsymbol)) 
		throw std::logic_error("Trie edge override");
	else edges[nextsymbol] = nextnode;
}
void TTrieNode::maketerminal(TStringID id) {
	if (terminal_ != -1) 
		throw std::logic_error("string already exists");
	else 
		terminal_ = id;
}
TStringID TTrieNode::terminal() {
	return terminal_;
}
bool TTrieNode::isEdge(char newchar) {
	return edges.count(newchar);
}
TTrie::TTrie():root_(new TTrieNode()) {
}
shared_ptr<TTrieNode> TTrie::getroot() {
	return root_;
}
size_t TTrieNode::height(){
	return height_;
}
char TTrieNode::parentsymbol() {
	return parentsymbol_;
}
shared_ptr<TTrieNode> TTrieNode::parent() {
	return parentnode_;
}

shared_ptr<TTrieNode> TTrie::addstring(const string& newstring, TStringID id) {
	shared_ptr<TTrieNode> currentnode = getroot();
	for (size_t curpos = 0; curpos < newstring.length(); ++curpos) {
		if (currentnode->isEdge(newstring[curpos])) {
			currentnode = currentnode->next(newstring[curpos]);
		}
		else {
			currentnode->setEdge(newstring[curpos],shared_ptr<TTrieNode>(new TTrieNode(currentnode, newstring[curpos],currentnode->height() + 1)));
			currentnode = currentnode->next(newstring[curpos]);  
		}
	}
	currentnode->maketerminal(id);
	return currentnode;
}
shared_ptr<TTrieNode> TTrie::addReverseString(const string& newstring, TStringID id) {
	shared_ptr<TTrieNode> currentnode = getroot();
	for (size_t curpos = newstring.length(); curpos > 0; --curpos) {
		if (currentnode->isEdge(newstring[curpos - 1])) 
			currentnode = currentnode->next(newstring[curpos - 1]);
		else {
			currentnode->setEdge(newstring[curpos - 1],shared_ptr<TTrieNode>(new TTrieNode(currentnode, newstring[curpos - 1],currentnode->height() + 1)));
			currentnode = currentnode->next(newstring[curpos - 1]);  
		}
	}
	currentnode->maketerminal(id);
	return currentnode;
}




