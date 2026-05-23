#include "battery1.h"


unsigned long lastBatteryUpdate = 0;
float getBatteryPercent() {
    int pinreading = analogRead(A0);
    float voltage = ((pinreading / 1023.0) * 5 * 5)-batterydead;
    float percent = (voltage / batterymax) * 100.0;
    if(percent > 100) percent = 100;
    return percent; // YOU MUST ADD THIS
}
void monitorBattery() 
{
    if (millis() - lastBatteryUpdate > updateInterval) {
        float currentSoC = getBatteryPercent();

        
        Serial.print("B:"); 
        Serial.print(currentSoC);
        Serial.println("%");

        
        if (currentSoC < BATT_CRITICAL) {
            Serial.println("ALERT:LOW_BATT");
            
        }

        lastBatteryUpdate = millis();
    }
}

