#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <fstream>
#include <map>
#include <string.h>


#include "TMidasFile.h"
#include "TMidasEvent.h"

#include "TFile.h"
#include "TTree.h"

using namespace std;

int main(int argc, char** argv)
{
	char outputfilename[512];
	char outputfilename2[512];
	sprintf(outputfilename,"output%s.root",argv[argc-1]);
	sprintf(outputfilename2,"output%s.dat",argv[argc-1]);
	TFile *outfile;
	// define output root file
	outfile = new TFile(outputfilename,"RECREATE");

        ofstream outfile2;
        outfile2.open (outputfilename2);

	// The tree to fill.
	TTree *fTree;
	int eventid1;
	int eventid2;
	int eventid3;
	int eventid7;
	int ary1_eventid;
	int ary2_eventid;
	//uint32_t ary1_scalerlower;
	//uint32_t ary1_scalerupper;
	//uint32_t ary2_scalerlower;
	//uint32_t ary2_scalerupper;
	//uint32_t bs_scalerlower;
	//uint32_t bs_scalerupper;	
	//uint32_t tdc1_scalerlower;
	//uint32_t tdc1_scalerupper;
	//uint32_t tdc2_scalerlower;
	//uint32_t tdc2_scalerupper;

	uint32_t v2495_1scalerlower;
	uint32_t v2495_1scalerupper;
        uint32_t v2495_2scalerlower;
	uint32_t v2495_2scalerupper;
	uint32_t v2495_3scalerlower;
	uint32_t v2495_3scalerupper;

	uint64_t gCOUNTt1; //global count from scaler
	uint64_t gCOUNTt2;
	uint64_t gCOUNTt3;
	uint64_t gCOUNTt7;
	uint64_t gCOUNTa3;
	uint64_t gCOUNTar1;
	uint64_t gCOUNTar2;
	uint64_t gCOUNTscaler;
	int geo;
	int channel1[64]={-999};
	int tdc_value1[64]={-999};
	int ch_fired1;
	int channel2[64]={-999};
	int tdc_value2[64]={-999};
	int ch_fired2;    
	int channel7[64]={-999};
	int tdc_value7[64]={-999};
	int ch_fired7; 

	int channel3[64]={-999};    
	int adc_value3[1500]={-999};
	int adc_channel3[1500]={-999};
	int ary1_adc[1500]={-999};
	int ary1_channel[1500]={-999};
	int channel6[64]={-999};    
	int ary2_adc[1500]={-999};
	int ary2_channel[1500]={-999};


	// Create a TTree
	fTree = new TTree("midas_data","MIDAS data");
	//--------------------TDC1--------------------//
	fTree->Branch("eventid1",&eventid1,"eventid1/I");
		fTree->Branch("gCOUNTt1",&gCOUNTt1,"gCOUNTt1/i");
	fTree->Branch("geo",&geo,"geo/I");
	fTree->Branch("channel1",channel1,"channel1[64]/I");
	fTree->Branch("tdc_value1",tdc_value1,"tdc_value1[64]/I");
	fTree->Branch("ch_fired1",&ch_fired1,"ch_fired1/I");
	//fTree->Branch("tdc1_scalerlower",&tdc1_scalerlower,"tdc1_scalerlower/i");
	//fTree->Branch("tdc1_scalerupper",&tdc1_scalerupper,"tdc1_scalerupper/i");

	//--------------------TDC2--------------------//
	fTree->Branch("eventid2",&eventid2,"eventid2/I");
	fTree->Branch("gCOUNTt2",&gCOUNTt2,"gCOUNTt2/i");
	fTree->Branch("channel2",channel2,"channel2[64]/I");
	fTree->Branch("tdc_value2",tdc_value2,"tdc_value2[64]/I");
	fTree->Branch("ch_fired2",&ch_fired2,"ch_fired2/I");
	//fTree->Branch("tdc2_scalerlower",&tdc2_scalerlower,"tdc2_scalerlower/i");
	//fTree->Branch("tdc2_scalerupper",&tdc2_scalerupper,"tdc2_scalerupper/i");

	//--------------------ADC1--------------------//
	fTree->Branch("eventid3",&eventid3,"eventid3/I");
	fTree->Branch("gCOUNTa3",&gCOUNTa3,"gCOUNTa3/i");
	fTree->Branch("adc_value3",adc_value3,"adc_value3[1500]/I");
	fTree->Branch("adc_channel3",adc_channel3,"adc_channel3[1500]/I");
	//fTree->Branch("bs_scalerlower",&bs_scalerlower,"bs_scalerlower/i");
	//fTree->Branch("bs_scalerupper",&bs_scalerupper,"bs_scalerupper/i");

	//--------------------TDC3--------------------//
	fTree->Branch("eventid7",&eventid7,"eventid7/I");
	fTree->Branch("gCOUNTt7",&gCOUNTt7,"gCOUNTt7/i");
	fTree->Branch("channel7",channel7,"channel7[64]/I");
	fTree->Branch("tdc_value7",tdc_value7,"tdc_value7[64]/I");
	fTree->Branch("ch_fired7",&ch_fired7,"ch_fired7/I");

	//--------------------ary ADC1--------------------//
	fTree->Branch("ary1_eventid",&ary1_eventid,"ary1_eventid/I");	
	fTree->Branch("gCOUNTar1",&gCOUNTar1,"gCOUNTar1/i");
	fTree->Branch("ary1_channel",ary1_channel,"ary1_channel[1500]/I");
	fTree->Branch("ary1_adc",ary1_adc,"ary1_adc[1500]/I");
//	fTree->Branch("ary1_scalerlower",&ary1_scalerlower,"ary1_scalerlower/i");
//	fTree->Branch("ary1_scalerupper",&ary1_scalerupper,"ary1_scalerupper/i");

	//--------------------ary ADC2--------------------//
	fTree->Branch("ary2_eventid",&ary2_eventid,"ary2_eventid/I");	
	fTree->Branch("gCOUNTar2",&gCOUNTar2,"gCOUNTar2/i");
	fTree->Branch("ary2_channel",ary2_channel,"ary2_channel[1500]/I");
	fTree->Branch("ary2_adc",ary2_adc,"ary2_adc[1500]/I");
//	fTree->Branch("ary2_scalerlower",&ary2_scalerlower,"ary2_scalerlower/i");
//	fTree->Branch("ary2_scalerupper",&ary2_scalerupper,"ary2_scalerupper/i");

	//---------------------FPGA-----------------------//
	fTree->Branch("gCOUNTscaler",&gCOUNTscaler,"gCOUNTscaler/i");
	fTree->Branch("v2495_1scalerupper",&v2495_1scalerupper,"v2495_1scalerupper/I");
	fTree->Branch("v2495_2scalerlower",&v2495_2scalerlower,"v2495_2scalerlower/I");
	fTree->Branch("v2495_2scalerupper",&v2495_2scalerupper,"v2495_2scalerupper/I");
	fTree->Branch("v2495_3scalerlower",&v2495_3scalerlower,"v2495_3scalerlower/I");
	fTree->Branch("v2495_3scalerupper",&v2495_3scalerupper,"v2495_3scalerupper/I");



	char path_file[512];
	const char* fBank1="F1PA";
	int bankLength1 = 0;
	int bankType1 =0;
	void *pdata1=0;
	const char* fBank2="F2PA";
	int bankLength2 = 0;
	int bankType2 =0;
	void *pdata2=0;
	const char* fBank3="F3SI";
	int bankLength3 = 0;
	int bankType3 =0;
	void *pdata3=0;
	const char* fBank7="F3PA";
	int bankLength7 = 0;
	int bankType7 =0;
	void *pdata7=0;
	
	const char* fBank4="F249";
	int bankLength4 = 0;
	int bankType4 =0;
	void *pdata4=0;
	const char* fBank5="U1SI";//"F3ARYA";
	int bankLength5 = 0;
	int bankType5 =0;
	void *pdata5=0;
	const char* fBank6="U2SI";
	int bankLength6 = 0;
	int bankType6 =0;
	void *pdata6=0;

	if(argc<2){printf("type run number!\n"); exit(-1);}
	sprintf(path_file, "/home/kobradaq/online/data/run%s.mid.lz4", argv[argc-1]);
	TMidasFile fp;

	if(! fp.Open(path_file)){
		printf("Invalid file name run%s.mid.lz4\n", argv[argc-1]);
		exit(-1);
	}
/*
	int N_ADC_f3_silicon = 32;
	float pedestal[N_ADC_f3_silicon];
	float y_inter_indiv[N_ADC_f3_silicon+1], slope_indiv[N_ADC_f3_silicon+1];
	char inputpedestalname[256];
	int line_ch;
	float line_ped;
	sprintf(inputpedestalname,  "/home/kobradaq/online/data/pedestal-449.txt");
	ifstream myfile(inputpedestalname);
	if (myfile.is_open())
	{
		while (!myfile.eof())
 		   {
		     	myfile>>line_ch>>line_ped;	
			pedestal[line_ch] = line_ped; 
		    	//printf( "pedestal ch, ped= %d %f \n",line_ch, line_ped);
			printf( "pedestal ch, ped = %d %f\n",line_ch, pedestal[line_ch]);
		   }
	    myfile.close();
	printf("pedestal data open\n");
	}
	else printf("no pdestal txt");

	char inputgainname[256];
	float line_interc;
	float line_slope;
	sprintf(inputgainname,  "/home/kobradaq/online/data/gain-472.txt");
*/
/*	ifstream myfileg(inputgainname);
	if (myfileg.is_open())
	{
		while (!myfileg.eof())
 		   {
		     	myfileg>>line_ch>>line_interc>>line_slope;	
			y_inter_indiv[line_ch] = line_interc; 
			slope_indiv[line_ch] = line_slope;
		    	//printf( "pedestal ch, ped= %d %f \n",line_ch, line_ped);
			printf( "gain ch, y_inter, slope = %d %f %f\n",line_ch, y_inter_indiv[line_ch],slope_indiv[line_ch]);
		   }
	    myfileg.close();
	printf("gain data open\n");
	}
	else printf("no gain txt");
*/
	uint32_t skipped1 = 0;
	uint32_t skipped2 = 0;
	uint32_t skippedsil = 0;
	uint32_t data1[1500]={0};
	uint32_t data2[1500]={0};
	uint32_t data3[1500]={0};
	uint32_t data4[1500]={0};
	uint32_t data5[1500]={0};
	uint32_t data6[1500]={0};
	uint32_t data7[1500]={0};
	int tmp_eventid1=-1;
	int tmp_eventid2=-1;
	int tmp_eventid3=-1;
	int tmp_eventid7=-1;
	int tmp_ary1_eventid=-1;
	int tmp_ary2_eventid=-1;
	int ch_du[1500] = {0};
        int ch_du2[1500] = {0};
        int ch_du7[1500] = {0};

while(1){

	TMidasEvent event;

		if(!fp.Read(&event)) break;
		uint16_t eventID=event.GetEventId();
	printf("Check eventID1....\n");
	if(eventID ==1){
			event.SetBankList();
			if(event.FindBank(fBank4, &bankLength4, &bankType4, &pdata4))
			{
				printf("F2495 entries======================================%i \n",bankLength4);
				for(int i=0; i<bankLength4; i++)
				{
					data4[i]=((uint32_t*)pdata4)[i];
					if (i==bankLength4-1)
					{					
						gCOUNTscaler = data4[bankLength4-1];
						printf("F2495;              GCOUNT:                   %lu \n",gCOUNTscaler);
					}
				}


			}
			else{printf("Bank name4 %s is not listed\n", fBank4);}

			if(event.FindBank(fBank1, &bankLength1, &bankType1, &pdata1))
			{
				printf("F1TDC entries======================================%i \n",bankLength1);
				for (int i=0; i<32; i++)//reset
				{
					tdc_value1[i] = -999;
				}
				for (int i=0; i<32; i++)
				{
				ch_du[i] = 0;
				}	
				for(int i=0; i<bankLength1; i++)
				{
					data1[i]=((uint32_t*)pdata1)[i];	
					if(i==0)
					{
						eventid1 = data1[0];
						//printf("tdc1 evnt id === %d \n", data1[0]);
					}					
						//geo = data[1];
						//ch_fired1 = data[bankLength1-1]-7;
						//uint32_t time_tag = data[bankLength1-2];
						//uint32_t global_time = data[bankLength1-1];
					if (i>1&&i%2==0&&i<bankLength1-1)//find channel and tdc value
					{
						//printf("=== %d, %d \n", data1[i-3], data1[i-2]);
						//channel1[(i-4)/2] = data1[i-3];
  						//int tmp_ch = channel1[(i-4)/2];
						//tdc_value1[tmp_ch] = data1[i]; 
						//printf("=== %d, %d \n", data1[i-1], data1[i]);
						channel1[(i-2)/2] = data1[i-1];
						int tmp_ch;
  						if(channel1[(i-2)/2]<33) {tmp_ch = channel1[(i-2)/2]; ch_du[data1[i-1]]++;}//increase ch_du for every channel when the channel has event
						if(ch_du[data1[i-1]]>1)//printf("multi hit channel=== %d \n", data1[i-1]);
						if(channel1[(i-2)/2]>33) tmp_ch = -999;
						if(ch_du[data1[i-1]]==1) {
                                          	tdc_value1[tmp_ch] = data1[i];
                                          	//outfile2 << tmp_ch << "\t" << data1[i] << "\t";
					  	//printf("tdc 1 === %d, %d \n", tmp_ch, data1[i]);
                                         	}
					}
					else if (i == bankLength1-1) 
					{
						gCOUNTt1 = data1[bankLength1-1];
						printf("F1TDC; event_count; GCOUNT; clock count: %d, %u\n",eventid1, gCOUNTt1);
					}
				}
				
				if(tmp_eventid1==eventid1)//check if event id is duplicated
				{
				//printf("======================================Skipped event1 %d \n", skipped1);
				skipped1++;
				}
				//if(eventid1%100==0)printf("Events analyzed1 === %d \n", eventid1);
				
			}
			else{printf("Bank name1 %s is not listed\n", fBank1);}

			if(event.FindBank(fBank2, &bankLength2, &bankType2, &pdata2))
			{
				printf("F2TDC entries======================================%i \n",bankLength2);
				for (int i=0; i<16; i++)//reset
				{
					tdc_value2[i] = -999;
				}
				for (int i=0; i<16; i++)
				{
				ch_du2[i] = 0;
				}
				for(int i=0; i<bankLength2; i++)
				{
					data2[i]=((uint32_t*)pdata2)[i];	
					if(i==0)
					{					
						eventid2 = data2[0];
						//printf("tdc2 evnt id === %d \n", data2[0]);
					}
						//geo = data[1];
						//ch_fired1 = data[bankLength1-1]-7;
						//uint32_t time_tag = data[bankLength1-2];
						//uint32_t global_time = data[bankLength1-1];
					if (i>1&&i%2==0&&i<bankLength2-1)//find channel and tdc value
					{ 
						//printf("=== %d, %d \n", data2[i-1], data2[i]);
						channel2[(i-2)/2] = data2[i-1];

						int tmp_ch;
  						if(channel2[(i-2)/2]<17) {tmp_ch = channel2[(i-2)/2]; ch_du2[data2[i-1]]++;}
						if(ch_du2[data2[i-1]]>1)//printf("multi hit channel=== %d \n", data2[i-1]);
						if(channel2[(i-2)/2]>17) tmp_ch = -999;
						if(ch_du2[data2[i-1]]==1) {
                                        	  tdc_value2[tmp_ch] = data2[i];
                                         	//outfile2 << tmp_ch << "\t" << data1[i] << "\t";
					  	//printf("tdc 3 === %d, %d \n", tmp_ch, data2[i]);
                                         	 }
					}
					else if (i == bankLength2-1) 
					{	
						gCOUNTt2 = data2[bankLength2-1];
						printf("F2TDC; event_count; GCOUNT; clock count: %d, %u\n",eventid2,gCOUNTt2);
					}
				}
				if(tmp_eventid2==eventid2)//check if event id is duplicated
				{
				//printf("======================================Skipped event2 %d \n", skipped2);
				skipped2++;
				}
			}
			else{printf("Bank name2 %s is not listed\n", fBank2);}
			
			if(event.FindBank(fBank3, &bankLength3, &bankType3, &pdata3))
			{
				printf("F3ADC entries======================================%i \n",bankLength3);
				for(int i=0; i<bankLength3; i++)
				{
					data3[i]=((uint32_t*)pdata3)[i];	
					//geo = data[1];
					//ch_fired1 = data[bankLength1-1]-7;
					//uint32_t time_tag = data[bankLength1-2];
					//uint32_t global_time = data[bankLength1-1];
					if (i<bankLength3-2)//find channel and tdc value
					{
						if((i+1)%2==0)
						{
						//printf("beam line silicon === ch %d, adc value %d \n", data3[i-1], data3[i]);
						int tmp_ch;
  						if(data3[i-1]<32) tmp_ch = data3[i-1];
						if(data3[i-1]>31) {tmp_ch = 32; printf("silicon === false channel \n");}
						adc_value3[(i+1)/2-1] = data3[i];
						//adc_value[(i+1)/2-1] = (data3[i]-pedestal[data3[i-1]]-y_inter_indiv[data3[i-1]]) slope_indiv[data3[i-1]]*slope_indiv[32]+y_inter_indiv[32];
						//printf("silicon === corrected adc value %d slope %f\n", adc_value[(i+1)/2-1],slope_indiv[32]);
						adc_channel3[(i+1)/2-1] = data3[i-1];
						//printf("beam line silicon ch === %d adc value %d \n", adc_channel3[(i+1)/2-1], adc_value3[(i+1)/2-1]);
						}
					}
					else if (i == bankLength3-2)
					{
						eventid3 = data3[bankLength3-2];
						gCOUNTa3 = data3[bankLength3-1];
						printf("F3ADC; event_count; GCOUNT; clock count: %d, %u\n",eventid3,gCOUNTa3);
						//printf("gCOUNTa3 = %u\n",gCOUNTa3);
					}
				}
				if(tmp_eventid3==eventid3)//check if event id is duplicated
				{
				//printf("======================================Skipped silicon event %d \n", skippedsil);
				skippedsil++;
				}
			}
			else{printf("Bank name3 %s is not listed\n", fBank3);}

			if(event.FindBank(fBank7, &bankLength7, &bankType7, &pdata7))
			{
				printf("F3TDC entries======================================%i \n",bankLength7);
				for (int i=0; i<32; i++)//reset
				{
					tdc_value7[i] = -999;
				}
				for (int i=0; i<32; i++)
				{
				ch_du7[i] = 0;
				}	
				for(int i=0; i<bankLength7; i++)
				{
					data7[i]=((uint32_t*)pdata7)[i];	
					if(i==0)
					{
						eventid7 = data7[0];
						//printf("tdc1 evnt id === %d \n", data1[0]);
					}					
						//geo = data[1];
						//ch_fired1 = data[bankLength1-1]-7;
						//uint32_t time_tag = data[bankLength1-2];
						//uint32_t global_time = data[bankLength1-1];
					if (i>1&&i%2==0&&i<bankLength7-1)//find channel and tdc value
					{
						//printf("=== %d, %d \n", data1[i-3], data1[i-2]);
						//channel1[(i-4)/2] = data1[i-3];
  						//int tmp_ch = channel1[(i-4)/2];
						//tdc_value1[tmp_ch] = data1[i]; 
						//printf("=== %d, %d \n", data1[i-1], data1[i]);
						channel7[(i-2)/2] = data7[i-1];
						int tmp_ch;
  						if(channel7[(i-2)/2]<33) {tmp_ch = channel7[(i-2)/2]; ch_du7[data7[i-1]]++;}//increase ch_du7 for every channel when the channel has event
						if(ch_du7[data7[i-1]]>1)//printf("multi hit channel=== %d \n", data7[i-1]);
						if(channel7[(i-2)/2]>33) tmp_ch = -999;
						if(ch_du7[data7[i-1]]==1) {
                                          	tdc_value7[tmp_ch] = data7[i];
                                          	//outfile2 << tmp_ch << "\t" << data1[i] << "\t";
					  	//printf("tdc 1 === %d, %d \n", tmp_ch, data1[i]);
                                         	}
					}
					else if (i == bankLength7-1) 
					{
						gCOUNTt7 = data7[bankLength7-1];
						printf("F3TDC; event_count; GCOUNT; clock count: %d, %u\n",eventid7, gCOUNTt7);
					}
				}
				
				if(tmp_eventid7==eventid7)//check if event id is duplicated
				{
				//printf("======================================Skipped event1 %d \n", skipped1);
				skipped1++;
				}
				//if(eventid1%100==0)printf("Events analyzed1 === %d \n", eventid1);
				
			}
			else{printf("Bank name7 %s is not listed\n", fBank7);}	
		}


	printf("Check eventID2....\n");
	if(eventID ==2){
			event.SetBankList();
			if(event.FindBank(fBank5, &bankLength5, &bankType5, &pdata5))
			{
				printf("AADC1 entries======================================%i \n",bankLength5);
				for(int i=0; i<bankLength5; i++)
				{
					data5[i]=((uint32_t*)pdata5)[i];	
					if (i<bankLength5-2)//find channel and tdc value
					{
						if((i+1)%2==0)
						{
						//printf("silicon ary === ch %d, adc value %d \n", data5[i-1], data5[i]);
						int tmp_ch;
  						if(data5[i-1]<32&&i<64) tmp_ch = data5[i-1];
						if(data5[i-1]>31) {tmp_ch = 32; }//printf("silicon ary a=== false channel \n");}

						if(i<64)ary1_adc[(i+1)/2-1] = data5[i];
						if(data5[i-1]<32&&i<64) ary1_channel[(i+1)/2-1] = data5[i-1];
						//printf("silicon ary ch = %d adc value = %d \n", ary1_channel[(i+1)/2-1], ary1_adc[(i+1)/2-1]);
						}
					}
					else if (i == bankLength5-2) 
					{
						ary1_eventid = data5[bankLength5-2];
						gCOUNTar1 = data5[bankLength5-1];
						//ary1_scalerlower = data5[bankLength5-2];
						//ary1_scalerupper = data5[bankLength5-1];
						
						printf("AADC1; event_count; GCOUNT; clock count: %d, %u\n",ary1_eventid,gCOUNTar1);
						//printf("ary 1 event id = %d\n",ary1_eventid);
						//printf("gCOUNTar1 = %d\n",gCOUNTar1);
						//printf("ary 1 scaler lower = %u\n",ary1_scalerlower);
						//printf("ary 1 scaler upper = %u\n",ary1_scalerupper);
					}
				}
				if(tmp_ary1_eventid==ary1_eventid)//check if event id is duplicated
				{
				//printf("======================================Skipped silicon event %d \n", skippedsil);
				skippedsil++;
				}
				/*
				if(tmp_ary1_eventid!=ary1_eventid)//to skip dulicated event number
				{
					//fTree->Fill();
					//tmp_ary1_eventid = ary1_eventid;
					//printf("silicon event id === %d \n",ary1_eventid );
				}
				for (int i=0; i<36; i++)//reset
				{
					//ary1_channel[i] = -999;
					//ary1_adc[i] = -999;
				}*/
			}
			else{printf("Bank name5 %s is not listed\n", fBank5);}

			if(event.FindBank(fBank6, &bankLength6, &bankType6, &pdata6))
			{
				printf("AADC2 entries======================================%i \n",bankLength6);
				for(int i=0; i<bankLength6; i++)
				{
					data6[i]=((uint32_t*)pdata6)[i];	
					if (i<bankLength6-2)//find channel and tdc value
					{
						if((i+1)%2==0)
						{
						//printf("silicon ary 2 === ch %d, adc value %d \n", data6[i-1], data6[i]);
						int tmp_ch;
  						if(data6[i-1]<32&&i<64) tmp_ch = data6[i-1];
						if(data6[i-1]>31) {tmp_ch = 32;} //printf("silicon ary b=== false channel \n");}

						if(i<64)ary2_adc[(i+1)/2-1] = data6[i];
						if(data6[i-1]<32&&i<64) ary2_channel[(i+1)/2-1] = data6[i-1];
						//printf("silicon ary 2 ch = %d adc value 2 = %d \n", ary2_channel[(i+1)/2-1], ary2_adc[(i+1)/2-1]);
						}
					}
					else if (i == bankLength6-2) 
					{
						ary2_eventid = data6[bankLength6-2];
						gCOUNTar2 = data6[bankLength6-1];
						//ary2_scalerlower = data6[bankLength6-2];
						//ary2_scalerupper = data6[bankLength6-1];
						printf("AADC2; event_count; GCOUNT; clock count: %d, %u\n",ary2_eventid,gCOUNTar2);
						//printf("ary 2 event id2 = %d\n",ary2_eventid);
						//printf("gCOUNTar2 = %d\n",gCOUNTar2);
						//printf("ary 2 scaler lower = %u\n",ary2_scalerlower);
						//printf("ary 2 scaler upper = %u\n",ary2_scalerupper);
					}
				}
				if(tmp_ary2_eventid==ary2_eventid)//check if event id is duplicated
				{
				//printf("======================================Skipped ary 2 silicon event %d \n", skippedsil);
				skippedsil++;
				}/*
				if(tmp_ary2_eventid!=ary2_eventid)//to skip dulicated event number
				{
					//fTree->Fill();
					//tmp_ary2_eventid = ary2_eventid;
					//printf("silicon event id === %d \n",ary2_eventid);
				}
				for (int i=0; i<36; i++)//reset
				{
					ary2_channel[i] = -999;
					ary2_adc[i] = -999;
				}*/
			}
			else{printf("Bank name6 %s is not listed\n", fBank6);}
		}
		if(tmp_eventid1!=eventid1&&tmp_eventid2!=eventid2&&tmp_eventid3!=eventid3&&tmp_ary1_eventid==ary1_eventid&&tmp_ary2_eventid==ary2_eventid)				
		{
			fTree->Fill();
			tmp_eventid1 = eventid1;
			tmp_eventid2 = eventid2;
			tmp_eventid3 = eventid3;
			tmp_ary1_eventid = ary1_eventid;
			tmp_ary2_eventid = ary2_eventid;
			//printf("silicon event id === %d \n",eventid3 );
				
				//if(eventid3%100==0)printf("Events analyzed silicon === %d \n", eventid3);
			for(int i=0; i<32; i++)
			{
                                outfile2 << tdc_value1[i] << "\t";
				tdc_value1[i] = -999;
			}
			for(int i=0; i<16; i++)
			{
                                outfile2 << tdc_value2[i] << "\t";
				tdc_value2[i] = -999;
			}
			for (int i=0; i<36; i++)//reset
			{
				adc_channel3[i] = -999;
				adc_value3[i] = -999;
			}
			for (int i=0; i<36; i++)//reset
			{
				ary1_channel[i] = -999;
				ary1_adc[i] = -999;
			}
			for (int i=0; i<36; i++)//reset
			{
				ary2_channel[i] = -999;
				ary2_adc[i] = -999;
			}
		}
	}

	printf("======================================TTotal skipped silicon event %d \n", skippedsil);
	fp.Close();
	outfile->Write();
	return 0;
}
