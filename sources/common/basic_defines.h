#pragma once

// C
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <cfenv>
#include <cinttypes>
#include <cstdint>
#include <cwchar>
#include <cwctype>

// C++
#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <chrono>
#include <complex>
#include <condition_variable>
#include <deque>
#include <exception>
#include <expected>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <set>
#include <shared_mutex>
#include <span>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>
#include <xutility>
#include <print>
#include <filesystem>

// GSL
#include <gsl/pointers> // owner, not_null
#undef GSL_ASSUME
#undef GSL_LIKELY
#undef Expects
#undef Ensures

using std::literals::string_view_literals::operator""sv;