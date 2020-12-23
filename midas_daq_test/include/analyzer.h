/********************************************************************\

  Name:         analyzer.h
  Created by:   Stefan Ritt

  Contents:     Analyzer global include file

  $Id:$

\********************************************************************/

/*-- Parameters ----------------------------------------------------*/

/* number of channels */
#define N_TDC_f1_PPAC               32
#define N_TDC_f2_PPAC               16
#define N_TDC_f3_ppac               32
#define N_ADC_f3_silicon            16

#define N_ADC_arya_silicon            32
#define N_ADC_aryb_silicon            32


/*-- Histo ID bases ------------------------------------------------*/

#define ADCCALIB_ID_BASE 1000
#define ADCSUM_ID_BASE   2000


#define F3SILICON_N_BINS 4096
#define F3SILICON_X_LOW 0
#define F3SILICON_X_HIGH 4096

#define F3SILICONARYA_N_BINS 5100
#define F3SILICONARYA_X_LOW 0
#define F3SILICONARYA_X_HIGH 5100

#define F3SILICONARYB_N_BINS 5100
#define F3SILICONARYB_X_LOW 0
#define F3SILICONARYB_X_HIGH 5100

#define PTAC_N_BINS 1000
#define PTAC_X_LOW  10
#define PTAC_X_HIGH 1000
