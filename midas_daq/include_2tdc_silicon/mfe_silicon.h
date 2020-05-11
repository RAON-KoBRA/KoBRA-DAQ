#pragma once
#define _MFE_silicon_INCLUDE_H


#include"odb_trigger.h"


typedef struct{
	int global_geo;
	int global_count;
	int channel;
	int tdc;
	int trailer_wcount;
}EVENT_DATA_SILICON;


INT f3_silicon_init(int32_t BHandle, TRIGGER_SETTINGS *ts);

INT f3_silicon_exit(int32_t BHandle);

INT f3_silicon_begin(int32_t BHandle, INT run_number, char *error, TRIGGER_SETTINGS *ts);

INT f3_silicon_end(int32_t BHandle, INT run_number, char *error);

INT f3_silicon_pause(INT run_number, char *error);

INT f3_silicon_resume(INT run_number, char *error);

INT f3_silicon_check_fifo(int32_t BHandle);

INT f3_silicon_read_fifo(int32_t BHandle, void *buff_tmp, int size);

INT f3_silicon_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);

INT f3_silicon_event(uint32_t *ev_buff, int size, EVENT_DATA_SILICON *ev);

BOOL f3_silicon_clear_buffer(int32_t BHandle);



