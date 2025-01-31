#include <Servo.h>
#include <Oscillator.h>
#include <Otto.h>

//----------------------Otto Similasyon Görseli--------------------------------------------------
/*
                            --------------- 
                           |     O   O     |
                           |---------------|
RIGHTLEG 3 nolu pinde ==>  |               |  <== LEFTLEG 2 nolu pinde
                            --------------- 
                               ||     ||
RIGHTFOOT 5 nolu pinde ==>   -----    -----   <== LEFTFOOT 4 nolu pinde
                            |-----    -----|
*/

// Otto'nun pin tanımlamaları
#define PIN_LEFTLEG 4
#define PIN_RIGHTLEG 5
#define PIN_LEFTFOOT 6
#define PIN_RIGHTFOOT 7
#define PIN_BUZZER 13
#define PIN_US_TRIGGER 8
#define PIN_US_ECHO 9

//Melodi için note tanımları
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// Ultrasonik sensör pinleri
#define trig_Pin PIN_US_TRIGGER
#define echo_Pin PIN_US_ECHO

Otto Otto;  // Otto Robot nesnesi

void Melodi();

void setup() {
  Serial.begin(9600);  // Seri haberleşmeyi başlat
  // Otto'nun başlatılması 
  Otto.init(PIN_LEFTLEG, PIN_RIGHTLEG, PIN_LEFTFOOT, PIN_RIGHTFOOT, true, PIN_BUZZER);
  
  // Robotun bağlantı sesi
  Otto.sing(S_connection);

  pinMode(trig_Pin, OUTPUT);
  pinMode(echo_Pin, INPUT);
}

void loop() {
  // Bluetooth'tan veri kontrolü
  if (Serial.available()) {
    char komut = Serial.read();  // Gelen komutu oku
    Serial.println(komut);

    // Komutlara göre işlemleri yap
    switch (komut) {
      case '1':  // Sabit durma
        Sabit_Dur();
        break;
      case '2':  // İleri bir adım
        Ileri_Bir_Adim();
        break;
      case '3':  // Sürekli yürüme
        Yurume();
        break;
      case '4':  // Ters Yürüme
        Ters_Yurume();
        break;
      case '5':  // Dans Etme
        Dans_Et();
        break;
      case '6':  // Oluşturulan Melodi
        Melodi();
        break;
      case '7':  // Melodi çalma
        Otto.sing(S_confused);  // Yanlış komut sesi
        break;
    }
  }
}

// Sabit durma fonksiyonu
void Sabit_Dur() {
  Otto.init(PIN_LEFTLEG, PIN_RIGHTLEG, PIN_LEFTFOOT, PIN_RIGHTFOOT, true, PIN_BUZZER);
  Otto.home();  // Tüm motorları başlangıç pozisyonuna döndür
}

// 1 adım ileri atma fonksiyonu
void Ileri_Bir_Adim() {
  Otto.walk(1, 1000, 1);  // 1 adım, hız 1000ms, ileri (1)
  delay(1000);  // Hareket sonrası bekleme
}

// Mesafe ölçümü yapan fonksiyon
int MesafeOlc(int maxrange, int minrange) {
  long duration, distance;

  // Mesafe sensörü tetikleme işlemi
  digitalWrite(trig_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_Pin, LOW);

  // Ses dalgasının sensörden dönme süresi ölçülüyor
  duration = pulseIn(echo_Pin, HIGH);

  // Mesafe hesaplanıyor
  distance = duration / 58.2;
  delay(50);

  // Sensör algılayamadığında -1 döndürülüyor
  if (distance >= maxrange || distance <= minrange) {
    return -1; // Mesafe algılanamadı
  }

  return distance;
}

// Sürekli yürüme fonksiyonu
void Yurume() {
    int distance = MesafeOlc(50, 2); // Mesafe ölçüm fonksiyonundan değer al (2 cm ile 50 cm arasında)

    if (distance != -1 && distance <= 50) { 
        // Engel algılandı
        Otto.sing(S_OhOoh);           // Uyarı sesi çal
        Otto.home();                  // Otomatik duruş pozisyonu
        delay(1000);                  // 1 saniye bekle
    } else {
        // Sensör hiçbir nesne algılamazsa veya mesafe sınırını aşarsa
        Otto.walk(2, 700, 1);         // 2 adım ileri, hız 700 ms
        delay(500);  
        Otto.walk(2, 1300, -1);       // 2 adım geri, hız 1300 ms
        delay(500);
        Otto.home();                  // Duruş pozisyonuna geç
    }
}

void Ters_Yurume() {
    Otto.walk(2, 1000, -1); // 2 adım geri, hız 1000 ms
    delay(1000);
}

void Daire_Ciz() {
    for (int i = 0; i < 4; i++) { // 4 küçük daire dilimi
        Otto.turn(1, 1000, 1);   // Hafif sağa dön
        Otto.walk(1, 1000, 1);   // Bir adım ileri
    }
}

void Dans_Et() {
  Otto.sing(S_superHappy); 

  // 1. Sağ-sol salınım
  Otto.swing(2, 500, 40);
  delay(2000);  // Hareketten sonra 2 saniye bekle

  // 2. Yan adımlar (Moonwalk)
  Otto.moonwalker(2, 1000, 30, 1);
  delay(2000);  // Hareketten sonra 2 saniye bekle
  Otto.moonwalker(2, 1000, 30, -1);
  delay(2000);

  // 3. Parmak uçlarında sağ-sol sallanma
  Otto.tiptoeSwing(2, 1000, 30);
  delay(2000);

  // 4. Yan yürüme
  Otto.crusaito(2, 1000, 40, 1);
  delay(2000);
  Otto.crusaito(2, 1000, 40, -1);
  delay(2000);
  
  Otto.sing(S_fart1);  // Komik bir ses çalar

  // 5. Hızlı titreme
  Otto.jitter(10, 500, 40);
  delay(2000);

  // 6. Ayak titretme
  Otto.shakeLeg(2, 1000, 1);
  delay(2000);
  
  Otto.sing(S_fart1);
  // 7. Zıplama
  Otto.jump(2, 500);
  delay(2000);

}

void Melodi() {
  int melody[] = {
    NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_G4
  };

  int durations[] = {
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 2, 4, 4, 2
  };

  int size = sizeof(melody) / sizeof(int);

  for (int i = 0; i < 2; i++) {  // İki tur çal
    for (int note = 0; note < size; note++) {
      int duration = 500 / durations[note];  // 500 ms = 0.5 saniye
      tone(PIN_BUZZER, melody[note], duration);

      int pauseBetweenNotes = duration * 1.30;
      delay(pauseBetweenNotes);

      noTone(PIN_BUZZER);
    }
    delay(20);  // 25 m. saniye bekleyerek 4. nota sonrasında uzun bir beklemeyi sağla
  }
  delay(20);  // 25 m. saniye bekle ve tekrar başla  
}