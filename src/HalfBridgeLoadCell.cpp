#include "HalfBridgeLoadCell.h" 


HalfBridgeLoadCell::HalfBridgeLoadCell(HX711* loadCell, int dataPin, int sckPin) {
	loadCell = loadCell;
	loadCell->begin(dataPin, sckPin);
	loadCell->set_scale(2280.f);
	loadCell->tare();
}

/**
 * @brief attempts to retreive data from HX711, then cycle through listener events
 * currentSensorValue will remain unchanged if we cannot get a new value from the HX711
 * 
 * @return float* - a pointer to the current sensor value
 */
float* HalfBridgeLoadCell::read() {
	if (loadCell->is_ready()){
		currentSensorValue = loadCell->get_units(10);
		lastSensorValue = currentSensorValue;
		onValueListenerRoutine();
		onErrorListenerRoutine();
		return &currentSensorValue;
	}
	return &currentSensorValue;
}

// event listeners
/**
 * @brief calls setOnValueListener overload
 * OnValueEvent is triggered when abs(currentSensorValue - valueToListen) > errorThreshold
 * 
 * @param callback 
 * @param valueToListen 
 */
void HalfBridgeLoadCell::setOnValueListener(void (*callback)(float), float* valueToListen) {
	setOnValueListener(callback, valueToListen, 0);
};
template <typename T>
void HalfBridgeLoadCell::setOnValueListener(void (T::*callback)(float), float* valueToListen) {
	setOnValueListener(callback, valueToListen, 0);
};
/**
 * @brief creates a new instance of the onValueListener struct, then adds it to onValueListenerArray
 * OnValueEvent is triggered when abs(currentSensorValue - valueToListen) > errorThreshold
 * 
 * @param callback - a function to call when the event is triggered.
 * @param valueToListen - desired float sensor value
 * @param errorThreshold - a tolerable error magnitude
 */
void HalfBridgeLoadCell::setOnValueListener(void (*callback)(float), float* valueToListen, float* errorThreshold) {
	onValueListener vl = onValueListener(callback, valueToListen, errorThreshold);
	onValueListenerArray.push_back(vl);
};
template <typename T>
void HalfBridgeLoadCell::setOnValueListener(void (T::*callback)(float), float* valueToListen, float* errorThreshold) {
	onValueListener vl = onValueListener(callback, valueToListen, errorThreshold);
	onValueListenerArray.push_back(vl);
};
/**
 * @brief creates a new instance of the onValueListener struct, then adds it to onErrorListenerArray
 * OnErrorEvent is triggered when abs(currentSensorValue - lastSensorValue) > errorThreshold
 * 
 * @param callback - a function to call when the event is triggered.
 * @param errorThreshold - a tolerable error magnitude
 */
void HalfBridgeLoadCell::setOnErrorListener(void (*callback)(float), float* errorThreshold) {
	onValueListener vl = onValueListener(callback, errorThreshold);
	onErrorListenerArray.push_back(vl);
};
template <typename T>
void HalfBridgeLoadCell::setOnErrorListener(void (T::*callback)(float), float* errorThreshold) {
	onValueListener vl = onValueListener(callback, errorThreshold);
	onErrorListenerArray.push_back(vl);
};