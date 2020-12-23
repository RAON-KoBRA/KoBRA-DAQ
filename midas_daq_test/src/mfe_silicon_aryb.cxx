
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

uint32_t jjs3b = 0;

uint32_t mcst_reg_orderb;
uint32_t user_channel_b;

//extern "C"{
static cvt_V792_data SILICON_aryb_type;
//}

uint32_t addr_aryb;
CVErrorCodes ret_aryb;
//}//extern

//extern uint64_t TT100; 
//extern uint32_t beamline_triggered;
//extern uint32_t u_detector_triggered;
//extern uint64_t GCOUNT;

//extern uint64_t TimeTag;
uint64_t TimeTag_ary_tmpb;

uint32_t u_detector_event_tmpb;
uint32_t Counters_tmp_udetectorb;


int iiab ,iibb, iicb;

INT silicon_aryb_init(int32_t BHandle, TRIGGER_SETTINGS *ts)
{
//  u_detector_event_tmpb = 0;
//  Counters_tmp_udetectorb = 0; 	
//  TimeTag_ary_tmpb = 0;

  UINT16 firmware_rev, reg_value, c_reg_value, geo_add, mcst_reg, mcst_reg_order, status_reg, boardversion_reg, Bit_set_1_reg;
  
	printf("\n========================== Initializing SILICON ARY ADC...\n");
	printf("V785N initialization...");
	if(!cvt_V792_open(&SILICON_aryb_type, SILICON_ARYB_ADDR, BHandle, CVT_V785_TYPE_N))
	{
		printf("\n Error executing cvt_SILICON_aryb_type_open \n");
		return FE_ERR_HW;
	}
	printf("Done\n");

	//==================================firmware version
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1000;
	ret_aryb=(CAENVME_ReadCycle(BHandle, addr_aryb, &firmware_rev, cvA32_S_DATA, cvD16));

	printf("Firmware Rev. : %x\n", firmware_rev);

	//==================================geo_add initial
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1002;
	ret_aryb=(CAENVME_ReadCycle(BHandle, addr_aryb, &geo_add, cvA32_S_DATA, cvD16));
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
*/
/*   	//==================================mcst reg for MCST address setting  
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1004;
	mcst_reg = 0xAA;
	ret_aryb=(CAENVME_WriteCycle(BHandle, addr_aryb, &mcst_reg, cvA32_S_DATA, cvD16));
	printf("mcst_reg(1004) set: %x\n", mcst_reg);

	//==================================mcst reg for MCST board last                                        
        addr_aryb=(SILICON_ARYB_ADDR<<16)|0x101A;
        mcst_reg_order = 0x01;
        ret_aryb=(CAENVME_WriteCycle(BHandle, addr_aryb, &mcst_reg_order, cvA32_S_DATA, cvD16));
	printf("mcst_reg(1004) order: %x\n", mcst_reg_order);
*/	
	//==================================status reg
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x100E;
	ret_aryb=(CAENVME_ReadCycle(BHandle, addr_aryb, &status_reg, cvA32_S_DATA, cvD16));
	printf("status_reg(100E) : %x\n", status_reg);

	//==================================board version
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x8032;
	ret_aryb=(CAENVME_ReadCycle(BHandle, addr_aryb, &boardversion_reg, cvA32_S_DATA, cvD16));
	printf("Board version(8032) : %x\n", boardversion_reg);
	//uint16_t c_reg_value;

	//=======================set Block Transfer Mode====================//--->see V785N manual page 65-66
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1010; 
	reg_value=0x0044;
	CAENVME_WriteCycle(BHandle, addr_aryb, &reg_value, cvA32_S_DATA, cvD16);


	//=======================set operation mode====================//--->see V785N manual page 53-55
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1032; //==================bit set 2
	reg_value=0x4880; //default 0x4980 4980 for *2 4880 for *16, sliding scale tech. only up to 3840 is valid, not 4096, 0x880->count only accepted event
	CAENVME_WriteCycle(BHandle, addr_aryb, &reg_value, cvA32_S_DATA, cvD16);

	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1034; //==================bit clear 2
	c_reg_value=(~reg_value)&0x7fff;
	CAENVME_WriteCycle(BHandle, addr_aryb, &c_reg_value, cvA32_S_DATA, cvD16);

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
		addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1080|2*i;
		th_val=ts->si_aryb_th[i];
		reg_value=0x0000|th_val;
	    ret_aryb=CAENVME_WriteCycle(BHandle, addr_aryb, &reg_value, cvA32_S_DATA, cvD16);
	    if(ret_aryb){printf("Error executing %d_channel threshold setting\n", i);}
	printf("array th ch: %i, val: %i\n",i,reg_value);
	}
	printf("Done\n");

	//==================================mcst reg bit set1                                                                                                             
        addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1006; //bit set 1                                                                                                               
        Bit_set_1_reg=0x0080;
        ret_aryb=CAENVME_WriteCycle(BHandle, addr_aryb, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
        if(ret_aryb){printf("\nError executing buffer writing_1\n");}

	//=================================data clear
	printf("Sending data clear...");
	ret_aryb=static_cast<CVErrorCodes>(silicon_aryb_clear_buffer(BHandle));
	printf("Done");

	return SUCCESS;
}

INT silicon_aryb_exit(int32_t BHandle)
{
	silicon_aryb_clear_buffer(BHandle);
	cvt_V792_close(&SILICON_aryb_type);
	return SUCCESS;
}



