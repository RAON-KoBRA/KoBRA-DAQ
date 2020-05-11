#pragma once
#define ODB_TRIGGER_H

#include "ppac_conf.h"

typedef struct {
#ifdef USE_PPAC_F1
	INT f1_window_width;
	INT f1_window_offset;
	INT f1_extra_search_margin;
	INT f1_reject_margin;
#endif
#ifdef USE_PPAC_F2
	INT f2_window_width;
	INT f2_window_offset;
	INT f2_extra_search_margin;
	INT f2_reject_margin;
#endif


} TRIGGER_SETTINGS;
