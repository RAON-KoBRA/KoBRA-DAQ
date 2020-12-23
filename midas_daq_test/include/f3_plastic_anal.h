#pragma once

#define F3PLASTIC_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>
#include "detector_conf.h"



/*-- Module declaration --------------------------------------------*/
INT F3PLASTIC_TDC(EVENT_HEADER *, void *);
INT F3PLASTIC_TDC_INIT(void);
INT F3PLASTIC_TDC_BOR(INT run_number);
INT F3PLASTIC_TDC_EOR(INT run_number);

typedef struct {
   double gain;
} F3_PL;

/*--Module initialization parameters----------------------------------------*/

#define F3_PL_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN          = DOUBLE : 0.025",\
"",\
NULL };


extern RUNINFO runinfo;

F3_PL f3_pl;
F3_PL_STR(f3_pl_str);

/*--Module definition--------------------------------------------*/
ANA_MODULE F3PLASTIC = {
	PLASTIC_EQ_NAME_F3,           /* module name           */
   "Stefan Ritt",               /* author                */
   F3PLASTIC_TDC,                   /* event routine         */
   F3PLASTIC_TDC_BOR,               /* BOR routine           */
   F3PLASTIC_TDC_EOR,               /* EOR routine           */
   F3PLASTIC_TDC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &f3_pl,             /* parameter structure   */
   sizeof(f3_pl),      /* structure size        */
   f3_pl_str,   /* initial parameters    */
};


