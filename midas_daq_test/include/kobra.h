//###############################################################################################################################
//#																#
//						kobra header file
//
//		Create backend space for the monitoring V2495 FPGA(Scaler & Timestamp) and User system
//		ver. 10.08.20, Dong-Yeup Lee, PH.D student at the Korea Univ.
//
//
//
//
//#																#
//###############################################################################################################################




#ifndef _KOBRA_H_
#define _KOBRA_H_



typedef struct {
   double total_events_sent;
   double total_events_per_sec;
   double livetime;
   double total_events_sent2;
   double total_events_per_sec2;
   double livetime2;
} BEAMLINE_STATS;


typedef struct bline *PEQUIPMENT0;

#define EQUIPMENT_SCAL_STR "\
Total Events sent = DOUBLE : 0\n\
Total Events per sec. = DOUBLE : 0\n\
Livetime = DOUBLE : 0\n\
2Total Events sent = DOUBLE : 0\n\
2Total Events per sec. = DOUBLE : 0\n\
2Livetime = DOUBLE : 0\n\
"

typedef struct bline {
   char name[NAME_LENGTH];              /**< Equipment name                            */
   EQUIPMENT_INFO info1;                 /**< From above                                */
    INT(*readout) (char *, INT);        /**< Pointer to user readout routine           */
    INT(*cd) (INT cmd1, PEQUIPMENT0);     /**< Class driver routine                      */
   DEVICE_DRIVER *driver1;               /**< Device driver list                        */
   void *event_descrip1;                 /**< Init string for fixed events or bank list */
   void *cd_info1;                       /**< private data for class driver             */
   INT Status1;                          /**< One of FE_xxx                             */
   DWORD last_called1;                   /**< Last time event was read                  */
   DWORD last_idle1;                     /**< Last time idle func was called            */
   DWORD poll_count1;                    /**< Needed to poll 'period'                   */
   INT format1;                          /**< FORMAT_xxx                                */
   HNDLE buffer_handle1;                 /**< MIDAS buffer handle                       */
   HNDLE hkey_variables1;                /**< Key to variables subtree in ODB           */
   DWORD serial_number1;                 /**< event serial number                       */
   DWORD subevent_number1;               /**< subevent number                           */
   DWORD odb_out1;                       /**< # updates FE -> ODB                       */
   DWORD odb_in1;                        /**< # updated ODB -> FE                       */
   DWORD bytes_sent1;                    /**< number of bytes sent                      */
   DWORD events_sent1;                   /**< number of events sent                     */
   BEAMLINE_STATS scal;
} BEAMLINE;


BEAMLINE beamline[] = {
  {"Trigger1 (Beamline)",            /* equipment name */
   {1, 0,                   /* event ID, trigger mask */
    "SYSTEM",               /* event buffer */
    EQ_PERIODIC,
    0,                       /* event source crate 0, all stations */
    "MIDAS",                /* format */
    TRUE,                   /* enabled */
    RO_RUNNING |
    RO_ODB,             /* read only when running */
    2000,
    0,                      /* stop run after this event limit */
    0,                      /* number of sub events */
    0,                      /* don't log history */
    "", "", "",},
        0,      /* readout routine */
  },
};


#endif

