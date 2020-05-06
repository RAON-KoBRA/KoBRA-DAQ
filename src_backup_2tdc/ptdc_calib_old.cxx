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

#include <midas.h>
/* midas includes */


#include "ppac_conf.h"
#include "ptdc_calib.h"
#include "odb_trigger.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TDirectory.h>



/* root includes */


/*-- Parameters ----------------------------------------------------*/


/*-- module-local variables ----------------------------------------*/


/*-- init routine --------------------------------------------------*/

const int PTAC_N_BINS = 1000;
const int PTAC_X_LOW =     10;
const int PTAC_X_HIGH = 1000;

static int NUM_HIST = 0;

static TH1D *hPTDCHists_raw[16];
static TH1D *hPXDIFFTDCHists_raw;
static TH1D *hPYDIFFTDCHists_raw;
static TH2D *hP2DTDCHists_raw;

INT ptdc_calib_init(void)
{
   char name[256];
   char title[256];

   /* book GTIG histos */
   int id;
   int i;

   for (i=0; i<16; i++)
   {
       //       printf("%0x\n",id);
       sprintf(name,  "F1PA-%03x", i);
       sprintf(title, "F1PA-%03x", i);
       hPTDCHists_raw[i] = h1_book<TH1D>(name, title, PTAC_N_BINS, PTAC_X_LOW, PTAC_X_HIGH);
    }
   hPXDIFFTDCHists_raw = h1_book<TH1D>("X DIFF", "X DIFF", 1000, -4000, 4000);
   hPYDIFFTDCHists_raw = h1_book<TH1D>("Y DIFF", "Y DIFF", 1000, -4000, 4000);
   hP2DTDCHists_raw = h2_book<TH2D>("2D", "2D plot", 1000, -4000, 4000, 1000, -4000, 4000);
   NUM_HIST = i;

   return SUCCESS;
}

/*-- BOR routine ---------------------------------------------------*/

INT ptdc_calib_bor(INT run_number)
{
   return SUCCESS;
}

/*-- eor routine ---------------------------------------------------*/

INT ptdc_calib_eor(INT run_number)
{
   return SUCCESS;
}

/*-- event routine -------------------------------------------------*/

INT ptdc_calib(EVENT_HEADER *pheader, void *pevent)
{
   INT i;
   DWORD *pdata;
   int bk_size;

   /* look for GTIG bank, return if not present */
   bk_size = bk_locate(pevent, "F1PA", &pdata);
   if (!bk_size) {
      printf("no data\n");
      return 1;
   }

   int trig_num;
   int32_t dummy;
   int32_t event_count, geo, trailer_wcount;
   int32_t channel;
   int32_t tdc_value[16];

   memset(tdc_value, 0, sizeof(int32_t)*16);



//printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
   for ( int i=0; i<2; i++) {dummy = *pdata++;}
   for (int i=0; i<bk_size-3; )
  {
	   channel=*pdata++; i++;
	   tdc_value[channel]=*pdata++; i++;
  }

 //  for(int i=0; i<16; i++)
 //  {
//	   printf("channel:%d, measure:%d\n", i, tdc_value[i]);
//   }

     char name[256];

   for(int i=0; i<16; i++)
    {
    	 sprintf(name,  "F1PA-%03x", i);
    	 hPTDCHists_raw[i]->Fill(tdc_value[i]*ptdccalib_param.gain, 1);
    }
    if (tdc_value[0]+tdc_value[1]>1000) hPXDIFFTDCHists_raw->Fill(tdc_value[0]-tdc_value[1],1);
    if (tdc_value[2]+tdc_value[3]>1000) hPYDIFFTDCHists_raw->Fill(tdc_value[2]-tdc_value[3],1);
    if (tdc_value[0]+tdc_value[1]>1000 && tdc_value[2]+tdc_value[3]>1000)hP2DTDCHists_raw->Fill(tdc_value[0]-tdc_value[1], tdc_value[2]-tdc_value[3], 1);

   return SUCCESS;
}
