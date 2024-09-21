#include <DHT11.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h" 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

//blk 5v reg
//BLU NEW GR_ R2: 9.79k, R1:21.5k A1
//GR NEW YLO_ R2: 6.72k, R1: 42.9k A0, BUS. RED IN - GEN BUS

float GBR1 = 46700.0;
float GBR2 = 6750.0;

float RBR1 = 46900.0;
float RBR2 = 6780.0;

float BBR1 = 47400.0;
float BBR2 = 6820.0;

#define GEN_BUS A0
#define REG_BUS A1
#define BAT_BUS A2

float adc=5.0;

DHT11 dht11(2);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 



void setup() {
  pinMode(4, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(4, LOW);
  
   digitalWrite(10, LOW);
  //digitalWrite(10, HIGH);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();


  lcd.setCursor(0, 1); 

  lcd.print("Initializing...");  
  lcd.setCursor(0, 0);
  int n1=0;
    if (!mpu.begin()) {
    lcd.print("Failed to find MPU6050 chip");
    while (true) {
      n1++;
      delay(10);
      if(n1==100)
      goto out;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("IMU ON!");
  lcd.setCursor(8, 0);


  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("2G");
    lcd.print("2G,");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    lcd.print("4G,");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("8G");
    lcd.print("8G,");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    lcd.print("16G,");
    break;
  }

  lcd.setCursor(11, 0);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);

  lcd.print("21Hz");

  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);

  digitalWrite(7, HIGH); //READY INDICATE BUZZER
  delay(300);
   digitalWrite(7, LOW);
  out:
  lcd.clear();


//pinMode(LED_BUILTIN, OUTPUT);

}


void loop(){
  
  dhtdisp();
  voltdisp2();
  //voltdisp();
  gyrodisp();
  

}



void   dhtdisp()
{
  int t = 0;
  int h = 0;
  Serial.print("temp:");
  int result = dht11.readTemperatureHumidity(t, h);

 
  Serial.print(t);
  Serial.print("  humi:");
  Serial.println(h);


  lcd.clear();
  
  // display temperature

  lcd.setCursor(0,0);
  lcd.print("TEMP: ");


  lcd.print(t);lcd.print("\xB0""C");
  
  // display humidity
  lcd.setCursor(0,1);
  lcd.print("RH% : ");


  lcd.print(h);lcd.print("%");


delay(700);
}




void   voltdisp()
{
  int n=0;
  while(true)
  {
  float BV1 = 0;
  int adc_value1 = analogRead(GEN_BUS);
  float adc_voltage1  = (adc_value1 * adc) / 1024.0; 
  BV1 = adc_voltage1 / (GBR2/(GBR1+GBR2)) ; 



  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("GEN. BUS: ");

  lcd.print(BV1);
  lcd.print("V");


  
  float BV2 = 0;
  int adc_value2 = analogRead(REG_BUS);
  float adc_voltage2  = (adc_value2 * adc) / 1024.0; 
  BV2 = adc_voltage2 / (RBR2/(RBR1+RBR2)) ; 
  lcd.setCursor(0,1);
  lcd.print("REG. BUS: ");

  n++;
 
  lcd.print(BV2);lcd.print("V");delay(200); if(n==8)
  break;
  }
}

void   gyrodisp()
{
  int n=0; float slopesum=0,tiltsum=0;
  while(n<=7)
  {
    n++;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    int samples=0;
    while(samples<90)
    {
      float slope= RAD_TO_DEG * (atan2(a.acceleration.y, a.acceleration.z));
      slopesum=slopesum+slope;
      float tilt= RAD_TO_DEG * (atan2(a.acceleration.x, a.acceleration.z));
      tiltsum=tiltsum+tilt;
      samples++;
      delay(2);
    }
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Slope: ");
    lcd.setCursor(7,0);
    
    lcd.print(slopesum/90,1);lcd.print(" degs");
    
    lcd.setCursor(0,1);
    lcd.print("Tilt: ");
    lcd.setCursor(6,1);

    lcd.print(tiltsum/90,1);lcd.print(" degs");
    slopesum=0;tiltsum=0;
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
    delay(50);

  }
}


void   voltdisp2()
{
  int n=0;
  while(true)
  {
  float BV1 = 0;
  int adc_value1 = analogRead(GEN_BUS);
  float adc_voltage1  = (adc_value1 * adc) / 1024.0; 
  BV1 = adc_voltage1 / (GBR2/(GBR1+GBR2)) ; 



  lcd.clear();
  
  lcd.setCursor(1,0);
  lcd.print("GEN");
  lcd.setCursor(7,0);
  lcd.print("REG");
  lcd.setCursor(13,0);
  lcd.print("BAT");
  lcd.setCursor(0,1);
  lcd.print(BV1);



  
  float BV2 = 0;
  int adc_value2 = analogRead(REG_BUS);
  float adc_voltage2  = (adc_value2 * adc) / 1024.0; 
  BV2 = adc_voltage2 / (RBR2/(RBR1+RBR2)) ; 
 


  
  lcd.setCursor(6,1);
  lcd.print(BV2);

  float BV3 = 0;
  int adc_value3 = analogRead(BAT_BUS);
  float adc_voltage3  = (adc_value3 * adc) / 1024.0; 
  BV3 = adc_voltage3 / (BBR2/(BBR1+BBR2)) ; 
  lcd.setCursor(12,1);
  lcd.print(BV3);
  
  delay(200);n++; if(n==8)
  break;
  }
}