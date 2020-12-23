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

#include "silicon_aryb_anal.h"
//#include "f1_ppac_anal.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TTree.h>
#include <TDirectory.h>



/* root includes */
/*-- Parameters ----------------------------------------------------*/
//ADC_CALIBRATION_PARAM adccalib_param;


/*-- module-local variables ----------------------------------------*/

/*-- Module declaration --------------------------------------------*/
//INT adc_calib(EVENT_HEADER *, void *);
//INT adc_calib_init(void);
//INT adc_calib_bor(INT run_number);
//INT adc_calib_eor(INT run_number);

//ADC_CALIBRATION_PARAM_STR(adc_calibration_param_str);

//ANA_MODULE adc_calib_module = {
//   "ADC calibration",           /* module name           */
//   "Stefan Ritt",               /* author                */
//   adc_calib,                   /* event routine         */
//   adc_calib_bor,               /* BOR routine           */
//  adc_calib_eor,               /* EOR routine           */
//   adc_calib_init,              /* init routine          */
//   NULL,                        /* exit routine          */
//   &adccalib_param,             /* parameter structure   */
//   sizeof(adccalib_param),      /* structure size        */
//   adc_calibration_param_str,   /* initial parameters    */
//};

/*-- init routine --------------------------------------------------*/

int first_Call_chk_aryb;
using namespace std;

//vector<double>slope_arr;
//vector<double>intercept_arr;


static TH1D *hADCCARYBHists_raw[N_ADC_aryb_silicon];
static TH1D *hADCCARYBHists_cal[N_ADC_aryb_silicon];
static TH1D *hADCCARYBHists_ped[N_ADC_aryb_silicon];
static TH1D *hADCCARYBHists_ped1[N_ADC_aryb_silicon];
static TH1D *hADCCARYBHists_ped2[N_ADC_aryb_silicon];
static TH2D *hP2DADCARYBHists_raw;
static TH1D *hSUMADCARYBHists_raw;

static TH2D *hP2DADCARYBHists_cal;
static TH2D *hP2DADCARYBHists_ped;
float pedestal_aryb[N_ADC_aryb_silicon];
extern float y_inter_indiv[N_ADC_aryb_silicon+1], slope_indiv[N_ADC_aryb_silicon+1];

//static TF1 *f1_ped_fit[N_ADC_f3_silicon];

