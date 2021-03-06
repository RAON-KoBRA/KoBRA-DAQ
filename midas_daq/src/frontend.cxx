//#include <iostream>
//#include <cstring>
//#include <cstdio>
//#include <cstdlib>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <inttypes.h>
#include "midas.h"

//#include "experim.h"
#include "msystem.h"
#include "mfe.h"

#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include <vector>
#include <memory.h>
#include <ctype.h>

#include "CAENVMElib.h"
#include "CAENVMEtypes.h"

#include "odb_trigger.h"
#include "detector_conf.h"
#include "mfe_TDC.h"
#include "mfe_ADC.h"
#include "mfe_common.h"
#include "CAEN_PLU.h"
#include "PLUscalerLib.h"

static HNDLE hDB_odb;
static HNDLE hDB_odb_ary;
static TRIGGER_SETTINGS_BEAMLINE tr_set;
static TRIGGER_SETTINGS_SILICONARY tr_set_ary;

uint32_t time_stamp1;
uint32_t time_stamp2;

unsigned int scaler_high;
unsigned int scaler_low;

unsigned int BOP;

uint32_t s_evt;
/* make frontend functions callable from the C framework */
//#ifdef __cplusplus
//extern "C" {
//#endif
/*-- Globals -------------------------------------------------------*/

/* The frontend name (client name) as seen by other MIDAS clients   */
const char *frontend_name = "frontend";

/* The frontend file name, don't change it */
const char *frontend_file_name = __FILE__;

/* frontend_loop is called periodically if this variable is TRUE    */
BOOL frontend_call_loop = FALSE;

/* a frontend status page is displayed with this frequency in ms */
  INT display_period = 0;

/* maximum event size produced by this frontend */
  INT max_event_size = 4*1024*1024;

/* maximum event size for fragmented events (EQ_FRAGMENTED) */
INT max_event_size_frag = 5 * 1024 * 1024;

/* buffer size to hold events */
INT event_buffer_size = 32 * 1024*1024;

/*-- Function declarations mfe_tigress.o -----------------------------------------*/

INT frontend_init();
INT frontend_exit();
INT begin_of_run(INT run_number, char *error);
INT end_of_run(INT run_number, char *error);
INT pause_run(INT run_number, char *error);
INT resume_run(INT run_number, char *error);
INT frontend_loop();

INT read_trigger_event(char *pevent, INT off);
INT BLT_block_comm(int32_t BHandler, int counter);
INT read_silicon_event(char *pevent1, INT off);

INT poll_event(INT source, INT count, BOOL test);
INT interrupt_configure(INT cmd, INT source, POINTER_T adr);

/*-- Equipment list ------------------------------------------------*/

//#ifdef USE_PPAC_F1 & USE_PPAC_F2
//#define TRIGGER_EQ_NAME PPAC_EQ_NAME_F1"+"PPAC_EQ_NAME_F2
//#endif



//EQUIPMENT equipment[] = {
//  {"Trigger1 (Beamline)",            /* equipment name */
//   {1, 0,                   /* event ID, trigger mask */
//    "SYSTEM",               /* event buffer */
//     EQ_PERIODIC,                    // equipment type 
//#endif
//    0,                       /* event source crate 0, all stations */
//    "MIDAS",                /* format */
//    TRUE,                   /* enabled */
//    RO_RUNNING |
//    RO_ODB,             /* read only when running */
//    500,
//    0,                      /* stop run after this event limit */
//    0,                      /* number of sub events */
//    0,                      /* don't log history */
//    "", "", "",},
//	read_trigger_event,      /* readout routine */
//         NULL,                         // class driver routine
//         NULL,                         // device driver list
//         NULL,                         // bank list/init string
//  },
//
//  {"Trigger2 (Detector)",            /* equipment name */
//   {2, 0,                   /* event ID, trigger mask */
//    "SYSTEM",               /* event buffer */
//    EQ_PERIODIC,
//    0,                       /* event source crate 0, all stations */
//    "MIDAS",                /* format */
//    TRUE,                   /* enabled */
//    RO_RUNNING |
//    RO_ODB,             /* read only when running */
//    1000,
//    0,                      /* stop run after this event limit */
//    0,                      /* number of sub events */
//    0,                      /* don't log history */
//    "", "", "",},
//	read_silicon_event,      /* readout routine */
//         NULL,                         // class driver routine
//         NULL,                         // device driver list
//         NULL,                         // bank list/init string
//  },
//
//   {""}
//};


