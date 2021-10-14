#include <LiquidCrystal.h>
#include <IRremote.h>

int RECV_PIN = 9; //PWM pin olmalı
IRrecv irrecv(RECV_PIN);
decode_results results;

int ledPin = 11;   
int ledPin2 = 12;
int PIRpin = 8;            
int pirState = LOW;            
int val = 0;    

int photocellPin = 0;     
int photocellReading;            
int pinSpeaker = 10;  // PWM pin olmalı 

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);                        


void setup() {    
  
  //pinleri çıkış veya giriş olarak ayarlamaya
  
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(PIRpin, INPUT);     
  pinMode(photocellPin, INPUT);
  pinMode(pinSpeaker, OUTPUT);
    
  Serial.begin(9600);
  irrecv.enableIRIn(); // Alıcıyı başlatmaya
   
  lcd.begin(16, 2); 
  lcd.setCursor(2, 0);                   	// LCD imleç konumunu ayarlamaya
  lcd.print("Hareket ve  ");                  //Metni LCD'ye yazdırmaya
  lcd.setCursor(0, 1);                      // LCD imleç konumunu ayarlamaya 
  lcd.print("Isik Sensorleri");           // Metni LCD'ye yazdırmaya
  delay(2000);   //2 saniye bekleme               // Metni okumada gecikme
                                                              
	lcd.clear();                            // LCD temizleme    
	lcd.setCursor(0, 0); 
    lcd.print("Veri isleniyor.");
	playTone(300, 300);
	delay(150);
	playTone(0, 0);
    delay(3000);
    lcd.clear(); 
    lcd.setCursor(3, 0);
    lcd.print("Hareket");
    lcd.setCursor(3, 1);
    lcd.print("Bekleniyor ...");    
  
}

void loop(){
  
  //yeşil ledi kontrol etmek için uzaktan kumandayı kullanma
  
   if(irrecv.decode(&results)){         
  Serial.println(results.value, HEX);
    switch(results.value){
      
    case 0xFD08F7:               // 1 kullanarak yeşil LED'i açma
      digitalWrite(ledPin2, HIGH);
      break;
       	
    case 0xFD8877:                //2 kullanarak yeşil LED'i kapatma
      digitalWrite(ledPin2, LOW); 
    }
    irrecv.resume();
  }

  val = digitalRead(PIRpin);  
  photocellReading = analogRead(photocellPin);

  if (val == HIGH) {            // girişin HIGH(YÜKSEK)olup olmadığını kontrol etme
    digitalWrite(ledPin, HIGH);  // kırmızı LED'i açma


    if (pirState == LOW) {
      
      Serial.println("Hareket algilandi!");
	
      lcd.clear() ;
      lcd.setCursor(0, 0);                
      lcd.print("Hareket algilandi!");   
      lcd.setCursor(0, 1);                  
      lcd.print(photocellReading); 
      
        //çıktı değişikliğine yazdırmak
      
      playTone(300, 300);
	  delay(150);
	  playTone(0, 0);
	  
      pirState = HIGH;   
    }   
     
  } else {
      digitalWrite(ledPin, LOW); // kırmızı LED'i kapatma
      delay(100);    

    
	  scrollScreenSaver() ;	 
	  
      if (pirState == HIGH){
       
		Serial.println("Hareket sona erdi!");
        pirState = LOW;
    }   
  }
}

// süresi, hertz cinsinden frekans
void playTone(long duration, int freq) {
    duration *= 1000;
    int period = (1.0 / freq) * 100000;
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        digitalWrite(13, HIGH);
    digitalWrite(pinSpeaker,HIGH);
      
        //delayMicroseconds(period / 2);
        digitalWrite(13, LOW);
    digitalWrite(pinSpeaker, LOW);
        //delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}

void scrollScreenSaver() {
	
	lcd.clear() ;
  	lcd.setCursor(15, 0);                 
	lcd.print("Hareket Yok ");   
	lcd.setCursor(15, 1); 
	lcd.print("Bekleniyor");	
	
    for (int positionCounter = 0; positionCounter < 22; positionCounter++) {
      
		lcd.scrollDisplayLeft();
		delay(5);
    
	}
}