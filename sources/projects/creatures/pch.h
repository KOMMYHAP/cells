#pragma once

#include "basic_defines.h"

template <class T>
using Ref = gsl::strict_not_null<T*>;
template <class T>
using ConstRef = gsl::strict_not_null<const T*>;