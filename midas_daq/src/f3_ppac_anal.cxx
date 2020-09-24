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
//#include "f3_silicon_anal.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TDirectory.h>



/* root includes */


/*-- Parameters ----------------------------------------------------*/


/*-- module-local variables ----------------------------------------*/


/*-- init routine --------------------------------------------------*/

int first_Call_chks;

static TH1D *hPTDCHists_raw[N_TDC_f3_ppac];
static TH1D *hPXDIFFTDCHists_raw;
static TH1D *hPYDIFFTDCHists_raw;
static TH2D *hP2DTDCHists_raw;
static TH1D *hPF1F3DIFFTDCHists_ch2_raw;
static TH1D *hPF1F3DIFFTDCHists_ch11_raw;
static TH1D *hPF1F3DIFFTDCHists_ch5_ch2_raw;
static TH1D *hPF1F3DIFFTDCHists_ch11_ch2_raw;
static TH1D *hPF2F3DIFFTDCHists_ch18_ch2_raw;
static TH2D *hP2DTDCHists_corel_raw;
static TH1D *hADCCHists_raw;

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
   hPF1F3DIFFTDCHists_ch2_raw = h1_book<TH1D>("F1-F3 Pos. DIFF_ch2", "F1-F3 DIFF", 1000, -500, 500);
   hPF1F3DIFFTDCHists_ch11_raw = h1_book<TH1D>("F1-F3 Pos. DIFF_ch11", "F1-F3 DIFF", 1000, -500, 500);
   hPF1F3DIFFTDCHists_ch5_ch2_raw = h1_book<TH1D>("F3 Pos. DIFF_ch5_ch2", "F3 ch5-ch2", 1000, -500, 500);
   hPF1F3DIFFTDCHists_ch11_ch2_raw = h1_book<TH1D>("F1 Pos. DIFF_ch11_ch2", "F3 ch11-ch2", 1000, -500, 500);
   hPF2F3DIFFTDCHists_ch18_ch2_raw = h1_book<TH1D>("F3-F2 Pos. DIFF_ch18_ch2", "F3-F2 ch18-ch2", 1000, -500, 500);

   hADCCHists_raw = h1_book<TH1D>("F3 silicon ch23", "F3 silicon ch23", F3SILICON_N_BINS, F3SILICON_X_LOW, F3SILICON_X_HIGH);
   hP2DTDCHists_corel_raw = h2_book<TH2D>("F3 2D PPAC_silicon correlation", "F3 2D PPAC_silicon correlation plot", 1000, -500, 500, F3SILICON_N_BINS, F3SILICON_X_LOW, F3SILICON_X_HIGH);
   first_Call_chks=1;

   float x1_1[4]; //1st ppac x position @ F1, 4 segmemts 
   float y1_1[2]; //1st ppac y position @ F1, 2 segments 

   float x2_1[1]; //1st ppac x position @ F2
   float y2_1[1]; //1st ppac y position @ F2

   float x3_1[1]; //1st ppac x position @ F3 
   float x3_2[1]; //2nd ppac x position @ F3
   float y3_1[1]; //1st ppac y position @ F3 
   float y3_2[1]; //2nd ppac y position @ F3 

   float tof; 

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
   int bk_size3;

   /* look for bank, return if not present */
   bk_size3 = bk_locate(pevent, BANK_NAME_F3PPAC, &pdata);
   if (!bk_size3) {
      printf("no F3 data\n");
      return 1;
   }

   int32_t event_count3;
   int32_t channel3;
   int32_t tdc_value3[N_TDC_f3_ppac];
   uint64_t global_time_stamp3;

   memset(tdc_value3, 0, sizeof(int32_t)*N_TDC_f3_ppac);



//printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
   for(int i=0; i<1; i++){event_count3 = *pdata++;}

   for (int i=0; i<bk_size3-3; )
   {
	   channel3=*pdata++; i++;
	   tdc_value3[channel3]=*pdata++; i++;
   }

   for(int i=0; i<2; )
   {
	   uint32_t time2=*pdata++; i++;
	   uint32_t time1=*pdata++; i++;
	   global_time_stamp3=time2|time1;
	 //  printf("\n\n%12f, %12f, %12f\n", time2*800e-9, time1*800e-9, global_time_stamp*800e-9);
   }

#if 1
   printf("F3PPAC ===============================%d\n", bk_size3);
   	 //printf("F3 event_count: %d\n",event_count);
   	 for(int i=0; i<N_TDC_f3_ppac; i++){printf("f3 channel:%d, measure:%8f\n", i, tdc_value3[i]*0.025);}
   	 printf("global time: %12f\n", global_time_stamp3*800e-9);
#endif


     char name[256];

   for(int i=0; i<N_TDC_f3_ppac; i++)
    {
    	 sprintf(name,  "F3PPAC-%03x", i);
    	 hPTDCHists_raw[i]->Fill(tdc_value3[i]*f3_ppac_tdc.gain, 1);
    }
    if (tdc_value3[0]+tdc_value3[1]>1000) hPXDIFFTDCHists_raw->Fill(tdc_value3[0]-tdc_value3[1],1);
    if (tdc_value3[2]+tdc_value3[3]>1000) hPYDIFFTDCHists_raw->Fill(tdc_value3[2]-tdc_value3[3],1);
    if (tdc_value3[0]+tdc_value3[1]>1000 && tdc_value3[2]+tdc_value3[3]>1000)hP2DTDCHists_raw->Fill(tdc_value3[0]-tdc_value3[1], tdc_value3[2]-tdc_value3[3], 1);
    hPF1F3DIFFTDCHists_ch5_ch2_raw->Fill((tdc_value3[2]-tdc_value3[5])*0.025);

////////////////////////////F1 PPAC/////////////////////////////////////
   /* look for bank, return if not present */
   int bk_size1;
    
   bk_size1 = bk_locate(pevent, BANK_NAME_F1PPAC, &pdata);
   if (!bk_size1) {
      printf("no F1 dataa\n");
      return 1;
   }

   int32_t event_count1;
   int32_t channel1;
   int32_t tdc_value1[N_TDC_f1_PPAC];
   uint64_t global_time_stamp1;

   memset(tdc_value1, 0, sizeof(int32_t)*N_TDC_f1_PPAC);

   for(int i=0; i<1; i++){event_count1 = *pdata++;}

   for (int i=0; i<bk_size1-3; )
   {
	   channel1=*pdata++; i++;
	   tdc_value1[channel1]=*pdata++; i++;
   }

   for(int i=0; i<2; )
   {
	   uint32_t time2=*pdata++; i++;
	   uint32_t time1=*pdata++; i++;
	   global_time_stamp1=time2|time1;
	 //  printf("\n\n%12f, %12f, %12f\n", time2*800e-9, time1*800e-9, global_time_stamp*800e-9);
   }
   for(int i=0; i<N_TDC_f1_PPAC; i++){
         printf("f1 channel:%d, measure:%8f\n", i, tdc_value1[i]*0.025);
  	 }
   hPF1F3DIFFTDCHists_ch2_raw->Fill((tdc_value3[2]-tdc_value1[17])*0.025,1);
   hPF1F3DIFFTDCHists_ch11_raw->Fill((tdc_value3[11]-tdc_value1[23])*0.025,1);
//   printf("tdc_value3:%d, tdc_value1:%d\n", tdc_value[2], tdc_value1[2]);
//   if (tdc_value[2]+tdc_value[3]>1000) hPYDIFFTDCHists_raw->Fill(tdc_value[2]-tdc_value[3],1);
   hPF1F3DIFFTDCHists_ch11_ch2_raw->Fill((tdc_value1[17]-tdc_value1[2])*0.025);
///////////////////////////////////////////////////////////////////////////

