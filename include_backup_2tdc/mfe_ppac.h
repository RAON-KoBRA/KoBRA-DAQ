#pragma once
#define _MFE_PPAC_INCLUDE_H


#include"odb_trigger.h"


typedef struct{
	int global_geo;
	int global_count;
	int channel;
	int tdc;
	int trailer_wcount;
}EVENT_DATA;


INT f1_ppac_init(int32_t BHandle);
INT f2_ppac_init(int32_t BHandle);

INT f1_ppac_exit();
INT f2_ppac_exit();

INT f1_ppac_begin(INT run_number, char *error, TRIGGER_SETTINGS *ts);
INT f2_ppac_begin(INT run_number, char *error, TRIGGER_SETTINGS *ts);

INT f1_ppac_end(INT run_number, char *error);
INT f2_ppac_end(INT run_number, char *error);

INT f1_ppac_pause(INT run_number, char *error);
INT f2_ppac_pause(INT run_number, char *error);

INT f1_ppac_resume(INT run_number, char *error);
INT f2_ppac_resume(INT run_number, char *error);

INT f1_ppac_check_fifo(int32_t BHandle);
INT f2_ppac_check_fifo(int32_t BHandle);

INT f1_ppac_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT f2_ppac_read_fifo(int32_t BHandle, void *buff_tmp, int size);

INT f1_ppac_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT f2_ppac_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);

INT f1_ppac_process_event(uint32_t *ev_buff, int size, EVENT_DATA *ev);
INT f2_ppac_process_event(uint32_t *ev_buff, int size, EVENT_DATA *ev);

