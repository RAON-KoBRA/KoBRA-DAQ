#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "midas.h"
#include "msystem.h"

#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include <memory.h>
#include <ctype.h>

#include "CAENVMEtypes.h"

#include "odb_trigger.h"
#include "ppac_conf.h"
#include "mfe_ppac.h"
#include "mfe_common.h"

static HNDLE hDB_trigger;
static TRIGGER_SETTINGS ts_trigger;

static const char *trigger_settings_str[]={
		"[.]",
#ifdef USE_PPAC_F1
		"F1 Window Width (ns)		 = INT 800",
		"F1 Window Offset (ns)       = INT 0",
		"F1 Extra Search Margin (ns) = INT 0",
		"F1 Reject Margin (ns)       = INT 10",
#endif
#ifdef USE_PPAC_F2
		"F2 Window Width (ns)		 = INT 800",
		"F2 Window Offset (ns)       = INT 0",
		"F2 Extra Search Margin (ns) = INT 0",
		"F2 Reject Margin (ns)       = INT 10",
#endif
		"",
		NULL};

/* make frontend functions callable from the C framework */
#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef USE_PPAC_F1
#define TRIGGER_EQ_NAME PPAC_EQ_NAME_F1
#endif

#ifdef USE_PPAC_F2
#define TRIGGER_EQ_NAME PPAC_EQ_NAME_F2
#endif

//#ifdef USE_PPAC_F1 & USE_PPAC_F2
//#define TRIGGER_EQ_NAME PPAC_EQ_NAME_F1"+"PPAC_EQ_NAME_F2
//#endif


EQUIPMENT equipment[] = {
  {TRIGGER_EQ_NAME,            /* equipment name */
   {1, 0,                   /* event ID, trigger mask */
    "SYSTEM",               /* event buffer */
    EQ_POLLED,
    0,                       /* event source crate 0, all stations */
    "MIDAS",                /* format */
    TRUE,                   /* enabled */
    RO_RUNNING,             /* read only when running */
    1,
    0,                      /* stop run after this event limit */
    0,                      /* number of sub events */
    0,                      /* don't log history */
    "", "", "",},
   read_trigger_event,      /* readout routine */
  },
   {""}
};

#ifdef __cplusplus
}
#endif

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

  end_of_run:     Called on a request to stop a run. Can send
                  end-of-run event and close run gates.

  pause_run:      When a run is paused. Should disable trigger events.

  resume_run:     When a run is resumed. Should enable trigger events.
\********************************************************************/

/*-- Frontend Init -------------------------------------------------*/

static int32_t BHandle;

const unsigned int DATA_BUFF_SIZE=4*1024*1024;
uint32_t *data_buff=NULL;

INT frontend_init() {

  INT ret;

  // Set ODB
  ret = set_odb(&hDB_trigger, equipment[0].name,
		  trigger_settings_str, "trigger",
		&ts_trigger, sizeof(TRIGGER_SETTINGS));
  if ( ret != SUCCESS ) return ret;

  //VME initialization

  	//hardware initialization
  	printf("\nV1718 initialization...");
  	CVBoardTypes VMEBoard = cvV1718;
  	short Link=0;
  	short Device=0;

  	//Make sure V1718 opened OK!
  	if(CAENVME_Init(VMEBoard, Link, Device, &BHandle)!=cvSuccess){
  		cm_msg(MERROR, "PPAC", "Failed to Open CAEN V1718");
  		return FE_ERR_HW;
  	}

  	printf("Done\n");

  	//=====================F1 ppac=================================//
#ifdef USE_PPAC_F1
  	f1_ppac_init(BHandle);
#endif

#ifdef USE_PPAC_F2
  	f2_ppac_init(BHandle);
#endif

	printf("\n==========  Enjoy!\n");

  return SUCCESS;
}

/*-- Frontend Exit -------------------------------------------------*/

INT frontend_exit() {
#ifdef USE_PPAC_F1
	f1_ppac_exit();
#endif

#ifdef USE_PPAC_F2
	f2_ppac_exit();
#endif

	return SUCCESS;
}

/*-- Begin of Run --------------------------------------------------*/

INT begin_of_run(INT run_number, char *error) {

	data_buff=malloc(DATA_BUFF_SIZE);
	//data_buff=new uint32_t(DATA_BUFF_SIZE);

#ifdef USE_PPAC_F1
	f1_ppac_begin(run_number, error, &ts_trigger);
#endif

#ifdef USE_PPAC_F2
	f2_ppac_begin(run_number, error, &ts_trigger);
#endif

  return SUCCESS;
}

/*-- End of Run ----------------------------------------------------*/

INT end_of_run(INT run_number, char *error) {

#ifdef USE_PPAC_F1
	f1_ppac_end(run_number, error);
#endif

#ifdef USE_PPAC_F2
	f2_ppac_end(run_number, error);
#endif

	free(data_buff);
//	delete data_buff;

  return SUCCESS;
}

/*-- Pause Run -----------------------------------------------------*/

INT pause_run(INT run_number, char *error) {

 // tigress_pause(run_number,error);

  return SUCCESS;
}

/*-- Resuem Run ----------------------------------------------------*/

INT resume_run(INT run_number, char *error) {


 // tigress_resume(run_number,error);

  return SUCCESS;
}

/*-- Frontend Loop -------------------------------------------------*/

INT frontend_loop() {
  /* if frontend_call_loop is true, this routine gets called when
     the frontend is idle or once between every event */
  return SUCCESS;
}

/*----------------INT f1_ppac_check_clear_mode();
INT f2_ppac_check_clear_mode();
 * --------------------------------------------------*/

/*****************************mfe_ppac_sub***************************************\

  Readout routines for different events

\********************************************************************/

/*-- Trigger event routines ----------------------------------------*/

int16_t event_stored_f1, event_stored_f2, event_stored_f3;

INT poll_event(INT source, INT count, BOOL test) {

	event_stored_f1=0; event_stored_f2=0; event_stored_f3=0;
	INT thresh_hold=0;

#ifdef USE_PPAC_F1
	event_stored_f1=f1_ppac_check_fifo(BHandle);
#endif

#ifdef USE_PPAC_F2
	event_stored_f2=f2_ppac_check_fifo(BHandle);
#endif


	if(event_stored_f1+event_stored_f2+event_stored_f3>thresh_hold)
	{
		//printf("f1:%d, f2:%d, f3:%d\n", event_stored_f1, event_stored_f2, event_stored_f3);
		return SUCCESS;
	}

  return FALSE;
}

/*-- Interrupt configuration ---------------------------------------*/

INT interrupt_configure(INT cmd, INT source, POINTER_T adr) {
#if 0
  switch (cmd) {
  case CMD_INTERRUPT_ENABLE:libtigfunc.o
    break;
  case CMD_INTERRUPT_DISABLE:
    break;
  case CMD_INTERRUPT_ATTACH:
    break;
  case CMD_INTERRUPT_DETACH:
    break;
  }
#endif
  return SUCCESS;
}

/*-- Event readout -------------------------------------------------*/

INT read_trigger_event(char *pevent, INT off) {

  uint32_t *pdata;

  bk_init(pevent);
  if(event_stored_f1>0){f1_ppac_read_event(BHandle, "F1PA", pevent, off, data_buff, DATA_BUFF_SIZE, pdata);}
 // if(event_stored_f2>0){f2_ppac_read_event(BHandle, "F1PA", pevent, off, data_buff, DATA_BUFF_SIZE, pdata);}
//  if(event_stored_f3>0){f2_ppac_read_event(BHandle, "F2PA", pevent, off, data_buff, DATA_BUFF_SIZE, pdata);}
 //
 // f2_ppac_read_event(BHandle, "F2PA", pevent, off, data_buff, DATA_BUFF_SIZE, pdata);

  return bk_size(pevent);
}

