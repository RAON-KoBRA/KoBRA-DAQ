#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <fstream>
#include <map>
#include <string.h>


#include "TMidasFile.h"
#include "TMidasEvent.h"

using namespace std;

int main(int argc, char** argv)
{
	const char* fBank="F1PA";

	char path_file[512];
	int bankLength=0;
	int bankType=0;
	void *pdata=0;


	if(argc<2){printf("type run number!\n"); exit(-1);}
	sprintf(path_file, "/home/midas/online/data/run%s.mid", argv[argc-1]);

	TMidasFile fp;

	if(! fp.Open(path_file)){
		printf("Invalid file name run%s.mid\n", argv[argc-1]);
		exit(-1);
	}

uint32_t data=0;

while(1){

	TMidasEvent event;

		if(!fp.Read(&event)) break;
		uint16_t eventID=event.GetEventId();

		if(eventID==1){
			event.SetBankList();
			if(event.FindBank(fBank, &bankLength, &bankType, &pdata) && bankLength==7)
			{
				printf("======================================\n");
				for(unsigned int i=0; i<bankLength; i++)
				{
					data=((uint32_t*)pdata)[i];
					printf("%d\n", data);
				}
			}
			else{printf("Bank name %s is not listed\n", fBank);}
		}
	}

	fp.Close();
	return 0;
}
