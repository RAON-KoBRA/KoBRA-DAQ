#include <stdio.h>

#include "mfe_common.h"

void seq_callback(INT hDB, INT hseq, void *info) {
  KEY key;
  db_get_key(hDB, hseq, &key);
  printf("odb ... %s touched (%d)\n", key.name, hseq);
}

INT set_odb(HNDLE *hDB, const char *eq_name,
            const char *odb_settings_str[], const char *file_name,
            void *rec_ptr, INT rec_size) {

  printf("\nSet ODB; /Equipment/%s/Settings...",eq_name);

  // get ODB
  int  status;
  cm_get_experiment_database(hDB, &status);

  // Map /equipment/<eq_name>/settings
  char set_str[200];
  sprintf(set_str, "/Equipment/%s/Settings", eq_name);

  // create the ODB Settings if not existing under Equipment/<eqname>/
  // If already existing, don't change anything
  status = db_create_record(*hDB, 0, set_str, strcomb(odb_settings_str));
  if ( status != DB_SUCCESS ) {
    cm_msg(MERROR, file_name, "cannot create record (%s)", set_str);
    return FE_ERR_ODB;
  }

  // Get the equipment/<eqname>/Settings key
  HNDLE hSubKey;
  status = db_find_key(*hDB, 0, set_str, &hSubKey);

  // Enable hot-link on /Settings of the equipment
  // Anything changed under /Settings will be triggering the callback (above)
  if ((status = db_open_record(*hDB, hSubKey, rec_ptr, rec_size,
                               MODE_READ, seq_callback, NULL)) != DB_SUCCESS) {
    cm_msg(MERROR, file_name, "cannot open record (%s)", set_str);
    return CM_DB_ERROR;
  }

  //setting logger dir
  char *dir="/home/kobradaq/online/data";
  db_set_value(*hDB, 0, "/Logger/Data dir", dir, sizeof(char[256]), 1, TID_STRING);
  printf("Done\n");

  return SUCCESS;

}

