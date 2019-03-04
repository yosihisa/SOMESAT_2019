#ifndef __GOAL_H__
#define __GOAL_H__

#define LOOP_TIME 1000	//1ループの時間 [ms]
#define DROP_TIME 20	//落下時間 [s]

//ゴールの座標を指定(dd.dddddd , ddd,dddddd)
//九工大校庭
#define GOAL_LATITUDE	 33890196
#define GOAL_LONGITUDE	130839266

//種子島
//#define GOAL_LATITUDE	 30.375900
//#define GOAL_LONGITUDE	130.960693

//能代
//#define GOAL_LATITUDE	 33.8903550
//#define GOAL_LONGITUDE	130.8402200


//ゴールとするエリアの半径
#define GOAL_ALEA	20
#define GOAL_ALEA2	GOAL_ALEA * GOAL_ALEA

//磁気偏角(西偏が正）
#define MAGNETIC_DECLINATION 7.71/360 * 3.1415926535


//色相の範囲
#define H_MAX_1 10
#define H_MIN_2 330	

#define H_MIN_1 0	//固定
#define H_MAX_2 360 //固定

//彩度の範囲
#define S_MIN 50
#define S_MAX 100

//明度の範囲
#define V_MIN 30
#define V_MAX 100

//面積
#define SQUARE 120 

//ゴール判定(過去５回の重心の移動距離の2乗の合計がこの値以下だったらゴール)
#define GOAL 100


//低電圧警告（単位[V])
#define LOW_BATTERY 7.0

#endif