INT sili_aryb_ADC_INIT(void)
{
   char name[256];
   char title[256];
   char nameped[256];
   char nameped1[256];
   char nameped2[256];	
   char titleped[256];
   char titleped1[256];
   char titleped2[256];
   char namecal[256];
   char titlecal[256];
/*
   for (int i=0; i<N_ADC_aryb_silicon; i++)
   {
       //       printf("%0x\n",id);d
       //TF1 *f1 = new TF1("f1","gaus",0,200);
       sprintf(name,  "F3SILICONARYB-%03d", i);
       sprintf(title, "F3SILICONARYB-%03d", i);
       hADCCARYBHists_raw[i] = h1_book<TH1D>(name, title, F3SILICONARYB_N_BINS, F3SILICONARYB_X_LOW, F3SILICONARYB_X_HIGH);
       sprintf(nameped,  "F3SILICONARYBPED-%03d", i);
       sprintf(nameped1,  "F3SILICOARYBNPED1-%03d", i);
       sprintf(nameped2,  "F3SILICOARYBNPED2-%03d", i);	
       sprintf(titleped, "F3SILICONARYBPED-%03d", i);
       sprintf(namecal,  "F3SILICONARYBCAL-%03d", i);
       sprintf(titlecal, "F3SILICONARYBCAL-%03d", i);
       hADCCARYBHists_ped[i] = h1_book<TH1D>(nameped, titleped, F3SILICONARYB_N_BINS, F3SILICONARYB_X_LOW, F3SILICONARYB_X_HIGH);
       hADCCARYBHists_ped1[i] = h1_book<TH1D>(nameped1, titleped1, F3SILICONARYB_N_BINS, F3SILICONARYB_X_LOW, F3SILICONARYB_X_HIGH);
       hADCCARYBHists_ped2[i] = h1_book<TH1D>(nameped2, titleped2, F3SILICONARYB_N_BINS, F3SILICONARYB_X_LOW, F3SILICONARYB_X_HIGH);	
       hADCCARYBHists_cal[i] = h1_book<TH1D>(namecal, titlecal, F3SILICONARYB_N_BINS, F3SILICONARYB_X_LOW, F3SILICONARYB_X_HIGH);
       //f1_ped_fit[i] = f1_book<TF1>(name, title, F3SILICON_N_BINS, F3SILICON_X_LOW, F3SILICON_X_HIGH);
   }
   hP2DADCARYBHists_raw = h2_book<TH2D>("F3 silicon ARYB 2D", "F3 silicon ARYB 2D plot", 40, 0, 40, 100, 0, 4100);
   hSUMADCARYBHists_raw = h1_book<TH1D>("silicon ARYB sum all channels", "silicon ARYB all channel sum", F3SILICONARYB_N_BINS, F3SILICONARYB_X_LOW, F3SILICONARYB_X_HIGH);
   hP2DADCARYBHists_ped = h2_book<TH2D>("F3 silicon ARYB 2D pedestal", "F3 silicon ARYB 2D plot pedestal", 40, 0, 40, 100, 0, 4100);
   hP2DADCARYBHists_cal = h2_book<TH2D>("F3 silicon ARYB 2D calibrated", "F3 silicon ARYB 2D plot calibrated", 40, 0, 40, 100, 0, 4100);

   first_Call_chk_aryb=1;

	char inputpedestalname[256];
	int line_ch;
	float line_ped;
	sprintf(inputpedestalname,  "~/online/data/pedestal-449.txt");
	ifstream myfile(inputpedestalname);
	if (myfile.is_open())
	{
		while (!myfile.eof())
 		   {
		     	myfile>>line_ch>>line_ped;	
			pedestal_aryb[line_ch] = line_ped; 
		    	//printf( "pedestal ch, ped= %d %f \n",line_ch, line_ped);
			printf( "pedestal ch, ped = %d %f\n",line_ch, pedestal_aryb[line_ch]);
		   }
	    myfile.close();
	printf("pedestal data open\n");
	}
	else printf("no pdestal txt");

	char inputgainname[256];
	float line_interc;
	float line_slope;
	//sprintf(inputgainname,  "/home/midas/online/data/gain-472.txt"); // from 2019/1/25 Gaussian fit
  	sprintf(inputgainname,  "/home/midas/online/data/gain-568.txt"); // from 2019/2/8 mean value
	//sprintf(inputgainname,  "/home/midas/online/data/gain-573.txt"); // from 2019/2/8 Gaussian fit
	ifstream myfileg(inputgainname);
	if (myfileg.is_open())
	{
		while (!myfileg.eof())
 		   {
		     	myfileg>>line_ch>>line_interc>>line_slope;	
			y_inter_indiv[line_ch] = line_interc; 
			slope_indiv[line_ch] = line_slope;
		    	//printf( "pedestal ch, ped= %d %f \n",line_ch, line_ped);
			//printf( "gain ch, y_inter, slope = %d %f %f\n",line_ch, y_inter_indiv[line_ch],slope_indiv[line_ch]);
		   }
	    myfileg.close();
	//printf("gain data open\n");
	}
	else printf("no gain txt");

*/
   return SUCCESS;
}

/*-- BOR routine ---------------------------------------------------*/

INT sili_aryb_BOR(INT run_number)
{
   return SUCCESS;
}

/*-- eor routine ---------------------------------------------------*/

