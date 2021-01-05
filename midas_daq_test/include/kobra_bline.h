//###############################################################################################################################
//#																#
//						kobra header file
//
//		Create backend space for the Beamline detectors status monitoring
//		ver. 12.25.20, Dong-Yeup Lee, PH.D student at the Korea Univ.
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
} BEAMLINE_STATS_ONLY;


typedef struct blineo *PEQUIPMENT2;

#define EQUIPMENT_SCAL_STR3 "\
Events sent = DOUBLE : 0\n\
Events per sec. = DOUBLE : 0\n\
Data per sec. = DOUBLE : 0\n\
"

typedef struct blineo {
   char name3[NAME_LENGTH];              /**< Equipment name                            */
   EQUIPMENT_INFO info3;                 /**< From above                                */
    INT(*readout) (char *, INT);        /**< Pointer to user readout routine           */
    INT(*cd) (INT cmd3, PEQUIPMENT2);     /**< Class driver routine                      */
   DEVICE_DRIVER *driver3;               /**< Device driver list                        */
   void *event_descrip3;                 /**< Init string for fixed events or bank list */
   void *cd_info3;                       /**< private data for class driver             */
   INT Status3;                          /**< One of FE_xxx                             */
   DWORD last_called3;                   /**< Last time event was read                  */
   DWORD last_idle3;                     /**< Last time idle func was called            */
   DWORD poll_count3;                    /**< Needed to poll 'period'                   */
   INT format3;                          /**< FORMAT_xxx                                */
   HNDLE buffer_handle3;                 /**< MIDAS buffer handle                       */
   HNDLE hkey_variables3;                /**< Key to variables subtree in ODB           */
   DWORD serial_number3;                 /**< event serial number                       */
   DWORD subevent_number3;               /**< subevent number                           */
   DWORD odb_out3;                       /**< # updates FE -> ODB                       */
   DWORD odb_in3;                        /**< # updated ODB -> FE                       */
   DWORD bytes_sent3;                    /**< number of bytes sent                      */
   DWORD events_sent3;                   /**< number of events sent                     */
   BEAMLINE_STATS_ONLY scal3;
} BEAMLINEONLY;


BEAMLINEONLY beamlineonly[] = {
  {"Trigger3 (Beamline only)",            /* equipment name */
   {1, 0,                   /* event ID, trigger mask */
    "SYSTEM",               /* event buffer */
    EQ_PERIODIC,
    0,                       /* event source crate 0, all stations */
    "MIDAS",                /* format */
    TRUE,                   /* enabled */
    RO_RUNNING |
    RO_ODB,             /* read only when running */
    1000,
    0,                      /* stop run after this event limit */
    0,                      /* number of sub events */
    0,                      /* don't log history */
    "", "", "",},
        0,      /* readout routine */
  },
};


#endif

