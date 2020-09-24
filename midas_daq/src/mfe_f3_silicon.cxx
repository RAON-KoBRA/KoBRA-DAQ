
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

int iix ,iiy, iiz;


uint32_t jjbs3 = 0;

//extern "C"{
static cvt_V792_data F3_SILICON_type;
//}


uint32_t addr;
CVErrorCodes ret;

extern uint64_t GCOUNT;
extern uint64_t TT100; 
//extern "C" {
INT f3_silicon_init(int32_t BHandle, TRIGGER_SETTINGS_BEAMLINE *ts)
{
  UINT16 firmware_rev, reg_value, c_reg_value, geo_add, mcst_reg, mcst_reg_order, status_reg, boardversion_reg, Bit_set_1_reg, fclear_reg;

	printf("\n========================== Initializing F3 SILICON ADC...\n");
	printf("V785N initialization...");
	if(!cvt_V792_open(&F3_SILICON_type, F3_SILICON_ADDR, BHandle, CVT_V785_TYPE_A))
	{
		printf("\n Error executing cvt_F3_SILICON_type_open \n");
		return FE_ERR_HW;
	}
	printf("Done\n");

	//==================================firmware version
	addr=(F3_SILICON_ADDR<<16)|0x1000;
	ret=(CAENVME_ReadCycle(BHandle, addr, &firmware_rev, cvA32_S_DATA, cvD16));

	printf("Firmware Rev. : %x\n", firmware_rev);

	        //==================================geo_add initial                                                          
        addr=(F3_SILICON_ADDR<<16)|0x1002;
        ret=(CAENVME_ReadCycle(BHandle, addr, &geo_add, cvA32_S_DATA, cvD16));
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
        //==================================mcst reg for MCST address setting                                        
/*        addr=(F3_SILICON_ADDR<<16)|0x1004;
        mcst_reg = 0xAA;
        ret=(CAENVME_WriteCycle(BHandle, addr, &mcst_reg, cvA32_S_DATA, cvD16));
        printf("mcst_reg(1004) set: %x\n", mcst_reg);

        //==================================mcst reg for MCST board last                                            
        addr=(F3_SILICON_ADDR<<16)|0x101A;
        mcst_reg_order = 0x01;
        ret=(CAENVME_WriteCycle(BHandle, addr, &mcst_reg_order, cvA32_S_DATA, cvD16));
        printf("mcst_reg(1004) order: %x\n", mcst_reg_order);
*/
	//==================================status reg                                                               
        addr=(F3_SILICON_ADDR<<16)|0x100E;
        ret=(CAENVME_ReadCycle(BHandle, addr, &status_reg, cvA32_S_DATA, cvD16));
        printf("status_reg(100E) : %x\n", status_reg);
/*
	//==================================fast clear set write                                                              
        addr=(F3_SILICON_ADDR<<16)|0x102E;
	fclear_reg = 0x3F0;
        CAENVME_WriteCycle(BHandle, addr, &fclear_reg, cvA32_S_DATA, cvD16);

	//==================================fast clear set read                                                              
        addr=(F3_SILICON_ADDR<<16)|0x102E;
        ret=(CAENVME_ReadCycle(BHandle, addr, &fclear_reg, cvA32_S_DATA, cvD16));
        printf("fast clear bit : %x\n", fclear_reg);
*/
        //==================================board version                                                            
        addr=(F3_SILICON_ADDR<<16)|0x8032;
        ret=(CAENVME_ReadCycle(BHandle, addr, &boardversion_reg, cvA32_S_DATA, cvD16));
        printf("Board version(8032) : %x\n", boardversion_reg);
        //uint16_t c_reg_value;                                                                                      
	        //=======================set operation mode====================//--->see V785N manual page 53-55             
        addr=(F3_SILICON_ADDR<<16)|0x1032; //==================bit set 2                                        
        reg_value=0x4880; //default 0x4980 4980 for *2 4880 for *16, sliding scale tech. only up to 3840 is valid, not 4096, 0x880->count only accepted event 
        CAENVME_WriteCycle(BHandle, addr, &reg_value, cvA32_S_DATA, cvD16);

        addr=(F3_SILICON_ADDR<<16)|0x1034; //==================bit clear 2                                      
        c_reg_value=(~reg_value)&0x7fff;
        CAENVME_WriteCycle(BHandle, addr, &c_reg_value, cvA32_S_DATA, cvD16);

        //=======================set Block Transfer Mode====================//--->see V785N manual page 65-66        
        addr=(F3_SILICON_ADDR<<16)|0x1010;
        reg_value=0x0044;
        CAENVME_WriteCycle(BHandle, addr, &reg_value, cvA32_S_DATA, cvD16);
	
	//================================set threshold=================//-->see V785N manual 59-60

	printf("Setting threshold...\n");
	int i=0;
	uint16_t th_val;
	
	for(i=0; i<32; i++)
	{
		addr=(F3_SILICON_ADDR<<16)|0x1080|2*i; //if 16 channel ADC(V785N), channel step should be 4 instead of 2
		th_val=ts->f3_si_th[i];//*4096./4000.;
		reg_value=0x0000|th_val;
	    ret=CAENVME_WriteCycle(BHandle, addr, &reg_value, cvA32_S_DATA, cvD16);
	    if(ret){printf("Error executing %d_channel threshold setting\n", i);}
	printf("beamline th ch: %i, val: %i\n",i,th_val);
	}
	printf("Done\n");
	/*
#if 0
	for(i=0; i<32; i++)
	{
		addr=(F3_SILICON_ADDR<<16)|0x1080|2*i;
		th_val=ts->f3_si_th[i]*4096/4000;
		reg_value=0x0000|th_val;
	    ret=CAENVME_WriteCycle(BHandle, addr, &reg_value, cvA32_S_DATA, cvD16);
	    if(ret){printf("Error executing %d_channel threshold setting\n", i);}
	}
	printf("Done\n");
#endif*/
	//==================================mcst reg bit set1                                                                    
        addr=(F3_SILICON_ADDR<<16)|0x1006; //bit set 1                         
        Bit_set_1_reg=0x0080;
        ret=CAENVME_WriteCycle(BHandle, addr, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
        if(ret){printf("\nError executing buffer writing_1\n");}
	
	//=================================data clear
	printf("Sending data clear...");
	ret=static_cast<CVErrorCodes>(f3_silicon_clear_buffer(BHandle));
	printf("Done");

	return SUCCESS;
}


INT f3_silicon_exit(int32_t BHandle)
{
	f3_silicon_clear_buffer(BHandle);
	cvt_V792_close(&F3_SILICON_type);
	return SUCCESS;
}

//}//extern

INT f3_silicon_begin(int32_t BHandle, INT run_number, char *error, TRIGGER_SETTINGS_BEAMLINE *ts)
{
  /*	uint16_t reg_value, c_reg_value;
	
	//=======================set operation mode====================//--->see V785N manual page 53-55
	addr=(F3_SILICON_ADDR<<16)|0x1032; //==================bit set 2
	reg_value=0x4880; //default 0x4980// 4980 for *2, 4880 for *16, sliding scale tech. only up to 3840 is valid, not 4096, 0x880->count only accepted event
	CAENVME_WriteCycle(BHandle, addr, &reg_value, cvA32_S_DATA, cvD16);

	addr=(F3_SILICON_ADDR<<16)|0x1034; //==================bit clear 2
	c_reg_value=(~reg_value)&0x7fff;
	CAENVME_WriteCycle(BHandle, addr, &c_reg_value, cvA32_S_DATA, cvD16);

	//=======================set Block Transfer Mode====================//--->see V785N manual page 65-66

	addr=(F3_SILICON_ADDR<<16)|0x1010; 
	reg_value=0x0044;
	CAENVME_WriteCycle(BHandle, addr, &reg_value, cvA32_S_DATA, cvD16);
*/
	return SUCCESS;
}


//extern "C"{

INT f3_silicon_end(int32_t BHandle, INT run_number, char *error)
{
	cvt_V792_data_clear(&F3_SILICON_type);
	return SUCCESS;
}

//}//extern

INT f3_silicon_check_fifo(int32_t BHandle)
{
	//===============================status register====================================//-->see V785N manual page 47
	uint16_t status_reg1;

	addr=(F3_SILICON_ADDR<<16)|0x100E;
	ret=CAENVME_ReadCycle(BHandle, addr, &status_reg1, cvA32_S_DATA, cvD16);

	return status_reg1&0x001;
	
	/*uint32_t addr;
	int16_t output_buffer_event;

	addr = (F3_SILICON_ADDR<<16)|CVT_V792_EVENT_COUNTER_LOW_ADD;
	CAENVME_ReadCycle(BHandle, addr, &output_buffer_event, CVT_V792_EVENT_COUNTER_LOW_AM, CVT_V792_LOAD_TEST_DATA_SIZE);

	return output_buffer_event;*/
}

INT f3_silicon_read_fifo(int32_t BHandle, void *buff_tmp, int size)
{
	int count;

	addr=(F3_SILICON_ADDR<<16)|0x0000;
	ret=CAENVME_BLTReadCycle(BHandle, addr, (char*)buff_tmp, size, cvA32_S_MBLT, cvD64, &count);
	return count;
}

uint32_t ch_check_bs;
INT f3_silicon_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int buff_size, uint32_t *pdata)
{
	int i;
	int count=f3_silicon_read_fifo(BHandle, buff, buff_size);
	ch_check_bs = 0;
//	printf("==============================%d\n", count);
	bk_create(pevent, bank_name, TID_DWORD, (void**)&pdata);


iix = iiy = iiz = 0;

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
						if(channel == 2) {ch_check_bs++;}
						if(ch_check_bs > 1) {printf("ADC over counting,, ch_check_bs = %d\n",ch_check_bs);}
						*pdata++=channel;
						*pdata++=measure;
						//tmp_channel = channel;
						//printf("(BS3 ADC) measurement; uv:%d, ov:%d, channel:%d, measurement:%d, \n", UV, OV, channel, measure);
		iix++;
					} break;


			case CVT_QTP_EOB:
					{
						UINT32 event_count= CVT_QTP_GET_EOB_EVENT_COUNT(data);
						*pdata++=event_count;
						printf("Processeed event reading(BS3 ADC): EOB event_count, clock count:%d,%lu \n", event_count, TT100);
						//printf("external clock_number_lower from scaler(beam line silicon); %u\n", clock_number_lower);
			iiy++;		} break;

			default:
				{iiz++;} break;
		}
	}
	jjbs3++;


	//printf("\n");
	//printf("x: %d, y: %d, z: %d\n",iix,iiy,iiz);
	//printf("f3 count value: %u breaking coord : %d\n\n",count,i);

	//printf("Processeed event reading(BS3 ADC): count:%d\n", jjbs3);//count
	bk_close(pevent, pdata);

return bk_size(pevent);
}


BOOL f3_silicon_clear_buffer(int32_t BHandle)
{

	UINT16 Bit_set_1_reg;

	addr=(F3_SILICON_ADDR<<16)|0x1006; //bit set 1
	Bit_set_1_reg=0x0080;
	ret=CAENVME_WriteCycle(BHandle, addr, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
	if(ret){printf("\nError executing buffer clean_1\n");}

	addr=(F3_SILICON_ADDR<<16)|0x1008; //bit clear 1
	Bit_set_1_reg=0x0080;
	ret=CAENVME_WriteCycle(BHandle, addr, &Bit_set_1_reg, cvA32_S_DATA, cvD16);
	if(ret){printf("\nError executing buffer clean_2\n");}

	return ret;
}








//#ifdef __cplusplus
//}
//#endif

