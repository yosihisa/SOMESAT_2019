#include <Edamame_S.h>
#define FLIGHT_PIN	0
#define LED_MODE	16
#define LED_ON		17
#define RX_PIN		8
#define TX_PIN		9

// OBC RX0 ---- USART1 TX1
// OBC TX0 ---- USART1 RX1

void setup() {
	//LED
	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);

	//FLIGHT PIN
	pinMode(FLIGHT_PIN, INPUT);

	//Mission
	pinMode(16, OUTPUT);
	pinMode(17, OUTPUT);

	//IMU
	pinMode(RX_PIN, INPUT);
	pinMode(TX_PIN, OUTPUT);

}

void loop() {
	
	//Send state of FLIGHT_PIN
	digitalWrite(TX_PIN, digitalRead(FLIGHT_PIN));
	digitalWrite(LED0, digitalRead(FLIGHT_PIN));
	digitalWrite(LED1, digitalRead(RX_PIN));

	if (digitalRead(FLIGHT_PIN) == HIGH) {
		if (digitalRead(RX_PIN) == HIGH) {
			digitalWrite(LED_ON, HIGH);
			digitalWrite(LED_MODE, LOW);
		} else {
			digitalWrite(LED_ON, HIGH);
			digitalWrite(LED_MODE, HIGH);
		}
	} else {
		digitalWrite(LED_ON, LOW);
		digitalWrite(LED_MODE, LOW);
	}
}
