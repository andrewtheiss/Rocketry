#include <SPI.h>

// Pin definitions
#define CS_SERIAL_PORT_ENABLE 10
#define SPC_SERIAL_PORT_CLOCK 13
#define SDI_SERIAL_PORT_DATA_IN 11
#define SDO_SERIAL_PORT_DATA_OUT 12

// Register addresses
#define OUT_X_L_G 0x28
#define OUT_X_H_G 0x29
#define OUT_Y_L_G 0x2A
#define OUT_Y_H_G 0x2B
#define OUT_Z_L_G 0x2C
#define OUT_Z_H_G 0x2D

////////////////////////////
// LSM9DS0 Gyro Registers //
////////////////////////////
#define WHO_AM_I_G			0x0F
#define CTRL_REG1_G			0x20
#define CTRL_REG2_G			0x21
#define CTRL_REG3_G			0x22
#define CTRL_REG4_G			0x23
#define CTRL_REG5_G			0x24
#define REFERENCE_G			0x25
#define STATUS_REG_G		0x27
#define OUT_X_L_G			0x28
#define OUT_X_H_G			0x29
#define OUT_Y_L_G			0x2A
#define OUT_Y_H_G			0x2B
#define OUT_Z_L_G			0x2C
#define OUT_Z_H_G			0x2D
#define FIFO_CTRL_REG_G		0x2E
#define FIFO_SRC_REG_G		0x2F
#define INT1_CFG_G			0x30
#define INT1_SRC_G			0x31
#define INT1_THS_XH_G		0x32
#define INT1_THS_XL_G		0x33
#define INT1_THS_YH_G		0x34
#define INT1_THS_YL_G		0x35
#define INT1_THS_ZH_G		0x36
#define INT1_THS_ZL_G		0x37
#define INT1_DURATION_G		0x38

byte readGyro(byte reg) {
  SPI.transfer(reg | 0x80); // Read operation (MSB = 1)
  return SPI.transfer(0x00); // Dummy write to receive data
}

void SPIreadBytes(uint8_t csPin, uint8_t subAddress, uint8_t * dest, uint8_t count)
{
	digitalWrite(csPin, LOW); // Initiate communication
	// To indicate a read, set bit 0 (msb) to 1
	// If we're reading multiple bytes, set bit 1 to 1
	// The remaining six bytes are the address to be read
	if (count > 1)
		SPI.transfer(0xC0 | (subAddress & 0x3F));
	else
		SPI.transfer(0x80 | (subAddress & 0x3F));
	for (int i=0; i<count; i++)
	{
		dest[i] = SPI.transfer(0x00); // Read into destination array
	}
	digitalWrite(csPin, HIGH); // Close communication
}
#define LSM9DS0_G   10 //0x6B // Would be 0x6A if SDO_G is LOW

uint8_t gReadByte(uint8_t subAddress)
{
	  uint8_t temp;
		SPIreadBytes(LSM9DS0_G, subAddress, &temp, 1);
    return temp;
}

// gWriteByte (uint8_t subAddress, uint8_t data) extension
void SPIwriteByte(uint8_t csPin, uint8_t subAddress, uint8_t data)
{
	digitalWrite(csPin, LOW); // Initiate communication
	
	// If write, bit 0 (MSB) should be 0
	// If single write, bit 1 should be 0
	SPI.transfer(subAddress & 0x3F); // Send Address
	SPI.transfer(data); // Send data
	
	digitalWrite(csPin, HIGH); // Close communication
}

void gWriteByte(uint8_t subAddress, uint8_t data)
{
  SPIwriteByte(LSM9DS0_G, subAddress, data);
}

