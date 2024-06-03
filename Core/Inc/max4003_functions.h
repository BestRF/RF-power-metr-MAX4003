
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAX4003_H__
#define __MAX4003_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

//-------------------------------my define ---------------------
#define DBM_SIZE      64     //calibration array size dbm_mhz[DBM_SIZE][FREQ_SIZE]
#define FREQ_SIZE      4
#define ADC_SAMPLES   100   //ADC samples per one mesurement with period ~3us // if you want <3us ADC period please use DMA ADC block
#define N_SAMPLES     100   //N_SAMPLES * ADC_SAMPLES =    samples per one mesurement


//-------------------------------my structura ---------------------
typedef struct{
	uint32_t freq;     //  expected freq (get it from external device)
	float    rf_power; // not used now!!
	uint8_t  command ; // recieved comand from VCP
} _Parametrs;



typedef struct {
	uint32_t adc_aver;   //calculated aver RF power  in the max40003  modul
	uint32_t adc_pick;   //calculeted pick FR power
} ADC_conv_return;


//-------------------------------my  variable  ---------------------
extern const int16_t dbm_mhz[DBM_SIZE][FREQ_SIZE];
float get_rf_power_dbm(uint32_t freq) ;
void VCP_Tx_Transmit_HS(uint8_t* Buf, uint16_t  Length) ;
extern _Parametrs parametrs ;



#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */





