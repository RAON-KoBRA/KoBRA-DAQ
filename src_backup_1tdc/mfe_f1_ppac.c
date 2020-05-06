#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <ctype.h>

#include "midas.h"
#include "mfe_ppac.h"
#include "odb_trigger.h"



#define F1_V1290N_ADDR 0X00FF
static cvt_V1190_data v1290n;


INT f1_ppac_init(int32_t BHandle)
{

	int32_t addr;
	//Make sure V1270N opened OK!
	printf("\n===================== Initializing V1290N board data...");

	memset( &v1290n, 0, sizeof(v1290n));

	if(!cvt_V1190_open(&v1290n, F1_V1290N_ADDR, BHandle, CVT_V1290_TYPE_N))
	{
		printf("\n Error executing cvt_V1290N_open \n");
		return FE_ERR_HW;
	}
	printf("Done\n");

	UINT16 firmware_rev, micro_firmware_rev, serial_number;
	int i;

	//get serial number of V1290N
	printf("\nGetting system information...");

	//read firmware revision


	addr=(F1_V1290N_ADDR<<16)|CVT_V1190_FW_REV_ADD;
	CAENVME_ReadCycle(BHandle, addr, &firmware_rev, CVT_V1190_FW_REV_AM, CVT_V1190_FW_REV_DATA_SIZE);
	printf("Firmware Rev. : %d.%d\n", firmware_rev&0xffff0000, firmware_rev&0xffff);

	//clear data
	printf("\nSending data clear...");
		if(!cvt_V1190_data_clear(&v1290n)){printf("\nError executing software clean\n");}
	printf("Done\n");


	INT16 default_val=0x0239; //see V1290 manual page 82
	INT16 data;

	//control_register setting
	addr=(F1_V1290N_ADDR<<16)|CVT_V1190_CONTROL_ADD;

	printf("\nControl register setting...");
	CAENVME_WriteCycle(BHandle, addr, &default_val, CVT_V1190_CONTROL_AM, CVT_V1190_CONTROL_DATA_SIZE);
	CAENVME_ReadCycle(BHandle, addr, &data, CVT_V1190_CONTROL_AM, CVT_V1190_CONTROL_DATA_SIZE);
	printf("0X%04x\n\n", data);

	sleep(1);



	return SUCCESS;
}

INT f1_ppac_exit()
{
	cvt_V1190_module_reset(&v1290n);
	cvt_V1190_close(&v1290n);
	return SUCCESS;

}

INT f1_ppac_begin(INT run_number, char *error, TRIGGER_SETTINGS *ts)
{

	INT16 window_width=ts->f1_window_width/25;
	INT16 window_offset=0;
	if (ts->f1_window_offset>0 || ts->f1_window_offset==0)
	{
	window_offset=ts->f1_window_offset/25;
	}
	if (ts->f1_window_offset<0)
	{
	window_offset=4096-(ts->f1_window_offset/25)*-1;
	}

	INT16 extra_search_margin=ts->f1_extra_search_margin/25;
	INT16 reject_margin=ts->f1_reject_margin/25;
	INT blt_event_num=1;
	const UINT16 *p_enable_msk=NULL;

	//trigger matching mode operation
//	printf("Setting_trigger_acquisition mode ...");

	if(! cvt_V1190_set_trigger_matching_acquisition_mode(&v1290n, window_width, window_offset,
			 extra_search_margin, reject_margin, CVT_V1190_ED_LEADING_ONLY,
			 CVT_V1190_PRW_100PS, p_enable_msk, 1, 1, 1))
	{
		printf("Error executing cvt_V1290N_set_trigger_matching_acqusition_mode\n");
		return FE_ERR_HW;
	}
//	printf("Done\n");


	//set enable subtraction of trigger time
//	printf("Setting_enable_subtraction_of_trigger_time...");
	if(!cvt_V1190_write_2_micro(&v1290n, CVT_V1190_EN_SUB_TRG_OPCODE, NULL, 0))
	{
		printf("Error setting enable subtraction of trigger time\n");
	}
//	printf("Done\n");



	//readout mode
//	printf("Setting readout mode...");
	if(!cvt_V1190_set_readout_mode(&v1290n, TRUE, TRUE, blt_event_num))
	{
		printf("Error executing cvt_V1290N_set_readout_mode\n");
		return FE_ERR_HW;
	}
//	printf("Done\n");
	sleep(1);


	return SUCCESS;

}


INT f1_ppac_end(INT run_number, char *error)
{
//	printf("\nSending data clear...");
	if(!cvt_V1190_data_clear(&v1290n)){printf("\nError executing software clean\n");}

//	printf("Done\n");

	return SUCCESS;
}


INT f1_ppac_check_fifo(int32_t BHandle)
{
	uint32_t addr;
	int16_t output_buffer_event;

	addr = (F1_V1290N_ADDR<<16)|CVT_V1190_EVENT_STORED_ADD;
	CAENVME_ReadCycle(BHandle, addr, &output_buffer_event, CVT_V1190_EVENT_STORED_AM, CVT_V1190_EVENT_STORED_DATA_SIZE);


	return output_buffer_event;
}

INT f1_ppac_read_fifo(int32_t BHandle, void *buff_tmp, int size)
{
	int count;
	uint32_t addr;

	addr = (F1_V1290N_ADDR<<16)|CVT_V1190_OUT_BUFFER_ADD;

	CAENVME_BLTReadCycle(BHandle, addr, (char*)buff_tmp, size, CVT_V1190_OUT_BUFFER_AM, CVT_V1190_OUT_BUFFER_DATA_SIZE, &count);

	return count;
}


INT f1_ppac_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int buff_size, uint32_t *pdata)
{

int i=0;
int count=f1_ppac_read_fifo(BHandle, buff, buff_size);

//printf("--------------- %d %d\n", nb*4, count);

bk_create(pevent, bank_name, TID_DWORD, (void**)&pdata);

	for(i=0; i<count/4; i++)
	{
		uint32_t data=buff[i];
		switch(data&CVT_V1190_DATA_TYPE_MSK)
		{
			case CVT_V1190_GLOBAL_HEADER:
			{
				UINT32 event_count= CVT_V1190_GET_GLB_HDR_EVENT_COUNT( data);
				UINT32 geo= CVT_V1190_GET_GLB_HDR_GEO( data);
				*pdata++=event_count;
				*pdata++=geo;
				printf("f1_Global_header;; event_count:%d, geo:%08x\n", event_count, geo);
			} break;

			case CVT_V1190_TDC_MEASURE:
			{
				UINT32 channel= CVT_V1290_GET_TDC_MSR_CHANNEL(data);
				UINT32 measure= CVT_V1290_GET_TDC_HDR_MEASURE(data);
				*pdata++=channel;
				*pdata++=measure;
				printf("f1_TDC measurement; channel:%d, measurement:%05f\n", channel, measure*0.025);
			} break;

			case CVT_V1190_GLOBAL_TRAILER:
			{
				UINT32 trailer_wcount=CVT_V1190_GET_GLB_TRL_WCOUNT(data);
				UINT32 trailer_geo=CVT_V1190_GET_GLB_TRL_GEO(data);
				*pdata++=trailer_wcount;
				printf("f1_Global_trailer; wcount:%08x\n", trailer_wcount);
			} break;

			default:
			{
				//printf("unknown data packets\n");
			} break;

		}
	}

bk_close(pevent, pdata);


return bk_size(pevent);
}
