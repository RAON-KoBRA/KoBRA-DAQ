#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
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
#include <mfe_TDC.h>


#include "mfe.h"
#include "midas.h"
#include "odb_trigger.h"
#include "detector_conf.h"

static cvt_V1190_data F1_PPAC_type;

extern uint32_t time_stamp1;
extern uint32_t time_stamp2;

extern "C"{

UINT32 f1_pp_f_time;
UINT64 f1_pp_time_tag;

}

uint32_t jjf1 = 0;

extern uint32_t beamline_triggered;
extern uint32_t u_detector_triggered;

extern uint64_t TimeTag;
uint64_t TimeTag_ppac_tmp;
extern uint64_t GCOUNT;

uint32_t f1ppac_event;
uint32_t f1ppac_event_tmp;
uint32_t Counters_tmp_ppac;
float ppac_time_tmp;
extern uint64_t TT100;
INT f1_ppac_init(int32_t BHandle)
{
	f1ppac_event = 0;
	Counters_tmp_ppac = 0;
	f1ppac_event_tmp = 0;
	TimeTag_ppac_tmp = 0;
	int32_t addr;
	//Make sure V1270N opened OK!
	printf("\n===================== Initializing F1 PPAC TDC...");

	memset( &F1_PPAC_type, 0, sizeof(F1_PPAC_type));

	if(!cvt_V1190_open(&F1_PPAC_type, F1_PPAC_ADDR, BHandle, CVT_V1290_TYPE_A))
	{
		printf("\n Error executing cvt_F1_PPAC_type_open \n");
		return FE_ERR_HW;
	}
	printf("Done\n");

	UINT16 firmware_rev;

	//get serial number of F1_PPAC_type
	printf("\nGetting system information...");

	//read firmware revision


	addr=(F1_PPAC_ADDR<<16)|CVT_V1190_FW_REV_ADD;
	CAENVME_ReadCycle(BHandle, addr, &firmware_rev, CVT_V1190_FW_REV_AM, CVT_V1190_FW_REV_DATA_SIZE);
	printf("Firmware Rev. : %d.%d\n", firmware_rev&0xffff0000, firmware_rev&0xffff);

	//clear data
	printf("\nSending data clear...");
		if(!cvt_V1190_data_clear(&F1_PPAC_type)){printf("\nError executing software clean\n");}
	printf("Done\n");


	INT16 default_val=0x0239; //see V1290 manual page 82
	INT16 data;
	//control_register setting
	addr=(F1_PPAC_ADDR<<16)|CVT_V1190_CONTROL_ADD;

	printf("\nControl register setting...");
	CAENVME_WriteCycle(BHandle, addr, &default_val, CVT_V1190_CONTROL_AM, CVT_V1190_CONTROL_DATA_SIZE);
	CAENVME_ReadCycle(BHandle, addr, &data, CVT_V1190_CONTROL_AM, CVT_V1190_CONTROL_DATA_SIZE);
	printf("0X%04x\n\n", data);



	sleep(1);



	return SUCCESS;
}

INT f1_ppac_exit()
{
	cvt_V1190_module_reset(&F1_PPAC_type);
	cvt_V1190_close(&F1_PPAC_type);
	return SUCCESS;

}

INT f1_ppac_begin(INT run_number, char *error, TRIGGER_SETTINGS_BEAMLINE *ts)
{

	INT16 window_width=ts->f1_ppac_window_width/25;

	INT16 window_offset=0;
	if (ts->f1_ppac_window_offset>0 || ts->f1_ppac_window_offset==0){window_offset=ts->f1_ppac_window_offset/25;}
	if (ts->f1_ppac_window_offset<0){window_offset=4096-(ts->f1_ppac_window_offset/25)*-1;}

	INT16 extra_search_margin=ts->f1_ppac_extra_search_margin/25;
	INT16 reject_margin=ts->f1_ppac_reject_margin/25;
	INT blt_event_num=1;
	const UINT16 *p_enable_msk=NULL;

	//trigger matching mode operation
        printf("F1 PPAC width: %i, offset: %i, S margin: %i, R Margin: %i \n",ts->f1_ppac_window_width,ts->f1_ppac_window_offset, ts->f1_ppac_extra_search_margin, ts->f1_ppac_reject_margin);
//	printf("Setting_trigger_acquisition mode ...");


	if(! cvt_V1190_set_trigger_matching_acquisition_mode(&F1_PPAC_type, window_width, window_offset,
			 extra_search_margin, reject_margin, CVT_V1190_ED_LEADING_ONLY,
			 CVT_V1190_PRW_100NS, p_enable_msk, 1, 1, 1))
	{
		printf("Error executing cvt_F1_PPAC_type_set_trigger_matching_acqusition_mode\n");
		return FE_ERR_HW;
	}
//	printf("Done\n");


	//set enable subtraction of trigger time
//	printf("Setting_enable_subtraction_of_trigger_time...");
	if(!cvt_V1190_write_2_micro(&F1_PPAC_type, CVT_V1190_EN_SUB_TRG_OPCODE, NULL, 0))
	{
		printf("Error setting enable subtraction of trigger time\n");
	}
//	printf("Done\n");



	//readout mode
//	printf("Setting readout mode...");
	if(!cvt_V1190_set_readout_mode(&F1_PPAC_type, TRUE, TRUE, blt_event_num))
	{
		printf("Error executing cvt_F1_PPAC_type_set_readout_mode\n");
		return FE_ERR_HW;
	}
//	printf("Done\n");

	f1_pp_f_time=0;
	f1_pp_time_tag=0;

	return SUCCESS;

}


