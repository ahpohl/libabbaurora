/*
 *  Simple sketch that shows the basic usage of the ABBAurora class.
 */
#include <ABBAurora.h>

#define INVERTER_ADDRESS 2

ABBAurora *inverter;
void setup()
{
    Serial.begin(115200);
    ABBAurora::setup(Serial2, RX2, TX2, TX_CONTROL_GPIO);
    inverter = new ABBAurora(INVERTER_ADDRESS);
    Serial.println("Setup done");
}

void loop()
{
    if (inverter->ReadVersion())
    {
        Serial.print("Inverter Name: ");
        Serial.println(inverter->Version.Par1);
    }
    else
    {
        Serial.print("Inverter could not be reached");
        delay(500);
        return;
    }

    if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
    {
        Serial.print("Pin1 : ");
        Serial.print(inverter->DSP.Value);
        Serial.println(" W");
    }

    if (inverter->ReadCumulatedEnergy(CURRENT_DAY))
    {
        Serial.print("Energy: ");
        Serial.print(inverter->CumulatedEnergy.Energy);
        Serial.println(" Wh");
    }
}
