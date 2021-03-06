#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#define ADDRESS1 0x68
#define ADDRESS2 0x69

unsigned long prevTime;
unsigned long currTime;
unsigned long loopTime;

String directory = "arduino";
String subsystem = "composites";

File out;

typedef union accel_t_gyro_union
{
  struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } reg;
  struct 
  {
    int16_t x_accel;
    int16_t y_accel;
    int16_t z_accel;
    int16_t temperature;
    int16_t x_gyro;
    int16_t y_gyro;
    int16_t z_gyro;
  } value;
};

void setup(){

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only.
  }
  
  
  
  //Setup Accelerometer
  Wire.begin();
  // Clear the 'sleep' bit to start the sensor.
  MPU6050_write_reg(ADDRESS1, 0x6B, 0);
  MPU6050_write_reg(ADDRESS2, 0x6B, 0);
  // Set to 16G range
  MPU6050_write_reg(ADDRESS1, 0x1C, bit(3)|bit(4));
  MPU6050_write_reg(ADDRESS2, 0x1C, bit(3)|bit(4));

  

  // Setup SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");



  // Create arduino directory
  if (!SD.exists(directory)) {
    SD.mkdir(directory);
    Serial.println("Created directory '" + directory + "'.");
  } else {
    Serial.println("Directory '" + directory + "' already exists.");
  }



  // Create new file
  int filename = 1;
  while(SD.exists(directory + "/" + String(filename) + ".csv")) {
    filename++;
  }
  out = SD.open(directory + "/" + String(filename) + ".csv", FILE_WRITE);
  Serial.println("Created file '" + String(filename) + ".csv' in directory.");

  // Setup file
  out.println("Hello " + subsystem + "!");
  out.println(F("The accelerometer is configured for a 16G range: 2048 counts = 1G."));
  out.println(F("The gyroscope is configured for 250 deg/sec: 131 counts = 1 deg/sec."));
  out.println(F("Sample time records time between the current row and the previous row."));
  out.println(F("-Electrons"));
  out.println();
  out.println();
//  if (SD.exists("readme.txt")){
//    Serial.println("README file exists!");
//    File readMe = SD.open("readme.txt", FILE_READ);
//    while (readMe.peek() != -1){
//      out.print(char(readMe.read()));
//    }
//    out.println();
//    out.println();
//  }
  out.print("Accel1_X,Accel1_Y,Accel1_Z,Gyro1_X,Gyro1_Y,Gyro1_Z,,");
  out.print("Accel2_X,Accel2_Y,Accel2_Z,Gyro2_X,Gyro2_Y,Gyro2_Z,,");
  out.println("Sample_Time(µs)");

  // Start timer
  prevTime = micros();
}


void loop() {

  accel_t_gyro_union accel_t_gyro1;
  accel_t_gyro_union accel_t_gyro2;

  // Update timer
  currTime = micros();
  loopTime = currTime - prevTime;
  prevTime = currTime;

  // Read raw values
  MPU6050_read(ADDRESS1, 0x3B, (uint8_t *) &accel_t_gyro1, sizeof(accel_t_gyro1));
  MPU6050_read(ADDRESS2, 0x3B, (uint8_t *) &accel_t_gyro2, sizeof(accel_t_gyro2));



  // Swap values
  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap

  SWAP (accel_t_gyro1.reg.x_accel_h, accel_t_gyro1.reg.x_accel_l);
  SWAP (accel_t_gyro1.reg.y_accel_h, accel_t_gyro1.reg.y_accel_l);
  SWAP (accel_t_gyro1.reg.z_accel_h, accel_t_gyro1.reg.z_accel_l);
  SWAP (accel_t_gyro1.reg.x_gyro_h, accel_t_gyro1.reg.x_gyro_l);
  SWAP (accel_t_gyro1.reg.y_gyro_h, accel_t_gyro1.reg.y_gyro_l);
  SWAP (accel_t_gyro1.reg.z_gyro_h, accel_t_gyro1.reg.z_gyro_l);

  SWAP (accel_t_gyro2.reg.x_accel_h, accel_t_gyro2.reg.x_accel_l);
  SWAP (accel_t_gyro2.reg.y_accel_h, accel_t_gyro2.reg.y_accel_l);
  SWAP (accel_t_gyro2.reg.z_accel_h, accel_t_gyro2.reg.z_accel_l);
  SWAP (accel_t_gyro2.reg.x_gyro_h, accel_t_gyro2.reg.x_gyro_l);
  SWAP (accel_t_gyro2.reg.y_gyro_h, accel_t_gyro2.reg.y_gyro_l);
  SWAP (accel_t_gyro2.reg.z_gyro_h, accel_t_gyro2.reg.z_gyro_l);



  // Print raw values

  // Print the raw acceleration values (1)
  out.print(accel_t_gyro1.value.x_accel, DEC);
  out.print(F(","));
  out.print(accel_t_gyro1.value.y_accel, DEC);
  out.print(F(","));
  out.print(accel_t_gyro1.value.z_accel, DEC);
  out.print(F(","));

  // Print the raw gyro values (1)
  out.print(accel_t_gyro1.value.x_gyro, DEC);
  out.print(F(","));
  out.print(accel_t_gyro1.value.y_gyro, DEC);
  out.print(F(","));
  out.print(accel_t_gyro1.value.z_gyro, DEC);
  out.print(F(",,"));

  // Print the raw acceleration values (2)
  out.print(accel_t_gyro2.value.x_accel, DEC);
  out.print(F(","));
  out.print(accel_t_gyro2.value.y_accel, DEC);
  out.print(F(","));
  out.print(accel_t_gyro2.value.z_accel, DEC);
  out.print(F(","));

  // Print the raw gyro values (2)
  out.print(accel_t_gyro2.value.x_gyro, DEC);
  out.print(F(","));
  out.print(accel_t_gyro2.value.y_gyro, DEC);
  out.print(F(","));
  out.print(accel_t_gyro2.value.z_gyro, DEC);
  out.print(F(",,"));

  // Print time
  out.print(loopTime);
  out.println(F(""));

  // Cleanup
  out.flush();
}

// --------------------------------------------------------
// MPU6050_read
//
// This is a common function to read multiple bytes 
// from an I2C device.
//
// It uses the boolean parameter for Wire.endTransMission()
// to be able to hold or release the I2C-bus. 
// This is implemented in Arduino 1.0.1.
//
// Only this function is used to read. 
// There is no function for a single byte.
//
int MPU6050_read(int address, int start, uint8_t *buffer, int size)
{
  int i, n, error;

  Wire.beginTransmission(address);
  n = Wire.write(start);
  if (n != 1)
    return (-10);

  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(address, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // return : no error
}

// --------------------------------------------------------
// MPU6050_write
//
// This is a common function to write multiple bytes to an I2C device.
//
// If only a single register is written,
// use the function MPU_6050_write_reg().
//
// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.
//
// If only a single register is written, a pointer
// to the data has to be used, and the size is
// a single byte:
//   int data = 0;        // the data to write
//   MPU6050_write (address, 0x6B, &c, 1);
//
int MPU6050_write(int address, int start, const uint8_t *pData, int size)
{
  int n, error;

  Wire.beginTransmission(address);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);

  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
    return (error);

  return (0);         // return : no error
}

// --------------------------------------------------------
// MPU6050_write_reg
//
// An extra function to write a single register.
// It is just a wrapper around the MPU_6050_write()
// function, and it is only a convenient function
// to make it easier to write a single register.
//
int MPU6050_write_reg(int address, int reg, uint8_t data)
{
  int error;

  error = MPU6050_write(address, reg, &data, 1);

  return (error);
}
