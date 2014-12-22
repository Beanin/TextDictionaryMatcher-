
#include "BaseTest.h"
#include "SingleMatcher.h"

using std::make_pair;

BOOST_AUTO_TEST_CASE(SingleMatcherFailTest)
{
	TSingleTemplateMatcher TestSingle1;
	TestSingle1.AddTemplate("pqr");
	BOOST_CHECK_THROW(TestSingle1.AddTemplate("123"),std::logic_error);
}

BOOST_AUTO_TEST_CASE(WildcardSingleMatcherFailTest) {
	TWildcardSingleTemplateMatcher TestWildcard;
	TestWildcard.AddTemplate("pqr");
	BOOST_CHECK_THROW(TestWildcard.AddTemplate("123"),std::logic_error);

}

BOOST_AUTO_TEST_CASE(SingleMatcherSimpleTest)
{
	TSingleTemplateMatcher TestSingle1;
	TStringStream Teststream1("aba");
	TStringID templateid = TestSingle1.AddTemplate("ba");
	TMatchResults TrueAns1;
	TrueAns1.push_back(make_pair(2,templateid));
	BOOST_CHECK(CompareResults(TrueAns1, TestSingle1.MatchStream(Teststream1)));
	TStringStream Teststream2("baacaababa");
	TMatchResults TrueAns2;
	TrueAns2.push_back(make_pair(1,templateid));
	TrueAns2.push_back(make_pair(7,templateid));
	TrueAns2.push_back(make_pair(9,templateid));
	BOOST_CHECK(CompareResults(TrueAns2,TestSingle1.MatchStream(Teststream2)));
	TMatchResults TrueAns3;
	TStringStream Teststream3("baaaaaaaaaaabc");
	TrueAns3.push_back(make_pair(1,templateid));
	BOOST_CHECK(CompareResults(TrueAns3,TestSingle1.MatchStream(Teststream3)));
	TMatchResults TrueAns4;
	TrueAns4.push_back(make_pair(8,templateid));
	TestSingle1.AppendCharToTemplate('b');
	Teststream2.reset();
	BOOST_CHECK(CompareResults(TestSingle1.MatchStream(Teststream2),TrueAns4));
	Teststream2.reset();
	TestSingle1.PrependCharToTemplate('a');
	BOOST_CHECK(CompareResults(TestSingle1.MatchStream(Teststream2),TrueAns4));
	}
BOOST_AUTO_TEST_CASE(SingleMatcherBoundariesTest) {
	TStringStream emptystream("");
	TSingleTemplateMatcher TestSingle1;
	TestSingle1.AddTemplate("abacabatest123");
	BOOST_CHECK(CompareResults(TestSingle1.MatchStream(emptystream),TMatchResults()));
	TStringStream teststream2("string");
	BOOST_CHECK(CompareResults(TestSingle1.MatchStream(teststream2),TMatchResults()));
	teststream2.reset();
	TSingleTemplateMatcher TestSingle2;
	TStringID templateid = TestSingle2.AddTemplate("string");
	TMatchResults TrueAns2;
	TrueAns2.push_back(make_pair(5,templateid));
	BOOST_CHECK(CompareResults(TestSingle2.MatchStream(teststream2),TrueAns2));
}

BOOST_AUTO_TEST_CASE(WildcardSingleMatcherBaseTest)
{
	TWildcardSingleTemplateMatcher TestSingle1;
	TStringStream Teststream1("aba");
	TStringID templateid = TestSingle1.AddTemplate("ba");
	TMatchResults TrueAns1;
	TrueAns1.push_back(make_pair(2,templateid));
	BOOST_CHECK(CompareResults(TrueAns1, TestSingle1.MatchStream(Teststream1)));
	TStringStream Teststream2("baacaababa");
	TMatchResults TrueAns2;
	TrueAns2.push_back(make_pair(1,templateid));
	TrueAns2.push_back(make_pair(7,templateid));
	TrueAns2.push_back(make_pair(9,templateid));
	BOOST_CHECK(CompareResults(TrueAns2,TestSingle1.MatchStream(Teststream2)));
	TMatchResults TrueAns3;
	TStringStream Teststream3("baaaaaaaaaaabc");
	TrueAns3.push_back(make_pair(1,templateid));
	BOOST_CHECK(CompareResults(TrueAns3,TestSingle1.MatchStream(Teststream3)));
}

BOOST_AUTO_TEST_CASE(WildcardSingleMatcherSimpleTest)
{
	TWildcardSingleTemplateMatcher TestWildcard1;
	TStringStream Teststream1("aba");
	TStringID templateid = TestWildcard1.AddTemplate("a?a");
	TMatchResults TrueAns1;
	TrueAns1.push_back(make_pair(2,templateid));
	BOOST_CHECK(CompareResults(TrueAns1, TestWildcard1.MatchStream(Teststream1)));
	TStringStream Teststream2("baacaababa");
	TMatchResults TrueAns2;
	TrueAns2.push_back(make_pair(4,templateid));
	TrueAns2.push_back(make_pair(7,templateid));
	TrueAns2.push_back(make_pair(9,templateid));
	BOOST_CHECK(CompareResults(TrueAns2,TestWildcard1.MatchStream(Teststream2)));
	TMatchResults TrueAns3;
	TStringStream Teststream3("baaaaaaaaaaabc");
	TrueAns3.push_back(make_pair(3,templateid));
	TrueAns3.push_back(make_pair(4,templateid));
	TrueAns3.push_back(make_pair(5,templateid));
	TrueAns3.push_back(make_pair(6,templateid));
	TrueAns3.push_back(make_pair(7,templateid));
	TrueAns3.push_back(make_pair(8,templateid));
	TrueAns3.push_back(make_pair(9,templateid));
	TrueAns3.push_back(make_pair(10,templateid));
	TrueAns3.push_back(make_pair(11,templateid));

	BOOST_CHECK(CompareResults(TrueAns3,TestWildcard1.MatchStream(Teststream3)));

	TWildcardSingleTemplateMatcher TestWildcard2;
	templateid = TestWildcard2.AddTemplate("totes?t???lateto");
	TStringStream Teststream4("totesttemplatetotesttestlateto");
	TMatchResults TrueAns4;
	TrueAns4.push_back(make_pair(15,templateid));
	TrueAns4.push_back(make_pair(29,templateid));

	BOOST_CHECK(CompareResults(TrueAns4, TestWildcard2.MatchStream(Teststream4)));
}