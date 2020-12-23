#pragma once
#define PPAC_CONF_INCLUDE_H


#include "midas.h"


//========================define module address======================================//
//set F1 module address
#define F1_PPAC_ADDR 0X0009 //0X00EE
#define F1_PLASTIC_ADDR 0X0002

//set F2 module address
#define F2_PPAC_ADDR 0X000B

//set F3 module address
#define F3_PPAC_ADDR  0x00EE// 0x0009
#define F3_PLASTIC_ADDR 0x0005
#define F3_SILICON_ADDR 0x000E //0x000E
//#define SCALER_ADDR 0x3210

//silicon arry
#define SILICON_ARYA_ADDR 0x000A //0x000A
#define SILICON_ARYB_ADDR 0x000C //0x000B


//set VME controller address
#define Controller_ADDR 0x0000 //PROG_3 switch setting ON SW9,SW8,x,x, PROG 3 OFF x, x, SW9, SW8

//========================define module type======================================//
//set F1 module type
#define F1_PPAC_type v1290a   //32 channel TDC module
#define F1_PLASTIC_type v1290n   //16 channel TDC module

//set F2 module type
#define F2_PPAC_type v1290n   //16 channel TDC module

//set F3 module type
#define F3_PPAC_type v1290a   //32 channel TDC mdoule
#define F3_PLASTIC_type v1290n   //16 channel TDC mdoule
#define F3_SILICON_type v785n   //32 channel ADC module, v785, 16ch v785n

//silicon ary
#define SILICON_arya_type v785   //32 channel ADC module, v785, 16ch v785n
#define SILICON_aryb_type v785   //32 channel ADC module, v785, 16ch v785n

//========================define module name======================================//
//set f1 detector name
#define PPAC_EQ_NAME_F1 "F1PPAC"
#define PLASTIC_EQ_NAME_F1 "F1PLASTIC"

//set f2 detector name
#define PPAC_EQ_NAME_F2 "F2PPAC"

//set f3 detector name
#define PPAC_EQ_NAME_F3 "F3PPAC"
#define	PLASTIC_EQ_NAME_F3 "F3PLASTIC"
#define	SILICON_EQ_NAME_F3 "F3SILICON"

//========================define bank name======================================//
//set f1 detector bank name
#define BANK_NAME_F1PPAC "F1PA"
#define BANK_NAME_F1PLASTIC "F1PL"

//set f2 detector bank name
#define BANK_NAME_F2PPAC "F2PA"

//set f3 detector bank name
#define BANK_NAME_F3PPAC "F3PA"
#define BANK_NAME_F3PLASTIC "F3PL"
#define BANK_NAME_F3SILICON "F3SI"

//set siliconary bank name
#define BANK_NAME_U1SILLICON "U1SI"
#define BANK_NAME_U2SILLICON "U2SI"

//set calibration bank name
//#define BANK_NAME_F3PPAC "F3PA"
//#define BANK_NAME_F3PLASTIC "F3PL"
#define BANK_NAME_F3SILICON_CAL "F3SICAL"


//###############
#define BANK_NAME_SCALER "F2495"
//#define BANK_NAME_REBUNCHER "RBC"
