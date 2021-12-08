#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN          53
#define RST_PIN         49

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int led_pin {8};
const int led_pin2{7};
const int button_pin{2};
const int button_pin2{3};
char state ='0';
int card[12] = {000000000000};
int test[12] = {000000000000};

void setup()
{
  pinMode(led_pin, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  pinMode(button_pin, INPUT);
  pinMode(button_pin2, INPUT);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.init();         
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Velkommen til");
  lcd.setCursor(0, 1);
  lcd.print("USN lanekontroller");
  delay(3000);
  lcd.clear();
}

void loop()
{
 	 if (state == '0')
	 {
    		lcd.backlight();
    		lcd.setCursor(0, 0);
    		lcd.print("   Scan         ");
    		lcd.setCursor(0, 1);
    		lcd.print("   studentkortet");
        		if (mfrc522.PICC_IsNewCardPresent())
     		 {
         			if (mfrc522.PICC_ReadCardSerial())
          			{
           				for (byte i = 0; i < mfrc522.uid.size; i++)
          				{
           					card[i] = mfrc522.uid.uidByte[i];
              					Serial.print(card[i]);
            				}
            				delay(1000);
        				state = Serial.read();
        				Serial.println();
        				mfrc522.PICC_HaltA();
          			}
      
     			if (state=='1')
      			{
        				digitalWrite(led_pin, HIGH);
        				delay(2000);
        				digitalWrite(led_pin, LOW);
        				state = '0';  
        				for (byte i = 0; i < 12; i++)
       				{
          					card[i] = 0;
        				}  
      			}
        			else
{
state='0';
}
   		   }
  	  }
  }
