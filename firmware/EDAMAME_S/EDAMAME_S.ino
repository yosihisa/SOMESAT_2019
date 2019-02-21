#include <Edamame_S.h>
#include <Edamame_S_EX.h>

#define LOOP_TIME 500	//1ループの時間 [ms]
#define DROP_TIME 20	//落下時間 [s]

//ゴール
#define GOAL_LATITUDE    33890292
#define GOAL_LONGITUDE  130840088

//ゴールとするエリアの半径
#define GOAL_ALEA 20
#define GOAL_ALEA2  GOAL_ALEA * GOAL_ALEA

struct control_data cansat;

unsigned long cnt = 1;

void setup() {
	
	Wire.begin();
	set_pinMode();

	Serial_PC.begin(57600);
	Serial_GPS.begin(9600);
	Serial_PC.print("EDAMAME_S HW:v1.20 SW:v0.10\n");

	ina226_init();
	LSM303_init_m();
	set_gpsHiSpeed();

	if ( SD.begin(SD_CS) ) {
		long file_num = 0;
		while (1) {
			cansat.log_path = "";
			cansat.log_path.concat(file_num);
			cansat.log_path.concat(".csv");
			File logFile = SD.open(cansat.log_path, FILE_READ);
			if (logFile == false) {
				digitalWrite(LED1, HIGH);
				break;
			}
			file_num++;
		}
	} else {
		Serial_PC.println("SD initialization failed!");
	}

	cansat.mode = 0;

}

void loop() {
	
	unsigned long t = millis();

	update(&cansat);

	switch (cansat.mode) {

	case 0: //待機

		if (cansat.flightPin == HIGH) {
			cansat.mode++;
			cnt = 0;
		}
		break;

	case 1: //落下・分離

		if (cnt >= DROP_TIME * (1000 / LOOP_TIME)) {

			cansat.nichrome = 255;

			if (cnt > (DROP_TIME + 1)*(1000 / LOOP_TIME)) {
				cansat.nichrome = 0;
				cansat.mode++;
				cnt = 0;
			}
		}
		break;

	case 2: //キャリブレーション
		calibration(&cansat, 20, cnt);
		if (cansat.mode != 2) {
			cnt = 0;
		}
		break;

	case 3://誘導
		if (cansat.gps_data.mode >= 1) {
			if (cansat.arg < -0.3) {
				cansat.motor_L = 30;
				cansat.motor_R = 255;
			}
			if (-0.3 < cansat.arg && cansat.arg < +0.3) {
				cansat.motor_L = 255;
				cansat.motor_R = 255;
			}
			if (+0.3 < cansat.arg) {
				cansat.motor_L = 255;
				cansat.motor_R = 30;
			}
		} else {
			cansat.motor_L = 50;
			cansat.motor_R = 100;
		}
		if (cansat.gps_data.dist < GOAL_ALEA2) {
			cansat.motor_L = 0;
			cansat.motor_R = 0;
			cansat.mode++;
			cnt = 0;
		}
		break;

	case 4://ゴール
		digitalWrite(LED1, (cnt/2) % 2);
		break;

	default:
		cansat.motor_L = 0;
		cansat.motor_R = 0;
		cansat.nichrome = 0;
		break;
	}
	
	apply(&cansat);

	print_data(&cansat);
	write_data(&cansat);
	digitalWrite(LED0, cnt % 2);
	cansat.log_num++;
	cnt++;
	while (millis() - t < LOOP_TIME);
}

void update(control_data *data) {
	const float pi =  3.1415926535;
	data->flightPin = digitalRead(FLIGHT_PIN);
	LSM303_update_m(&data->LSM303_data);
	data->voltage = ina226_voltage();
	data->current = ina226_current();
	get_gps(&data->gps_data, 500);
	calc_gps(&data->gps_data, GOAL_LATITUDE, GOAL_LONGITUDE);
	data->arg = data->gps_data.arg - data->LSM303_data.arg;

	while (data->arg > 1.0 * pi) data->arg -= 2 * pi;
	while (data->arg < -1.0 * pi) data->arg += 2 * pi;
}

void apply(control_data *data) {
	motor(data->motor_L, data->motor_R);
	nichrome(data->nichrome);
}

void calibration(control_data *data, const int n , unsigned long cnt) {
	
	static long x_max, y_max;
	static long x_min, y_min;

	data->motor_L = 70;
	data->motor_R = 255;

	if (cnt == 1) {
		x_max = data->LSM303_data.x;
		x_min = data->LSM303_data.x;
		y_max = data->LSM303_data.y;
		y_min = data->LSM303_data.y;
	}

	x_max = data->LSM303_data.x > x_max ? data->LSM303_data.x : x_max;
	x_min = data->LSM303_data.x < x_min ? data->LSM303_data.x : x_min;
	y_max = data->LSM303_data.y > y_max ? data->LSM303_data.y : y_max;
	y_min = data->LSM303_data.y < y_min ? data->LSM303_data.y : y_min;


	if (cnt >= n * (1000 / LOOP_TIME)) {
		data->LSM303_data.x_offset = (x_max + x_min) / 2;
		data->LSM303_data.y_offset = (y_max + y_min) / 2;
		data->motor_L = 0;
		data->motor_R = 0;
		data->mode ++;
	}
	
	return;
}
