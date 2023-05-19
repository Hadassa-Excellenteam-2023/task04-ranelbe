#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <functional>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::getline;
using std::string;


struct Location 
{ 
	double _x, _y;

	bool operator==(const Location& other) const
	{
		return _x == other._x && _y == other._y;
	}
	
	double getX() const { return _x; }
	double getY() const { return _y; }
};


// use in unordered_map to keep it sorted by x
struct LessX
{
	bool operator()(const Location& left, const Location& right) const 
	{
		return left._x < right._x;
	}
};

struct LessPairsX {
	bool const operator()(const std::pair<Location, std::string>& lhs,
		const std::pair<Location, std::string>& rhs) const {
		return lhs.first.getX() < rhs.first.getX();
	}
};

// use in unordered_map to keep it sorted by y
struct LessY 
{
	bool operator()(const Location& left, const Location& right) const
	{ 
		return left._y < right._y;
	}  
};

using City = std::pair<std::string, Location>;
using NormFunc = std::function<double(const Location&, const Location&)>;

