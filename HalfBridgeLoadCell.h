/**
 * @file HalfBridgeLoadCell.h
 * @author Shehab Attia
 * @brief 
 * @version 0.1
 * @date 2019-12-29
 * 
 * @copyright Copyright (c) 2019
 * 
 * we want to implement load sensing using the HX711:
 * we want to initialzie HX711 with HX711::begin(dtPin, sckPin), HX711::set_scale(2280.f), HX711::tare
 * we want to utilize HX711::is_ready when reading using HX711::get_units(10) so that we're not blocking the code
 * we also want to create an event listener for
 * 1. sensor value error > threshold
 * 2. sensor value at one or more specified voltages
 * the event listeners should call a callback
 */
#include <Arduino.h>
#include <vector>
#include <cmath>

#ifndef HalfBridgeLoadCell_h
#define HalfBridgeLoadCell_h

// struct for our event listener
struct onValueListener
{
    void (*callback)(float);
    float* valueToListen;
    float* errorThreshold;
    // struct constructors
    onValueListener(void (*callback)(float), float* errorThreshold) : callback(callback), errorThreshold(errorThreshold) {}
    onValueListener(void (*callback)(float), float* valueToListen, float* errorThreshold) : callback(callback), valueToListen(valueToListen), errorThreshold(errorThreshold) {}  
};

typedef HX711;
class HalfBridgeLoadCell {
    private:
        HX711* loadCell; // pointer to the HX711 instance

        // sensor values that we'll use to trigger events
        float lastSensorValue;
        float lastError;
        float currentSensorValue;

        /**
         * @brief Calculates the absolute difference between to floats
         * 
         * @param value1 
         * @param value2 
         */
        void calculateError(float* value1, float* value2) {
            lastError = std::abs(*value1 - *value2);
        }
        /**
         * @brief if the result from calculateError is small enough (iow smaller than the threshold), we don't trip any alarms.
         * 
         * @param value1 
         * @param value2 
         * @param errorThreshold 
         * @return true - if there's negligible error
         * @return false - if change is significant
         */
        bool withinErrorThreshold(float* value1, float* value2, float* errorThreshold) {
            calculateError(value1, value2);
            return lastError < *errorThreshold;
        };

        // vectors holding all the subscribed listeners
        std::vector<onValueListener> onValueListenerArray;
        std::vector<onValueListener> onErrorListenerArray;

        /**
         * @brief iterates through onValueListenerArray and triggers callback functions if necessary
         * 
         */
        void onValueListenerRoutine() {
            std::vector<onValueListener>::iterator ptr; 
            for (ptr = onValueListenerArray.begin(); ptr < onValueListenerArray.end(); ptr++) {
                if (withinErrorThreshold(ptr->valueToListen, &currentSensorValue, ptr->errorThreshold)){
                    ptr->callback(currentSensorValue);
                }
            }
        }
        /**
         * @brief iterates through onErrorListenerArray and triggers callback functions if necessary
         * 
         */
        void onErrorListenerRoutine() {
            std::vector<onValueListener>::iterator ptr; 
            for (ptr = onErrorListenerArray.begin(); ptr < onErrorListenerArray.end(); ptr++) {
                if (withinErrorThreshold(&lastSensorValue, &currentSensorValue, ptr->errorThreshold)){
                    ptr->callback(currentSensorValue);
                }
            }
        }
    public:
        ~ HalfBridgeLoadCell();
        HalfBridgeLoadCell();
        HalfBridgeLoadCell(HX711* loadCell, int dataPin, int sckPin);

        float* read();

        void setOnValueListener(void (*callback)(float), float* valueToListen);
        void setOnValueListener(void (*callback)(float), float* valueToListen, float* errorThreshold);
        void setOnErrorListener(void (*callback)(float), float* errorThreshold);
};

#endif // !HalfBridgeLoadCell_h
