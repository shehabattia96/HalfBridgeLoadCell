#include <Arduino.h>
#include <HalfBridgeLoadCell.h>
#include <HX711.h>
HX711 hx711;
void setup() {
    HalfBridgeLoadCell loadcell = HalfBridgeLoadCell(&hx711, 1, 2);
    loadcell.read();
}

void loop() {
  // put your main code here, to run repeatedly:
}