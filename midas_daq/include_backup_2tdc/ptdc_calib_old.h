#pragma once

#define PTDC_CALIB_INCLUDE_H

#include "midas.h"
#include "rmidas.h"
#include "ppac_conf.h"
#include <stdio.h>



/*-- Module declaration --------------------------------------------*/
INT ptdc_calib(EVENT_HEADER *, void *);
INT ptdc_calib_init(void);
INT ptdc_calib_bor(INT run_number);
INT ptdc_calib_eor(INT run_number);

typedef struct {
   double gain;
} PTDC_CALIBRATION_PARAM;


#define PTDC_CALIBRATION_PARAM_STR(_name) const char *_name[] = {\
"[.]",\
"GAIN          = DOUBLE : 0.025",\
"",\
NULL };


extern RUNINFO runinfo;


PTDC_CALIBRATION_PARAM ptdccalib_param;
PTDC_CALIBRATION_PARAM_STR(ptdc_calibration_param_str);

ANA_MODULE ptdc_calib_module = {
   "F1PPAC_TDC",           /* module name           */
   "Stefan Ritt",               /* author                */
   ptdc_calib,                   /* event routine         */
   ptdc_calib_bor,               /* BOR routine           */
   ptdc_calib_eor,               /* EOR routine           */
   ptdc_calib_init,              /* init routine          */
   NULL,                        /* exit routine          */
   &ptdccalib_param,             /* parameter structure   */
   sizeof(ptdccalib_param),      /* structure size        */
   ptdc_calibration_param_str,   /* initial parameters    */
};

