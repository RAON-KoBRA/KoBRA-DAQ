//###############################################################################################################################
//#																#
//						kobra header file
//
//		Create backend space for the monitoring V2495 FPGA(Scaler & Timestamp) and User system
//		ver. 12.24.20, Dong-Yeup Lee, PH.D student at the Korea Univ.
//
//
//
//
//#																#
//###############################################################################################################################



#ifndef _KOBRA_H_
#define _KOBRA_H_



typedef struct {
   double events;
   double events_per_sec;
   double data_per_sec;
} USER_STATS;


typedef struct userD *PEQUIPMENT1;

#define EQUIPMENT_SCAL_STR2 "\
Events sent = DOUBLE : 0\n\
Events per sec. = DOUBLE : 0\n\
Data per sec. = DOUBLE : 0\n\
"

typedef struct bline {
   char name2[NAME_LENGTH];              /**< Equipment name                            */
   EQUIPMENT_INFO info2;                 /**< From above                                */
    INT(*readout) (char *, INT);        /**< Pointer to user readout routine           */
    INT(*cd) (INT cmd2, PEQUIPMENT1);     /**< Class driver routine                      */
   DEVICE_DRIVER *driver2;               /**< Device driver list                        */
   void *event_descrip2;                 /**< Init string for fixed events or bank list */
   void *cd_info2;                       /**< private data for class driver             */
   INT Status2;                          /**< One of FE_xxx                             */
   DWORD last_called2;                   /**< Last time event was read                  */
   DWORD last_idle2;                     /**< Last time idle func was called            */
   DWORD poll_count2;                    /**< Needed to poll 'period'                   */
   INT format2;                          /**< FORMAT_xxx                                */
   HNDLE buffer_handle2;                 /**< MIDAS buffer handle                       */
   HNDLE hkey_variables2;                /**< Key to variables subtree in ODB           */
   DWORD serial_number2;                 /**< event serial number                       */
   DWORD subevent_number2;               /**< subevent number                           */
   DWORD odb_out2;                       /**< # updates FE -> ODB                       */
   DWORD odb_in2;                        /**< # updated ODB -> FE                       */
   DWORD bytes_sent2;                    /**< number of bytes sent                      */
   DWORD events_sent2;                   /**< number of events sent                     */
   USER_STATS dstat;
} USER_DETECTOR;



USER_DETECTOR detector[]={
 {"Trigger2 (Detector)",            /* equipment name */
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