INT sili_aryb_EOR(INT run_number)
{
	char pedestalname[256];
	sprintf(pedestalname,  "/home/midas/online/data/pedestal-%d.txt", run_number);

	ofstream myfile;
/*	myfile.open (pedestalname);

	float pedmean[N_ADC_f3_silicon];
	for (int i=0; i<N_ADC_f3_silicon; i++) {
	pedmean[i] = hADCCHists_raw[i]->GetMean();
	printf("Pedestal mean value =%d %f\n",i, pedmean[i]);
	myfile <<i <<" "<<pedmean[i] << " \n";
	}

	myfile.close();*/

	float mean_gd[N_ADC_aryb_silicon];
  	float sd_gd[N_ADC_aryb_silicon];
	float mean_gd_avg = 0.;
	float mean_am[N_ADC_aryb_silicon];
  	float sd_am[N_ADC_aryb_silicon];
	float mean_am_avg = 0.;

	float e_gd = 3.271;
	float e_am = 5.486;
	
	int channel_gd_min = 1500;
	int channel_gd_max = 2500;

	int channel_am_min = 3500;
	int channel_am_max = 4100;

    /*	TF1* f1 = new TF1("f1","gaus",channel_gd_min,channel_gd_max);
    	TF1* f2 = new TF1("f2","gaus",channel_am_min,channel_am_max);
	
	for (int i=15; i<N_ADC_f3_silicon; i++) {
    	hADCCARYBHists_ped[i]->Fit(f1,"R");
    	hADCCARYBHists_ped[i]->Fit(f2,"R+");

	//mean_gd[i] = hADCCHists_ped1[i]->GetMean();
	mean_gd[i] = f1->GetParameter(1);
	sd_gd[i] = f1->GetParameter(2);
	//mean_am[i] = hADCCHists_ped2[i]->GetMean();
	mean_am[i] = f2->GetParameter(1);
	sd_am[i] = f2->GetParameter(2);
	mean_gd_avg = mean_gd_avg+mean_gd[i];
	mean_am_avg = mean_am_avg+mean_am[i];
	}
	mean_gd_avg = mean_gd_avg/16;
	mean_am_avg = mean_am_avg/16;
	printf("Gd Am fit value =%f %f\n", mean_gd_avg, mean_am_avg);

	int n = 2;
	float x[2],y[2];
	x[0] = e_gd, x[1] = e_am;
	y[0] = mean_gd_avg, y[1] = mean_am_avg;
	TGraph* gr = new TGraph(n,x,y);
	//gr->Draw("ap");
	TF1* f3 = new TF1("f3","pol1",2.2,6.4);
	gr->Fit(f3);
	float y_inter, slope;
	y_inter = f3->GetParameter(0);
	slope = f3->GetParameter(1);

	float y_inter_indi[N_ADC_f3_silicon], slope_indi[N_ADC_f3_silicon];

	float yi0[2];	
	yi0[0] = mean_gd[0], yi0[1] = mean_am[0];
	TGraph* gr0 = new TGraph(n,x,yi0);
	gr0->Fit(f3);
	y_inter_indi[0] = f3->GetParameter(0);
	slope_indi[0] = f3->GetParameter(1);

	float yi1[2];	
	yi1[0] = mean_gd[1], yi1[1] = mean_am[1];
	TGraph* gr1 = new TGraph(n,x,yi1);
	gr1->Fit(f3);
	y_inter_indi[1] = f3->GetParameter(0);
	slope_indi[1] = f3->GetParameter(1);

	float yi2[2];	
	yi2[0] = mean_gd[2], yi2[1] = mean_am[2];
	TGraph* gr2 = new TGraph(n,x,yi2);
	gr2->Fit(f3);
	y_inter_indi[2] = f3->GetParameter(0);
	slope_indi[2] = f3->GetParameter(1);

	float yi3[2];	
	yi3[0] = mean_gd[3], yi3[1] = mean_am[3];
	TGraph* gr3 = new TGraph(n,x,yi3);
	gr3->Fit(f3);
	y_inter_indi[3] = f3->GetParameter(0);
	slope_indi[3] = f3->GetParameter(1);

	float yi4[2];	
	yi4[0] = mean_gd[4], yi4[1] = mean_am[4];
	TGraph* gr4 = new TGraph(n,x,yi4);
	gr4->Fit(f3);
	y_inter_indi[4] = f3->GetParameter(0);
	slope_indi[4] = f3->GetParameter(1);

	float yi5[2];	
	yi5[0] = mean_gd[5], yi5[1] = mean_am[5];
	TGraph* gr5 = new TGraph(n,x,yi5);
	gr5->Fit(f3);
	y_inter_indi[5] = f3->GetParameter(0);
	slope_indi[5] = f3->GetParameter(1);

	float yi6[2];	
	yi6[0] = mean_gd[6], yi6[1] = mean_am[6];
	TGraph* gr6 = new TGraph(n,x,yi6);
	gr6->Fit(f3);
	y_inter_indi[6] = f3->GetParameter(0);
	slope_indi[6] = f3->GetParameter(1);

	float yi7[2];	
	yi7[0] = mean_gd[7], yi7[1] = mean_am[7];
	TGraph* gr7 = new TGraph(n,x,yi7);
	gr7->Fit(f3);
	y_inter_indi[7] = f3->GetParameter(0);
	slope_indi[7] = f3->GetParameter(1);

	float yi8[2];	
	yi8[0] = mean_gd[8], yi8[1] = mean_am[8];
	TGraph* gr8 = new TGraph(n,x,yi8);
	gr8->Fit(f3);
	y_inter_indi[8] = f3->GetParameter(0);
	slope_indi[8] = f3->GetParameter(1);

	float yi9[2];	
	yi9[0] = mean_gd[9], yi9[1] = mean_am[9];
	TGraph* gr9 = new TGraph(n,x,yi9);
	gr9->Fit(f3);
	y_inter_indi[9] = f3->GetParameter(0);
	slope_indi[9] = f3->GetParameter(1);

	float yi10[2];	
	yi10[0] = mean_gd[10], yi10[1] = mean_am[10];
	TGraph* gr10 = new TGraph(n,x,yi10);
	gr10->Fit(f3);
	y_inter_indi[10] = f3->GetParameter(0);
	slope_indi[10] = f3->GetParameter(1);

	float yi11[2];	
	yi11[0] = mean_gd[11], yi11[1] = mean_am[11];
	TGraph* gr11 = new TGraph(n,x,yi11);
	gr11->Fit(f3);
	y_inter_indi[11] = f3->GetParameter(0);
	slope_indi[11] = f3->GetParameter(1);

	float yi12[2];	
	yi12[0] = mean_gd[12], yi12[1] = mean_am[12];
	TGraph* gr12 = new TGraph(n,x,yi12);
	gr12->Fit(f3);
	y_inter_indi[12] = f3->GetParameter(0);
	slope_indi[12] = f3->GetParameter(1);

	float yi13[2];	
	yi13[0] = mean_gd[13], yi13[1] = mean_am[13];
	TGraph* gr13 = new TGraph(n,x,yi13);
	gr13->Fit(f3);
	y_inter_indi[13] = f3->GetParameter(0);
	slope_indi[13] = f3->GetParameter(1);

	float yi14[2];	
	yi14[0] = mean_gd[14], yi14[1] = mean_am[14];
	TGraph* gr14 = new TGraph(n,x,yi14);
	gr14->Fit(f3);
	y_inter_indi[14] = f3->GetParameter(0);
	slope_indi[14] = f3->GetParameter(1);

	float yi15[2];	
	yi15[0] = mean_gd[15], yi15[1] = mean_am[15];
	TGraph* gr15 = new TGraph(n,x,yi15);
	gr15->Fit(f3);
	y_inter_indi[15] = f3->GetParameter(0);
	slope_indi[15] = f3->GetParameter(1);

	float yi16[2];	
	yi16[0] = mean_gd[16], yi16[1] = mean_am[16];
	TGraph* gr16 = new TGraph(n,x,yi16);
	gr16->Fit(f3);
	y_inter_indi[16] = f3->GetParameter(0);
	slope_indi[16] = f3->GetParameter(1);

	float yi17[2];	
	yi17[0] = mean_gd[17], yi17[1] = mean_am[17];
	TGraph* gr17 = new TGraph(n,x,yi17);
	gr17->Fit(f3);
	y_inter_indi[17] = f3->GetParameter(0);
	slope_indi[17] = f3->GetParameter(1);

	float yi18[2];	
	yi18[0] = mean_gd[18], yi18[1] = mean_am[18];
	TGraph* gr18 = new TGraph(n,x,yi18);
	gr18->Fit(f3);
	y_inter_indi[18] = f3->GetParameter(0);
	slope_indi[18] = f3->GetParameter(1);

	float yi19[2];	
	yi19[0] = mean_gd[19], yi19[1] = mean_am[19];
	TGraph* gr19 = new TGraph(n,x,yi19);
	gr19->Fit(f3);
	y_inter_indi[19] = f3->GetParameter(0);
	slope_indi[19] = f3->GetParameter(1);

	float yi20[2];	
	yi20[0] = mean_gd[20], yi20[1] = mean_am[20];
	TGraph* gr20 = new TGraph(n,x,yi20);
	gr20->Fit(f3);
	y_inter_indi[20] = f3->GetParameter(0);
	slope_indi[20] = f3->GetParameter(1);

	float yi21[2];	
	yi21[0] = mean_gd[21], yi21[1] = mean_am[21];
	TGraph* gr21 = new TGraph(n,x,yi21);
	gr21->Fit(f3);
	y_inter_indi[21] = f3->GetParameter(0);
	slope_indi[21] = f3->GetParameter(1);

	float yi22[2];	
	yi22[0] = mean_gd[22], yi22[1] = mean_am[22];
	TGraph* gr22 = new TGraph(n,x,yi22);
	gr22->Fit(f3);
	y_inter_indi[22] = f3->GetParameter(0);
	slope_indi[22] = f3->GetParameter(1);

	float yi23[2];	
	yi23[0] = mean_gd[23], yi23[1] = mean_am[23];
	TGraph* gr23 = new TGraph(n,x,yi23);
	gr23->Fit(f3);
	y_inter_indi[23] = f3->GetParameter(0);
	slope_indi[23] = f3->GetParameter(1);

	float yi24[2];	
	yi24[0] = mean_gd[24], yi24[1] = mean_am[24];
	TGraph* gr24 = new TGraph(n,x,yi24);
	gr24->Fit(f3);
	y_inter_indi[24] = f3->GetParameter(0);
	slope_indi[24] = f3->GetParameter(1);

	float yi25[2];	
	yi25[0] = mean_gd[25], yi25[1] = mean_am[25];
	TGraph* gr25 = new TGraph(n,x,yi25);
	gr25->Fit(f3);
	y_inter_indi[25] = f3->GetParameter(0);
	slope_indi[25] = f3->GetParameter(1);

	float yi26[2];	
	yi26[0] = mean_gd[26], yi26[1] = mean_am[26];
	TGraph* gr26 = new TGraph(n,x,yi26);
	gr26->Fit(f3);
	y_inter_indi[26] = f3->GetParameter(0);
	slope_indi[26] = f3->GetParameter(1);
	
	float yi27[2];	
	yi27[0] = mean_gd[27], yi27[1] = mean_am[27];
	TGraph* gr27 = new TGraph(n,x,yi27);
	gr27->Fit(f3);
	y_inter_indi[27] = f3->GetParameter(0);
	slope_indi[27] = f3->GetParameter(1);

	float yi28[2];	
	yi28[0] = mean_gd[28], yi28[1] = mean_am[28];
	TGraph* gr28 = new TGraph(n,x,yi28);
	gr28->Fit(f3);
	y_inter_indi[28] = f3->GetParameter(0);
	slope_indi[28] = f3->GetParameter(1);

	float yi29[2];	
	yi29[0] = mean_gd[29], yi29[1] = mean_am[29];
	TGraph* gr29 = new TGraph(n,x,yi29);
	gr29->Fit(f3);
	y_inter_indi[29] = f3->GetParameter(0);
	slope_indi[29] = f3->GetParameter(1);

	float yi30[2];	
	yi30[0] = mean_gd[30], yi30[1] = mean_am[30];
	TGraph* gr30 = new TGraph(n,x,yi30);
	gr30->Fit(f3);
	y_inter_indi[30] = f3->GetParameter(0);
	slope_indi[30] = f3->GetParameter(1);

	float yi31[2];	
	yi31[0] = mean_gd[31], yi31[1] = mean_am[31];
	TGraph* gr31 = new TGraph(n,x,yi31);
	gr31->Fit(f3);
	y_inter_indi[31] = f3->GetParameter(0);
	slope_indi[31] = f3->GetParameter(1);

	char gainname[256];*/
//	sprintf(gainname,  "/home/midas/online/data/gain-%d.txt", run_number);
/*
	myfile.open (gainname);

	for (int i=0; i<N_ADC_f3_silicon; i++) {
	printf("gain value =%d %f %f\n",i, y_inter_indi[i], slope_indi[i]);
	myfile <<i <<" "<<y_inter_indi[i] << " "<< slope_indi[i]<< " \n";
	}
	myfile <<N_ADC_f3_silicon <<" "<<y_inter << " "<< slope;
	myfile.close();
		*/
   return SUCCESS;
}

