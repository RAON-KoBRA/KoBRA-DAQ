#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <ctype.h>

#include <cvt_board_commons.h>
#include <cvt_common_defs.h>
#include <cvt_V1190.h>
#include "cvt_V1190.c"
#include <mfe_TDC.h>

#include "midas.h"
#include "odb_trigger.h"
#include "detector_conf.h"
#include "CAEN_PLU.h"

static cvt_V1190_data F3_PPAC_type;

extern uint32_t time_stamp1;
extern uint32_t time_stamp2;
extern uint64_t GCOUNT;
extern uint64_t TT100;
UINT32 f3_pp_f_time;
UINT64 f3_pp_time_tag;

uint32_t jjf3 = 0;

INT f3_ppac_init(int32_t BHandle){

	int32_t addr;

	//Make sure V1270N opened OK!
	printf("\n===================== Initializing F3 PPAC TDC...");

	memset( &F3_PPAC_type, 0, sizeof(F3_PPAC_type));

	if(!cvt_V1190_open(&F3_PPAC_type, F3_PPAC_ADDR, BHandle, CVT_V1290_TYPE_A))
	{
		printf("\n Error executing cvt_F3_PPAC_type_open \n");
		return FE_ERR_HW;
	}
	printf("Done\n");

	UINT16 firmware_rev;

	//get serial number of F1_PPAC_type
	printf("\nGetting system information...");

	//read firmware revision


	addr=(F3_PPAC_ADDR<<16)|CVT_V1190_FW_REV_ADD;
	CAENVME_ReadCycle(BHandle, addr, &firmware_rev, CVT_V1190_FW_REV_AM, CVT_V1190_FW_REV_DATA_SIZE);
	printf("Firmware Rev. : %d.%d\n", firmware_rev&0xffff0000, firmware_rev&0xffff);

	//clear data
	printf("\nSending data clear...");
		if(!cvt_V1190_data_clear(&F3_PPAC_type)){printf("\nError executing software clean\n");}
	printf("Done\n");

	INT16 default_val=0x0239; //see V1290 manual page 82
	INT16 data;

	//control_register setting
	addr=(F3_PPAC_ADDR<<16)|CVT_V1190_CONTROL_ADD;

	printf("\nControl register setting...");
	CAENVME_WriteCycle(BHandle, addr, &default_val, CVT_V1190_CONTROL_AM, CVT_V1190_CONTROL_DATA_SIZE);
	CAENVME_ReadCycle(BHandle, addr, &data, CVT_V1190_CONTROL_AM, CVT_V1190_CONTROL_DATA_SIZE);
	printf("0X%04x\n\n", data);

	sleep(1);

	return SUCCESS;
}

INT f3_ppac_exit(){

	cvt_V1190_module_reset(&F3_PPAC_type);
	cvt_V1190_close(&F3_PPAC_type);

	return SUCCESS;

}

INT f3_ppac_begin(INT run_number, char *error, TRIGGER_SETTINGS_BEAMLINE *ts){

	INT16 window_width=ts->f3_ppac_window_width/25;
	INT16 window_offset=0;
	if (ts->f3_ppac_window_offset>0 || ts->f3_ppac_window_offset==0){window_offset=ts->f3_ppac_window_offset/25;}
	if (ts->f3_ppac_window_offset<0){window_offset=4096-(ts->f3_ppac_window_offset/25)*-1;}

	INT16 extra_search_margin=ts->f3_ppac_extra_search_margin/25;
	INT16 reject_margin=ts->f3_ppac_reject_margin/25;
	INT blt_event_num=1;
	const UINT16 *p_enable_msk=NULL;
        printf("F3 PPAC width: %i, offset: %i, S margin: %i, R Margin: %i \n",ts->f3_ppac_window_width,ts->f3_ppac_window_offset, ts->f3_ppac_extra_search_margin, ts->f3_ppac_reject_margin);
	//trigger matching mode operation

//	printf("Setting_trigger_acquisition mode ...");

	if(! cvt_V1190_set_trigger_matching_acquisition_mode(&F3_PPAC_type, window_width, window_offset, extra_search_margin, reject_margin, CVT_V1190_ED_LEADING_ONLY,
		CVT_V1190_PRW_100NS, p_enable_msk, 1, 1, 1)){

			printf("Error executing cvt_F3_PPAC_type_set_trigger_matching_acqusition_mode\n");

			return FE_ERR_HW;

	}
//	printf("Done\n");

	//set enable subtraction of trigger time
//	printf("Setting_enable_subtraction_of_trigger_time...");
	if(!cvt_V1190_write_2_micro(&F3_PPAC_type, CVT_V1190_EN_SUB_TRG_OPCODE, NULL, 0)) 
	{
	printf("Error setting enable subtraction of trigger time\n");
	}
//	printf("Done\n");



	//readout mode
//	printf("Setting readout mode...");
	if(!cvt_V1190_set_readout_mode(&F3_PPAC_type, TRUE, TRUE, blt_event_num))
	{

		printf("Error executing cvt_F3_PPAC_type_set_readout_mode\n");
		return FE_ERR_HW;

	}
//	printf("Done\n");

	f3_pp_f_time=0;
	f3_pp_time_tag=0;

	return SUCCESS;

}