INT silicon_aryb_begin(int32_t BHandle, INT run_number, char *error, TRIGGER_SETTINGS *ts)
{
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

INT silicon_aryb_end(int32_t BHandle, INT run_number, char *error)
{
	cvt_V792_data_clear(&SILICON_aryb_type);
	return SUCCESS;
}

//}//extern

INT silicon_aryb_check_fifo(int32_t BHandle)
{
	//===============================status register====================================//-->see V785N manual page 47
	uint16_t status_reg1;

	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x100E;
	ret_aryb=CAENVME_ReadCycle(BHandle, addr_aryb, &status_reg1, cvA32_S_DATA, cvD16);

	return status_reg1&0x001;
}

INT silicon_aryb_read_fifo(int32_t BHandle, void *buff_tmp, int size)
{
	int count;

	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x0000;
	ret_aryb=CAENVME_BLTReadCycle(BHandle, addr_aryb, (char*)buff_tmp, size, cvA32_S_MBLT, cvD64, &count);

	return count;
}


/*INT silicon_aryb_read_async(int32_t BHandle, void *buff_tmp, int size)
{
	int count;

	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x0000;
	count = CAENVME_BLTReadAsync(BHandle, addr_aryb, (char*)buff_tmp, size, cvA32_S_MBLT, cvD64);

	return count;
}*/


//uint32_t u_detector_triggered_tmpb;
uint32_t ch_check_b;
INT silicon_aryb_read_event(int32_t BHandle, const char *bank_name, char *pevent1, INT off, uint32_t *buff, int buff_size, uint32_t *pdata)
{


	//int wcount;
	//CAENVME_BLTReadWait(BHandle, &wcount);

	int i;
	int count=silicon_aryb_read_fifo(BHandle, buff, buff_size);

	//int count = silicon_aryb_read_async(BHandle, buff, buff_size);

	ch_check_b = 0;
//	printf("==============================%d\n", count);
	uint32_t event_countb;
	bk_create(pevent1, bank_name, TID_DWORD, (void**)&pdata);


	//printf("Arr approaching count loop########\n");
	iiab = iibb = iicb = 0;


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
						if(channel == 1) {ch_check_b++;}
						if(ch_check_b > 1) {/*printf("ADC over counting,, ch_check_b = %d\n",ch_check_b);*/ break;}
						*pdata++=channel;
						*pdata++=measure;
						//tmp_channel = channel;
						//printf("(ARY B ADC) measurement; uv:%d, ov:%d, channel:%d, measurement:%d, \n", UV, OV, channel, measure);
						//printf("external trigger from scaler(ary b u_detector); %u\n", u_detector_triggered);
						//printf("external clock_number_lower from scaler(ary b u_detector); %u\n", clock_number_lower);
						
						iiab++;

					} break;


			case CVT_QTP_EOB:
					{

						user_channel_b = CVT_QTP_GET_HDR_CH_COUNT(data);
						event_countb= CVT_QTP_GET_EOB_EVENT_COUNT(data);
						*pdata++=event_countb;
						//*pdata++=GCOUNT;
					//	printf("AADC2; event_count; GCOUNT; clock count:%u, %lu, %lu \n", event_countb, GCOUNT, TT100);
						//printf("Processeed event reading(SB3 ADC): EOB event_count:%d\n", event_countb);
						//printf("external trigger from scaler(ary b u_detector); %u\n", u_detector_triggered);
						//printf("external clock_number_lower from scaler(ary b u_detector); %u\n", clock_number_lower);
						//printf("external clock_number_upper from scaler(ary b u_detector); %u\n", clock_number_upper);
						iibb++;

					} break;

			default:
				{iicb++;} break;
		}
	}
        jjs3b++;
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
//	float livelive; 
//	livelive = (float)(event_countb-u_detector_event_tmpb)/(float)(u_detector_triggered-Counters_tmp_udetectorb);

//	printf("Live time(beamline): %u / %u\n", f1ppac_event-f1ppac_event_tmp,beamline_triggered-Counters_tmp_ppac);
//	if( (u_detector_triggered-Counters_tmp_udetector) !=0 && u_detector_triggered>u_detector_triggered_tmp)printf("Live time(u_detector): %u \ %u \n",event_countb-u_detector_event_tmpb,  u_detector_triggered-Counters_tmp_udetector);

	//if( (u_detector_triggered-Counters_tmp_udetectorb) !=0 && u_detector_triggered>u_detector_triggered_tmpb)printf("Live time(u_detector): %f\n",livelive);
	/*if(TimeTag > TimeTag_ary_tmpb+1000000){//reset per 1 sec.
//		jj_tmp = 0;
		Counters_tmp_udetectorb = u_detector_triggered;
		u_detector_event_tmpb = event_countb;
		TimeTag_ary_tmpb = TimeTag;
		}
	u_detector_triggered_tmpb = u_detector_triggered;*/ 		// custom space def
	bk_close(pevent1, pdata);

return bk_size(pevent1);
}


BOOL silicon_aryb_clear_buffer(int32_t BHandle)
{

	UINT16 Bit_set_1_reg;
	
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1006; //bit set 1
	Bit_set_1_reg=0x0080;
	ret_aryb=CAENVME_WriteCycle(BHandle, addr_aryb, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
	if(ret_aryb){printf("\nError executing buffer clean_1\n");}
        
	addr_aryb=(SILICON_ARYB_ADDR<<16)|0x1008; //bit clear 1
	Bit_set_1_reg=0x0080;
	ret_aryb=CAENVME_WriteCycle(BHandle, addr_aryb, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
	if(ret_aryb){printf("\nError executing buffer clean_2\n");}

	return ret_aryb;
}








//#ifdef __cplusplus
//}
//#endif


