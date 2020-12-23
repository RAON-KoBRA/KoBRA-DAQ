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
	int geo;
	int channel1[64]={-999};
	int tdc_value1[64]={0};
	int ch_fired1;
	int channel2[64]={-999};
	int tdc_value2[64]={-999};
	int ch_fired2;    
	int channel3[64]={-999};    
	int adc_value[33]={-999};
	int adc_channel[33]={-999};

	// Create a TTree
	fTree = new TTree("midas_data","MIDAS data");
	fTree->Branch("eventid1",&eventid1,"eventid1/I");
	fTree->Branch("geo",&geo,"geo/I");
	fTree->Branch("channel1",channel1,"channel1[32]/I");
	fTree->Branch("tdc_value1",tdc_value1,"tdc_value1[32]/I");
	fTree->Branch("ch_fired1",&ch_fired1,"ch_fired1/I");
	fTree->Branch("channel2",channel2,"channel2[32]/I");
	fTree->Branch("tdc_value2",tdc_value2,"tdc_value2[32]/I");
	fTree->Branch("ch_fired2",&ch_fired2,"ch_fired2/I");
	fTree->Branch("adc_value",adc_value,"adc_value[33]/I");
	fTree->Branch("adc_channel",adc_channel,"adc_channel[33]/I");
	fTree->Branch("eventid3",&eventid3,"eventid3/I");	

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

	if(argc<2){printf("type run number!\n"); exit(-1);}
	sprintf(path_file, "/home/daqcontrol/online/data/run%s.mid", argv[argc-1]);

	TMidasFile fp;

	if(! fp.Open(path_file)){
		printf("Invalid file name run%s.mid\n", argv[argc-1]);
		exit(-1);
	}

	int N_ADC_f3_silicon = 32;
	float pedestal[N_ADC_f3_silicon];
	float y_inter_indiv[N_ADC_f3_silicon+1], slope_indiv[N_ADC_f3_silicon+1];
	char inputpedestalname[256];
	int line_ch;
	float line_ped;
	sprintf(inputpedestalname,  "/home/daqcontrol/online/data/pedestal-449.txt");
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
	sprintf(inputgainname,  "/home/daqcontrol/online/data/gain-472.txt");

	ifstream myfileg(inputgainname);
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

	uint32_t skipped1 = 0;
//	uint32_t skipped2 = 0;
	uint32_t skippedsil = 0;
	uint32_t data1[64]={0};
//	uint32_t data2[64]={0};
	uint32_t data3[64]={0};
	int tmp_eventid1=-1;
