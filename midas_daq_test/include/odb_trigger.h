#pragma once
#define ODB_TRIGGER_H

#include "detector_conf.h"

typedef struct {
#ifdef USE_PPAC_F1
	int f1_ppac_window_width;
	int f1_ppac_window_offset;
	int f1_ppac_extra_search_margin;
	int f1_ppac_reject_margin;
#endif
#ifdef USE_PLASTIC_F1
	int f1_pla_window_width;
	int f1_pla_window_offset;
	int f1_pla_extra_search_margin;
	int f1_pla_reject_margin;
#endif
#ifdef USE_PPAC_F2
	int f2_ppac_window_width;
	int f2_ppac_window_offset;
	int f2_ppac_extra_search_margin;
	int f2_ppac_reject_margin;
#endif
#ifdef USE_PPAC_F3
	int f3_ppac_window_width;
	int f3_ppac_window_offset;
	int f3_ppac_extra_search_margin;
	int f3_ppac_reject_margin;
#endif
#ifdef USE_PLASTIC_F3
	int f3_pla_window_width;
	int f3_pla_window_offset;
	int f3_pla_extra_search_margin;
	int f3_pla_reject_margin;
#endif
#ifdef USE_SILICON_F3
	double f3_si_th[16];
#endif
#ifdef USE_SILICON_ARYA
	double si_arya_th[32];
#endif
#ifdef USE_SILICON_ARYB
	double si_aryb_th[32];
#endif
} TRIGGER_SETTINGS;