// Configure Gyro
void configGyroInt(uint8_t int1Cfg, uint16_t int1ThsX, uint16_t int1ThsY, uint16_t int1ThsZ, uint8_t duration)
{
	gWriteByte(INT1_CFG_G, int1Cfg);
	gWriteByte(INT1_THS_XH_G, (int1ThsX & 0xFF00) >> 8);
	gWriteByte(INT1_THS_XL_G, (int1ThsX & 0xFF));
	gWriteByte(INT1_THS_YH_G, (int1ThsY & 0xFF00) >> 8);
	gWriteByte(INT1_THS_YL_G, (int1ThsY & 0xFF));
	gWriteByte(INT1_THS_ZH_G, (int1ThsZ & 0xFF00) >> 8);
	gWriteByte(INT1_THS_ZL_G, (int1ThsZ & 0xFF));
	if (duration)
		gWriteByte(INT1_DURATION_G, 0x80 | duration);
	else
		gWriteByte(INT1_DURATION_G, 0x00);
}
void initGyro()
{
	/* CTRL_REG1_G sets output data rate, bandwidth, power-down and enables
	Bits[7:0]: DR1 DR0 BW1 BW0 PD Zen Xen Yen
	DR[1:0] - Output data rate selection
		00=95Hz, 01=190Hz, 10=380Hz, 11=760Hz
	BW[1:0] - Bandwidth selection (sets cutoff frequency)
		 Value depends on ODR. See datasheet table 21.
	PD - Power down enable (0=power down mode, 1=normal or sleep mode)
	Zen, Xen, Yen - Axis enable (o=disabled, 1=enabled)	*/
	gWriteByte(CTRL_REG1_G, 0x0F); // Normal mode, enable all axes
	
	/* CTRL_REG2_G sets up the HPF
	Bits[7:0]: 0 0 HPM1 HPM0 HPCF3 HPCF2 HPCF1 HPCF0
	HPM[1:0] - High pass filter mode selection
		00=normal (reset reading HP_RESET_FILTER, 01=ref signal for filtering,
		10=normal, 11=autoreset on interrupt
	HPCF[3:0] - High pass filter cutoff frequency
		Value depends on data rate. See datasheet table 26.
	*/
	gWriteByte(CTRL_REG2_G, 0x00); // Normal mode, high cutoff frequency
	
	/* CTRL_REG3_G sets up interrupt and DRDY_G pins
	Bits[7:0]: I1_IINT1 I1_BOOT H_LACTIVE PP_OD I2_DRDY I2_WTM I2_ORUN I2_EMPTY
	I1_INT1 - Interrupt enable on INT_G pin (0=disable, 1=enable)
	I1_BOOT - Boot status available on INT_G (0=disable, 1=enable)
	H_LACTIVE - Interrupt active configuration on INT_G (0:high, 1:low)
	PP_OD - Push-pull/open-drain (0=push-pull, 1=open-drain)
	I2_DRDY - Data ready on DRDY_G (0=disable, 1=enable)
	I2_WTM - FIFO watermark interrupt on DRDY_G (0=disable 1=enable)
	I2_ORUN - FIFO overrun interrupt on DRDY_G (0=disable 1=enable)
	I2_EMPTY - FIFO empty interrupt on DRDY_G (0=disable 1=enable) */
	// Int1 enabled (pp, active low), data read on DRDY_G:
	gWriteByte(CTRL_REG3_G, 0x88); 
	
	/* CTRL_REG4_G sets the scale, update mode
	Bits[7:0] - BDU BLE FS1 FS0 - ST1 ST0 SIM
	BDU - Block data update (0=continuous, 1=output not updated until read
	BLE - Big/little endian (0=data LSB @ lower address, 1=LSB @ higher add)
	FS[1:0] - Full-scale selection
		00=245dps, 01=500dps, 10=2000dps, 11=2000dps
	ST[1:0] - Self-test enable
		00=disabled, 01=st 0 (x+, y-, z-), 10=undefined, 11=st 1 (x-, y+, z+)
	SIM - SPI serial interface mode select
		0=4 wire, 1=3 wire */
	gWriteByte(CTRL_REG4_G, 0x00); // Set scale to 245 dps
	
	/* CTRL_REG5_G sets up the FIFO, HPF, and INT1
	Bits[7:0] - BOOT FIFO_EN - HPen INT1_Sel1 INT1_Sel0 Out_Sel1 Out_Sel0
	BOOT - Reboot memory content (0=normal, 1=reboot)
	FIFO_EN - FIFO enable (0=disable, 1=enable)
	HPen - HPF enable (0=disable, 1=enable)
	INT1_Sel[1:0] - Int 1 selection configuration
	Out_Sel[1:0] - Out selection configuration */
	gWriteByte(CTRL_REG5_G, 0x00);
	
	// Temporary !!! For testing !!! Remove !!! Or make useful !!!
	configGyroInt(0x2A, 0, 0, 0, 0); // Trigger interrupt when above 0 DPS...
}

void setup() {
  Serial.begin(115200);
  
  pinMode(CS_SERIAL_PORT_ENABLE, OUTPUT);
  digitalWrite(CS_SERIAL_PORT_ENABLE, HIGH);
	SPI.begin();
	// Maximum SPI frequency is 10MHz, could divide by 2 here:
	SPI.setClockDivider(SPI_CLOCK_DIV4);
	// Data is read and written MSb first.
	SPI.setBitOrder(MSBFIRST);
	// Data is captured on rising edge of clock (CPHA = 0)
	// Base value of the clock is HIGH (CPOL = 1)
	SPI.setDataMode(SPI_MODE1);

  
	uint8_t gTest = gReadByte(WHO_AM_I_G);		// Read the gyro WHO_AM_I
  Serial.print(gTest, HEX);

	// Gyro initialization stuff:
	initGyro();	// This will "turn on" the gyro. Setting up interrupts, etc.
	//setGyroODR(gODR); // Set the gyro output data rate and bandwidth.
	//setGyroScale(gScale); // Set the gyro range
}

void loop() {
  int16_t xGyro, yGyro, zGyro;

  // Begin SPI transaction
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_SERIAL_PORT_ENABLE, LOW);

  // Read X-axis gyro data
  xGyro = readGyro(OUT_X_L_G) | (readGyro(OUT_X_H_G) << 8);

  // Read Y-axis gyro data
  yGyro = readGyro(OUT_Y_L_G) | (readGyro(OUT_Y_H_G) << 8);

  // Read Z-axis gyro data
  zGyro = readGyro(OUT_Z_L_G) | (readGyro(OUT_Z_H_G) << 8);

  // End SPI transaction
  digitalWrite(CS_SERIAL_PORT_ENABLE, HIGH);
  SPI.endTransaction();

  // Print the gyro data
  Serial.print("X: ");
  Serial.print(xGyro);
  Serial.print(" Y: ");
  Serial.print(yGyro);
  Serial.print(" Z: ");
  Serial.println(zGyro);

  delay(10); // Delay for readability
}
