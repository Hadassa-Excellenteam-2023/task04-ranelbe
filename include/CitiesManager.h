#pragma once

#include "macros.h"
#include <fstream>
#include <sstream>
#include <format>
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>


constexpr auto FILE_NAME = "data.txt";
constexpr auto ERROR_FILE_OPEN = "Failed to open the file {}";
constexpr auto ERROR_FILE_EMPTY = "File {} is empty";
constexpr auto ERROR_FILE_PARSE = "Failed in parsing the file {}";

using umapCityToLoc = std::unordered_map <string, Location>; // (city, loc) mapping

// (loc, city) sorted by x cords and by y cords.
using mmapLocToCityByX = std::multimap <Location, string, LessX>; 
using mmapLocToCityByY = std::multimap <Location, string, LessY>;

using mmapDistanceToCity = std::multimap <double, string>; // (distance, city) mapping


/*
a singleton class that manages the cities data.
this class is responsible for reading the data from the file,
and for searching and prinring the cities.
*/
class CitiesManager {

public:

	static CitiesManager& instance(); //singelton
	~CitiesManager() = default;
	bool isCityExist(const string& cityName) const;
	void printSearchResults(const string& cityName, double radius, const NormFunc& normFunc) const;

private:

	CitiesManager();
	void readCities(const string& fileName);
	Location extractLoc(const string& fileName, const string& locRow) const;

	mmapLocToCityByX findIntersectionSquare(const Location& centerLoc, double radius) const;
	mmapDistanceToCity mapCitiesByDistance(const Location& centerLoc,
		const NormFunc& normFunc, const mmapLocToCityByX& square) const;

	// map city to location so we can perform an efficient search in average of O(1):
	umapCityToLoc _umapCities;
	
	// (loc, city) sorted by x cords and by y cords.
	mmapLocToCityByX _mmapCitiesX;
	mmapLocToCityByY _mmapCitiesY;

};
