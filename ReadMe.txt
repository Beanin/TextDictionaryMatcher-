Base.h - Базовый интерфейс
BaseTest.h - включение файлов Boost::test, объявление функции сравнения TMatchResults
BaseTest.cpp реализация BaseTest.h
trie.h - интерфейс бора
trie.cpp - реализация бора 
NaiveMatcher.h - интерфейс TNaiveTemplateMatcher
NaiveMatcher.cpp - реализация TNaiveTemplateMatcher
SingleMatcher.h - интерфейс TSingleTemplateMatcher, TWildcardSingleTemplateMatcher
SingleMatcher.cpp - реализация TSingleTemplateMatcher, TWildcardSingleTemplateMatcher
StaticMatcher.h - интерфейс TStaticTemplateMatcher, TDynamicTemplateMatcher, T2DSingleTemplateMatcher 
StaticMatcher.cpp - реализация TStaticTemplateMatcher, TDynamicTemplateMatcher, T2DSingleTemplateMatcher 

TestNaive.cpp, TestStatic.cpp, TestSingle.cpp - минимальный набор тестов к классам из соответствующих .h файлов. Пополняется.

 
