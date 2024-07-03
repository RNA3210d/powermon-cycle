#include <DHT11.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h" 

//blk 5v reg
//BLU NEW GR_ R2: 9.79k, R1:21.5k A1
//GR NEW YLO_ R2: 6.72k, R1: 42.9k A0, BUS. RED IN - GEN BUS

float GBR1 = 43200.0;
float GBR2 = 6720.0;

float RBR1 = 21500.0;
float RBR2 = 9870.0;

#define GEN_BUS A0
#define REG_BUS A1


float adc=5.0;

DHT11 dht11(2);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 



void setup() {
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
  lcd.print("MELCOW!"); // Print the string "Hello World!"
  lcd.setCursor(0, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print("Initializing...");
  delay(200);
  lcd.clear();


//pinMode(LED_BUILTIN, OUTPUT);

}


void loop(){
  
  dhtdisp();
  delay(800);
  voltdisp();
  

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


//delay(700);
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
 
  lcd.print(BV2);lcd.print("V");delay(320); if(n==8)
  break;
  }

}