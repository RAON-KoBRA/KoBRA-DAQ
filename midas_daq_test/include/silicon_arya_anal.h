#pragma once

#define SILICON_ARYA_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>

#include "detector_conf.h"
/*-- Module declaration --------------------------------------------*/
INT sili_arya_ADC(EVENT_HEADER *, void *);
INT sili_arya_ADC_INIT(void);
INT sili_arya_BOR(INT run_number);
INT sili_arya_EOR(INT run_number);

typedef struct {
   float calib;
} SILI_ARYA_ADC_CALI_PAR;


#define SILI_ARYA_ADC_CALI_PAR_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN = FLOAT : 10",\
"",\
NULL };

#ifndef EXCL_ADC_ARYA_CALIBRATION

#define ADC_ARYA_CALIBRATION_PARAM_DEFINED

typedef struct {
   INT pedestal[32];
   float software_gain[32];
   double histo_threshold;
} ADC_ARYA_CALIBRATION_PARAM;

#define ADC_ARYA_CALIBRATION_PARAM_STR(_name) const char *_name[] = {\
"[.]",\
"Pedestal = INT[32] :",\
"[0] 174",\
"[1] 194",\
"[2] 176",\
"[3] 182",\
"[4] 185",\
"[5] 215",\
"[6] 202",\
"[7] 202",\
"[8] 174",\
"[9] 194",\
"[10] 176",\
"[11] 182",\
"[12] 185",\
"[13] 215",\
"[14] 202",\
"[15] 202",\
"[16] 108",\
"[17] 121",\
"[18] 109",\
"[19] 107",\
"[20] 126",\
"[21] 135",\
"[22] 119",\
"[23] 127",\
"[24] 124",\
"[25] 122",\
"[26] 126",\
"[27] 124",\
"[28] 118",\
"[29] 164",\
"[30] 130",\
"[31] 117",\
"Software Gain = FLOAT[32] :",\
"[0] 1",\
"[1] 1",\
"[2] 1",\
"[3] 1",\
"[4] 1",\
"[5] 1",\
"[6] 1",\
"[7] 1",\
"[8] 1",\
"[9] 1",\
"[10] 1",\
"[11] 1",\
"[12] 1",\
"[13] 1",\
"[14] 1",\
"[15] 1",\
"[16] 1",\
"[17] 1",\
"[18] 1",\
"[19] 1",\
"[20] 1",\
"[21] 1",\
"[22] 1",\
"[23] 1",\
"[24] 1",\
"[25] 1",\
"[26] 1",\
"[27] 1",\
"[28] 1",\
"[29] 1",\
"[30] 1",\
"[31] 1",\
"Histo threshold = DOUBLE : 20",\
"",\
NULL }

#endif

extern RUNINFO runinfo;

SILI_ARYA_ADC_CALI_PAR si_arya_adc;
SILI_ARYA_ADC_CALI_PAR_STR(si_adc_arya_str);

ADC_ARYA_CALIBRATION_PARAM adcaryacalib_param;

ADC_ARYA_CALIBRATION_PARAM_STR(adc_arya_calibration_param_str);

ANA_MODULE ARYA_ADC = {
	"SILICON_ARYA_ANAL_HH",           /* module name           */
   "K Lee",               /* author                */
   sili_arya_ADC,                   /* event routine         */
   sili_arya_BOR,               /* BOR routine           */
   sili_arya_EOR,               /* EOR routine           */
   sili_arya_ADC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &adcaryacalib_param,             /* parameter structure   */
   sizeof(adcaryacalib_param),      /* structure size        */
   adc_arya_calibration_param_str,   /* initial parameters    */
};

//ANA_MODULE adc_calib_module = {
//   "ADC calibration",           /* module name           */
//   "Stefan Ritt",               /* author                */
//   adc_calib,                   /* event routine         */
//   adc_calib_bor,               /* BOR routine           */
//   adc_calib_eor,               /* EOR routine           */
//   adc_calib_init,              /* init routine          */
//   NULL,                        /* exit routine          */
//   &adccalib_param,             /* parameter structure   */
//   sizeof(adccalib_param),      /* structure size        */
//   adc_calibration_param_str,   /* initial parameters    */
//};

