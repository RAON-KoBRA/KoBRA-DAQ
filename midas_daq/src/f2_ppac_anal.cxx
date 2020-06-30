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
#include <string>
#include "detector_conf.h"
#include "analyzer.h"
/* midas includes */

#include "f2_ppac_anal.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TTree.h>
#include <TDirectory.h>


/* root includes */


/*-- Parameters ----------------------------------------------------*/


/*-- module-local variables ----------------------------------------*/


/*-- init routine --------------------------------------------------*/

static int NUM_HIST = 0;

static TH1D *hPTDCHists_raw[N_TDC_f2_PPAC];
static TH1D *hPXDIFFTDCHists_raw;
static TH1D *hPYDIFFTDCHists_raw;
static TH1D *hPXDIFFcalibTDCHists_raw;
static TH1D *hPYDIFFcalibTDCHists_raw;
static TH2D *hP2DTtotHists_calib;
static TH1D *hPXDIFFTDCHists_silicon[N_ADC_f3_silicon];
static TH1D *hPYDIFFTDCHists_silicon[N_ADC_f3_silicon];
static TH2D *hP2DTDCHists_raw;
static TH2D *hP2DXHists_raw;
static TH2D *hP2DPosTtotHists;
static TH2D *PosLarge2D;
static TH2D *hP2DPosLargeMulthitHists;
static TH1I *hNhit;

int first_Call_chkt;
using namespace std;

INT F2_TDC_INIT(void)
{
   char name[256];
   char title[256];

   /* book GTIG histos */
   int i;

   for (i=0; i<N_TDC_f2_PPAC; i++)
   {
       //       printf("%0x\n",id);
       sprintf(name,  "F2PA-%03x", i);
       sprintf(title, "F2PA-%03x", i);
       hPTDCHists_raw[i] = h1_book<TH1D>(name, title, PTAC_N_BINS, PTAC_X_LOW, PTAC_X_HIGH);
    }
/*
   for (int i=0; i<N_ADC_f3_silicon; i++)
   {
       //       printf("%0x\n",id);d
       sprintf(name,  "F3SILICONCorrPPACX-%03d", i);
       sprintf(title, "F3SILICONCorrPPACX-%03d", i);
       hPXDIFFTDCHists_silicon[i] = h1_book<TH1D>(name, title, 100, -4000, 4000);
       sprintf(name,  "F3SILICONCorrPPACY-%03d", i);
       sprintf(title, "F3SILICONCorrPPACY-%03d", i);
       hPYDIFFTDCHists_silicon[i] = h1_book<TH1D>(name, title, 100, -4000, 4000);
    }
*/
   hPXDIFFTDCHists_raw = h1_book<TH1D>("F2 X DIFF","F2 X DIFF", 500, -4000, 4000);
   hPYDIFFTDCHists_raw = h1_book<TH1D>("F2 Y DIFF","F2 Y DIFF", 500, -4000, 4000);
   hP2DTDCHists_raw = h2_book<TH2D>("F2 2D", "F2 2D plot", 1000, -4000, 4000, 1000, -4000, 4000);
   hPXDIFFcalibTDCHists_raw = h1_book<TH1D>("F2 X DIFF calib","F2 X DIFF calib", 500, -60, 60);
   hPYDIFFcalibTDCHists_raw = h1_book<TH1D>("F2 Y DIFF calib","F2 Y DIFF calib", 500, -60, 60);
   hP2DTtotHists_calib = h2_book<TH2D>("F2 Ttot 2D","F2 Ttot 2D plot", 100, 60, 90, 100, 60, 90);
   hP2DPosTtotHists = h2_book<TH2D>("F2 pos vs. Ttot","F2 Tpos vs. Ttot",300,-3400,3400,100,45,100);
   PosLarge2D = h2_book<TH2D>("PosLarge2D","PosLarge2D",800,-200,200,400,-100,100);
   hP2DPosLargeMulthitHists = h2_book<TH2D>("PosLargeMulthit","PosLargeMulthit",3000,-200,200,1500,-100,100);
   hNhit = h1_book<TH1I>("Nhit","Nhit",10,0,10);
//   hP2DXHists_raw = h2_book<TH2D>("F2 2D cor", "F2 2D cor", 40, 0, 40, 1000, -4000, 4000);
   NUM_HIST = i;

   return SUCCESS;
}

/*-- BOR routine ---------------------------------------------------*/

INT F2_TDC_BOR(INT run_number)
{
   return SUCCESS;
}

/*-- eor routine ---------------------------------------------------*/

