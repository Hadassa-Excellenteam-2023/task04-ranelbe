#include "Controller.h"
#include "macros.h"
#include <iostream>
#include <sstream>


/*
* the main loop - read input from user and print the results
*/
void Controller::run()
{
    string cityName;

    cityName = readCityName();
	while (cityName != "0")
    {
        double radius = readRadius();
        NormFunc norm = readNorm();

        CitiesManager::instance().printSearchResults(cityName, radius, norm);

        cityName = readCityName();
	}
    cout << "Bye Bye :)" << endl;

}

/*
* read city name from user and check if it's valid
* return the city name or throw exception if the input is invalid
*/
string Controller::readCityName() const
{
    const Input<string> cityInput = {
        ENTER_CITY_NAME,
        ERROR_CITY_NOT_FOUND,
        [](string& cityName) {
            getline(cin, cityName);
        },
        [](const string& cityName) {
            return cityName!="0" && !CitiesManager::instance().isCityExist(cityName);
        }
    };
    return readInput(cityInput);
}

/*
* read radius from user and check if it's valid
* return the radius or throw exception if the input is invalid
*/
double Controller::readRadius() const
{
    const Input<double> radiusInput = {
           ENTER_RADIUS,
           ERROR_RADIUS_NOT_VALID,
           [](double& radius) {
               cin >> radius;
               cin.ignore();
           },
           [](const double& radius) {
               return radius < 0;
           }
    };
    return readInput(radiusInput);
}

/*
* read norm from user and check if it's valid
* return pointer to the wanted norm function or throw exception if the input is invalid
*/
NormFunc Controller::readNorm() const
{
    const Input<int> normInput = {
		   ENTER_NORM,
		   ERROR_NORM_NOT_VALID,
           [](int& norm) {
			   cin >> norm;
			   cin.ignore();
		   },
           [&](const int& norm) {
			   return _normFunctions.find(norm) == _normFunctions.end();
		   }
	};
    int norm = readInput(normInput);
    return _normFunctions.at(norm); // pointer to the wanted norm function
}
