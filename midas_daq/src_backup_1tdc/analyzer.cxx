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

#include "odb_trigger.h"
#include "ppac_conf.h"

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
// TRIGGER_SETTINGS trigger_settings;

/*-- Module declarations -------------------------------------------*/

extern ANA_MODULE ptdc_calib_module;

// extern ANA_MODULE adc_summing_module;
ANA_MODULE *trigger_module[] = {
  &ptdc_calib_module,
  NULL
};

/*-- Bank definitions ----------------------------------------------*/

// ASUM_BANK_STR(asum_bank_str);

BANK_LIST ana_trigger_bank_list[] = {
		/* online banks */
  {"F1PA", TID_WORD, N_ADC, NULL},

  {""},
};


/*-- Event request list --------------------------------------------*/

#define TRIGGER_EQ_NAME PPAC_EQ_NAME_F1

ANALYZE_REQUEST analyze_request[] = {
  {TRIGGER_EQ_NAME,               /* equipment name */
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
   char str[80];

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
