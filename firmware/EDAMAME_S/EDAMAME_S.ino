#include <Edamame_S.h>
#include <Edamame_S_EX.h>

#define LOOP_TIME 200
struct control_data cansat;

unsigned long cnt = 1;

void setup() {
	
	Wire.begin();
	set_pinMode();

	Serial_PC.begin(115200);
	Serial_GPS.begin(9600);
	Serial_PC.print("Hello Calibration test\n");

	ina226_init();
	LSM303_init_m();
	set_gpsHiSpeed();

	cansat.mode = 0;

}

void loop() {
	
	unsigned long t = millis();

	update(&cansat);

	switch (cansat.mode) {
	case 0: //待機
		break;

	case 1: //落下・分離
		break;

	case 2: //キャリブレーション
		calibration(&cansat, 20, cnt);
		if (cansat.mode != 0)cnt = 0;
		break;

	case 3://誘導
		break;

	case 4://ゴール

	default:
		cansat.motor_L = 0;
		cansat.motor_R = 0;
		cansat.nichrome = 0;
		break;
	}
	
	apply(&cansat);

	print_data(&cansat);
	//write_data(&cansat);
	digitalWrite(LED0, cnt % 2);
	cansat.log_num++;
	cnt++;
	while (millis() - t < LOOP_TIME);
}

void update(control_data *data) {
	LSM303_update_m(&data->LSM303_data);
	data->voltage = ina226_voltage();
	data->current = ina226_current();
	get_gps(&data->gps_data, 500);
	
	//calc_gps
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
		data->mode = 1;
	}
	
	return;
}