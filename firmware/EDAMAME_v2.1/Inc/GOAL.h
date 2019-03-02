#ifndef __GOAL_H__
#define __GOAL_H__

#define LOOP_TIME 1000	//1���[�v�̎��� [ms]
#define DROP_TIME 20	//�������� [s]

//�S�[���̍��W���w��(dd.dddddd , ddd,dddddd)
//��H��Z��
#define GOAL_LATITUDE	 33890196
#define GOAL_LONGITUDE	130839266

//��q��
//#define GOAL_LATITUDE	 30.375900
//#define GOAL_LONGITUDE	130.960693

//�\��
//#define GOAL_LATITUDE	 33.8903550
//#define GOAL_LONGITUDE	130.8402200


//�S�[���Ƃ���G���A�̔��a
#define GOAL_ALEA	20
#define GOAL_ALEA2	GOAL_ALEA * GOAL_ALEA

//���C�Ίp(���΂����j
#define MAGNETIC_DECLINATION 7.71/360 * 3.1415926535


//�F���͈̔�
#define H_MAX_1 10
#define H_MIN_2 330	

#define H_MIN_1 0	//�Œ�
#define H_MAX_2 360 //�Œ�

//�ʓx�͈̔�
#define S_MIN 50
#define S_MAX 100

//���x�͈̔�
#define V_MIN 30
#define V_MAX 100

//�ʐ�
#define SQUARE 120 

//�S�[������(�ߋ��T��̏d�S�̈ړ�������2��̍��v�����̒l�ȉ���������S�[��)
#define GOAL 100


//��d���x���i�P��[V])
#define LOW_BATTERY 7.0

#endif
