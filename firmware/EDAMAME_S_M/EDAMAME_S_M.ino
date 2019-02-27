#include <Edamame_S.h>
#define FLIGHT_PIN	0
#define LED_MODE	16
#define LED_ON		17
#define RX_PIN		8
#define TX_PIN		9

#define LED_MODE_N HIGH
#define LED_MODE_G LOW

// OBC RX0 ---- USART1 TX1
// OBC TX0 ---- USART1 RX1

String log_path = "";
uint32_t cnt = 0;

void setup() {
  //LED
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);

  //FLIGHT PIN
  pinMode(FLIGHT_PIN, INPUT);

  //Mission
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(LED_ON, LOW); //OFF

  //IMU
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);

  //GPS
  Serial_GPS.begin(9600);
  delay(5000);
  set_gpsHiSpeed();
  delay(1000);
  set_gpsHiSpeed();

  Serial_GPS.setTimeout(500);

  //SD
  if (SD.begin(SD_CS)) {
    long file_num = 0;
    while (1) {
      log_path = "";
      log_path.concat(file_num);
      log_path.concat(".log");
      File logFile = SD.open(log_path, FILE_READ);
      if (logFile == false) {
        digitalWrite(LED0, HIGH);
        digitalWrite(LED1, HIGH);
        delay(500);
        break;
      }
      file_num++;
    }
  }
}

void loop() {

  String str = Serial_GPS.readStringUntil('\r');

  File logFile = SD.open(log_path, FILE_WRITE);
  if (logFile.print(str) >= 1) {
    digitalWrite(LED0, cnt % 2 ? HIGH : LOW);
  }
  logFile.close();


  digitalWrite(TX_PIN, digitalRead(FLIGHT_PIN));
  digitalWrite(LED1, digitalRead(RX_PIN));

  if (digitalRead(FLIGHT_PIN) == HIGH) {
    if (digitalRead(RX_PIN) == HIGH) {
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_MODE, LED_MODE_N);
    } else {
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_MODE, LED_MODE_G);
    }
  } else {
    digitalWrite(LED_ON, LOW);
    digitalWrite(LED_MODE, LED_MODE_N);
  }
  cnt++;
}
