/********************************************************************\

  Name:         analyzer.c
  Created by:   Stefan Ritt

  Contents:     System part of Analyzer code for sample experiment

  $Id$

\********************************************************************/

/* standard includes */
#include <stdio.h>
#include <string.h>
#include <time.h>

/* midas includes */
#include "midas.h"
#include "analyzer.h"
#include "detector_conf.h"

/* cernlib includes */
#ifdef OS_WINNT
#define VISUAL_CPLUSPLUS
#endif
#ifdef __linux__
#define f2cFortran
#endif


//#include <cfortran.h>
//#include <hbook.h>

PAWC_DEFINE(1000000);

/* The analyzer name (client name) as seen by other MIDAS clients   */
const char *analyzer_name = "Analyzer";

/* analyzer_loop is called with this interval in ms (0 to disable)  */
INT analyzer_loop_period = 0;

/* default ODB size */
INT odb_size = DEFAULT_ODB_SIZE;

/* ODB structures */
RUNINFO runinfo;

/*-- Module declarations -------------------------------------------*/

#ifdef USE_PPAC_F1
extern ANA_MODULE F1PPAC_TDC;
#endif

#ifdef USE_PLASTIC_F1
extern ANA_MODULE F1PLASTIC;
#endif

#ifdef USE_PPAC_F2
extern ANA_MODULE F2PPAC_TDC;
#endif

#ifdef USE_SILICON_F3
extern ANA_MODULE F3SILI_ADC;
#endif

#ifdef USE_PLASTIC_F3
extern ANA_MODULE F3PLASTIC;
#endif

#ifdef USE_PPAC_F3
extern ANA_MODULE F3PPAC_TDC;
#endif

extern ANA_MODULE scaler_accum_module;
extern ANA_MODULE adc_calib_module;
extern ANA_MODULE adc_summing_module;

ANA_MODULE *trigger_module[] = {
#ifdef USE_PPAC_F1
	&F1PPAC_TDC,
#endif

#ifdef USE_PLASTIC_F1
	&F1PLASTIC,
#endif

#ifdef USE_PPAC_F2
	&F2PPAC_TDC,
#endif

#ifdef USE_SILICON_F3
	&F3SILI_ADC,
#endif

#ifdef USE_PLASTIC_F3
	&F3PLASTIC,
#endif

#ifdef USE_PPAC_F3
	&F3PPAC_TDC,
#endif
  NULL
};

/*-- Bank definitions ----------------------------------------------*/

// ASUM_BANK_STR(asum_bank_str);

BANK_LIST ana_trigger_bank_list[] = {
		/* online banks */
#ifdef USE_PPAC_F1
  {BANK_NAME_F1PPAC, TID_WORD, N_TDC_f1_PPAC, NULL},
#endif

#ifdef USE_PLASTIC_F1
  {BANK_NAME_F1PLASTIC, TID_WORD, N_TDC_f1_PLASTIC, NULL},
#endif

#ifdef USE_PPAC_F2
  {BANK_NAME_F2PPAC, TID_WORD, N_TDC_f2_PPAC, NULL},
#endif

#ifdef USE_SILICON_F3
  {BANK_NAME_F3SILICON, TID_WORD, N_ADC_f3_silicon, NULL},
  {BANK_NAME_F3SILICON_CAL, TID_WORD, N_ADC_f3_silicon, NULL},
#endif

#ifdef USE_PLASTIC_F3
  {BANK_NAME_F3PLASTIC, TID_WORD, N_TDC_f3_plastic, NULL},
#endif

#ifdef USE_PPAC_F3
  {BANK_NAME_F3PPAC, TID_WORD, N_TDC_f3_ppac, NULL},
#endif

  {""},

};


/*-- Event request list --------------------------------------------*/

ANALYZE_REQUEST analyze_request[] = {
  {"Trigger1 (Beamline)",               /* equipment name */
   {1,                         /* event ID */
    TRIGGER_ALL,               /* trigger mask */
    GET_NONBLOCKING,           /* get events without blocking producer */
    "SYSTEM",                  /* event buffer */
    TRUE,                      /* enabled */
    "", "",}
   ,
   NULL,                       /* analyzer routine */
   trigger_module,             /* module list */
   ana_trigger_bank_list,      /* bank list */
   1000,                       /* RWNT buffer size */
   TRUE,                       /* Use tests for this event */
  }
  ,

   {""}
   ,
};

/*-- Analyzer Init -------------------------------------------------*/

INT analyzer_init()
{
   HNDLE hDB, hKey;

   RUNINFO_STR(runinfo_str);


   /* open ODB structures */
   cm_get_experiment_database(&hDB, NULL);

   db_create_record(hDB, 0, "/Runinfo", strcomb((const char **)runinfo_str));
   db_find_key(hDB, 0, "/Runinfo", &hKey);
   if (db_open_record(hDB, hKey, &runinfo, sizeof(runinfo), MODE_READ, NULL, NULL) !=
       DB_SUCCESS) {
      cm_msg(MERROR, "analyzer_init", "Cannot open \"/Runinfo\" tree in ODB");
      return 0;
   }


   return SUCCESS;
}

/*-- Analyzer Exit -------------------------------------------------*/

INT analyzer_exit()
{
   return CM_SUCCESS;
}

/*-- Begin of Run --------------------------------------------------*/

INT ana_begin_of_run(INT run_number, char *error)
{
   return CM_SUCCESS;
}

/*-- End of Run ----------------------------------------------------*/

INT ana_end_of_run(INT run_number, char *error)
{

   return CM_SUCCESS;
}

/*-- Pause Run -----------------------------------------------------*/

INT ana_pause_run(INT run_number, char *error)
{
   return CM_SUCCESS;
}

/*-- Resume Run ----------------------------------------------------*/

INT ana_resume_run(INT run_number, char *error)
{
   return CM_SUCCESS;
}

/*-- Analyzer Loop -------------------------------------------------*/

INT analyzer_loop()
{
   return CM_SUCCESS;
}

/*------------------------------------------------------------------*/
