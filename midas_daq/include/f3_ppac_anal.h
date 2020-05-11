#pragma once

#define F3PPAC_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>

#include "detector_conf.h"



/*-- Module declaration --------------------------------------------*/
INT F3_PPAC_TDC(EVENT_HEADER *, void *);
INT F3_PPAC_TDC_INIT(void);
INT F3_PPAC_TDC_BOR(INT run_number);
INT F3_PPAC_TDC_EOR(INT run_number);

typedef struct {
   double gain;
} F3_PPAC_TDC_CAL;


#define F3_PPAC_TDC_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN          = DOUBLE : 0.025",\
"",\
NULL };


extern RUNINFO runinfo;

F3_PPAC_TDC_CAL f3_ppac_tdc;
F3_PPAC_TDC_STR(f3_ppac_tdc_str);


ANA_MODULE F3PPAC_TDC = {
	PPAC_EQ_NAME_F3,           /* module name           */
   "Stefan Ritt",               /* author                */
   F3_PPAC_TDC,                   /* event routine         */
   F3_PPAC_TDC_BOR,               /* BOR routine           */
   F3_PPAC_TDC_EOR,               /* EOR routine           */
   F3_PPAC_TDC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &f3_ppac_tdc,             /* parameter structure   */
   sizeof(f3_ppac_tdc),      /* structure size        */
   f3_ppac_tdc_str,   /* initial parameters    */
};

