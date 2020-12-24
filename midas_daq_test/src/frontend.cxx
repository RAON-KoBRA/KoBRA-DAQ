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

static HNDLE hDB_obd;
static TRIGGER_SETTINGS tr_set;
uint32_t time_stamp1;
uint32_t time_stamp2;

unsigned int scaler_high;
unsigned int scaler_low;

unsigned int BOP;

uint32_t s_evt;

static const char *ts_str_kd[]={
		"[.]",
#ifdef USE_PPAC_F1
		"F1PPAC Window Width (ns)		 = INT 940",
		"F1PPAC Window Offset (ns)       = INT -400",
		"F1PPAC Extra Search Margin (ns) = INT 200",
		"F1PPAC Reject Margin (ns)       = INT 100",
#endif
/*#ifdef USE_PLASTIC_F1
		"F1PLA. Window Width (ns)		 = INT 300",
		"F1PLA. Window Offset (ns)       = INT -400",
		"F1PLA. Extra Search Margin (ns) = INT 200",
		"F1PLA. Reject Margin (ns)       = INT 100",
#endif*/
#ifdef USE_PPAC_F2
		"F2PPAC Window Width (ns)		 = INT 940",
		"F2PPAC Window Offset (ns)       = INT -400",
		"F2PPAC Extra Search Margin (ns) = INT 200",
		"F2PPAC Reject Margin (ns)       = INT 100",
#endif
#ifdef USE_PPAC_F3
		"F3PPAC Window Width (ns)		 = INT 940",
		"F3PPAC Window Offset (ns)       = INT -400",
		"F3PPAC Extra Search Margin (ns) = INT 200",
		"F3PPAC Reject Margin (ns)       = INT 100",
#endif
/*#ifdef USE_PLASTIC_F3
		"F3PLA. Window Width (ns)		 = INT 300",
		"F3PLA. Window Offset (ns)       = INT -400",
		"F3PLA. Extra Search Margin (ns) = INT 200",
		"F3PLA. Reject Margin (ns)       = INT 100",
#endif*/
#ifdef USE_SILICON_F3
		"F3 Si Detector Threshold (mV) = DOUBLE[16] :",
		"[0]	10",
		"[1]	10",
		"[2]	10",
		"[3]	10",
		"[4]	10",
		"[5]	10",
		"[6]	10",
		"[7]	10",
		"[8]	10",
		"[9]	10",
		"[10]	10",
		"[11]	10",
		"[12]	10",
		"[13]	10",
		"[14]	10",
		"[15]	10",

#endif

#ifdef USE_SILICON_ARYA
		"Si ary a Threshold (x16) = DOUBLE[32] :",
		"[0]	0",
		"[1]	0",
		"[2]	0",
		"[3]	0",
		"[4]	0",
		"[5]	0",
		"[6]	0",
		"[7]	0",
		"[8]	0",
		"[9]	0",
		"[10]	0",
		"[11]	0",
		"[12]	0",
		"[13]	0",
		"[14]	0",
		"[15]	0",
		"[16]	0",
		"[17]	0",
		"[18]	0",
		"[19]	0",
		"[20]	0",
		"[21]	0",
		"[22]	0",
		"[23]	0",
		"[24]	0",
		"[25]	0",
		"[26]	0",
		"[27]	0",
		"[28]	0",
		"[29]	0",
		"[30]	0",
		"[31]	0",
#endif

#ifdef USE_SILICON_ARYB
		"Si ary b Threshold (x16) = DOUBLE[32] :",
		"[0]	0",
		"[1]	0",
		"[2]	0",
		"[3]	0",
		"[4]	0",
		"[5]	0",
		"[6]	0",
		"[7]	0",
		"[8]	0",
		"[9]	0",
		"[10]	0",
		"[11]	0",
		"[12]	0",
		"[13]	0",
		"[14]	0",
		"[15]	0",
		"[16]	0",
		"[17]	0",
		"[18]	0",
		"[19]	0",
		"[20]	0",
		"[21]	0",
		"[22]	0",
		"[23]	0",
		"[24]	0",
		"[25]	0",
		"[26]	0",
		"[27]	0",
		"[28]	0",
		"[29]	0",
		"[30]	0",
		"[31]	0",
#endif

		"",
		NULL};




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

INT poll_event(INT source, INT count, BOOL test);
INT interrupt_configure(INT cmd, INT source, POINTER_T adr);

/*-- Equipment list ------------------------------------------------*/

//#ifdef USE_PPAC_F1 & USE_PPAC_F2
//#define TRIGGER_EQ_NAME PPAC_EQ_NAME_F1"+"PPAC_EQ_NAME_F2
//#endif


EQUIPMENT equipment[] = {
  {"KOBRA_Beamline_Detectors",            /* equipment name */
   {1, 0,                   /* event ID, trigger mask */
    "SYSTEM",               /* event buffer */
    EQ_POLLED,
    0,                       /* event source crate 0, all stations */
    "MIDAS",                /* format */
    TRUE,                   /* enabled */
    RO_RUNNING,             /* read only when running */
    10,
    0,                      /* stop run after this event limit */
    0,                      /* number of sub events */
    0,                      /* don't log history */
    "", "", "",},
	read_trigger_event,      /* readout routine */
  },

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


const unsigned int DATA_BUFF_SIZE=4*1024*1024;

uint32_t *f1_ppac_buff=NULL;
uint32_t *f1_plastic_buff=NULL;

uint32_t *f2_ppac_buff=NULL;

uint32_t *f3_silicon_buff=NULL;
uint32_t *f3_plastic_buff=NULL;
uint32_t *f3_ppac_buff=NULL;
uint32_t *silicon_arya_buff=NULL;
uint32_t *silicon_aryb_buff=NULL;
uint32_t *v2495_buff=NULL;



INT frontend_init() {




  INT ret;

  // Set ODB

  ret = set_odb(&hDB_obd, equipment[0].name, ts_str_kd, "kobra_detectors", &tr_set, sizeof(TRIGGER_SETTINGS));
  if ( ret != SUCCESS ) return ret;


  //VME initialization
  v2495_init(BHandle); // v2495 intialization take v2718 together
/*
        short Link=0;
  	short Device=0;
        CVBoardTypes VMEBoard = cvV2718;
        
        printf("\n1st V2718 initialization...");

  	//Make sure V2718 opened OK!
  	if(CAENVME_Init(VMEBoard, Link, Device, &BHandle)!=cvSuccess){
  		cm_msg(MERROR, "kobra_detector", "Failed to Open CAEN V2718_board1");
  		return FE_ERR_HW;
  	}
  	printf("Done\n");
*/
//	unsigned int Rotaryswitch;

	//CAENVME_ReadRegister(BHandle, static_cast<CVRegisters>(0x36), &Rotaryswitch);
	//printf("1st Controller rotary switch:%i\n", Rotaryswitch);


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

#ifdef USE_PPAC_F2
  	f2_ppac_init(BHandle);
#endif

#ifdef USE_PPAC_F3
  	f3_ppac_init(BHandle);
#endif

#ifdef USE_SILICON_F3
  	f3_silicon_init(BHandle, &tr_set);
#endif


  	//2nd controller initialization
  	printf("\n2nd V2718 initialization...");


  	printf("Done\n");

	CAENVME_ReadRegister(BHandle2, static_cast<CVRegisters>(0x36), &Rotaryswitch);
	printf("2nd Controller rotary switch:%i\n", Rotaryswitch);

#ifdef USE_SILICON_ARYA
  	silicon_arya_init(BHandle2, &tr_set);
#endif

#ifdef USE_SILICON_ARYB
  	silicon_aryb_init(BHandle2, &tr_set);
#endif




	printf("\n==========  Enjoy!\n");

  return SUCCESS;
}






/*-- Frontend Exit -------------------------------------------------*/

INT frontend_exit() {

#ifdef USE_PPAC_F1
	f1_ppac_exit();
	free(f1_ppac_buff);
#endif

#ifdef USE_PLASTIC_F1
	f1_plastic_exit();
	free(f1_plastic_buff);
#endif

#ifdef USE_PPAC_F2
	f2_ppac_exit();
	free(f2_ppac_buff);
#endif

#ifdef USE_SILICON_F3
	f3_silicon_exit(BHandle);
	free(f3_silicon_buff);
#endif

#ifdef USE_PLASTIC_F3
	f3_plastic_exit(BHandle);
	free(f3_plastic_buff);
#endif

#ifdef USE_PPAC_F3
	f3_ppac_exit();
	free(f3_ppac_buff);
#endif

#ifdef USE_SILICON_ARYA
	silicon_arya_exit(BHandle2);
	free(silicon_arya_buff);
#endif

#ifdef USE_SILICON_ARYB
	silicon_aryb_exit(BHandle2);
	free(silicon_aryb_buff);
#endif

#ifdef USE_V2495
	v2495_exit(BHandle);
	free(v2495_buff);
#endif

	return SUCCESS;
}

/*-- Begin of Run --------------------------------------------------*/

INT begin_of_run(INT run_number, char *error) {


//scaler_high=0; 
//scaler_low=0; 

//BOP=1;

#ifdef USE_PPAC_F1
	f1_ppac_buff= static_cast<uint32_t*>( malloc(DATA_BUFF_SIZE));
	f1_ppac_begin(run_number, error, &tr_set);
#endif

#ifdef USE_PPAC_F2
	f2_ppac_buff= static_cast<uint32_t*>( malloc(DATA_BUFF_SIZE));
	f2_ppac_begin(run_number, error, &tr_set);
#endif

#ifdef USE_SILICON_F3
	f3_silicon_buff= static_cast<uint32_t*>(malloc(DATA_BUFF_SIZE));
	f3_silicon_begin(BHandle, run_number, error, &tr_set);
#endif

#ifdef USE_PPAC_F3
	f3_ppac_buff= static_cast<uint32_t*>( malloc(DATA_BUFF_SIZE));
	f3_ppac_begin(run_number, error, &tr_set);
#endif


#ifdef USE_SILICON_ARYA
	silicon_arya_buff= static_cast<uint32_t*>(malloc(DATA_BUFF_SIZE));
	silicon_arya_begin(BHandle2, run_number, error, &tr_set);
#endif

#ifdef USE_SILICON_ARYB
	silicon_aryb_buff= static_cast<uint32_t*>(malloc(DATA_BUFF_SIZE));
	silicon_aryb_begin(BHandle2, run_number, error, &tr_set);
#endif

#ifdef USE_V2495
	v2495_buff = static_cast<uint32_t*>( malloc(DATA_BUFF_SIZE));
#endif



  return SUCCESS;
}

/*-- End of Run ----------------------------------------------------*/

INT end_of_run(INT run_number, char *error) {

#ifdef USE_PPAC_F1
	f1_ppac_end(run_number, error);
	free(f1_ppac_buff);
#endif


#ifdef USE_PPAC_F2
	f2_ppac_end(run_number, error);
	free(f2_ppac_buff);
#endif

#ifdef USE_SILICON_F3
	f3_silicon_end(BHandle, run_number, error);
	free(f3_silicon_buff);
#endif


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
  return SUCCESS;
}

/*-- Trigger event routines ----------------------------------------*/
//unsigned short tmp_Data = 0;
INT poll_event(INT source, INT count, BOOL test) {
        /*CVRegisters Reg;
	unsigned int Data;
	CAENVME_ReadRegister(BHandle, Reg, &Data);
	//printf("Scaler Accumulated:%d\n", Data);
	if(Data==0 && tmp_Data==0)
	{
	s_evt++;
	tmp_Data = 1;
	}
	//printf("Accumulated:%d\n", s_evt);*/
	uint lam=0;

/*
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
*/
        #ifdef USE_PPAC_F3
	lam=f3_ppac_check_fifo(BHandle);
	#endif
/*        
        #ifdef USE_PPAC_F1
	lam=f1_ppac_check_fifo(BHandle);
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

INT read_trigger_event(char *pevent, INT off) {

	bk_init(pevent);

#ifdef USE_PPAC_F1
	uint32_t *f1ppac_data;
	f1_ppac_read_event(BHandle, BANK_NAME_F1PPAC, pevent, off, f1_ppac_buff, DATA_BUFF_SIZE, f1ppac_data);
#endif

#ifdef USE_PPAC_F2
	uint32_t *f2ppac_data;
	f2_ppac_read_event(BHandle, BANK_NAME_F2PPAC, pevent, off, f2_ppac_buff, DATA_BUFF_SIZE, f2ppac_data);
#endif

#ifdef USE_PPAC_F3
	uint32_t *f3ppac_data;
	f3_ppac_read_event(BHandle, BANK_NAME_F3PPAC, pevent, off, f3_ppac_buff, DATA_BUFF_SIZE, f3ppac_data);
#endif

#ifdef USE_SILICON_F3
	uint32_t *f3silicon_data;
	f3_silicon_read_event(BHandle, BANK_NAME_F3SILICON, pevent, off, f3_silicon_buff, DATA_BUFF_SIZE, f3silicon_data);	
#endif

#ifdef USE_SILICON_ARYA
	uint32_t *siliconarya_data;
	silicon_arya_read_event(BHandle2, BANK_NAME_U1SILLICON, pevent, off, silicon_arya_buff, DATA_BUFF_SIZE, siliconarya_data);
#endif

#ifdef USE_SILICON_ARYB
	uint32_t *siliconaryb_data;
	silicon_aryb_read_event(BHandle2, BANK_NAME_U2SILLICON, pevent, off, silicon_aryb_buff, DATA_BUFF_SIZE, siliconaryb_data);
#endif

#ifdef USE_V2495
	uint32_t *v2495_data;
	v2495_read_event(BHandle, BANK_NAME_SCALER, pevent, off, v2495_buff, DATA_BUFF_SIZE, v2495_data);
#endif

	return bk_size(pevent);
}