INT f3_ppac_end(INT run_number, char *error){

//	printf("\nSending data clear...");
	if(!cvt_V1190_data_clear(&F3_PPAC_type)){printf("\nError executing software clean\n");}

//	printf("Done\n");

	return SUCCESS;
}


INT f3_ppac_check_fifo(int32_t BHandle){

	uint32_t addr;
	int16_t output_buffer_event;

	addr = (F3_PPAC_ADDR<<16)|CVT_V1190_EVENT_STORED_ADD;
	CAENVME_ReadCycle(BHandle, addr, &output_buffer_event, CVT_V1190_EVENT_STORED_AM, CVT_V1190_EVENT_STORED_DATA_SIZE);


	return output_buffer_event;
}

INT f3_ppac_read_fifo(int32_t BHandle, void *buff_tmp, int size){

	int count;
	uint32_t addr;

	addr = (F3_PPAC_ADDR<<16)|CVT_V1190_OUT_BUFFER_ADD;

	CAENVME_BLTReadCycle(BHandle, addr, (char*)buff_tmp, size, CVT_V1190_OUT_BUFFER_AM, CVT_V1190_OUT_BUFFER_DATA_SIZE, &count);

	return count;
}

//uint32_t scalenumber123;
INT f3_ppac_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int buff_size, uint32_t *pdata){

	//int wcount;
	//CAENVME_BLTReadWait(BHandle, &wcount);

	int i=0;
	int count=f3_ppac_read_fifo(BHandle, buff, buff_size);
	UINT32 event_count;
//printf("--------------- %d %d\n", nb*4, count);

	bk_create(pevent, bank_name, TID_DWORD, (void**)&pdata);
//       CAEN_PLU_ReadReg(BHandle, 0x1200, &scalenumber123); //D connector 0x1200-, ch:0 (A connector 0x1100-, )
//        printf("FW2495SC scalenumber at trigger event %u\n", scalenumber123);// /100000000
	for(i=0; i<count/4; i++){

		uint32_t data=buff[i];
		switch(data&CVT_V1190_DATA_TYPE_MSK){

			case CVT_V1190_GLOBAL_HEADER:
				{
					event_count= CVT_V1190_GET_GLB_HDR_EVENT_COUNT(data);
					//UINT32 geo= CVT_V1190_GET_GLB_HDR_GEO(data);
					//*pdata++=GCOUNT;//event_count;
					//if(event_count%1000 == 0)
					*pdata++=event_count;
					printf("F3TDC; event_count; GCOUNT; clock count:%u ,%lu, %lu\n", event_count,GCOUNT,TT100);
					//printf("F3PPAC_Global_header; event id:%d\n", CVT_V1190_GET_TDC_HDR_EVENT_ID(data));
				} break;

			case CVT_V1190_TDC_MEASURE:
				{
					UINT32 channel= CVT_V1290_GET_TDC_MSR_CHANNEL(data);
					UINT32 measure= CVT_V1290_GET_TDC_HDR_MEASURE(data);
					*pdata++=channel;
					*pdata++=measure;
					//printf("F3PPAC_TDC measurement; channel:%d, measurement:%05f\n", channel, measure*0.025);
				} break;

			case CVT_V1190_GLOBAL_TRIGGER_TIME:
				{
				#ifndef USE_PPAC_F2
					UINT32 global_time=CVT_V1190_GET_GLB_TRG_TIME_TAG(data);
					if(f3_pp_f_time>global_time){f3_pp_time_tag+=0x8000000;}
					f3_pp_f_time=global_time;
				#else
					UINT32 global_time=time_stamp1;
					f3_pp_time_tag=time_stamp2;
				#endif
					//*pdata++=f3_pp_time_tag;
					//*pdata++=global_time;
					*pdata++=GCOUNT;
					//printf("F3PPAC_Global_time_tag:%d, u_time:%11f\n", global_time, (f3_pp_time_tag|global_time)*800e-9);
				} break;
			case CVT_V1190_TDC_TRAILER:
				{
					//printf("F3PPAC_Trailer event id; Event id:%d\n", CVT_V1190_GET_TDC_TRL_EVENT_ID(data));
				} break;
			default:
				{
					//printf("unknown data packets\n");
				} break;

		}
	}
	jjf3++;
	//printf("Processeed event reading(F3 TDC): count:%d\n", jjf3);//count
	bk_close(pevent, pdata);

	return bk_size(pevent);

}