////////////////////////////F2 PPAC/////////////////////////////////////
   /* look for bank, return if not present */
   int bk_size2;

   bk_size2 = bk_locate(pevent, BANK_NAME_F2PPAC, &pdata);
   if (!bk_size2) {
      printf("no F2 data\n");
      return 1;
   }

   int32_t event_count2;
   int32_t channel2;
   int32_t tdc_value2[N_TDC_f2_PPAC];
   uint64_t global_time_stamp2;

   memset(tdc_value2, 0, sizeof(int32_t)*N_TDC_f2_PPAC);

   for(int i=0; i<1; i++){event_count1 = *pdata++;}

   for (int i=0; i<bk_size2-3; )
   {
	   channel2=*pdata++; i++;
	   tdc_value2[channel2]=*pdata++; i++;
   }

   for(int i=0; i<2; )
   {
	   uint32_t time2=*pdata++; i++;
	   uint32_t time1=*pdata++; i++;
	   global_time_stamp2=time2|time1;
	 //  printf("\n\n%12f, %12f, %12f\n", time2*800e-9, time1*800e-9, global_time_stamp*800e-9);
   }
   #if 1
   //printf("F2PPAC ===============================%d\n", bk_size2);
   //printf("event_count: %d\n",event_count2);
   	 for(int i=0; i<N_TDC_f2_PPAC; i++){
         printf("f2 channel:%d, measure:%8f\n", i, tdc_value2[i]*0.025);
  	 }
//   	 printf("global time: %12f\n", global_time_stamp*800e-9);
   #endif

   hPF2F3DIFFTDCHists_ch18_ch2_raw->Fill((tdc_value3[2]-tdc_value2[18])*0.025,1);
   
////////////////////////////////////////////////////////////////////////////

//////////////////////////F3 Silicon///////////////////////////////////////
   int bk_size;

   bk_size = bk_locate(pevent, BANK_NAME_F3SILICON, &pdata);
   if (!bk_size) {
      printf("no F3 silicon data\n");
      return 1;
   }

   int32_t dumps;
   int32_t event_counts;
   int32_t channels;
   int32_t ADC_value[N_ADC_f3_silicon];
/*
   int32_t event_count_tdc2;
   int32_t channel_tdc2;
   int32_t tdc_value[N_TDC_f2_PPAC];
   uint64_t global_time_stamp;
*/
//   memset(tdc_value, 0, sizeof(int32_t)*16);
   memset(ADC_value, 0, sizeof(int32_t)*N_ADC_f3_silicon);

   printf("F3SILICON for correlation====================== %d\n", bk_size);

   if(first_Call_chks)
   {
	   for(int i=0; i<bk_size; i++){dumps=*pdata++;}
	   first_Call_chks=0;
   }
   if(! first_Call_chks)
   {
	   for(int i=0; i<bk_size-1; )
	   {
		   channels=*pdata++; i++;
		   //printf("%d\n", channel);
		   if (channels>N_ADC_f3_silicon)
		   {
			//dump=channel;
			//printf("channel number from event_counter = %d\n", channel);
		    	channels=*pdata++; i++;
			//*pdata_cal++=channel;
		   }	
		   if (channels<N_ADC_f3_silicon)
		   {
		   ADC_value[channels]=*pdata++; i++;
			//*pdata_cal++=channel;
			//*pdata_cal++=ADC_value[channel];
		   //printf("channel:%d, ADC value:%d\n", channel, ADC_value[channel]);
		   }
	   }
   }
   //printf("F3SILICON ch19 =======================%d\n", ADC_value[19]);
   hADCCHists_raw->Fill(ADC_value[19],1);
   hP2DTDCHists_corel_raw->Fill((tdc_value3[11]-tdc_value1[17])*0.025,ADC_value[19],1);
//   printf("F3SILICON ====================== %d\n", bk_size);
////////////////////////////////////////////////////////////////////////////

   return SUCCESS;
}

