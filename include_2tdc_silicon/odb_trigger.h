#pragma once
#define ODB_TRIGGER_H

#include "detector_conf.h"

typedef struct {
#ifdef USE_PPAC_F1
	int f1_window_width;
	int f1_window_offset;
	int f1_extra_search_margin;
	int f1_reject_margin;
#endif
#ifdef USE_PPAC_F2
	int f2_window_width;
	int f2_window_offset;
	int f2_extra_search_margin;
	int f2_reject_margin;
#endif
#ifdef USE_SILICON_F3
	double f3_si_th[32];
#endif

} TRIGGER_SETTINGS;