/*-- event routine -------------------------------------------------*/

INT sili_aryb_ADC(EVENT_HEADER *pheader, void *pevent)
{
 //  INT i;
/*   DWORD *pdata;
   DWORD *pdata_cal;
   int bk_size;
 //  int bk_size_tdc2;
 //  float *cadc;


   bk_size = bk_locate(pevent, BANK_NAME_F3SILICON, &pdata);
   if (!bk_size) {
      printf("no data\n");
      return 1;
   }
*/
/*
   bk_size_tdc2 = bk_locate(pevent, BANK_NAME_F2PPAC, &pdata_tdc2);
   if (!bk_size_tdc2) {
      printf("no data\n");
      return 1;
   }
*/
   /* create calibrated ADC bank */
//   bk_create(pevent, BANK_NAME_F3SILICON_CAL, TID_FLOAT, (void**)&pdata_cal);

   /* zero cadc bank */
//   for (i = 0; i < N_ADC_f3_silicon; i++)
//      cadc[i] = 0.f;
   /* subtract pedestal */
//   for (i = 0; i < N_ADC_f3_silicon; i++)
//      cadc[i] = (float) ((double) pdata[i] - 1.0 + 0.5);

   /* apply software gain calibration */
//   for (i = 0; i < N_ADC_f3_silicon; i++)
//      cadc[i] *= adccalib_param.software_gain[i];

   /* fill ADC histos if above threshold */
//   for (i = 0; i < N_ADC_f3_silicon; i++)
//      if (cadc[i] > (float) adccalib_param.histo_threshold)
//        hAdcHists[i]->Fill(cadc[i], 1);


//  bk_close(pevent, cadc + N_ADC_f3_silicon);
//  bk_close(pevent, cadc + N_ADC_f3_silicon);
/*   int32_t dump;
   int32_t event_count;
   int32_t channel;
   int32_t ADC_value[N_ADC_f3_silicon];

   int32_t event_count_tdc2;
   int32_t channel_tdc2;
   int32_t tdc_value[N_TDC_f2_PPAC];
   uint64_t global_time_stamp;
*/
//   memset(tdc_value, 0, sizeof(int32_t)*16);
/*   memset(ADC_value, 0, sizeof(int32_t)*N_ADC_f3_silicon);

   printf("F3SILICON ====================== %d\n", bk_size);

   if(first_Call_chk)
   {
	   for(int i=0; i<bk_size; i++){dump=*pdata++;}
	   first_Call_chk=0;
   }

   if(! first_Call_chk)
   {
	   for(int i=0; i<bk_size-1; )
	   {
		   channel=*pdata++; i++;
		   //printf("%d\n", channel);
		   if (channel>N_ADC_f3_silicon)
		   {
			//dump=channel;
			//printf("channel number from event_counter = %d\n", channel);
		    	channel=*pdata++; i++;
			//*pdata_cal++=channel;
		   }	
		   if (channel<N_ADC_f3_silicon)
		   {
		   ADC_value[channel]=*pdata++; i++;
			*pdata_cal++=channel;
			*pdata_cal++=ADC_value[channel];
		   //printf("channel:%d, ADC value:%d\n", channel, ADC_value[channel]);
		   }
	   }

	   event_count=*pdata++;
	   *pdata_cal++=event_count;
	   //printf("F3SILICON event_counter =======================%d\n", event_count);

	   char name[256];
           //bk_close(pevent, pdata_cal);
           //printf("F3SILICON cal might be closed =======================\n");
	    for(int i=0; i<N_ADC_f3_silicon; i++)
	     {
	     	 sprintf(name,  "F3SILICON-%03d", i);
	     	 if(ADC_value[i]>10) 
                    {
			hADCCHists_raw[i]->Fill(ADC_value[i],1);
			hADCCHists_ped[i]->Fill(ADC_value[i]-pedestal[i],1);
			if((ADC_value[i]-pedestal[i])<3000&&(ADC_value[i]-pedestal[i])>1000) hADCCHists_ped1[i]->Fill(ADC_value[i]-pedestal[i],1);
			if((ADC_value[i]-pedestal[i])<4100&&(ADC_value[i]-pedestal[i])>3000) hADCCHists_ped2[i]->Fill(ADC_value[i]-pedestal[i],1);
			hADCCHists_cal[i]->Fill((ADC_value[i]-pedestal[i]-y_inter_indiv[i])/slope_indiv[i]*slope_indiv[N_ADC_f3_silicon]+y_inter_indiv[N_ADC_f3_silicon],1);
 	                hP2DADCHists_raw->Fill(i,ADC_value[i], 1);
			hP2DADCHists_ped->Fill(i,ADC_value[i]-pedestal[i], 1);
			hP2DADCHists_cal->Fill(i,(ADC_value[i]-pedestal[i]-y_inter_indiv[i])/slope_indiv[i]*slope_indiv[N_ADC_f3_silicon]+y_inter_indiv[N_ADC_f3_silicon], 1);
			hSUMADCHists_raw->Fill(ADC_value[i],1);
		     }
             //printf("F3SILICON ch23 =======================%d, %d\n", i, ADC_value[i]);
	     }
             //printf("F3SILICON ch23 =======================%d\n", ADC_value[19]);
   }*/
   /* close calculated bank */
//   bk_close(pevent, pdata_cal);
//   bk_close(pevent, cadc + N_ADC_f3_silicon);
/*
//printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
   for(int i=0; i<1; i++){event_count_tdc2 = *pdata_tdc2++;}

   for (int i=0; i<bk_size_tdc2-3; )
   {
	   channel_tdc2=*pdata_tdc2++; i++;
	   tdc_value[channel_tdc2]=*pdata_tdc2++; i++;
   }

   for(int i=0; i<2; )
   {
	   uint32_t time2=*pdata_tdc2++; i++;
	   uint32_t time1=*pdata_tdc2++; i++;
	   global_time_stamp=time2|time1;
	 //  printf("\n\n%12f, %12f, %12f\n", time2*800e-9, time1*800e-9, global_time_stamp*800e-9);
   }

#if 1
   printf("F2PPAC_for_silicon_correlation ===============================%d\n", bk_size_tdc2);
   	 printf("event_count: %d\n",event_count_tdc2);
   	 for(int i=0; i<N_TDC_f2_PPAC; i++){
	 if(tdc_value[i]*0.025>0)printf("channel:%d, measure:%8f\n", i, tdc_value[i]*0.025);
         }
   	 printf("global time: %12f\n", global_time_stamp*800e-9);
#endif
*/

   return SUCCESS;

}

