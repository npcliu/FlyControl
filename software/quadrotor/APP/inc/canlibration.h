#ifndef __CALIBRATION_H
#define __CALIBRATION_H

char GyroCalibration(void);
void CompassCalibration(int ristrict);
char AccCalibration(PACC pacc,PGYRO pgyro);

#endif