INT F2_TDC_EOR(INT run_number)
{
        char pedestalname[256];
	sprintf(pedestalname,  "~/online/data/pedestal-%d.txt", run_number);

/*	ofstream myfile;
	myfile.open (pedestalname);

	float pedmean[N_TDC_f2_PPAC];
	for (int i=0; i<N_TDC_f2_PPAC; i++) {
	pedmean[i] = hPTDCHists_raw[i]->GetMean();
	printf("Pedestal mean value =%d %f\n",i, pedmean[i]);
	myfile <<i <<" "<<pedmean[i] << " \n";
	}*/
   return SUCCESS;
}

/*-- event routine -------------------------------------------------*/

INT F2_TDC(EVENT_HEADER *pheader, void *pevent)
{

   DWORD *pdata;
   int bk_size;

   DWORD *pdata_adc;
   int bk_size_adc;

   /* look for bank, return if not present */
   bk_size = bk_locate(pevent, BANK_NAME_F2PPAC, &pdata);
   if (!bk_size) {
      printf("no data\n");
      return 1;
   }
/*
   bk_size_adc = bk_locate(pevent, BANK_NAME_F3SILICON, &pdata_adc);
   if (!bk_size_adc) {
      printf("no data\n");
      return 1;
   }*/
   int32_t event_count;
   int32_t channel;
   int32_t tdc_value[16];
   uint64_t global_time_stamp;

   memset(tdc_value, 0, sizeof(int32_t)*16);

   int32_t dump;
   int32_t event_count_adc;
   int32_t channel_adc;
   int32_t ADC_value[N_ADC_f3_silicon];

   memset(ADC_value, 0, sizeof(int32_t)*N_ADC_f3_silicon);

//printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
   for(int i=0; i<1; i++){event_count = *pdata++;}

   for (int i=0; i<bk_size-3; )
   {
	   channel=*pdata++; i++;
	   tdc_value[channel]=*pdata++; i++;
   }

   for(int i=0; i<2; )
   {
	   uint32_t time2=*pdata++; i++;
	   uint32_t time1=*pdata++; i++;
	   global_time_stamp=time2|time1;
	   printf("f2 ppac \n\n%12f, %12f, %12f\n", time2*800e-9, time1*800e-9, global_time_stamp*800e-9);
   }

#if 1
   printf("F2PPAC ===============================%d\n", bk_size);
   	 printf("event_count: %d\n",event_count);
   	 for(int i=0; i<N_TDC_f2_PPAC; i++){
         if(tdc_value[i]*0.025>0)printf("channel:%d, measure:%8f\n", i, tdc_value[i]*0.025);
  	 }
   	 printf("global time: %12f\n", global_time_stamp*800e-9);
#endif


     char name[256];

   for(int i=0; i<N_TDC_f2_PPAC; i++)
    {
    	 sprintf(name,  "F2PPAC-%03x", i);
    	 hPTDCHists_raw[i]->Fill(tdc_value[i]*f2_tdc_cali_param.gain, 1);
    }
    if (tdc_value[0]>0 && tdc_value[1]>0 && tdc_value[0]-tdc_value[1]<5000 && tdc_value[0]-tdc_value[1]>-5000 && tdc_value[12]>0) hPXDIFFTDCHists_raw->Fill(tdc_value[0]-tdc_value[1],1);
    printf("FOR PPAC consistency check one = %d\n", tdc_value[0]-tdc_value[1]);

    if (tdc_value[2]+tdc_value[3]>1000) hPYDIFFTDCHists_raw->Fill(tdc_value[2]-tdc_value[3],1);

    if (tdc_value[0]+tdc_value[1]>1000 && tdc_value[2]+tdc_value[3]>1000)hP2DTDCHists_raw->Fill(tdc_value[0]-tdc_value[1], tdc_value[2]-tdc_value[3], 1);

    if (tdc_value[0]>0 && tdc_value[1]>0 && tdc_value[0]-tdc_value[1]<5000 && tdc_value[0]-tdc_value[1]>-5000 && tdc_value[12]>0) hPXDIFFcalibTDCHists_raw->Fill((tdc_value[0]-tdc_value[1])*0.015338,1);

    if (tdc_value[2]+tdc_value[3]>1000) hPYDIFFcalibTDCHists_raw->Fill((tdc_value[2]-tdc_value[3])*0.015338,1);

    if (tdc_value[4]>0 && tdc_value[5]>0 && tdc_value[12]>0) hP2DTtotHists_calib->Fill((tdc_value[4]-tdc_value[12]+tdc_value[5]-tdc_value[12])*25./1000.,(tdc_value[2]-tdc_value[12]+tdc_value[3]-tdc_value[12])*25./1000.,1);

    if ((tdc_value[6]-tdc_value[12]+tdc_value[7]-tdc_value[12])*25./1000.>40. && (tdc_value[6]-tdc_value[12]+tdc_value[7]-tdc_value[12])*25./1000.<90.) {
      hP2DPosTtotHists->Fill(tdc_value[6]-tdc_value[7],(tdc_value[6]-tdc_value[12]+tdc_value[7]-tdc_value[12])*25./1000.,1);
    }

//Fills large 40x20cm2 position histogram and find multiple position hit events

    int nyhit=0;//counts number of positions for each event
    int nxhit=0;//counts number of positions for each event

    double x1x2_0 = 0.01522;//conversion gradient (mm/chan)
    double x1x2_1 = 0.7914;//conversion intercept (mm)
    double x3x4_0 = 0.01529;
    double x3x4_1 = 0.3573;
    double x5x6_0 = 0.01515;
    double x5x6_1 = 0.5610;
    double x7x8_0 = 0.01512;
    double x7x8_1 = 0.4472;

    double y1y2_0 = 0.01512;
    double y1y2_1 = -0.2145;
    double y3y4_0 = 0.01506;
    double y3y4_1 = -0.1396;

    double ypos[2];//stores y positions for the 2 different segments
    double xpos[4];//stores x positions for the 4 different segments

//calculates nuber of Y positions
    if (tdc_value[8]>0 && tdc_value[9]>0 && tdc_value[8]-tdc_value[9]<5000 && tdc_value[8]-tdc_value[9]>-5000 && tdc_value[12]>0) {
      ypos[nyhit] = (tdc_value[8]-tdc_value[9]-16.61)*y1y2_0+y1y2_1-50.;
      nyhit++;
    }

    if (tdc_value[10]>0 && tdc_value[11]>0 && tdc_value[10]-tdc_value[11]<5000 && tdc_value[10]-tdc_value[11]>-5000 && tdc_value[12]>0) {
      ypos[nyhit] = (tdc_value[10]-tdc_value[11]+2.674)*y3y4_0+y3y4_1+50.;
      nyhit++;
    }


    if (tdc_value[0]>0 && tdc_value[1]>0 && tdc_value[0]-tdc_value[1]<5000 && tdc_value[0]-tdc_value[1]>-5000 && tdc_value[12]>0) {
      xpos[nxhit] = (tdc_value[0]-tdc_value[1]+6.398)*x1x2_0+x1x2_1-150.;
      nxhit++;
    }
    if (tdc_value[2]>0 && tdc_value[3]>0 && tdc_value[2]-tdc_value[3]<5000 && tdc_value[2]-tdc_value[3]>-5000 && tdc_value[12]>0) {
      xpos[nxhit] = (tdc_value[2]-tdc_value[3]+5.481)*x3x4_0+x3x4_1-50.;
      nxhit++;
    }
    if (tdc_value[4]>0 && tdc_value[5]>0 && tdc_value[4]-tdc_value[5]<5000 && tdc_value[4]-tdc_value[5]>-5000 && tdc_value[12]>0) {
      xpos[nxhit] = (tdc_value[4]-tdc_value[5]+2.42)*x5x6_0+x5x6_1+50.;
      nxhit++;
    }
    if (tdc_value[6]>0 && tdc_value[7]>0 && tdc_value[6]-tdc_value[7]<5000 && tdc_value[6]-tdc_value[7]>-5000 && tdc_value[12]>0) {
      xpos[nxhit] = (tdc_value[6]-tdc_value[7]+6.685)*x7x8_0+x7x8_1+150.;
      nxhit++;
    }


    if (nyhit>1) {
//      DoublehitY->Fill(ypos[0],1);
//      DoublehitY->Fill(ypos[1],1);
    }
    if (nyhit>1 && (abs(ypos[0])<5. || abs(ypos[1])<5.)) {
      ypos[0] = (1.1*ypos[0]+1.3*ypos[1])/2.4;
//      PosYcor->Fill(ypos[0],1);
    }
    if (nyhit==1) {
      if (ypos[0]>-7.5 && ypos[0]<0){
        ypos[0] = ypos[0]+(ypos[0]+7.5)*0.3;
      }
      if (ypos[0]<7.5 && ypos[0]>0){
        ypos[0] = ypos[0]+(ypos[0]-7.5)*0.3;
      }
//      PosYcor->Fill(ypos[0],1);
//      SinglehitY->Fill(ypos[0],1);
    }

    if (nxhit>1) {
//      DoublehitX->Fill(xpos[0],1);
//      DoublehitX->Fill(xpos[1],1);
    }

    if (nxhit==2 && ((xpos[0]<-100. && xpos[0]>-105.) || (xpos[1]>-100. && xpos[0]<-95.))) {
      xpos[0] = (1.1*xpos[0]+1.3*xpos[1])/2.4;
//      PosXcor->Fill(xpos[0],1);
    }
    if (nxhit==2 && ((xpos[0]<0. && xpos[0]>-5.) || (xpos[1]>0. && xpos[0]<-5.))) {
      xpos[0] = (1.1*xpos[0]+1.3*xpos[1])/2.4;
//      PosXcor->Fill(xpos[0],1);
    }
    if (nxhit==2 && ((xpos[0]<100. && xpos[0]>95.) || (xpos[1]>100. && xpos[0]<105.))) {
      xpos[0] = (1.1*xpos[0]+1.3*xpos[1])/2.4;
//      PosXcor->Fill(xpos[0],1);
    }


    if (nxhit==1) {

      if (xpos[0]<-92.5 && xpos[0]>-100.){
        xpos[0] = xpos[0]+(xpos[0]+100.-7.5)*0.2;
      }
      if (xpos[0]<-100. && xpos[0]>-107.5){
        xpos[0] = xpos[0]+(xpos[0]+100.+7.5)*0.2;
      }

      if (xpos[0]<0. && xpos[0]>-7.5){
        xpos[0] = xpos[0]+(xpos[0]+7.5)*0.15;
      }
      if (xpos[0]<7.5 && xpos[0]>0.){
        xpos[0] = xpos[0]+(xpos[0]-7.5)*0.15;
      }

      if (xpos[0]>92.5 && xpos[0]<100.){
        xpos[0] = xpos[0]+(xpos[0]-100.+7.5)*0.15;
      }
      if (xpos[0]>100. && xpos[0]<107.5){
        xpos[0] = xpos[0]+(xpos[0]-100.-7.5)*0.15;
      }

//      PosXcor->Fill(xpos[0],1);
//      SinglehitX->Fill(xpos[0],1);
    }

    if (nyhit>0 && nxhit>0) {PosLarge2D->Fill(xpos[0],ypos[0],1);}

/*

printf("F3SILICON for PPAC2 ====================== %d\n", bk_size_adc);

   if(first_Call_chkt)
   {
	   for(int i=0; i<bk_size_adc; i++){dump=*pdata_adc++;}
	   first_Call_chkt=0;
   }

   if(! first_Call_chkt)
   {
	   for(int i=0; i<bk_size_adc-1; )
	   {
		   channel_adc=*pdata_adc++; i++;
		   //printf("%d\n", channel);
		   if (channel_adc>N_ADC_f3_silicon)
		   {
			dump=channel_adc;
			printf("channel number from event_counter = %d\n", channel_adc);
		    	channel_adc=*pdata_adc++; i++;
		   }	
		   if (channel_adc<N_ADC_f3_silicon)
		   {
		   ADC_value[channel_adc]=*pdata_adc++; i++;
		   printf("channel:%d, ADC value:%d\n", channel_adc, ADC_value[channel_adc]);
		   }
	   }

	   event_count_adc=*pdata_adc++;
	   printf("F3SILICON event_counter for PPAC2 =======================%d\n", event_count_adc);

	   char namex[256];
           char namey[256];

	    for(int i=0; i<N_ADC_f3_silicon; i++)
	     {
	     	 sprintf(namex,  "F3SILICONCorrPPACX-%03d", i);
		 sprintf(namey,  "F3SILICONCorrPPACY-%03d", i);
	     	 if(ADC_value[i]>10) 
                    {
			if (tdc_value[0]+tdc_value[1]>1000) hPXDIFFTDCHists_silicon[i]->Fill(tdc_value[0]-tdc_value[1],1);
 	                if (tdc_value[2]+tdc_value[3]>1000) hPYDIFFTDCHists_silicon[i]->Fill(tdc_value[2]-tdc_value[3],1);
                        if (tdc_value[0]+tdc_value[1]>1000) 
				{
		              		hP2DXHists_raw->Fill(i,tdc_value[0]-tdc_value[1], 1);
					printf("FOR PPAC consistency check two= %d\n", tdc_value[0]-tdc_value[1]);
				}
		     }
	     }
   }
   
*/
   return SUCCESS;
}