EQUIPMENT equipment[] = {

   {"Trigger1 (Beamline)",               /* equipment name */
      {1, 0,                 /* event ID, trigger mask */
         "SYSTEM",           /* event buffer */
         EQ_POLLED,          /* equipment type */ //1: PERIODIC 2: POLLED 
         0,                  /* event source */
         "MIDAS",            /* format */
         TRUE,               /* enabled */
         RO_RUNNING |        /* read only when running */
         RO_ODB,             /* and update ODB */
         1,                /* poll for 100ms */
         0,                  /* stop run after this event limit */
         0,                  /* number of sub events */
         TRUE,                  /* don't log history */
         "", "", "",},
      read_trigger_event,    /* readout routine */
   },

  /* {"Trigger2 (Detector)",             
      {2, 0,                 
         "SYSTEM",           
         EQ_POLLED,        
         0,                  
         "MIDAS",            
         TRUE,               
         RO_RUNNING |   
         RO_ODB,             
         1,               
         0,                 
         0,                
         TRUE,               
         "", "", "",},
      read_silicon_event,   
   },*/

   {""}
};
//#ifdef __cplusplus
//}
//#endif

/********************************************************************\
              Callback routines for system transitions

  These routines are called whenever a system transition like start/
  stop of a run occurs. The routines are called on thenb_frames following
  occations:
libtigfunc.o
  frontend_init:  When the frontend program is started. This routine
                  should initialize the hardware.

  frontend_exit:  When the frontend program is shut down. Can be used
                  to releas any locked resources like memory, commu-
                  nications ports etc.

  begin_of_run:   When a new run is started. Clear scalers, open
                  rungates, etc.

  end_of_run:     Called on a request to stop a runkobra_det. Can send
                  end-of-run event and close run gates.

  pause_run:      When a run is paused. Should disable trigger events.

  resume_run:     When a run is resumed. Should enable trigger events.
\********************************************************************/

/*-- Frontend Init -------------------------------------------------*/

static int32_t BHandle;
static int32_t BHandle2;
int handle;


const unsigned int DATA_BUFF_SIZE=4*1024*1024;
const unsigned int SCALER_DATA_BUFF_SIZE=4*1024*1024;//4*4*1024;//2048*128;

uint32_t *f1_ppac_buff=NULL;
uint32_t *f1_plastic_buff=NULL;

uint32_t *f2_ppac_buff=NULL;

uint32_t *f3_silicon_buff=NULL;
uint32_t *f3_plastic_buff=NULL;
uint32_t *f3_ppac_buff=NULL;

uint32_t *v2495_buff=NULL;
uint32_t *silicon_arya_buff=NULL;
uint32_t *silicon_aryb_buff=NULL;


