#include <MD_TCS230.h>

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5

MD_TCS230 colorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Press enter to start calibration");
  readInput();
  colorSensor.begin();
}

void loop() 
{
    calibrate();
}

String readInput()
{
    while (!Serial.available());
    return Serial.readStringUntil('\n');
}

void calibrate()
{
    sensorData blackCalibration, whiteCalibration;

    readRaw(blackCalibration, "BLACK");
    colorSensor.setDarkCal(&blackCalibration);

    readRaw(whiteCalibration, "WHITE");
    colorSensor.setWhiteCal(&whiteCalibration);

    Serial.println("Calibration values:\n");
    printCalibration("whiteCalibration", whiteCalibration);
    Serial.println();
    printCalibration("blackCalibration", blackCalibration);
    Serial.println();
    Serial.println("Press enter to read RGB value. Send 'retry' to calibrate again");
    while (true) {
        String command = readInput();
        if (command == "retry") {
            return;
        }

        colorData rgb;
        readRgb(rgb);
    }
}

void readRaw(sensorData &sd, const char *cardColor)
{
    Serial.print("Put ");
    Serial.print(cardColor);
    Serial.println(" card on the sensor and press enter");
    readInput();
    colorSensor.read();

    while (!colorSensor.available());

    colorSensor.getRaw(&sd);
    Serial.print(cardColor);
    Serial.println(" calibration finished");
}

void readRgb(colorData &cd)
{
    Serial.println("Reading RGB...");
    colorSensor.read();

    while (!colorSensor.available());

    colorSensor.getRGB(&cd);
    Serial.print(cd.value[TCS230_RGB_R]);
    Serial.print(' ');
    Serial.print(cd.value[TCS230_RGB_G]);
    Serial.print(' ');
    Serial.print(cd.value[TCS230_RGB_B]);
    Serial.println();
}

void printCalibration(const char *varName, const sensorData &sd)
{
    Serial.print("sensorData ");
    Serial.print(varName);
    Serial.println(";");
    printAssignment(varName, "R", sd.value[TCS230_RGB_R]);
    printAssignment(varName, "G", sd.value[TCS230_RGB_G]);
    printAssignment(varName, "B", sd.value[TCS230_RGB_B]);
}

void printAssignment(const char *varName, const char *colorName, int32_t value)
{
    Serial.print(varName);
    Serial.print(".value[TCS230_RGB_");
    Serial.print(colorName);
    Serial.print("] = ");
    Serial.print(value);
    Serial.println(";");
}
