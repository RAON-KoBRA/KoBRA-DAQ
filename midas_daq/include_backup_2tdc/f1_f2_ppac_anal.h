#pragma once

#define F2TDC_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>



/*-- Module declaration --------------------------------------------*/
INT F1_F2_TDC(EVENT_HEADER *, void *);
INT F1_F2_TDC_INIT(void);
INT F1_F2_TDC_BOR(INT run_number);
INT F1_F2_TDC_EOR(INT run_number);

typedef struct {
   double gain;
} F12_TDC_CALI_PAR;
/*
typedef struct {
   double gain1;
} F1_TDC_CALI_PAR;
*/
#define F12_TDC_CALI_PAR_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN          = DOUBLE : 0.025",\
"",\
NULL };
/*
#define F1_TDC_CALI_PAR_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN1          = DOUBLE : 0.025",\
"",\
NULL };
*/
extern RUNINFO runinfo;

F12_TDC_CALI_PAR f12_tdc_cali_param;
F12_TDC_CALI_PAR_STR(f12_tdc_cali_param_str);
/*
F1_TDC_CALI_PAR f1_tdc_cali_param;
F1_TDC_CALI_PAR_STR(f1_tdc_cali_param_str);
*/
ANA_MODULE F1F2PPAC_TDC = {
   "F1F2PPAC_TDC",           /* module name           */
   "Stefan Ritt",               /* author                */
   F1_F2_TDC,                   /* event routine         */
   F1_F2_TDC_BOR,               /* BOR routine           */
   F1_F2_TDC_EOR,               /* EOR routine           */
   F1_F2_TDC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &f12_tdc_cali_param,             /* parameter structure   */
   sizeof(f12_tdc_cali_param),      /* structure size        */
   f12_tdc_cali_param_str,   /* initial parameters    */
};

