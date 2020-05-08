#include <string>
#include <cmath>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {

    // calculations
    
    const double mintues = double(seconds) / 60.0f;
    const double hours = mintues / 60.0f;
    const double seconds_epoch = mintues - double(long(mintues)); // Extract numbers after decimal point
    const double mintues_epoch = hours - double(long(hours)); // Extract numbers after decimal point
    
    const int seconds_result = round(seconds_epoch * 60.0f);
    const int mintues_result = floor(mintues_epoch * 60.0f);

    // string casting
    
    string hours_string = std::to_string(long(hours));
    string mintues_string = std::to_string(mintues_result);
    string seconds_string = std::to_string(seconds_result);

    // left zero padding

    if(hours_string.length() == 1) hours_string = "0" + hours_string;
    if(mintues_string.length() == 1) mintues_string = "0" + mintues_string;
    if(seconds_string.length() == 1) seconds_string = "0" + seconds_string;

    return hours_string + ":" + mintues_string + ":" + seconds_string;
}