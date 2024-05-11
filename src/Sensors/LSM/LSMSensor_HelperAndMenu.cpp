
#include "LSMSensor.h"
#include <Arduino.h>


void LSMSensor::setScale()
{
  char c;

  Serial.println(F("Set accelerometer scale:"));
  Serial.println(F("\t1) +/- 2G"));
  Serial.println(F("\t2) +/- 4G"));
  Serial.println(F("\t3) +/- 6G"));
  Serial.println(F("\t4) +/- 8G"));
  Serial.println(F("\t5) +/- 16G"));
  while (Serial.available() < 1)
    ;
  c = Serial.read();
  switch (c)
  {
    case '1':
      dof.setAccelScale(dof.A_SCALE_2G);
      break;
    case '2':
      dof.setAccelScale(dof.A_SCALE_4G);
      break;
    case '3':
      dof.setAccelScale(dof.A_SCALE_6G);
      break;
    case '4':
      dof.setAccelScale(dof.A_SCALE_8G);
      break;
    case '5':
      dof.setAccelScale(dof.A_SCALE_16G);
      break;
  }

  Serial.println(F("Set gyroscope scale:"));
  Serial.println(F("\t1) +/- 245 DPS"));
  Serial.println(F("\t2) +/- 500 DPS"));
  Serial.println(F("\t3) +/- 2000 DPS"));
  while (Serial.available() < 1)
    ;
  c = Serial.read();
  switch (c)
  {
    case '1':
      dof.setGyroScale(dof.G_SCALE_245DPS);
      break;
    case '2':
      dof.setGyroScale(dof.G_SCALE_500DPS);
      break;
    case '3':
      dof.setGyroScale(dof.G_SCALE_2000DPS);
      break;
  }

  Serial.println(F("Set magnetometer scale:"));
  Serial.println(F("\t1) +/- 2GS"));
  Serial.println(F("\t2) +/- 4GS"));
  Serial.println(F("\t3) +/- 8GS"));
  Serial.println(F("\t4) +/- 12GS"));
  while (Serial.available() < 1)
    ;
  c = Serial.read();
  switch (c)
  {
    case '1':
      dof.setMagScale(dof.M_SCALE_2GS);
      break;
    case '2':
      dof.setMagScale(dof.M_SCALE_4GS);
      break;
    case '3':
      dof.setMagScale(dof.M_SCALE_8GS);
      break;
    case '4':
      dof.setMagScale(dof.M_SCALE_12GS);
      break;
  }
}

void LSMSensor::setRaw()
{
  if (printRaw)
  {
    printRaw = false;
    Serial.println(F("Printing calculated readings"));
  }
  else
  {
    printRaw = true;
    Serial.println(F("Printing raw readings"));
  }
}

