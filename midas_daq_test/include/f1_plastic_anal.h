#pragma once

#define F1PLASTIC_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>
#include "detector_conf.h"



/*-- Module declaration --------------------------------------------*/
INT F1PLASTIC_TDC(EVENT_HEADER *, void *);
INT F1PLASTIC_TDC_INIT(void);
INT F1PLASTIC_TDC_BOR(INT run_number);
INT F1PLASTIC_TDC_EOR(INT run_number);

typedef struct {
   double gain;
} F1_PL;

/*--Module initialization parameters----------------------------------------*/

#define F1_PL_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN          = DOUBLE : 0.025",\
"",\
NULL };


extern RUNINFO runinfo;

F1_PL f1_pl;
F1_PL_STR(f1_pl_str);

/*--Module definition--------------------------------------------*/
ANA_MODULE F1PLASTIC = {
	PLASTIC_EQ_NAME_F1,           /* module name           */
   "Stefan Ritt",               /* author                */
   F1PLASTIC_TDC,                   /* event routine         */
   F1PLASTIC_TDC_BOR,               /* BOR routine           */
   F1PLASTIC_TDC_EOR,               /* EOR routine           */
   F1PLASTIC_TDC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &f1_pl,             /* parameter structure   */
   sizeof(f1_pl),      /* structure size        */
   f1_pl_str,   /* initial parameters    */
};