INT frontend_init() {

  INT ret;

  v2495_init(BHandle); // v2495 intialization take v2718 together

  // Set ODB

  ret = set_odb(&hDB_odb, equipment[0].name, ts_str_kd, "kobra_detectors", &tr_set, sizeof(TRIGGER_SETTINGS_BEAMLINE));
  if ( ret != SUCCESS ) return ret;

 /* ret = set_odb(&hDB_odb_ary, equipment[1].name, ts_str_kd_ary, "silicon array detectors", &tr_set_ary, sizeof(TRIGGER_SETTINGS_SILICONARY));
  if ( ret != SUCCESS ) return ret;*/

  //VME initialization

        short Link=0;
  	short Device=1;
        CVBoardTypes VMEBoard = cvV2718;

        printf("\n1st V2718 initialization...");

  	//Make sure V2718 opened OK!
  	if(CAENVME_Init(VMEBoard, Link, Device, &BHandle2)!=cvSuccess){
  		cm_msg(MERROR, "kobra_detector", "Failed to Open CAEN V2718_board1");
  		return FE_ERR_HW;
  	}
  	printf("Done\n");

	unsigned int Rotaryswitch;

	CAENVME_ReadRegister(BHandle, static_cast<CVRegisters>(0x36), &Rotaryswitch);
	printf("1st Controller rotary switch:%i\n", Rotaryswitch);

	char str0[256];
	char str00[128];



	



#ifdef USE_PPAC_F1
  	f1_ppac_init(BHandle);
#endif

#ifdef USE_SILICON_ARYA
  	silicon_arya_init(BHandle2, &tr_set_ary);
#endif

#ifdef USE_SILICON_ARYB
  	silicon_aryb_init(BHandle2, &tr_set_ary);
#endif
  	//hardware initialization
  	printf("\n2nd V2718 initialization..."); 
/*
  	Link=0;
  	Device=1;
//	unsigned int Rotaryswitch;
  	//Make sure V2718 opened OK!
 	if(CAENVME_Init(VMEBoard, Link, Device, &BHandle2)!=cvSuccess){
  		cm_msg(MERROR, "kobra_detector", "Failed to Open CAEN V2718_board2");
  		return FE_ERR_HW;
  	}*/
  	printf("Done\n");

	CAENVME_ReadRegister(BHandle2, static_cast<CVRegisters>(0x36), &Rotaryswitch);
	printf("2nd Controller rotary switch:%i\n", Rotaryswitch);





#ifdef USE_SILICON_F3
 	f3_silicon_init(BHandle, &tr_set);
#endif

/*
#ifdef USE_PLASTIC_F3
  	f3_plastic_init(BHandle2);
#endif
*/
#ifdef USE_PPAC_F3
  	f3_ppac_init(BHandle);
#endif

#ifdef USE_PPAC_F2
  	f2_ppac_init(BHandle);
#endif


//	Creat Link for the KOBRA DAQ STATUS PAGE
 	sprintf(str00,"/home/kobradaq/%s","online/midas_daq/custom/custom.html");
	sprintf(str0, "/Custom/%s", "custom page&");

	db_set_value(1, 0, str0, str00, 128, 1, TID_STRING);




	printf("\n==========  Enjoy!\n");

  return SUCCESS;
}






/*-- Frontend Exit -------------------------------------------------*/

INT frontend_exit() {

#ifdef USE_V2495
	v2495_exit(BHandle);
	free(v2495_buff);
#endif

#ifdef USE_SILICON_ARYA
	silicon_arya_exit(BHandle2);
	free(silicon_arya_buff);
#endif

#ifdef USE_SILICON_ARYB
	silicon_aryb_exit(BHandle2);
	free(silicon_aryb_buff);
#endif

#ifdef USE_PPAC_F1
	f1_ppac_exit();
	free(f1_ppac_buff);
#endif
/*
#ifdef USE_PLASTIC_F1
	f1_plastic_exit();
	free(f1_plastic_buff);
#endif
*/
#ifdef USE_PPAC_F2
	f2_ppac_exit();
	free(f2_ppac_buff);
#endif

#ifdef USE_SILICON_F3
	f3_silicon_exit(BHandle);
	free(f3_silicon_buff);
#endif

/*
#ifdef USE_PLASTIC_F3
	f3_plastic_exit(BHandle2);
	free(f3_plastic_buff);
#endif
*/
#ifdef USE_PPAC_F3
	f3_ppac_exit();
	free(f3_ppac_buff);
#endif

	return SUCCESS;
}

