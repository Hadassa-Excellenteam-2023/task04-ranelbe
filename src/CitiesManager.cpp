#include "CitiesManager.h"


/*
* return the singleton instance of the class
*/
CitiesManager& CitiesManager::instance()
{
	static CitiesManager instance;
	return instance;
}


/*
* ctor - read the cities data from the file
*/
CitiesManager::CitiesManager()
{
	readCities(FILE_NAME);
}


/*
* read the cities data from the file
* @param fileName - the name of the file
* throw exception if file is unaccessible, empty or in wrong format
*/
void CitiesManager::readCities(const string& fileName)
{
	std::ifstream dataFile(fileName);
	if (!dataFile.is_open()) {
		throw std::runtime_error(std::format(ERROR_FILE_OPEN, fileName)); //cannot open the file
	} 
	else if (dataFile.peek() == std::ifstream::traits_type::eof()) {
		dataFile.close();
		throw std::runtime_error(std::format(ERROR_FILE_EMPTY, fileName)); //file is empty
	}

	// read the data from the file
    string cityName, locRow;
    while (getline(dataFile, cityName) && getline(dataFile, locRow)) {

		Location loc = extractLoc(fileName, locRow);
		_umapCities.insert(City(cityName, loc));
		_mmapCitiesX.insert(std::make_pair(loc, cityName)); //by x cords
		_mmapCitiesY.insert(std::make_pair(loc, cityName)); //by y cords
    }
	
	dataFile.close();
}


/*
* extract the location from the given row 
* @param fileName - the name of the file
* @param locRow - the row to extract the location from "xcords - ycords"
* return the extracted location
*/
Location CitiesManager::extractLoc(const string& fileName, 
	const string& locRow) const
{
	std::stringstream ss(locRow);
	double x, y;
	char c;
	if (!(ss >> x >> c >> y)) {
		throw std::runtime_error(std::format(ERROR_FILE_PARSE, fileName));
	}
	return Location(x, y);
}


/*
* check if the given city exists in the data
* @param cityName - the name of the city
* return true if the city exists, false otherwise
*/
bool CitiesManager::isCityExist(const string& cityName) const
{
	auto it = _umapCities.find(cityName);
	return it != _umapCities.end();
}


/*
* print the search results for the given city and radius:
* the number of cities found in the given radius,
* the number of cities that are to the north of the selected city,
* all the cities found in the given radius sorted by distance
* @param cityName - the name of the city
* @param radius - the radius to search in
* @param norm - the norm to calculate the distance by (pointer to function)
*/
void CitiesManager::printSearchResults(const string& cityName, double radius, const NormFunc& norm) const
{
	City centerCity = *_umapCities.find(cityName);
	Location centerCityLoc = centerCity.second;
	
	// calculate the intersection square
	auto square = findIntersectionSquare(centerCityLoc, radius);

	// map cities by distance (sorted by distance)
	auto squareSortedByDistance = mapCitiesByDistance(centerCityLoc, norm, square);

	auto closest = ++squareSortedByDistance.begin(); // exclude the center city
	auto farthest = squareSortedByDistance.upper_bound(radius);
	auto numOfCities = std::distance(closest, farthest);

	// count the number of cities that are to the north of the selected city
	int numOfNothCities = std::count_if(closest, farthest,
		[&](const auto& pair) {
			return _umapCities.find(pair.second)->
				second._y > centerCityLoc._y;
		});
	
	// print the results
	cout << "Search result:\n";
	cout << numOfCities << " city / cities found in the given radius.\n";
	cout << numOfNothCities << " cities are to the north of the selected city.\n";

	cout << "Cities list:\n";
	std::transform(closest, farthest, 
		std::ostream_iterator<string>(cout, "\n"),
		[](const auto& pair) { return pair.second; });
}


/*
* find the intersection square
* @param centerLoc - the location of the center city
* @radius - the radius to search in
* return a mmap of all the cities in the given radius square.
*/
mmapLocToCityByX CitiesManager::findIntersectionSquare(const Location& centerLoc, double radius) const
{

	// find the bounds iterators
	auto itBeginX = _mmapCitiesX.lower_bound(
		Location(centerLoc._x - radius, 0)
	);
	auto itEndX = _mmapCitiesX.upper_bound(
		Location(centerLoc._x + radius, 0)
	);
	auto itBeginY = _mmapCitiesY.lower_bound(
		Location(0, centerLoc._y - radius)
	);
	auto itEndY = _mmapCitiesY.upper_bound(
		Location(0, centerLoc._y + radius)
	);

	mmapLocToCityByX mmapCitiesY(itBeginY, itEndY); // technical, for the intersection

	// find the intersection square
	mmapLocToCityByX square;
	std::set_intersection(itBeginX, itEndX, mmapCitiesY.begin(), mmapCitiesY.end(), 
		std::inserter(square, square.begin()), LessPairsX());

	return square;
}


/*
* map cities by distance (sorted by distance)
* @param centerLoc - the location of the center city
* @param normFunc - the norm to calculate the distance by (pointer to function)
* @param square - the square of cities to map by distance
* return a mmap of all the cities in the given radius square, sorted by distance
*/
mmapDistanceToCity CitiesManager::mapCitiesByDistance(const Location& centerLoc,
	const NormFunc& normFunc, const mmapLocToCityByX& square) const
{
	mmapDistanceToCity mmapDistanceToCity;
	std::transform(square.cbegin(), square.cend(), 
		std::inserter(mmapDistanceToCity, mmapDistanceToCity.begin()),
			[&](const auto& curCity) {
				double distance = normFunc(curCity.first, centerLoc);
				return std::make_pair(distance, curCity.second);
			}
	);
	return mmapDistanceToCity;
}
