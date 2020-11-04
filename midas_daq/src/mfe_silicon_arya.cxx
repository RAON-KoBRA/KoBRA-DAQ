
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

//#ifdef __cplusplus
//extern "C" {
//#endif

extern "C"{
#include <cvt_V792.h>
}

#include "midas.h"
#include "mfe_ADC.h"
#include "odb_trigger.h"
#include "detector_conf.h"
#include "kobra_user.h"
#include "msystem.h"



BEAMLINE_STATS *eq_user;
INT statusa = 0;
BEAMLINE *eqa;

HNDLE hDba = 1;
HNDLE hKeya;

DWORD tick, tok;

char stra[256];

extern uint32_t user_channel_b;
uint32_t user_channel_a;
uint32_t mcst_reg_order;
uint32_t silicon_ary_event;

static cvt_V792_data SILICON_arya_type;

uint32_t addr_ary;
CVErrorCodes ret_ary;

extern uint64_t TT100;
extern uint32_t beamline_triggered;
extern uint32_t u_detector_triggered; //extern

extern uint64_t TimeTag; //TimeTag
uint64_t TimeTag_ary_tmp;
extern uint64_t GCOUNT; //extern

uint32_t u_detector_event_tmp;
uint32_t Counters_tmp_udetector;

int iia ,iib, iic;
uint32_t jjs3 = 0;

float erate,drate,intv, ediff;
uint32_t event_count_tmp;

