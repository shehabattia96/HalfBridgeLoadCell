
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <cmath>
#include "catch.hpp" // we are going to use catch2 to test native functions.

//-------------- error calculation --------------//

// we're pulling the logic out of the main class to test since we cannot compile Arduino on native.
float calculateError(float* value1, float* value2) {
    return std::abs((*value1 - *value2));
}

bool withinErrorThreshold(float* value1, float* value2, float* errorThreshold) {
    return calculateError(value1, value2) < *errorThreshold;
};

TEST_CASE("Error is computed", "[calculateError]" ) {
    float a = 1.2f;
    float b = 1.2f;
    REQUIRE( calculateError(&a,&b) == 0.0f);
    float c = 3.2f;
    REQUIRE( calculateError(&a,&c) == 2.0f);
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