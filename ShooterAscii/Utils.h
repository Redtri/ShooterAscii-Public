#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <Windows.h>

/*
=====================================================
	Handling common macros and operators overloading
=====================================================
*/

template <typename T>
class ComparePointers {
public:
	bool operator() (T * a, T * b) const {
		return *a < *b;
	}
};

inline
bool operator==(COORD const &a, COORD const& b) {
	return (a.X == b.X && a.Y == b.Y);
}

inline
bool operator==(COORD const &a, SHORT const& b) {
	return (a.X == b && a.Y == b);
}

inline
bool operator<(COORD const &a, COORD const& b) {
	return (a.X == b.X) ? (a.Y < b.Y) : (a.X < b.X);
}

#endif
