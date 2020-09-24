#pragma once
#define ODB_TRIGGER_H

#include "detector_conf.h"

typedef struct {
//#ifdef USE_PPAC_F1
	int f1_ppac_window_width;
	int f1_ppac_window_offset;
	int f1_ppac_extra_search_margin;
	int f1_ppac_reject_margin;
//#endif
/*
//#ifdef USE_PLASTIC_F1
	int f1_pla_window_width;
	int f1_pla_window_offset;
	int f1_pla_extra_search_margin;
	int f1_pla_reject_margin;
//#endif
*/
//#ifdef USE_PPAC_F2
	int f2_ppac_window_width;
	int f2_ppac_window_offset;
	int f2_ppac_extra_search_margin;
	int f2_ppac_reject_margin;
//#endif
//#ifdef USE_PPAC_F3
	int f3_ppac_window_width;
	int f3_ppac_window_offset;
	int f3_ppac_extra_search_margin;
	int f3_ppac_reject_margin;
//#endif
/*
//#ifdef USE_PLASTIC_F3
	int f3_pla_window_width;
	int f3_pla_window_offset;
	int f3_pla_extra_search_margin;
	int f3_pla_reject_margin;
//#endif
*/
#ifdef USE_SILICON_F3
	double f3_si_th[32];
#endif
} TRIGGER_SETTINGS_BEAMLINE;

typedef struct {
#ifdef USE_SILICON_ARYA
	double si_arya_th[32];
#endif
#ifdef USE_SILICON_ARYB
	double si_aryb_th[32];
#endif
} TRIGGER_SETTINGS_SILICONARY;


static const char *ts_str_kd[]={
		"[.]",
//#ifdef USE_PPAC_F1
		"F1TDC Window Width (ns)		 = INT : 910",
		"F1TDC Window Offset (ns)       = INT : -400",
		"F1TDC Extra Search Margin (ns) = INT : 200",
		"F1TDC Reject Margin (ns)       = INT : 100",
//#endif
/*
//#ifdef USE_PLASTIC_F1
		"F1PLA. Window Width (ns)		 = INT : 920",
		"F1PLA. Window Offset (ns)       = INT : -400",
		"F1PLA. Extra Search Margin (ns) = INT : 200",
		"F1PLA. Reject Margin (ns)       = INT : 100",
//#endif
*/
//#ifdef USE_PPAC_F2
		"F2TDC Window Width (ns)		 = INT : 930",
		"F2TDC Window Offset (ns)       = INT : -400",
		"F2TDC Extra Search Margin (ns) = INT : 200",
		"F2TDC Reject Margin (ns)       = INT : 100",
//#endif
//#ifdef USE_PPAC_F3
		"F3TDC Window Width (ns)		 = INT : 940",
		"F3TDC Window Offset (ns)       = INT : -400",
		"F3TDC Extra Search Margin (ns) = INT : 200",
		"F3TDC Reject Margin (ns)       = INT : 100",
//#endif
/*
//#ifdef USE_PLASTIC_F3
		"F3PLA. Window Width (ns)		 = INT : 950",
		"F3PLA. Window Offset (ns)       = INT : -400",
		"F3PLA. Extra Search Margin (ns) = INT : 200",
		"F3PLA. Reject Margin (ns)       = INT : 100",
//#endif
*/
//#ifdef USE_SILICON_F3
		"Si beamline Threshold (x16) = DOUBLE[32] :",
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
//#endif

		"",
		NULL};

static const char *ts_str_kd_ary[]={
		"[.]",
//#ifdef USE_SILICON_ARY
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
//#endif
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

		"",
		NULL};
