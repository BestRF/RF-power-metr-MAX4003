


//--- max 4003 functions modul------------------------------

#include "max4003_functions.h"
#include "adc.h"

//-----------calibration table
const int16_t dbm_mhz[DBM_SIZE][FREQ_SIZE] ={ {864,864,     915,915},
	                	{4000,+3,  4000,+3, },  // > 2dbm = 2 dbm

		                {2136,+2,  2128,+2},
					    {2128,+1,  2121,+1},
		                {2117, 0,  2111, 0},
					    {2106,-1,  2101,-1},
                        {2095,-2,  2090,-2},
					    {2084,-3,  2079,-3},
		                {2072,-4,  2065,-4},
					    {2057,-5,  2050,-5},
                        {2038,-6,  2030,-6},
	     			    {2017,-7,  2008,-7},
	                    {1993,-8,  1982,-8},
		    		    {1966,-9,  1954,-9},
                        {1937,-10, 1924,-10},
	     			    {1906,-11, 1891,-11},
	                    {1870,-12, 1855,-12},
		    		    {1836,-13, 1821,-13},
                        {1803,-14, 1787,-14},
	     			    {1769,-15, 1754,-15},
	                    {1738,-16, 1723,-16},
		    		    {1707,-17, 1693,-17},
                        {1672,-18, 1658,-18},
	     			    {1638,-19, 1624,-19},
	                    {1604,-20, 1589,-20},
		    		    {1569,-21, 1554,-21},
						{1533,-22, 1517,-22},
						{1495,-23, 1485,-23},
						{1460,-24, 1450,-24},
						{1425,-25, 1415,-25},
						{1390,-26, 1381,-26},
						{1357,-27, 1348,-27},
						{1325,-28, 1315,-28},
						{1289,-29, 1281,-29},
                        {1256,-30, 1248,-30},
						{1221,-31, 1213,-31},
						{1186,-32, 1180,-32},
						{1151,-33, 1144,-33},
                        {1116,-34, 1109,-34},
						{1088,-35, 1082,-35},
						{1053,-36, 1047,-36},
						{1019,-37, 1012,-37},
                        {986, -38, 978, -38},
						{954, -39, 946, -39},
						{922, -40, 914, -40},
						{884, -41, 882, -41},
                        {851, -42, 848, -42},
						{818, -43, 814, -43},
						{784, -44, 780, -44},
						{750, -45, 746, -45},
                        {718, -46, 713, -46},
						{694, -47, 685, -47},
						{664, -48, 655, -48},
						{636, -49, 628, -49},
                        {610, -50, 602, -50},
						{587, -51, 580, -51},
						{566, -52, 560, -52},
						{537, -53, 535, -53},
                        {522, -54, 519, -54},
						{508, -55, 506, -55},
						{497, -56, 495, -56},
						{488, -57, 486, -57},
                        {480, -58, 479, -58},
                        {1  , -59, 1   ,-59}  //  >58 dbm = 58 dbm

};




//----- this function return aver and pick values of n_sampes
static ADC_conv_return ADC_ret_period(uint32_t n_samples) {
	ADC_conv_return ret;
	uint32_t adc_value;
	uint32_t adc_samp_temp = 0;
	uint32_t adc_pick_temp = 0;

	for(uint32_t i = 0; i < n_samples; i++) {
		//------------------------------------------------------1 tik 3 us
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		adc_value = HAL_ADC_GetValue(&hadc1);
		adc_samp_temp += adc_value;
		if(adc_value > adc_pick_temp)
			adc_pick_temp = adc_value;
	}

	ret.adc_pick = adc_pick_temp;
	ret.adc_aver = adc_samp_temp / n_samples;
	return ret;
}








float get_rf_power_dbm(uint32_t freq) {
	ADC_conv_return adc_ret;
	float delta;
	float rf_level = 0;
	uint32_t n_samples = 600;
	uint32_t best_freq = 0;
	uint32_t delta_freq = 1000000;   //  reset state
	uint32_t delta_freq_best = 1000000;   //  reset state

	//----- found the nearest freq from calibration table----
	//---if you use wide band  range you can add new freq and calculated  interpolation for
	//---other freq
	for(uint16_t i = 0; i < FREQ_SIZE; i += 2) {
		if(freq > (uint32_t) dbm_mhz[0][i])
			delta_freq = freq - (uint32_t) dbm_mhz[0][i];
		else
			delta_freq = (uint32_t) dbm_mhz[0][i] - freq;

		if(delta_freq < delta_freq_best) {
			delta_freq_best = delta_freq;
			best_freq = i;
		}
	}

	for(uint32_t i = 0; i < n_samples; i++) {
		adc_ret = ADC_ret_period(ADC_SAMPLES);
		for(uint16_t i = 2; i < DBM_SIZE; i++) {
			if(adc_ret.adc_aver > dbm_mhz[i][best_freq]) {
				delta = -(1.0 / (-dbm_mhz[i - 1][best_freq] + dbm_mhz[i][best_freq]));
				rf_level += (dbm_mhz[i - 1][best_freq + 1] - (delta * (dbm_mhz[i - 1][best_freq] - (float) adc_ret.adc_aver)));
				break;
			}
		}
	}

	return (rf_level / n_samples);
}


//---My Tx VCP  TX
void VCP_Tx_Transmit_HS(uint8_t* Buf, uint16_t  Length) {
	while (CDC_Wait_End_Tx()) {
	};
	CDC_Transmit_HS(Buf,  Length);
}







