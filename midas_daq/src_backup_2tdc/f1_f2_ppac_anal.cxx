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

#include "ppac_conf.h"
#include "analyzer.h"
/* midas includes */

#include "f1_f2_ppac_anal.h"

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


INT F1_F2_TDC_INIT(void)
{
   char name[256];
   char title[256];

   /* book GTIG histos */
   int id;
   int i;

   /*for (i=0; i<N_TDC_f2_PPAC; i++)
   {
       //       printf("%0x\n",id);
       sprintf(name,  "F2PA-%03x", i);
       sprintf(title, "F2PA-%03x", i);
       hPTDCHists_raw[i] = h1_book<TH1D>(name, title, PTAC_N_BINS, PTAC_X_LOW, PTAC_X_HIGH);
    }*/
   hPXDIFFTDCHists_raw = h1_book<TH1D>("F1 X DIFF - F2 X DIFF", "F1 X DIFF - F2 X DIFF", 1000, -4000, 4000);
   hPYDIFFTDCHists_raw = h1_book<TH1D>("F1 Y DIFF - F2 Y DIFF", "F1 Y DIFF - F2 Y DIFF", 1000, -4000, 4000);
   hP2DTDCHists_raw = h2_book<TH2D>("F1 - F2 2D", "F1 - F2 2D plot", 1000, -4000, 4000, 1000, -4000, 4000);
   NUM_HIST = i;

   return SUCCESS;
}

/*-- BOR routine ---------------------------------------------------*/

INT F1_F2_TDC_BOR(INT run_number)
{
   return SUCCESS;
}

/*-- eor routine ---------------------------------------------------*/

INT F1_F2_TDC_EOR(INT run_number)
{
   return SUCCESS;
}

/*-- event routine -------------------------------------------------*/

INT F1_F2_TDC(EVENT_HEADER *pheader, void *pevent)
{

   INT i;
   DWORD *pdata2;
   int bk_size2;

   /* look for bank, return if not present */
   bk_size2 = bk_locate(pevent, BANK_NAME_F2FFAC, &pdata2);
   if (!bk_size2) {
      printf("no data\n");
      return 1;
   }

   int trig_num2;
   int32_t event_count2;
   int32_t channel2;
   int32_t tdc_value2[16];
   uint64_t global_time_stamp2;

   memset(tdc_value2, 0, sizeof(int32_t)*16);



//printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
   for(int i=0; i<1; i++){event_count2 = *pdata2++;}

   for (int i=0; i<bk_size2-3; )
   {
	   channel2=*pdata2++; i++;
	   tdc_value2[channel2]=*pdata2++; i++;
   }

   for(int i=0; i<2; )
   {
	   uint32_t time2=*pdata2++; i++;
	   uint32_t time1=*pdata2++; i++;
	   global_time_stamp2=time2|time1;
	 //  printf("\n\n%12f, %12f, %12f\n", time2*800e-9, time1*800e-9, global_time_stamp2*800e-9);
   }

#if 1
   printf("F1F2_F2PPAC ===============================%d\n", bk_size2);
   	 printf("event_count2: %d\n",event_count2);
   	 for(int i=0; i<N_TDC_f2_PPAC; i++){printf("channel2:%d, measure:%8f\n", i, tdc_value2[i]*0.025);}
   	 printf("global time: %12f\n", global_time_stamp2*800e-9);
#endif

     char name2[256];

   for(int i=0; i<N_TDC_f2_PPAC; i++)
    {
    	 sprintf(name2,  "F2PPAC-%03x", i);
    	 hPTDCHists_raw[i]->Fill(tdc_value2[i]*f12_tdc_cali_param.gain, 1);
    }
/*    if (tdc_value2[0]+tdc_value2[1]>1000) hPXDIFFTDCHists_raw->Fill(tdc_value2[0]-tdc_value2[1],1);
    if (tdc_value2[2]+tdc_value2[3]>1000) hPYDIFFTDCHists_raw->Fill(tdc_value2[2]-tdc_value2[3],1);
    if (tdc_value2[0]+tdc_value2[1]>1000 && tdc_value2[2]+tdc_value2[3]>1000)hP2DTDCHists_raw->Fill(tdc_value2[0]-tdc_value2[1], tdc_value2[2]-tdc_value2[3], 1);
*/


   DWORD *pdata1;
   int bk_size1;
   bk_size1 = bk_locate(pevent, BANK_NAME_F1FFAC, &pdata1);
   if (!bk_size1) {
      printf("no data\n");
      return 1;
   }

   int trig_num1;
   int32_t event_count1;
   int32_t channel1;
   int32_t tdc_value1[16];
   uint64_t global_time_stamp1;

   memset(tdc_value1, 0, sizeof(int32_t)*16);

   for(int i=0; i<1; i++){event_count1 = *pdata1++;}

   for (int i=0; i<bk_size1-3; )
   {
	   channel1=*pdata1++; i++;
	   tdc_value1[channel1]=*pdata1++; i++;
   }

   for(int i=0; i<2; )
   {
	   uint32_t time2=*pdata1++; i++;
	   uint32_t time1=*pdata1++; i++;
	   global_time_stamp1=time2|time1;
	 //  printf("\n\n%12f, %12f, %12f\n", time2*800e-9, time1*800e-9, global_time_stamp2*800e-9);
   }

#if 1
   printf("F1F2_F1PPAC ===============================%d\n", bk_size1);
   	 printf("event_count1: %d\n",event_count1);
   	 for(int i=0; i<N_TDC_f1_PPAC; i++){printf("channel1:%d, measure:%8f\n", i, tdc_value1[i]*0.025);}
   	 printf("global time: %12f\n", global_time_stamp1*800e-9);
#endif

     char name1[256];

   for(int i=0; i<N_TDC_f1_PPAC; i++)
    {
    	 sprintf(name1,  "F1PPAC-%03x", i);
    	 hPTDCHists_raw[i]->Fill(tdc_value1[i]*f12_tdc_cali_param.gain, 1);
    }
    if (tdc_value2[0]+tdc_value2[1]>1000 && tdc_value1[0]+tdc_value1[1]>1000) hPXDIFFTDCHists_raw->Fill(tdc_value2[0]-tdc_value2[1]-tdc_value1[0]+tdc_value1[1],1);
    if (tdc_value2[2]+tdc_value2[3]>1000 && tdc_value1[2]+tdc_value1[3]>1000) hPYDIFFTDCHists_raw->Fill(tdc_value2[2]-tdc_value2[3]-tdc_value1[2]+tdc_value1[3],1);
    if (tdc_value2[0]+tdc_value2[1]>1000 && tdc_value2[2]+tdc_value2[3]>1000 && tdc_value1[0]+tdc_value1[1]>1000 && tdc_value1[2]+tdc_value1[3]>1000) hP2DTDCHists_raw->Fill(tdc_value2[0]-tdc_value2[1]-tdc_value1[0]+tdc_value1[1], tdc_value2[2]-tdc_value2[3]-tdc_value1[2]+tdc_value1[3], 1);


   return SUCCESS;
}