void LSMSensor::setODR()
{
  char c;

  Serial.println(F("Set Accelerometer ODR (Hz):"));
  Serial.println(F("\t1) 3.125 \t 6) 100"));
  Serial.println(F("\t2) 6.25  \t 7) 200"));
  Serial.println(F("\t3) 12.5  \t 8) 400"));
  Serial.println(F("\t4) 25    \t 9) 800"));
  Serial.println(F("\t5) 50    \t A) 1600"));
  while (Serial.available() < 1)
    ;
  c = Serial.read();
  switch (c)
  {
    case '1':
      dof.setAccelODR(dof.A_ODR_3125);
      break;
    case '2':
      dof.setAccelODR(dof.A_ODR_625);
      break;
    case '3':
      dof.setAccelODR(dof.A_ODR_125);
      break;
    case '4':
      dof.setAccelODR(dof.A_ODR_25);
      break;
    case '5':
      dof.setAccelODR(dof.A_ODR_50);
      break;
    case '6':
      dof.setAccelODR(dof.A_ODR_100);
      break;
    case '7':
      dof.setAccelODR(dof.A_ODR_200);
      break;
    case '8':
      dof.setAccelODR(dof.A_ODR_400);
      break;
    case '9':
      dof.setAccelODR(dof.A_ODR_800);
      break;
    case 'A':
    case 'a':
      dof.setAccelODR(dof.A_ODR_1600);
      break;
  }

  Serial.println(F("Set Gyro ODR/Cutoff (Hz):"));
  Serial.println(F("\t1) 95/12.5 \t 8) 380/25"));
  Serial.println(F("\t2) 95/25   \t 9) 380/50"));
  Serial.println(F("\t3) 190/125 \t A) 380/100"));
  Serial.println(F("\t4) 190/25  \t B) 760/30"));
  Serial.println(F("\t5) 190/50  \t C) 760/35"));
  Serial.println(F("\t6) 190/70  \t D) 760/50"));
  Serial.println(F("\t7) 380/20  \t E) 760/100"));
  while (Serial.available() < 1)
    ;
  c = Serial.read();
  switch (c)
  {
    case '1':
      dof.setGyroODR(dof.G_ODR_95_BW_125);
      break;
    case '2':
      dof.setGyroODR(dof.G_ODR_95_BW_25);
      break;
    case '3':
      dof.setGyroODR(dof.G_ODR_190_BW_125);
      break;
    case '4':
      dof.setGyroODR(dof.G_ODR_190_BW_25);
      break;
    case '5':
      dof.setGyroODR(dof.G_ODR_190_BW_50);
      break;
    case '6':
      dof.setGyroODR(dof.G_ODR_190_BW_70);
      break;
    case '7':
      dof.setGyroODR(dof.G_ODR_380_BW_20);
      break;
    case '8':
      dof.setGyroODR(dof.G_ODR_380_BW_25);
      break;
    case '9':
      dof.setGyroODR(dof.G_ODR_380_BW_50);
      break;
    case 'A':
    case 'a':
      dof.setGyroODR(dof.G_ODR_380_BW_100);
      break;
    case 'B':
    case 'b':
      dof.setGyroODR(dof.G_ODR_760_BW_30);
      break;
    case 'C':
    case 'c':
      dof.setGyroODR(dof.G_ODR_760_BW_35);
      break;
    case 'D':
    case 'd':
      dof.setGyroODR(dof.G_ODR_760_BW_50);
      break;
    case 'E':
    case 'e':
      dof.setGyroODR(dof.G_ODR_760_BW_100);
      break;
  }

  Serial.println(F("Set Magnetometer ODR (Hz):"));
  Serial.println(F("\t1) 3.125 \t 4) 25"));
  Serial.println(F("\t2) 6.25  \t 5) 50"));
  Serial.println(F("\t3) 12.5  \t 6) 100"));
  while (Serial.available() < 1)
    ;
  c = Serial.read();
  switch (c)
  {
    case '1':
      dof.setMagODR(dof.M_ODR_3125);
      break;
    case '2':
      dof.setMagODR(dof.M_ODR_625);
      break;
    case '3':
      dof.setMagODR(dof.M_ODR_125);
      break;
    case '4':
      dof.setMagODR(dof.M_ODR_25);
      break;
    case '5':
      dof.setMagODR(dof.M_ODR_50);
      break;
    case '6':
      dof.setMagODR(dof.M_ODR_100);
      break;
  }
}

void LSMSensor::printMenu()
{
  Serial.println();
  Serial.println(F("////////////////////////////////////////////"));
  Serial.println(F("// LSM9DS0 Super Awesome Amazing Fun Time //"));
  Serial.println(F("////////////////////////////////////////////"));
  Serial.println();
  Serial.println(F("1) Stream Accelerometer"));
  Serial.println(F("2) Stream Gyroscope"));
  Serial.println(F("3) Stream Magnetometer"));
  Serial.println(F("4) Stream output from all sensors"));
  Serial.println(F("5) Set Sensor Scales"));
  Serial.println(F("6) Switch To/From Raw/Calculated Readings"));
  Serial.println(F("7) Set Output Data Rates")); 
  Serial.println(); 
}

void LSMSensor::parseMenu(char c)
{
  switch (c)
  {
    case '1':
      while(!Serial.available())
        printAccel();
      break;
    case '2':
      while(!Serial.available())
        printGyro();
      break;
    case '3':
      while(!Serial.available())
        printMag();
      break;
    case '4':
      while(!Serial.available())
      {
        streamAll();
      }
      break;
    case '5':
      setScale();
      break;
    case '6':
      setRaw();
      break;
    case '7':
      setODR();
      break;
  }
}


void LSMSensor::printAccel()
{
  // Only read from the accelerometer if the accel interrupts,
  // which means that new data is ready.
  //if (digitalRead(INT1XM))
  //{
    // Use the readAccel() function to get new data from the accel.
    // After calling this function, new values will be stored in
    // the ax, ay, and az variables.
    dof.readAccel();

    Serial.print("A: ");
    if (false) // Overroode this cause i never use
    {
      Serial.print(dof.ax);
      Serial.print(", ");
      Serial.print(dof.ay);
      Serial.print(", ");
      Serial.println(dof.az);
    }
    else
    {    
      float magnitude = sqrt(dof.calcAccel(dof.ax) * dof.calcAccel(dof.ax) + dof.calcAccel(dof.ay) * dof.calcAccel(dof.ay) + dof.calcAccel(dof.az) *dof.calcAccel(dof.az));
      Serial.print("Mag: " + String(magnitude) + " g, ");
      Serial.print(dof.calcAccel(dof.ax));
      Serial.print(", ");
      Serial.print(dof.calcAccel(dof.ay));
      Serial.print(", ");
      Serial.println(dof.calcAccel(dof.az));
      
    // Calculate the magnitude of the overall g-force
    }
  //}
}

