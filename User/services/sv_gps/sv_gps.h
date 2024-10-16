#ifndef _SV_GPS_H_
#define _SV_GPS_H_

#include "sm_l76x_gps.h"

sm_l76x_t* gps_drv = NULL;
static inline void sv_gps_init(sm_l76x_t* driver){
	if (!driver) return;
	gps_driver = driver;
}

static inline float sv_gps_get_lat(){
	if (!gps_drv) return 0.0;

	return sm_l76x_get_lat(gps_drv);
}

static inline float sv_gps_get_lon(){
	if (!gps_drv) return 0.0;

	return sm_l76x_get_lon(gps_drv);
}

static inline void sv_gps_process(){
	return sm_l76x_process(gps_drv);
}

#endif