/*-- Begin of Run --------------------------------------------------*/

INT begin_of_run(INT run_number, char *error) {



//scaler_high=0; 
//scaler_low=0; 

//BOP=1;

#ifdef USE_V2495
	v2495_buff = static_cast<uint32_t*>( malloc(SCALER_DATA_BUFF_SIZE));
#endif

#ifdef USE_SILICON_ARYA
	silicon_arya_buff= static_cast<uint32_t*>(malloc(DATA_BUFF_SIZE));
	silicon_arya_begin(BHandle2, run_number, error, &tr_set_ary);
#endif

#ifdef USE_SILICON_ARYB
	silicon_aryb_buff= static_cast<uint32_t*>(malloc(DATA_BUFF_SIZE));
	silicon_aryb_begin(BHandle2, run_number, error, &tr_set_ary);
#endif

#ifdef USE_PPAC_F1
	f1_ppac_buff= static_cast<uint32_t*>( malloc(DATA_BUFF_SIZE));
	f1_ppac_begin(run_number, error, &tr_set);
#endif
/*
#ifdef USE_PLASTIC_F1
	f1_plastic_buff=malloc(DATA_BUFF_SIZE);
	f1_plastic_begin(run_number, error, &tr_set);
#endif
*/
#ifdef USE_PPAC_F2
	f2_ppac_buff= static_cast<uint32_t*>( malloc(DATA_BUFF_SIZE));
	f2_ppac_begin(run_number, error, &tr_set);
#endif

#ifdef USE_SILICON_F3
	f3_silicon_buff= static_cast<uint32_t*>(malloc(DATA_BUFF_SIZE));
	f3_silicon_begin(BHandle, run_number, error, &tr_set);
#endif

/*
#ifdef USE_PLASTIC_F3
	f3_plastic_buff=malloc(DATA_BUFF_SIZE);
	f3_plastic_begin(run_number, error, &tr_set);
#endif
*/
#ifdef USE_PPAC_F3
	f3_ppac_buff= static_cast<uint32_t*>( malloc(DATA_BUFF_SIZE));
	f3_ppac_begin(run_number, error, &tr_set);
#endif

  return SUCCESS;
}

/*-- End of Run ----------------------------------------------------*/

INT end_of_run(INT run_number, char *error) {

#ifdef USE_PPAC_F1
	f1_ppac_end(run_number, error);
	free(f1_ppac_buff);
#endif
/*
#ifdef USE_PLASTIC_F1
	f1_plastic_end(run_number, error);
	free(f1_plastic_buff);
#endif
*/
#ifdef USE_PPAC_F2
	f2_ppac_end(run_number, error);
	free(f2_ppac_buff);
#endif

#ifdef USE_SILICON_F3
	f3_silicon_end(BHandle, run_number, error);
	free(f3_silicon_buff);
#endif

/*
#ifdef USE_PLASTIC_F3
	f3_plastic_end(run_number, error);
	free(f3_plastic_buff);
#endif
*/
#ifdef USE_PPAC_F3
	f3_ppac_end(run_number, error);
	free(f3_ppac_buff);
#endif

#ifdef USE_SILICON_ARYA
	silicon_arya_end(BHandle2, run_number, error);
	free(silicon_arya_buff);
#endif

#ifdef USE_SILICON_ARYB
	silicon_aryb_end(BHandle2, run_number, error);
	free(silicon_aryb_buff);
#endif

  char *vme_base_address = "32100000";

  CAEN_PLU_ERROR_CODE ret_2495;

                ret_2495 = CAEN_PLU_CloseDevice(BHandle);
                if (ret_2495 != CAEN_PLU_OK) {
                        printf("close Error %d\n", ret_2495);
                        exit(0);
                }


	db_close_all_records ();

  printf("PLU stopped OK\n");
  free(v2495_buff);

  return SUCCESS;

}

/*-- Pause Run -----------------------------------------------------*/

