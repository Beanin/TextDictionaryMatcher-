#include "NaiveMatcher.h"
#include "SingleMatcher.h"
#include <iostream>
#include <map>
using namespace std;
int main(){
	/*std::map<char, TTrieNode*> q;
	std::cout << q.count('a');*/
	TNaiveTemplateMatcher Matcher1;
	TSingleTemplateMatcher Matcher2;
	std::cout << "aba " << Matcher1.AddTemplate("aba") << std::endl;
	cout << "cac "<< Matcher1.AddTemplate("cac") << endl;
	cout << "bac " <<  Matcher1.AddTemplate("bac") << endl;
	cout << "aca " << Matcher1.AddTemplate("aca") << endl;
	cout << "ba " << Matcher1.AddTemplate("ba") << endl;
	cout << "abacaba " <<   Matcher1.AddTemplate("abacaba") << endl;
	Matcher2.AddTemplate("aba");
	TStringStream stream("abacaba");
	
	TMatchResults q(Matcher1.MatchStream(stream));
	for (size_t i=0;i < q.size(); i++)
		std::cout << q[i].first << " " << q[i].second << std::endl;
	stream.reset();
	cout << "asda";
	TMatchResults p(Matcher2.MatchStream(stream));
	for (size_t i = 0; i < p.size(); ++i) 
		std::cout << p[i].first << " " << p[i].second << std::endl;
	
	
}