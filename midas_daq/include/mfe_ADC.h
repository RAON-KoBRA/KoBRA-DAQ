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


INT f3_silicon_init(int32_t BHandle, TRIGGER_SETTINGS_BEAMLINE *ts);
INT f3_silicon_exit(int32_t BHandle);
INT f3_silicon_begin(int32_t BHandle, INT run_number, char *error, TRIGGER_SETTINGS_BEAMLINE *ts);
INT f3_silicon_end(int32_t BHandle, INT run_number, char *error);
INT f3_silicon_pause(INT run_number, char *error);
INT f3_silicon_resume(INT run_number, char *error);
INT f3_silicon_check_fifo(int32_t BHandle);
INT f3_silicon_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT f3_silicon_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT f3_silicon_event(uint32_t *ev_buff, int size, EVENT_DATA_SILICON *ev);
BOOL f3_silicon_clear_buffer(int32_t BHandle);


//============ silicon array

INT silicon_arya_init(int32_t BHandle2, TRIGGER_SETTINGS_SILICONARY *ts);
INT silicon_arya_exit(int32_t BHandle);
INT silicon_arya_begin(int32_t BHandle, INT run_number, char *error, TRIGGER_SETTINGS_SILICONARY *ts);
INT silicon_arya_end(int32_t BHandle, INT run_number, char *error);
INT silicon_arya_pause(INT run_number, char *error);
INT silicon_arya_resume(INT run_number, char *error);
INT silicon_arya_check_fifo(int32_t BHandle);
INT silicon_arya_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT silicon_arya_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT silicon_arya_event(uint32_t *ev_buff, int size, EVENT_DATA_SILICON *ev);
BOOL silicon_arya_clear_buffer(int32_t BHandle);

INT silicon_aryb_init(int32_t BHandle2, TRIGGER_SETTINGS_SILICONARY *ts);
INT silicon_aryb_exit(int32_t BHandle);
INT silicon_aryb_begin(int32_t BHandle, INT run_number, char *error, TRIGGER_SETTINGS_SILICONARY *ts);
INT silicon_aryb_end(int32_t BHandle, INT run_number, char *error);
INT silicon_aryb_pause(INT run_number, char *error);
INT silicon_aryb_resume(INT run_number, char *error);
INT silicon_aryb_check_fifo(int32_t BHandle);
INT silicon_aryb_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT silicon_aryb_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT silicon_aryb_event(uint32_t *ev_buff, int size, EVENT_DATA_SILICON *ev);
BOOL silicon_aryb_clear_buffer(int32_t BHandle);


