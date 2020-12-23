#pragma once

#define F2TDC_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>

#include "detector_conf.h"



/*-- Module declaration --------------------------------------------*/
INT F2_TDC(EVENT_HEADER *, void *);
INT F2_TDC_INIT(void);
INT F2_TDC_BOR(INT run_number);
INT F2_TDC_EOR(INT run_number);

typedef struct {
   double gain;
} F2_TDC_CALI_PAR;


#define F2_TDC_CALI_PAR_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN          = DOUBLE : 0.025",\
"",\
NULL };


extern RUNINFO runinfo;

F2_TDC_CALI_PAR f2_tdc_cali_param;
F2_TDC_CALI_PAR_STR(f2_tdc_cali_param_str);


ANA_MODULE F2PPAC_TDC = {
	PPAC_EQ_NAME_F2,           /* module name           */
   "Stefan Ritt",               /* author                */
   F2_TDC,                   /* event routine         */
   F2_TDC_BOR,               /* BOR routine           */
   F2_TDC_EOR,               /* EOR routine           */
   F2_TDC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &f2_tdc_cali_param,             /* parameter structure   */
   sizeof(f2_tdc_cali_param),      /* structure size        */
   f2_tdc_cali_param_str,   /* initial parameters    */
};


