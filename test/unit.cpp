#ifndef CATCH_CONFIG_MAIN
    #define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#endif // !CATCH_CONFIG_MAIN
#include <cmath>
#include "catch.hpp"

float calculateError(float* value1, float* value2) {
    return std::abs(*value1 - *value2);
}

bool withinErrorThreshold(float* value1, float* value2, float* errorThreshold) {
    return calculateError(value1, value2) < *errorThreshold;
};

TEST_CASE("Error is computed", "[calculateError]" ) {
    float a = 1.2f;
    float b = 1.2f;
    REQUIRE( calculateError(&a,&b) == 0);
}

TEST_CASE("Error is within threshold", "[withinErrorThreshold]" ) {
    float a = 10.f;
    float b = 5.f;
    float c = 12.f;
    REQUIRE( withinErrorThreshold(&a,&b,&c) == true);
}

TEST_CASE("Error is not within threshold", "[withinErrorThreshold]" ) {
    float a = 10.f;
    float b = 5.f;
    float c = 2.f;
    REQUIRE( withinErrorThreshold(&a,&b,&c) == false);
}