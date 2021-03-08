#ifndef __ISR_H
#define __ISR_H 

//extern uint16 last_adcval,adcval;
extern char pit_5ms_flag;
extern char pit_10ms_flag;
extern char pit_25ms_flag;
extern char pit_50ms_flag;
extern char pit_500ms_flag;
extern char pit_5s_flag;

extern short acc_chip_out[3];                   //data the chip gives
extern short gyro_chip_out[3];                   //data the chip gives
extern short cps_chip_out[3];                   //data the chip gives

#endif