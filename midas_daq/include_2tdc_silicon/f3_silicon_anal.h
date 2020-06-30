#pragma once

#define F3SILICON_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>


/*-- Module declaration --------------------------------------------*/
INT F3_sili_ADC(EVENT_HEADER *, void *);
INT F3_sili_ADC_INIT(void);
INT F3_sili_BOR(INT run_number);
INT F3_sili_EOR(INT run_number);

typedef struct {
   float calib;
} F3_SILI_ADC_CALI_PAR;


#define F3_SILI_ADC_CALI_PAR_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN = FLOAT : 10",\
"",\
NULL };


extern RUNINFO runinfo;

F3_SILI_ADC_CALI_PAR f3_si_adc;
F3_SILI_ADC_CALI_PAR_STR(f3_si_adc_str);


ANA_MODULE F3SILI_ADC = {
   "F3SILICON_ADC",           /* module name           */
   "Stefan Ritt",               /* author                */
   F3_sili_ADC,                   /* event routine         */
   F3_sili_BOR,               /* BOR routine           */
   F3_sili_EOR,               /* EOR routine           */
   F3_sili_ADC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &f3_si_adc,             /* parameter structure   */
   sizeof(f3_si_adc),      /* structure size        */
   f3_si_adc_str,   /* initial parameters    */
};


