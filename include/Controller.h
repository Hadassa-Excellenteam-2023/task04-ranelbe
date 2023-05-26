#pragma once

#include "macros.h"
#include "CitiesManager.h"
#include <unordered_map>
#include <functional>
#include <math.h>

constexpr auto ENTER_CITY_NAME = "Please enter selected city name (with line break after it) or '0' to exit.\n";
constexpr auto ENTER_RADIUS = "Please enter the wanted radius:\n";
constexpr auto ENTER_NORM = "Please enter the wanted norm(0 - L2, Euclidean distance, 1 - Linf, "
							"Chebyshev distance, 2 - L1, Manhattan distance):\n";

constexpr auto ERROR_CITY_NOT_FOUND = "isn't found in the city list. Please try again.";
constexpr auto ERROR_RADIUS_NOT_VALID = "is not >= 0. Please try again.";
constexpr auto ERROR_NORM_NOT_VALID = "is not in {0,1,2}. Please try again.";
constexpr auto ERROR_INPUT = "Problem with reading the input.";


using std::unordered_map;
using std::runtime_error;

// data struct container for reading input from user
template <typename T>
struct Input {
    const string& promptMsg;
    const string& errorMsg;
    std::function<void(T&)> readFunction;
    std::function<bool(const T&)> isBadInput;
};

class Controller {
public:
	void run();

private:

    // read input from user and check if it's valid
    template <typename T>
    T readInput(const Input<T>& input) const
    {
		T inputVal;
		cout << input.promptMsg;
		input.readFunction(inputVal);
        while (input.isBadInput(inputVal)) {
            cerr << inputVal << " " << input.errorMsg << endl;
            cout << input.promptMsg;
			input.readFunction(inputVal);
            if (cin.fail()) {
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );
                cin.clear();
                throw runtime_error(ERROR_INPUT);
            }
		}
		return inputVal;
	}

    string readCityName() const;
    double readRadius() const;
    NormFunc readNorm() const;

    
    // map of all the norm functions
    unordered_map < int, NormFunc > _normFunctions =
    {
        {0, [](const Location& loc1, const Location& loc2) {
			return std::sqrt(std::pow((loc1._x - loc2._x), 2) +
                std::pow((loc1._y - loc2._y), 2));
		}},
        {1, [](const Location& loc1, const Location& loc2) {
            return std::max(std::abs(loc1._x - loc2._x),
                std::abs(loc1._y - loc2._y));
        }},
        {2, [](const Location& loc1, const Location& loc2) {
            return std::abs(loc1._x - loc2._x) +
                std::abs(loc1._y - loc2._y);
        }}
	};
};
