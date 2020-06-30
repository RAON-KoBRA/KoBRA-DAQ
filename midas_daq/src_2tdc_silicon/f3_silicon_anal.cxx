/********************************************************************\

  Name:         tdccalib.c
  Created by:   Stefan Ritt

  Contents:     Example analyzer module for TDC calibration. Looks
                for TDC0 bank, subtracts pedestals and applies gain
                calibration. The resulting values are appended to
                the event as an CGTIG bank ("calibrated TDC"). The
                pedestal values and software gains are stored in
                tdccalib_param structure which was defined in the ODB
                and transferred to experim.h.

  $Id:$

\********************************************************************/

/*-- Include files -------------------------------------------------*/

/* standard includes */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <fstream>
#include <iostream>

#include <midas.h>

#include "detector_conf.h"
#include "analyzer.h"
/* midas includes */

#include "f3_silicon_anal.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TDirectory.h>



/* root includes */


/*-- Parameters ----------------------------------------------------*/


/*-- module-local variables ----------------------------------------*/


/*-- init routine --------------------------------------------------*/

int first_Call_chk;
using namespace std;

vector<double>slope_arr;
vector<double>intercept_arr;


static TH1D *hADCCHists_raw[N_ADC_f3_silicon];

INT F3_sili_ADC_INIT(void)
{
   char name[256];
   char title[256];
   char dump[256];
   char path_name[256];

   double slope, intercept;

   for (int i=0; i<N_ADC_f3_silicon; i++)
   {
       //       printf("%0x\n",id);d
       sprintf(name,  "F3SILICON-%03d", i);
       sprintf(title, "F3SILICON-%03d", i);
       hADCCHists_raw[i] = h1_book<TH1D>(name, title, F3SILICON_N_BINS, F3SILICON_X_LOW, F3SILICON_X_HIGH);
    }

   first_Call_chk=1;

   return SUCCESS;
}

/*-- BOR routine ---------------------------------------------------*/

INT F3_sili_BOR(INT run_number)
{
   return SUCCESS;
}

/*-- eor routine ---------------------------------------------------*/

INT F3_sili_EOR(INT run_number)
{
   return SUCCESS;
}

/*-- event routine -------------------------------------------------*/

INT F3_sili_ADC(EVENT_HEADER *pheader, void *pevent)
{

   DWORD *pdata;
   int bk_size;

   /* look for bank, return if not present */
   bk_size = bk_locate(pevent, BANK_NAME_F3SILICON, &pdata);
   if (!bk_size) {
      printf("no data\n");
      return 1;
   }

   int32_t event_count;
   int32_t channel;
   int32_t ADC_value[N_ADC_f3_silicon];
   uint64_t global_time_stamp;



   memset(ADC_value, 0, sizeof(int32_t)*N_ADC_f3_silicon);

//   printf("======================%d\n", bk_size);

   if(first_Call_chk)
   {
	   for(int i=0; i<bk_size; i++){*pdata++;}
	   first_Call_chk=0;
   }

   if(! first_Call_chk)
   {
	   for(int i=0; i<bk_size-1; )
	   {
		   channel=*pdata++; i++;
		   printf("%d\n", channel);
		   ADC_value[channel]=*pdata++; i++;
		   printf("%d\n", ADC_value[channel]);
	   }

	   event_count=*pdata++;
//	   printf("=======================%d", event_count);

	   char name[256];

	    for(int i=0; i<N_ADC_f3_silicon; i++)
	     {
	     	 sprintf(name,  "F3SILICON-%03d", i);
	     	 hADCCHists_raw[i]->Fill(ADC_value[i],1);
	     }
   }

   return SUCCESS;

}

