#pragma once

#define F1TDC_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>

#include "detector_conf.h"



/*-- Module declaration --------------------------------------------*/
INT F1_TDC(EVENT_HEADER *, void *);
INT F1_TDC_INIT(void);
INT F1_TDC_BOR(INT run_number);
INT F1_TDC_EOR(INT run_number);

typedef struct {
   double gain;
} F1_TDC_CALI_PAR;

/*--Module initialization parameters----------------------------------------*/

#define F1_TDC_CALI_PAR_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN          = DOUBLE : 0.025",\
"",\
NULL };


extern RUNINFO runinfo;

F1_TDC_CALI_PAR f1_tdc_cali_param;
F1_TDC_CALI_PAR_STR(f1_tdc_cali_param_str);

/*--Module definition--------------------------------------------*/
ANA_MODULE F1PPAC_TDC = {
	PPAC_EQ_NAME_F1,           /* module name           */
   "Stefan Ritt",               /* author                */
   F1_TDC,                   /* event routine         */
   F1_TDC_BOR,               /* BOR routine           */
   F1_TDC_EOR,               /* EOR routine           */
   F1_TDC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &f1_tdc_cali_param,             /* parameter structure   */
   sizeof(f1_tdc_cali_param),      /* structure size        */
   f1_tdc_cali_param_str,   /* initial parameters    */
};
