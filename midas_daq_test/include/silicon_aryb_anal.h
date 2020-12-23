#pragma once

#define SILICON_ARYB_ANAL_HH

#include "midas.h"
#include "rmidas.h"
#include <stdio.h>

#include "detector_conf.h"
/*-- Module declaration --------------------------------------------*/
INT sili_aryb_ADC(EVENT_HEADER *, void *);
INT sili_aryb_ADC_INIT(void);
INT sili_aryb_BOR(INT run_number);
INT sili_aryb_EOR(INT run_number);

typedef struct {
   float calib;
} SILI_ARYB_ADC_CALI_PAR;


#define SILI_ARYB_ADC_CALI_PAR_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN = FLOAT : 10",\
"",\
NULL };

#ifndef EXCL_ADC_ARYB_CALIBRATION

#define ADC_ARYB_CALIBRATION_PARAM_DEFINED

typedef struct {
   INT pedestal[32];
   float software_gain[32];
   double histo_threshold;
} ADC_ARYB_CALIBRATION_PARAM;

#define ADC_ARYB_CALIBRATION_PARAM_STR(_name) const char *_name[] = {\
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

SILI_ARYB_ADC_CALI_PAR si_aryb_adc;
SILI_ARYB_ADC_CALI_PAR_STR(si_aryb_str);

ADC_ARYB_CALIBRATION_PARAM adcarybcalib_param;

ADC_ARYB_CALIBRATION_PARAM_STR(adc_aryb_calibration_param_str);

ANA_MODULE ARYB_ADC = {
	"SILICON_ARYB_ANAL_HH",           /* module name           */
   "K Lee",               /* author                */
   sili_aryb_ADC,                   /* event routine         */
   sili_aryb_BOR,               /* BOR routine           */
   sili_aryb_EOR,               /* EOR routine           */
   sili_aryb_ADC_INIT,              /* init routine          */
   NULL,                        /* exit routine          */
   &adcarybcalib_param,             /* parameter structure   */
   sizeof(adcarybcalib_param),      /* structure size        */
   adc_aryb_calibration_param_str,   /* initial parameters    */
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

