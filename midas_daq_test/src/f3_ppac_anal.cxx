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

#include "detector_conf.h"
#include "analyzer.h"
/* midas includes */

#include "f3_ppac_anal.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TDirectory.h>



/* root includes */


/*-- Parameters ----------------------------------------------------*/


/*-- module-local variables ----------------------------------------*/


/*-- init routine --------------------------------------------------*/

static TH1D *hPTDCHists_raw[N_TDC_f3_ppac];
static TH1D *hPXDIFFTDCHists_raw;
static TH1D *hPYDIFFTDCHists_raw;
static TH2D *hP2DTDCHists_raw;

static TH1D *hPXDIFFTDCHists_cor;

int first_Call_chks_arya;

INT F3_PPAC_TDC_INIT(void)
{
   char name[256];
   char title[256];

   /* book GTIG histos */

   for (int i=0; i<N_TDC_f3_ppac; i++)
   {
       //       printf("%0x\n",id);
       sprintf(name,  "F3PA-%03x", i);
       sprintf(title, "F3PA-%03x", i);
       hPTDCHists_raw[i] = h1_book<TH1D>(name, title, PTAC_N_BINS, PTAC_X_LOW, PTAC_X_HIGH);
    }
   hPXDIFFTDCHists_raw = h1_book<TH1D>("F3 X DIFF", "F3 X DIFF", 1000, -4000, 4000);
   hPYDIFFTDCHists_raw = h1_book<TH1D>("F3 Y DIFF", "F3 Y DIFF", 1000, -4000, 4000);
   hP2DTDCHists_raw = h2_book<TH2D>("F3 2D", "F3 2D plot", 1000, -4000, 4000, 1000, -4000, 4000);

	hPXDIFFTDCHists_cor = h1_book<TH1D>("F3 X COR", "F3 X COR", 250, -50, 50);


	first_Call_chks_arya = 1;


   return SUCCESS;
}

/*-- BOR routine ---------------------------------------------------*/

INT F3_PPAC_TDC_BOR(INT run_number)
{
   return SUCCESS;
}

/*-- eor routine ---------------------------------------------------*/

INT F3_PPAC_TDC_EOR(INT run_number)
{
   return SUCCESS;
}

/*-- event routine -------------------------------------------------*/

INT F3_PPAC_TDC(EVENT_HEADER *pheader, void *pevent)
{

   DWORD *pdata;
   int bk_size;

   /* look for bank, return if not present */
   bk_size = bk_locate(pevent, BANK_NAME_F3PPAC, &pdata);
   if (!bk_size) {
      printf("no data\n");
      return 1;
   }

   int32_t event_count;
   int32_t channel;
   int32_t tdc_value[N_TDC_f3_ppac];
   uint64_t global_time_stamp,gcount1;

   memset(tdc_value, 0, sizeof(int32_t)*N_TDC_f3_ppac);



//printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
   for(int i=0; i<1; i++){event_count = *pdata++;}

   for (int i=0; i<bk_size-4; )
   {
	   channel=*pdata++; i++;
	   tdc_value[channel]=*pdata++; i++;
	  
   }

   for(int i=0; i<3; )
   {
	 gcount1=*pdata++; i++;
	   uint32_t time2=*pdata++; i++;
	   uint32_t time1=*pdata++; i++;
	   global_time_stamp=time2|time1;
	 //  printf("\n\n%12f, %12f, %12f\n", time2*800e-9, time1*800e-9, global_time_stamp*800e-9);
   }

#if 1
   printf("F3PPAC ===============================%d\n", bk_size);
   	 printf("event_count: %d\n",event_count);
   	 for(int i=0; i<N_TDC_f3_ppac; i++){printf("channel:%d, measure:%8f\n", i, tdc_value[i]*0.025);}
   	 printf("global time: %12f\n", global_time_stamp*800e-9);
#endif


     char name[256];

   for(int i=0; i<N_TDC_f3_ppac; i++)
    {
    	 sprintf(name,  "F3PPAC-%03x", i);
    	 hPTDCHists_raw[i]->Fill(tdc_value[i]*f3_ppac_tdc.gain, 1);
    }
    if (tdc_value[0]+tdc_value[1]>1000) hPXDIFFTDCHists_raw->Fill(tdc_value[0]-tdc_value[1],1);
    if (tdc_value[2]+tdc_value[3]>1000) hPYDIFFTDCHists_raw->Fill(tdc_value[2]-tdc_value[3],1);
    if (tdc_value[0]+tdc_value[1]>1000 && tdc_value[2]+tdc_value[3]>1000)hP2DTDCHists_raw->Fill(tdc_value[0]-tdc_value[1], tdc_value[2]-tdc_value[3], 1);

 int bk_size_a;	 
  bk_size_a = bk_locate(pevent, BANK_NAME_U1SILLICON, &pdata);
   if (!bk_size_a) {
      //printf("no silicon ary a data\n");
      return 1;
   }

   int32_t dumps_arya;
   int32_t event_counts_arya;
   int32_t channels_arya;
   int32_t ev_tag1, ev_tag2;
   int32_t ADC_arya_value[N_ADC_arya_silicon];
   uint64_t gcount2;

//   memset(tdc_value, 0, sizeof(int32_t)*16);
   memset(ADC_arya_value, 0, sizeof(int32_t)*N_ADC_arya_silicon);

   //printf("SILICON arya for correlation====================== %d\n", bk_size_a);

   if(first_Call_chks_arya)
   {
	   for(int i=0; i<bk_size_a; i++){dumps_arya=*pdata++;  }
	   first_Call_chks_arya=0;
   }
   if(! first_Call_chks_arya){
	   for(int i=0; i<bk_size_a; ){
		   channels_arya=*pdata++; i++;	

		 if(channels_arya<N_ADC_arya_silicon){  
			ADC_arya_value[channels_arya]=*pdata++; i++;
			//printf("ary a channel:%d, ADC value:%d\n", channels_arya, ADC_arya_value[channels_arya]);
		}
		
		//else{
		//		ev_tag1 = *pdata++; i++;
		//		ev_tag2 = *pdata++; i++;
			
		//}
			//gcount2=*pdata++; i++;
		   
	   }

   }

bool logic = 0;

for(int i=0; i < 16; i++) if(ADC_arya_value[i] > 300 && ADC_arya_value[i] < 700) logic = 1;

 if (tdc_value[1]+tdc_value[2]-2.*tdc_value[0]>2700 && tdc_value[3]+tdc_value[4]-2.*tdc_value[0] > 2700 && tdc_value[1]+tdc_value[2]-2.*tdc_value[0] < 3200 && tdc_value[3]+tdc_value[4]-2.*tdc_value[0] < 3200 && logic)hPXDIFFTDCHists_cor->Fill(tdc_value[1]*0.0153-tdc_value[2]*0.0153,1);



   return SUCCESS;
}