//	int tmp_eventid2=-1;
	int tmp_eventid3=-1;
	int ch_du[32] = {0};
	while(1){

	TMidasEvent event;

		if(!fp.Read(&event)) break;
		uint16_t eventID=event.GetEventId();

		if(eventID ==1){
			event.SetBankList();
//			if(event.FindBank(fBank, &bankLength, &bankType, &pdata) && bankLength==7)
			if(event.FindBank(fBank2, &bankLength1, &bankType1, &pdata1))
			{
				//printf("======================================%i \n",bankLength1);
			//if(bankLength1>6)
			  // {
				for (int i=0; i<32; i++)
				{
				ch_du[i] = 0;
				}	
				for(int i=0; i<bankLength1; i++)
				{
					data1[i]=((uint32_t*)pdata1)[i];	
					eventid1 = data1[0];
					//geo = data[1];
					//ch_fired1 = data[bankLength1-1]-7;
					//uint32_t time_tag = data[bankLength1-2];
					//uint32_t global_time = data[bankLength1-1];
					if (i>1&&i%2==0&&i<bankLength1-2)//find channel and tdc value
					{
					//printf("=== %d, %d \n", data1[i-3], data1[i-2]);
					//channel1[(i-4)/2] = data1[i-3];
  					//int tmp_ch = channel1[(i-4)/2];
					//tdc_value1[tmp_ch] = data1[i]; 
					//printf("=== %d, %d \n", data1[i-1], data1[i]);
					channel1[(i-2)/2] = data1[i-1];
					int tmp_ch;
  					if(channel1[(i-2)/2]<33) {tmp_ch = channel1[(i-2)/2]; ch_du[data1[i-1]]++;}
					if(ch_du[data1[i-1]]>1)printf("multi hit channel=== %d \n", data1[i-1]);
					if(channel1[(i-2)/2]>33) tmp_ch = -999;
					if(ch_du[data1[i-1]]==1) {
                                          tdc_value1[tmp_ch] = data1[i];
//                                          outfile2 << tmp_ch << "\t" << data1[i] << "\t";
                                          }
					}
				}
				
				if(tmp_eventid1==eventid1)//check if event id is duplicated
				{
				printf("======================================Skipped event1 %d \n", skipped1);
				skipped1++;
				}
				if(tmp_eventid1!=eventid1)//to skip dulicated event number				
				{
					fTree->Fill();
					tmp_eventid1 = eventid1;
					for(int i=0; i<32; i++)
						{
                                                outfile2 << tdc_value1[i] << "\t";
						tdc_value1[i] = 0;
						}
                                        outfile2 << "\n";
				}
				//if(eventid1%100==0)printf("Events analyzed1 === %d \n", eventid1);
				/*for (int i=0; i<bankLength1; i++)//reset
				{
					channel1[i] = -999;
					tdc_value1[i] = -999;
				}*/
			      //}
			}
			else{printf("Bank name1 %s is not listed\n", fBank1);}

			/*if(event.FindBank(fBank2, &bankLength2, &bankType2, &pdata2))
			{
				//printf("======================================%i \n",bankLength1);
				for(int i=0; i<bankLength2; i++)
				{
					data2[i]=((uint32_t*)pdata2)[i];	
					eventid2 = data2[0];
					//geo = data[1];
					//ch_fired1 = data[bankLength1-1]-7;
					//uint32_t time_tag = data[bankLength1-2];
					//uint32_t global_time = data[bankLength1-1];
					if (i>1&&i%2==0&&i<bankLength2-2)//find channel and tdc value
					{ 
					//printf("=== %d, %d \n", data2[i-1], data2[i]);
					channel2[(i-2)/2] = data2[i-1];
  					int tmp_ch = channel2[(i-2)/2];
					tdc_value2[tmp_ch] = data2[i];
					}
				}
				if(tmp_eventid2==eventid2)//check if event id is duplicated
				{
				printf("======================================Skipped event2 %d \n", skipped2);
				skipped2++;
				}
				//if(tmp_eventid1!=eventid1 && tmp_eventid2!=eventid2)//to skip dulicated event number				
				//{
					fTree->Fill();
					tmp_eventid1 = eventid1;
					tmp_eventid2 = eventid2;
				//}
			}*/
			/*else{printf("Bank name2 %s is not listed\n", fBank1);}
				if(eventid1%10==0)printf("Events analyzed1 === %d \n", eventid1);
				for (int i=0; i<bankLength1; i++)//reset
				{
					channel1[i] = -999;
					tdc_value1[i] = -999;
				}*/
				/*if(eventid2%10==0)printf("Events analyzed2 === %d \n", eventid2);
				for (int i=0; i<bankLength2; i++)//reset
				{
					channel2[i] = -999;
					tdc_value2[i] = -999;
				}*/
			//}
			//else{printf("Bank name2 %s is not listed\n", fBank1);}
			/*if(event.FindBank(fBank3, &bankLength3, &bankType3, &pdata3))
			{
				printf("silicon ======================================%i \n",bankLength3);
			//if(bankLength1>6)
			  // {
				for(int i=0; i<bankLength3; i++)
				{
					data3[i]=((uint32_t*)pdata3)[i];	
					//geo = data[1];
					//ch_fired1 = data[bankLength1-1]-7;
					//uint32_t time_tag = data[bankLength1-2];
					//uint32_t global_time = data[bankLength1-1];
					if (i<bankLength3-1)//find channel and tdc value
					{
					//printf("=== %d, %d \n", data1[i-3], data1[i-2]);
					//channel1[(i-4)/2] = data1[i-3];
  					//int tmp_ch = channel1[(i-4)/2];
					//tdc_value1[tmp_ch] = data1[i]; 
					if((i+1)%2==0)
					{
					printf("silicon === ch %d, adc value %d \n", data3[i-1], data3[i]);
					int tmp_ch;
  					if(data3[i-1]<32) tmp_ch = data3[i-1];
					if(data3[i-1]>31) {tmp_ch = 32; printf("silicon === false channel \n");}
					//if(channel1[(i-2)/2]>33) tmp_ch = -999;
					//adc_value[tmp_ch] = data3[i];
					//adc_channel[tmp_ch] = tmp_ch;
					adc_value[(i+1)/2-1] = data3[i];
					printf("silicon === adc value %d \n", adc_value[(i+1)/2-1]);
					adc_value[(i+1)/2-1] = (data3[i]-pedestal[data3[i-1]]-y_inter_indiv[data3[i-1]])/slope_indiv[data3[i-1]]*slope_indiv[32]+y_inter_indiv[32];
					printf("silicon === corrected adc value %d slope %f\n", adc_value[(i+1)/2-1],slope_indiv[32]);
					adc_channel[(i+1)/2-1] = data3[i-1];
					}
					}
					else if (i == bankLength3-1) eventid3 = data3[bankLength3-1];
				}
				if(tmp_eventid3==eventid3)//check if event id is duplicated
				{
				printf("======================================Skipped silicon event %d \n", skippedsil);
				skippedsil++;
				}
				if(tmp_eventid3!=eventid3)//to skip dulicated event number				
				{
					fTree->Fill();
					tmp_eventid3 = eventid3;
					printf("silicon event id === %d \n",eventid3 );
				}
				//if(eventid3%100==0)printf("Events analyzed silicon === %d \n", eventid3);
				for (int i=0; i<36; i++)//reset
				{
					adc_channel[i] = -999;
					adc_value[i] = -999;
				}
				for (int i=0; i<bankLength1; i++)//reset
				{
					channel1[i] = -999;
					tdc_value1[i] = -999;
				}
			      //}
			}
			else{}//printf("Bank name3 %s is not listed\n", fBank1);}*/
		}
	}
	printf("======================================Total skipped silicon event %d \n", skippedsil);
	fp.Close();
	outfile->Write();
	return 0;
}