void LSMSensor::printGyro()
{
  // Only read from the gyro if the DRDY interrupts,
  // which means that new data is ready.
  // if (digitalRead(DRDYG))
  // {
    // Use the readGyro() function to get new data from the gyro.
    // After calling this function, new values will be stored in
    // the gx, gy, and gz variables.
    dof.readGyro();

    Serial.print("G: ");
    if (printRaw)
    {
      Serial.print(dof.gx);
      Serial.print(", ");
      Serial.print(dof.gy);
      Serial.print(", ");
      Serial.println(dof.gz);
    }
    else
    {
      Serial.print(dof.calcGyro(dof.gx));
      Serial.print(", ");
      Serial.print(dof.calcGyro(dof.gy));
      Serial.print(", ");
      Serial.println(dof.calcGyro(dof.gz));
    }
  //}
}

void LSMSensor::printMag()
{
  // Only read from the magnetometer if the INT2XM interrupts,
  // which means that new data is ready.
  // if (digitalRead(INT2XM))
  // {
    // Use the readMag() function to get new data from the mag.
    // After calling this function, new values will be stored in
    // the mx, my, and mz variables.
    dof.readMag();

    Serial.print("M: ");
    if (printRaw)
    {
      Serial.print(dof.mx);
      Serial.print(", ");
      Serial.print(dof.my);
      Serial.print(", ");
      Serial.print(dof.mz);
      Serial.print(", ");
      Serial.println(calcHeading(dof.mx, dof.my, dof.mz));
    }
    else
    {
      Serial.print(dof.calcMag(dof.mx), 4);
      Serial.print(", ");
      Serial.print(dof.calcMag(dof.my), 4);
      Serial.print(", ");
      Serial.print(dof.calcMag(dof.mz), 4);
      Serial.print(", ");
      Serial.println(calcHeading(dof.mx, dof.my, dof.mz));
    }
  //}
}

// Helper function to convert integer to string and save to buffer
int LSMSensor::intToString(int value, char* buffer) {
  char* start = buffer;
  if (value < 0) {
      *buffer++ = '-';
      value = -value;
  }

  int length = 0;
  int temp = value;

  // Count digits and prepare for reverse filling
  do {
      temp = temp / 10;
      length++;
  } while (temp);

  // Reverse fill the buffer
  buffer += length;
  *buffer = '\0'; // null-terminate
  do {
      *(--buffer) = '0' + (value % 10);
      value = value / 10;
  } while (value);

  return length + (start != buffer); // length + 1 if negative
}

// Helper function to convert float to string and save to buffer
// buffer must be large enough to hold the resulting string
// precision is the number of digits after the decimal point
int LSMSensor::floatToString(float value, char* buffer, int precision) {
    char* start = buffer;

    // Handle negative numbers
    if (value < 0) {
        *buffer++ = '-';
        value = -value;
    }

    // Handle the integer part
    int intPart = static_cast<int>(value);
    int len = intToString(intPart, buffer);
    buffer += len;

    // Handle the decimal point and fractional part
    if (precision > 0) {
        *buffer++ = '.'; // Add decimal point

        // Extract fractional part based on specified precision
        float fractional = value - intPart;
        for (int i = 0; i < precision; ++i) {
            fractional *= 10;
            int digit = static_cast<int>(fractional) % 10;
            *buffer++ = '0' + digit;
        }
        *buffer = '\0'; // Null-terminate
    }

    return buffer - start; // Total length of the string
}

void LSMSensor::selectMenu() {
  
//   // LSM Sensor Operation
//   SPI.beginTransaction(settingsLSMSensor);
//   // digitalWrite(LSM9DS0_CSG, LOW); // Assuming LSM_CS is the CS pin for your LSM sensor
//   // digitalWrite(LSM9DS0_CSXM, LOW); // Assuming LSM_CS is the CS pin for your LSM sensor
//   // Perform LSM sensor operations
//   adaFruitLSM.printMenu();
//   // Then wait for any serial data to come in:
//   while (!Serial.available())
//     ;
//   // Once serial data is received, call parseMenu to act on it:
//   adaFruitLSM.parseMenu(Serial.read());
//   // digitalWrite(LSM9DS0_CSG, HIGH); // Assuming LSM_CS is the CS pin for your LSM sensor
//   // digitalWrite(LSM9DS0_CSXM, HIGH); // Assuming LSM_CS is the CS pin for your LSM sensor
//   SPI.endTransaction();

//  adaFruitLSM.printMenu();
//   // Then wait for any serial data to come in:
//   while (!Serial.available())
//     ;
//   // Once serial data is received, call parseMenu to act on it:
//   adaFruitLSM.parseMenu(Serial.read());
}