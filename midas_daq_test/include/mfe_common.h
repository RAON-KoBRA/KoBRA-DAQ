#pragma once
#include "midas.h"

void seq_callback(INT hDB, INT hseq, void *info);

INT set_odb(HNDLE *hDB, const char *eq_name,
	    const char *odb_settings_str[], const char *file_name,
	    void *rec_ptr, INT rec_size);


