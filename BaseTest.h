#pragma once
#include <boost/test/unit_test.hpp>
#include <exception>
#include "Base.h"
#include <algorithm>
#include "NaiveMatcher.h"

bool CompareResults(TMatchResults sample, TMatchResults standart);