#pragma once
#define PPAC_CONF_INCLUDE_H


#include "midas.h"



//set TDC module address
#define F1_PPAC_ADDR 0X00FF
#define F2_PPAC_ADDR 0X00EE

#define F3_SILICON_ADDR 0x000F

//set TDC module type
#define F1_PPAC_type v1290n
#define F2_PPAC_type v1290a

#define F3_SILICON_type v785a

//set detector name
#define PPAC_EQ_NAME_F1 "F1PPAC"
#define PPAC_EQ_NAME_F2 "F2PPAC"
#define PPAC_EQ_NAME_F3 "F3PPAC"

#define	SILICON_EQ_NAME_F3 "F3SILICON"

//set detector bank name
#define BANK_NAME_F1FFAC "F1PA"
#define BANK_NAME_F2FFAC "F2PA"
#define BANK_NAME_F3FFAC "F3PA"

#define BANK_NAME_F3SILICON "F3SI"