INT pause_run(INT run_number, char *error) {

 // tigress_pause(run_number,error);

  return SUCCESS;
}

/*-- Resume Run ----------------------------------------------------*/

INT resume_run(INT run_number, char *error) {

 // tigress_resume(run_number,error);

  return SUCCESS;
}

/*-- Frontend Loop -------------------------------------------------*/

INT frontend_loop() {

	unsigned int timebomb;
	CAEN_PLU_ReadReg(BHandle2, 0x1040, &timebomb);
        printf("FW2495SC timebomb at loop %x\n", timebomb);

	return SUCCESS;

}

/*-- Trigger event routines ----------------------------------------*/
//unsigned short tmp_Data = 0;

INT poll_event(INT source, INT count, BOOL test) {

	uint lam=0;
	lam=f3_ppac_check_fifo(BHandle) || silicon_arya_check_fifo(BHandle2);
	//lam=silicon_ary_check_fifo(BHandle2);
	//lam=(f3_ppac_check_fifo(BHandle2)||f1_ppac_check_fifo(BHandle));
        //lam=f3_silicon_check_fifo(BHandle2);
/*
#ifdef USE_V2495
//	lam=v2495_check_fifo(BHandle2);
#endif

#ifndef USE_PLASTIC_F3

	#ifdef USE_PLASTIC_F1
	lam=f1_plastic_check_fifo(BHandle);
	#endif

	#ifdef USE_PPAC_F1
	lam=f1_ppac_check_fifo(BHandle);
	#endif

	#ifdef USE_PPAC_F2
	lam=f2_ppac_check_fifo(BHandle);
	#endif

	#ifdef USE_PLASTIC_F3
	lam=f3_plastic_check_fifo(BHandle);
	#endif

	#ifdef USE_PPAC_F3
	lam=f3_ppac_check_fifo(BHandle);
	#endif

	#ifdef USE_SILICON_F3
	lam=f3_silicon_check_fifo(BHandle2);
	#endif

        #ifdef USE_PPAC_F3
	//lam=f3_ppac_check_fifo(BHandle2);
 	lam=silicon_ary_check_fifo(BHandle);
	#endif

        #ifdef USE_PPAC_F1
	lam=f1_ppac_check_fifo(BHandle);
	#endif

#endif

#ifdef USE_PLASTIC_F3
	lam=f3_plastic_check_fifo(BHandle2);
#endif
*/
/*
CVRegisters Reg2;
unsigned int Data;
CAENVME_ReadRegister(BHandle, static_cast<CVRegisters>(0x1D), &Data );
//printf("Scaler:%d\n", Data);
s_evt++;
//printf("Accumulated:%d\n", s_evt);

BOOL lam=FALSE;
unsigned int ac_scaler;
unsigned int input_Reg;
CVRegisters Reg = cvScaler1;

CAENVME_ReadRegister(BHandle, cvScaler1, &ac_scaler);
if(BOP>ac_scaler){lam=SUCCESS; scaler_high=scaler_high+1; scaler_low=ac_scaler;}
if(ac_scaler!=BOP){lam=SUCCESS; scaler_low=ac_scaler;}
BOP=ac_scaler;
//if(lam){printf("lam=%d, scaler=%d, fifo=%d\n", lam, ac_scaler, f3_silicon_check_fifo(BHandle));}
if(lam){printf("=========================== lam=%d, scaler_high=%d, scaler_low=%d\n", lam, scaler_high, scaler_low);}
*/
	return lam;
}

/*-- Interrupt configuration ---------------------------------------*/

INT interrupt_configure(INT cmd, INT source, POINTER_T adr) {

  switch (cmd) {
  case CMD_INTERRUPT_ENABLE:
    break;
  case CMD_INTERRUPT_DISABLE:
    break;
  case CMD_INTERRUPT_ATTACH:
    break;
  case CMD_INTERRUPT_DETACH:
    break;
  }

  return SUCCESS;
}

