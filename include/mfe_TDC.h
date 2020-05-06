#pragma once
#define _MFE_PPAC_INCLUDE_H


#include"odb_trigger.h"


typedef struct{
	int global_geo;
	int global_count;
	int channel;
	int tdc;
	int trailer_wcount;
}EVENT_DATA_PPAC;


//=============f1 ppac readout function
INT f1_ppac_init(int32_t BHandle);
INT f1_ppac_exit();
INT f1_ppac_begin(INT run_number, char *error, TRIGGER_SETTINGS *ts);
INT f1_ppac_end(INT run_number, char *error);
INT f1_ppac_pause(INT run_number, char *error);
INT f1_ppac_resume(INT run_number, char *error);
INT f1_ppac_check_fifo(int32_t BHandle);
INT f1_ppac_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT f1_ppac_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT f1_ppac_process_event(uint32_t *ev_buff, int size, EVENT_DATA_PPAC *ev);

//=============f1 plastic readout function
INT f1_plastic_init(int32_t BHandle);
INT f1_plastic_exit();
INT f1_plastic_begin(INT run_number, char *error, TRIGGER_SETTINGS *ts);
INT f1_plastic_end(INT run_number, char *error);
INT f1_plastic_pause(INT run_number, char *error);
INT f1_plastic_resume(INT run_number, char *error);
INT f1_plastic_check_fifo(int32_t BHandle);
INT f1_plastic_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT f1_plastic_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT f1_plastic_process_event(uint32_t *ev_buff, int size, EVENT_DATA_PPAC *ev);

//=============f2 ppac function
INT f2_ppac_init(int32_t BHandle);
INT f2_ppac_exit();
INT f2_ppac_begin(INT run_number, char *error, TRIGGER_SETTINGS *ts);
INT f2_ppac_end(INT run_number, char *error);
INT f2_ppac_pause(INT run_number, char *error);
INT f2_ppac_resume(INT run_number, char *error);
INT f2_ppac_check_fifo(int32_t BHandle);
INT f2_ppac_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT f2_ppac_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT f2_ppac_process_event(uint32_t *ev_buff, int size, EVENT_DATA_PPAC *ev);

//=============f3 ppac function
INT f3_ppac_init(int32_t BHandle);
INT f3_ppac_exit();
INT f3_ppac_begin(INT run_number, char *error, TRIGGER_SETTINGS *ts);
INT f3_ppac_end(INT run_number, char *error);
INT f3_ppac_pause(INT run_number, char *error);
INT f3_ppac_resume(INT run_number, char *error);
INT f3_ppac_check_fifo(int32_t BHandle);
INT f3_ppac_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT f3_ppac_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT f3_ppac_process_event(uint32_t *ev_buff, int size, EVENT_DATA_PPAC *ev);

//=============f3 plastic readout function
INT f3_plastic_init(int32_t BHandle);
INT f3_plastic_exit();
INT f3_plastic_begin(INT run_number, char *error, TRIGGER_SETTINGS *ts);
INT f3_plastic_end(INT run_number, char *error);
INT f3_plastic_pause(INT run_number, char *error);
INT f3_plastic_resume(INT run_number, char *error);
INT f3_plastic_check_fifo(int32_t BHandle);
INT f3_plastic_read_fifo(int32_t BHandle, void *buff_tmp, int size);
INT f3_plastic_read_event(int32_t BHandle, const char *bank_name, char *pevent, INT off, uint32_t *buff, int size, uint32_t *pdata);
INT f3_plastic_process_event(uint32_t *ev_buff, int size, EVENT_DATA_PPAC *ev);
