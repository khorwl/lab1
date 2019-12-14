#include <Arduino.h>
#include <MD_TCS230.h>

#include "button.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5

#define R_OUT 6
#define G_OUT 7
#define B_OUT 8

#define R_CONTROL_PIN 9
#define G_CONTROL_PIN 10
#define B_CONTROL_PIN 11

MD_TCS230 colorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

bool rButtonActivated = false;
bool gButtonActivated = false;
bool bButtonActivated = false;

Button rButton(R_CONTROL_PIN);
Button gButton(G_CONTROL_PIN);
Button bButton(B_CONTROL_PIN);

void setup()
{
    Serial.begin(115200);
    Serial.println("Started!");

    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 0;
    whiteCalibration.value[TCS230_RGB_G] = 0;
    whiteCalibration.value[TCS230_RGB_B] = 0;

    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 0;
    blackCalibration.value[TCS230_RGB_G] = 0;
    blackCalibration.value[TCS230_RGB_B] = 0;

    colorSensor.begin();
    colorSensor.setDarkCal(&blackCalibration);
    colorSensor.setWhiteCal(&whiteCalibration);

    pinMode(R_OUT, OUTPUT);
    pinMode(G_OUT, OUTPUT);
    pinMode(B_OUT, OUTPUT);
}

void loop() 
{
    colorData rgb;
    colorSensor.read();

    while (!colorSensor.available());

    if (rButton.wasPressed()){
      Serial.println("Red button pressed");
      rButtonActivated = !rButtonActivated;
    }

    if (gButton.wasPressed()) {
      Serial.println("Green button pressed");
      gButtonActivated = !gButtonActivated;
    }

    if (bButton.wasPressed()){
      Serial.println("Blue button pressed");
      bButtonActivated = !bButtonActivated;
    }
    
    colorSensor.getRGB(&rgb); 
    print_rgb(rgb);
    set_rgb_led(rgb);
}

void print_rgb(colorData rgb)
{
  Serial.print(rgb.value[TCS230_RGB_R]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_G]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_B]);
  Serial.println();
}


void set_rgb_led(colorData rgb)
{
    analogWrite(R_OUT, 255 - !rButtonActivated * rgb.value[TCS230_RGB_R]);
    analogWrite(G_OUT, 255 - !gButtonActivated * rgb.value[TCS230_RGB_G]);
    analogWrite(B_OUT, 255 - !bButtonActivated * rgb.value[TCS230_RGB_B]);
}