/*-- Event readout -------------------------------------------------*/
        uint32_t scalenumber;
        uint32_t scalenumbertmp;
        uint32_t scalenumber2; //more 32 bits for scalenumber
        uint32_t scalenumber3;
        uint32_t scalenumber4;
        uint32_t *pointer;

INT read_trigger_event(char *pevent, INT off) {

	int wcount;
	//CAENVME_BLTReadWait(BHandle, &wcount);

	bk_init(pevent);


#ifdef USE_V2495
	uint32_t *v2495_data;
	v2495_read_event(BHandle, BANK_NAME_SCALER, pevent, off, v2495_buff, SCALER_DATA_BUFF_SIZE, v2495_data);
#endif


#ifdef USE_PPAC_F1
	uint32_t *f1ppac_data;
	f1_ppac_read_event(BHandle, BANK_NAME_F1PPAC, pevent, off, f1_ppac_buff, DATA_BUFF_SIZE, f1ppac_data);
#endif

/*
#ifdef USE_PLASTIC_F1
	uint32_t *f1plastic_data;
	f1_plastic_read_event(BHandle, BANK_NAME_F1PLASTIC, pevent, off, f1_plastic_buff, DATA_BUFF_SIZE, f1plastic_data);
#endif
*/

	
#ifdef USE_PPAC_F2
	uint32_t *f2ppac_data;
	f2_ppac_read_event(BHandle, BANK_NAME_F2PPAC, pevent, off, f2_ppac_buff, DATA_BUFF_SIZE, f2ppac_data);
#endif

#ifdef USE_SILICON_F3
	uint32_t *f3silicon_data;
	f3_silicon_read_event(BHandle, BANK_NAME_F3SILICON, pevent, off, f3_silicon_buff, DATA_BUFF_SIZE, f3silicon_data);
#endif

#ifdef USE_PPAC_F3
	uint32_t *f3ppac_data;
	f3_ppac_read_event(BHandle, BANK_NAME_F3PPAC, pevent, off, f3_ppac_buff, DATA_BUFF_SIZE, f3ppac_data);
#endif

#ifdef USE_SILICON_ARYA
	uint32_t *siliconarya_data;
	silicon_arya_read_event(BHandle2, BANK_NAME_U1SILLICON, pevent, off, silicon_arya_buff, DATA_BUFF_SIZE, siliconarya_data);
#endif

#ifdef USE_SILICON_ARYB
	uint32_t *siliconaryb_data;
	silicon_aryb_read_event(BHandle2, BANK_NAME_U2SILLICON, pevent, off, silicon_aryb_buff, DATA_BUFF_SIZE, siliconaryb_data);
#endif

	//BLT_block_comm(BHandle,wcount);
	//CAENVME_BLTReadWait(BHandle, &wcount);
	return bk_size(pevent);
}

INT read_silicon_event(char *pevent1, INT off) {  
/*
	int wcount;
	//BLT_block_comm(BHandle2,wcount);
	//BLT_block_comm(BHandle,wcount);

	bk_init(pevent1);


#ifdef USE_SILICON_ARYA
	uint32_t *siliconarya_data;
	silicon_arya_read_event(BHandle2, BANK_NAME_U1SILLICON, pevent1, off, silicon_arya_buff, DATA_BUFF_SIZE, siliconarya_data);
#endif

	//BLT_block_comm(BHandle2,ucount);

#ifdef USE_SILICON_ARYB
	uint32_t *siliconaryb_data;
	silicon_aryb_read_event(BHandle2, BANK_NAME_U2SILLICON, pevent1, off, silicon_aryb_buff, DATA_BUFF_SIZE, siliconaryb_data);
#endif
*/

	return bk_size(pevent1);
}


INT BLT_block_comm(int32_t BHandler, int counter){

	CAENVME_BLTReadWait(BHandler, &counter);

	return counter;
}