INT silicon_arya_init(int32_t BHandle, TRIGGER_SETTINGS_BEAMLINE *ts)
{

	sprintf(stra, "/Custom/%s",detector[0].name);

	db_create_key(hDba, 0, stra, TID_KEY);
	db_find_key(hDba,0, stra, &hKeya);


	     statusa = db_check_record(hDba, 0, stra, EQUIPMENT_SCAL_STR, TRUE);
	      if (statusa != DB_SUCCESS) {
	         printf("Cannot create/check statistics record \'%s\', error %d\n", stra, statusa);
	         ss_sleep(500);
	      }

	      statusa = db_find_key(hDba, 0, stra, &hKeya);
	      if (statusa != DB_SUCCESS) {
	         printf("Cannot find statistics record \'%s\', error %d\n", stra, statusa);
	         ss_sleep(500);
	      }

		statusa = 0;
		eqa = NULL;

		eq_user = &detector[0].dstat;
		eq_user->events = 0;
		eq_user->events_per_sec = 0;
		eq_user->data_per_sec = 0;

  u_detector_event_tmp = 0;
  Counters_tmp_udetector = 0;
  TimeTag_ary_tmp = 0;

  UINT16 firmware_rev, reg_value, c_reg_value, geo_add, mcst_reg, msct_reg_order, status_reg, boardversion_reg, Bit_set_1_reg; 
  UINT16 interrupt_level,interrupt_vector,evt_tri, evt_tri_write, interrupt_level_write;
  
	printf("\n========================== Initializing SILICON ARYA ADC...\n");
	printf("V785N initialization...");
	if(!cvt_V792_open(&SILICON_arya_type, SILICON_ARYA_ADDR, BHandle, CVT_V785_TYPE_N))
	{
		printf("\n Error executing cvt_SILICON_arya_type_open \n");
		return FE_ERR_HW;
	}
	printf("Done\n");

	//==================================firmware version
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1000;
	ret_ary=(CAENVME_ReadCycle(BHandle, addr_ary, &firmware_rev, cvA32_S_DATA, cvD16));

	printf("Firmware Rev. : %x\n", firmware_rev);

	//==================================geo_add initial
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1002;
	ret_ary=(CAENVME_ReadCycle(BHandle, addr_ary, &geo_add, cvA32_S_DATA, cvD16));
	printf("geo_add(1002) : %x\n", geo_add);
/*
	//==================================geo_add write
	addr_ary=(SILICON_ARY_ADDR<<16)|0x1002;
	geo_add = 2;
	ret_ary=(CAENVME_WriteCycle(BHandle, addr_ary, &geo_add, cvA32_S_DATA, cvD16));
	printf("geo_add(1002) : %x\n", geo_add);

	//==================================geo_add read again
	addr_ary=(SILICON_ARY_ADDR<<16)|0x1002;
	ret_ary=(CAENVME_ReadCycle(BHandle, addr_ary, &geo_add, cvA32_S_DATA, cvD16));
	printf("geo_add(1002) : %x\n", geo_add);
*//*
   	//==================================mcst reg for MCST address setting  
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1004;
	mcst_reg = 0xAA;
	ret_ary=(CAENVME_WriteCycle(BHandle, addr_ary, &mcst_reg, cvA32_S_DATA, cvD16));
	printf("mcst_reg(1004) set: %x\n", mcst_reg);

	//==================================mcst reg for MCST board first                                        
        addr_ary=(SILICON_ARYA_ADDR<<16)|0x101A;
        mcst_reg_order = 0x02;
        ret_ary=(CAENVME_WriteCycle(BHandle, addr_ary, &mcst_reg_order, cvA32_S_DATA, cvD16));
	printf("mcst_reg(1004) order: %x\n", mcst_reg_order);
	
	//==================================interrupt level write
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x100A;
	interrupt_level_write = 1;
	ret_ary=(CAENVME_WriteCycle(BHandle, addr_ary, &interrupt_level_write, cvA32_S_DATA, cvD16));
*/
	//==================================interrupt level
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x100A;
	ret_ary=(CAENVME_ReadCycle(BHandle, addr_ary, &interrupt_level, cvA32_S_DATA, cvD16));
	printf("interrupt level(100A) : %x\n", interrupt_level);        

	//==================================interrupt vector
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x100C;
	ret_ary=(CAENVME_ReadCycle(BHandle, addr_ary, &interrupt_vector, cvA32_S_DATA, cvD16));
	printf("interrupt vector(100C) : %x\n", interrupt_vector);

	//==================================status reg
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x100E;
	ret_ary=(CAENVME_ReadCycle(BHandle, addr_ary, &status_reg, cvA32_S_DATA, cvD16));
	printf("status_reg(100E) : %x\n", status_reg);
	/*
	//==================================event trigger register write
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1020;
	evt_tri_write = 1;
	ret_ary=(CAENVME_WriteCycle(BHandle, addr_ary, &evt_tri_write, cvA32_S_DATA, cvD16));
*/
	//==================================event trigger register
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1020;
	ret_ary=(CAENVME_ReadCycle(BHandle, addr_ary, &evt_tri, cvA32_S_DATA, cvD16));
	printf("event_trigger(1020) : %x\n", evt_tri);

	//==================================board version
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x8032;
	ret_ary=(CAENVME_ReadCycle(BHandle, addr_ary, &boardversion_reg, cvA32_S_DATA, cvD16));
	printf("Board version(8032) : %x\n", boardversion_reg);
	//uint16_t c_reg_value;

	//=======================set Block Transfer Mode====================//--->see V785N manual page 65-66
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1010; 
	reg_value=0x0044;
	CAENVME_WriteCycle(BHandle, addr_ary, &reg_value, cvA32_S_DATA, cvD16);


	//=======================set operation mode====================//--->see V785N manual page 53-55
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1032; //==================bit set 2
	reg_value=0x4880; //default 0x4980 4980 for *2 4880 for *16, sliding scale tech. only up to 3840 is valid, not 4096, 0x880->count only accepted event
	CAENVME_WriteCycle(BHandle, addr_ary, &reg_value, cvA32_S_DATA, cvD16);

	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1034; //==================bit clear 2
	c_reg_value=(~reg_value)&0x7fff;
	CAENVME_WriteCycle(BHandle, addr_ary, &c_reg_value, cvA32_S_DATA, cvD16);

	//================================set threshold=================//-->see V785N manual 59-60

	printf("Setting threshold...\n");
	int i=0;
	uint16_t th_val;
/* to kill the noisy channel
	for(i=0; i<4; i++)
	{
		addr_ary=(SILICON_ARY_ADDR<<16)|0x1080|2*i;
		th_val=ts->si_ary_th[i]*4096./4000.;
		reg_value=0x0100|th_val;
	    ret_ary=CAENVME_WriteCycle(BHandle, addr_ary, &reg_value, cvA32_S_DATA, cvD16);
	    if(ret_ary){printf("Error executing %d_channel threshold setting\n", i);}
	printf("array th ch: %d, val: %i\n",i,reg_value);
	}
	printf("Done\n");
*/
	for(i=0; i<32; i++) //if 16 channel ADC(V785N), channel step should be 4 instead of 2
	{
		addr_ary=(SILICON_ARYA_ADDR<<16)|0x1080|2*i;
		th_val=ts->si_arya_th[i];
		reg_value=0x0000|th_val;
	    ret_ary=CAENVME_WriteCycle(BHandle, addr_ary, &reg_value, cvA32_S_DATA, cvD16);
	    if(ret_ary){printf("Error executing %d_channel threshold setting\n", i);}
	printf("array th ch: %i, val: %i\n",i,reg_value);
	}
	printf("Done\n");

	//==================================mcst reg bit set1                                                                                                             
        addr_ary=(SILICON_ARYA_ADDR<<16)|0x1006; //bit set 1                                                                                                               
        Bit_set_1_reg=0x0080;
        ret_ary=CAENVME_WriteCycle(BHandle, addr_ary, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
        if(ret_ary){printf("\nError executing buffer writing_1\n");}

	//=================================data clear
	printf("Sending data clear...");
	ret_ary=static_cast<CVErrorCodes>(silicon_arya_clear_buffer(BHandle));
	printf("Done");


//########################	Passive record of new odb tree	######################################


    statusa = db_open_record(hDba, hKeya, eq_user, sizeof(BEAMLINE_STATS), MODE_WRITE, NULL, NULL);
      if (statusa == DB_NO_ACCESS) {

        statusa = db_set_mode(hDba, hKeya, MODE_READ | MODE_WRITE | MODE_DELETE, TRUE);
         if (statusa != DB_SUCCESS)
            cm_msg(MERROR, "register_equipment", "Cannot change access mode for record \'%s\', error %d", stra, statusa);
         else
            cm_msg(MINFO, "register_equipment", "Recovered access mode for record \'%s\'", stra);
         statusa = db_open_record(hDba, hKeya, eq_user, sizeof(BEAMLINE_STATS), MODE_WRITE, NULL, NULL);
      }
      if (statusa != DB_SUCCESS) {
         cm_msg(MERROR, "register_equipment", "Cannot open statistics record, error %d. Probably other FE is using it", statusa);
         ss_sleep(3000);
      }

//#######################################################################################################





	return SUCCESS;
}

INT silicon_arya_exit(int32_t BHandle)
{
	silicon_arya_clear_buffer(BHandle);
	cvt_V792_close(&SILICON_arya_type);

	eq_user->events_per_sec = 0;
	eq_user->data_per_sec = 0;
	eq_user->events = 0;

	return SUCCESS;
}



INT silicon_arya_begin(int32_t BHandle, INT run_number, char *error, TRIGGER_SETTINGS_BEAMLINE *ts)
{

	tick = ss_millitime(); 

	/*
	uint16_t reg_value, c_reg_value;

	//=======================set Block Transfer Mode====================//--->see V785N manual page 65-66
	addr_ary=(SILICON_ARY_ADDR<<16)|0x1010; 
	reg_value=0x0044;
	CAENVME_WriteCycle(BHandle, addr_ary, &c_reg_value, cvA32_S_DATA, cvD16);


	//=======================set operation mode====================//--->see V785N manual page 53-55
	addr_ary=(SILICON_ARY_ADDR<<16)|0x1032; //==================bit set 2
	reg_value=0x4880; //default 0x4980 4980 for *2 4880 for *16, sliding scale tech. only up to 3840 is valid, not 4096, 0x880->count only accepted event
	CAENVME_WriteCycle(BHandle, addr_ary, &reg_value, cvA32_S_DATA, cvD16);

	addr_ary=(SILICON_ARY_ADDR<<16)|0x1034; //==================bit clear 2
	c_reg_value=(~reg_value)&0x7fff;
	CAENVME_WriteCycle(BHandle, addr_ary, &c_reg_value, cvA32_S_DATA, cvD16);
	*/
	return SUCCESS;
}


//extern "C"{

INT silicon_arya_end(int32_t BHandle, INT run_number, char *error)
{
	cvt_V792_data_clear(&SILICON_arya_type);




	return SUCCESS;
}

//}//extern

INT silicon_arya_check_fifo(int32_t BHandle)
{
	//===============================status register====================================//-->see V785N manual page 47
	uint16_t status_reg1;

	addr_ary=(SILICON_ARYA_ADDR<<16)|0x100E;
	ret_ary=CAENVME_ReadCycle(BHandle, addr_ary, &status_reg1, cvA32_S_DATA, cvD16);

	return status_reg1&0x001;
}

INT silicon_arya_read_fifo(int32_t BHandle, void *buff_tmp, int size)
{
	int count;

	addr_ary=(SILICON_ARYA_ADDR<<16)|0x0000;
	ret_ary=CAENVME_BLTReadCycle(BHandle, addr_ary, (char*)buff_tmp, size, cvA32_S_MBLT, cvD64, &count);

	return count;
}


INT silicon_arya_read_async(int32_t BHandle, void *buff_tmp, int size)
{
	int count;

	addr_ary=(SILICON_ARYA_ADDR<<16)|0x0000;
	count = CAENVME_BLTReadAsync(BHandle, addr_ary, (char*)buff_tmp, size, cvA32_S_MBLT, cvD64);

	return count;
}



uint32_t u_detector_triggered_tmp;
uint32_t ch_check;
INT silicon_arya_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int buff_size, uint32_t *pdata)
{
	int i;
	uint32_t event_count;

	float pdsize= 0.000001*4;		// size of data buffer per event channel

	//int qcount;
	//CAENVME_BLTReadWait(BHandle, &qcount);

	int count=silicon_arya_read_fifo(BHandle, buff, buff_size);
	//int count;	

	//count=silicon_arya_read_async(BHandle, buff, buff_size);
	ch_check = 0;
//	printf("==============================%d\n", count);
	
	bk_create(pevent, bank_name, TID_DWORD, (void**)&pdata);


	//printf("Arr approaching count loop########\n");
	iia = iib = iic = 0;

	

//        UINT32 tmp_channel = 999;
	for(i=0; i<count/4; i++)
	{
		uint32_t data=buff[i];

		switch(data&CVT_QTP_DATA_TYPE_MSK)
		{
			case CVT_QTP_DATUM:
					{
						UINT32 channel= CVT_V785A_GET_DATUM_CH(data);
						UINT32 measure= CVT_V785_GET_DATUM_ADC(data);
						UINT32 UV=CVT_V785_GET_DATUM_UN(data);
						UINT32 OV=CVT_V785_GET_DATUM_OV(data);
						if(channel == 1) {ch_check++;}
						//if(jjs3%500 == 0) if(ch_check > 1) {printf("ADC over counting,, ch_check = %d\n",ch_check); break;}
						*pdata++=channel;
						*pdata++=measure;
						//tmp_channel = channel;
						if(channel > 15 && channel < 24)printf("(ARY A ADC) measurement; uv:%d, ov:%d, channel:%d, measurement:%d, \n", UV, OV, channel, measure);
						//printf("external trigger from scaler(ary a u_detector); %u\n", u_detector_triggered);
						//printf("external clock_number_lower from scaler(ary a u_detector); %u\n", clock_number_lower);

						iia++;

					} break;


			case CVT_QTP_EOB:
					{
						event_count= CVT_QTP_GET_EOB_EVENT_COUNT(data);
						user_channel_a = CVT_QTP_GET_HDR_CH_COUNT(data);
						*pdata++=event_count;
						*pdata++=GCOUNT;

						tok = ss_millitime();			
			
						eq_user = &detector[0].dstat;
						eq_user->events = event_count;

						silicon_ary_event = event_count;
						intv = (float)(tok - tick)/1000.;

						if(intv > 3.){

							
							ediff = event_count - event_count_tmp;
							erate = (float)ediff/(float)intv;
							drate = pdsize*ediff*(float)(user_channel_a + user_channel_b)/intv;

							eq_user->events_per_sec = erate;
							eq_user->data_per_sec = drate;
							
							event_count_tmp = event_count;		
							tick = ss_millitime();

						}



						//if(jjs3%500 == 0)printf("AADC1; event_count; GCOUNT; clock count:%u, %lu, %lu\n", event_count, GCOUNT, TT100);
						//printf("external trigger from scaler(ary a u_detector); %u\n", u_detector_triggered);
						//printf("external clock_number_lower from scaler(ary a u_detector); %u\n", clock_number_lower);
						//printf("external clock_number_ from scaler(ary a u_detector); %u\n", clock_number_upper);
						iib++;

					} break;

			default:
				{iic++;} break;
		}
	}
        jjs3++;
	//printf("Processeed event reading(S3 ADC): count:%d\n", jjs3);//count
	//printf("Processeed event reading(S3 ADC): count:%d\n", jjs3);//count
/*
	if(u_detector_triggered>u_detector_triggered_tmp){
	
	printf("\n");
	printf("a: %d, b: %d, c: %d\n",iia,iib,iic);
	printf("count value: %u breaking coord : %d\n\n",count,i);

	printf("external trigger from scaler(u_detector); %u\n", u_detector_triggered);
	printf("external clock_number_lower from scaler(u_detector); %u\n", clock_number_lower);
	printf("TimeTag:%u ,TimeTag_ary_tmp:%u\n",TimeTag,TimeTag_ary_tmp);
	}*/
	float livelive; 
	livelive = (float)(event_count-u_detector_event_tmp)/(float)(u_detector_triggered-Counters_tmp_udetector);

//	printf("Live time(beamline): %u / %u\n", f1ppac_event-f1ppac_event_tmp,beamline_triggered-Counters_tmp_ppac);
//	if( (u_detector_triggered-Counters_tmp_udetector) !=0 && u_detector_triggered>u_detector_triggered_tmp)printf("Live time(u_detector): %u \ %u \n",event_count-u_detector_event_tmp,  u_detector_triggered-Counters_tmp_udetector);

	//if( (u_detector_triggered-Counters_tmp_udetector) !=0 && u_detector_triggered>u_detector_triggered_tmp)printf("Live time(u_detector): %f\n",livelive);
	if(TimeTag > TimeTag_ary_tmp+1000000){//reset per 1 sec.
//		jj_tmp = 0;
		Counters_tmp_udetector = u_detector_triggered;
		u_detector_event_tmp = event_count;
		TimeTag_ary_tmp = TimeTag;
		}
	u_detector_triggered_tmp = u_detector_triggered;
	bk_close(pevent, pdata);

return bk_size(pevent);
}


BOOL silicon_arya_clear_buffer(int32_t BHandle)
{

	UINT16 Bit_set_1_reg;
	
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1006; //bit set 1
	Bit_set_1_reg=0x0080;
	ret_ary=CAENVME_WriteCycle(BHandle, addr_ary, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
	if(ret_ary){printf("\nError executing buffer clean_1\n");}
        
	addr_ary=(SILICON_ARYA_ADDR<<16)|0x1008; //bit clear 1
	Bit_set_1_reg=0x0080;
	ret_ary=CAENVME_WriteCycle(BHandle, addr_ary, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
	if(ret_ary){printf("\nError executing buffer clean_2\n");}

	return ret_ary;
}








//#ifdef __cplusplus
//}
//#endif