INT f1_ppac_end(INT run_number, char *error)
{
//	printf("\nSending data clear...");
	if(!cvt_V1190_data_clear(&F1_PPAC_type)){printf("\nError executing software clean\n");}

//	printf("Done\n");

	return SUCCESS;
}


INT f1_ppac_check_fifo(int32_t BHandle)
{
	uint32_t addr;
	int16_t output_buffer_event;

	addr = (F1_PPAC_ADDR<<16)|CVT_V1190_EVENT_STORED_ADD;
	CAENVME_ReadCycle(BHandle, addr, &output_buffer_event, CVT_V1190_EVENT_STORED_AM, CVT_V1190_EVENT_STORED_DATA_SIZE);


	return output_buffer_event;
}

INT f1_ppac_read_fifo(int32_t BHandle, void *buff_tmp, int size)
{
	int count;
	uint32_t addr;

	addr = (F1_PPAC_ADDR<<16)|CVT_V1190_OUT_BUFFER_ADD;

	CAENVME_BLTReadCycle(BHandle, addr, (char*)buff_tmp, size, CVT_V1190_OUT_BUFFER_AM, CVT_V1190_OUT_BUFFER_DATA_SIZE, &count);

	return count;
}

uint32_t beamline_triggered_tmp;
INT f1_ppac_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int buff_size, uint32_t *pdata)
{


	//int wcount;
	//CAENVME_BLTReadWait(BHandle, &wcount);

int i;
int count=f1_ppac_read_fifo(BHandle, buff, buff_size);

//printf("--------------- %d %d\n", nb*4, count);
float ppac_time;
float live_time_f1ppac;
UINT32 event_count;
bk_create(pevent, bank_name, TID_DWORD, (void**)&pdata);

	for(i=0; i<count/4; i++)
	{
		uint32_t data=buff[i];
		switch(data&CVT_V1190_DATA_TYPE_MSK)
		{
			case CVT_V1190_GLOBAL_HEADER:
				{
					event_count= CVT_V1190_GET_GLB_HDR_EVENT_COUNT(data);
					f1ppac_event = event_count;
					//UINT32 geo= CVT_V1190_GET_GLB_HDR_GEO(data);
					//*pdata++=GCOUNT;//event_count;
					*pdata++=event_count;
					printf("F1TDC; event_count; GCOUNT; clock count:%u ,%lu, %lu\n", event_count,GCOUNT,TT100);
					//printf("external trigger from scaler(beamline); %u\n", beamline_triggered);
					//printf("F1TDC external clock_number_lower from scaler(beamline); %u\n", clock_number_lower);
				} break;

			case CVT_V1190_TDC_MEASURE:
				{
					UINT32 channel= CVT_V1290_GET_TDC_MSR_CHANNEL(data);
					UINT32 measure= CVT_V1290_GET_TDC_HDR_MEASURE(data);
					*pdata++=channel;
					*pdata++=measure;
					//printf("f1_TDC measurement; channel:%d, measurement:%05f\n", channel, measure*0.025);
				} break;

			case CVT_V1190_GLOBAL_TRIGGER_TIME:
				{
				#ifndef USE_PPAC_F2
					UINT32 global_time=CVT_V1190_GET_GLB_TRG_TIME_TAG(data);
					if(f1_pp_f_time>global_time){f1_pp_time_tag+=0x8000000;}
					f1_pp_f_time=global_time;
				#else
					UINT32 global_time=time_stamp1;
					f1_pp_time_tag=time_stamp2;
				#endif
					//*pdata++=f1_pp_time_tag;
					//*pdata++=global_time;
					*pdata++=GCOUNT;
  					ppac_time = (f1_pp_time_tag|global_time)*800e-9;
					//printf("f1_Global_time_tag:%d, u_time:%11f\n", global_time, ppac_time);
				} break;

			default:
				{
					//printf("unknown data packets\n");
				} break;

		}
	}
	if(beamline_triggered>beamline_triggered_tmp){ 
	//printf("external trigger from scaler(beamline); %u\n", beamline_triggered);
	//printf("external clock_number_lower from scaler(beamline); %u\n", clock_number_lower);
	//printf("TimeTag:%u ,TimeTag_ppac_tmp:%u\n",TimeTag,TimeTag_ppac_tmp);
      	}
	jjf1++;
	//printf("Processeed event reading(F1 TDC): count:%d\n", jjf1);//count

	//live_time_f1ppac = f1ppac_event_tmp/Counters_tmp_ppac;
	float livelive; 
	livelive = (float)(f1ppac_event-f1ppac_event_tmp)/(float)(beamline_triggered-Counters_tmp_ppac);
//	printf("Live time(beamline): %u / %u\n", f1ppac_event-f1ppac_event_tmp,beamline_triggered-Counters_tmp_ppac);
	//if( (beamline_triggered-Counters_tmp_ppac) !=0 && beamline_triggered>beamline_triggered_tmp)printf("Live time(beamline): %f\n",livelive);
	if(TimeTag > TimeTag_ppac_tmp+1000000){//reset per 1 sec.
//		jj_tmp = 0;
		Counters_tmp_ppac = beamline_triggered;
		f1ppac_event_tmp = f1ppac_event;
		TimeTag_ppac_tmp = TimeTag;
		}
	beamline_triggered_tmp = beamline_triggered;
bk_close(pevent, pdata);

return bk_size(pevent);
}








