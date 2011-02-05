/*
 * planner.h - cartesian trajectory planning and excution
 * Part of TinyG
 *
 *	Copyright (c) 2011 Alden S. Hart Jr.
 *
 *	TinyG is free software: you can redistribute it and/or modify
 *	it under the terms of the Creative Commons CC-BY-NC license 
 *	(Creative Commons Attribution Non-Commercial Share-Alike license)
 *	as published by Creative Commons. You should have received a copy 
 *	of the Creative Commons CC-BY-NC license along with TinyG.
 *	If not see http://creativecommons.org/licenses/
 *
 *	TinyG is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef planner_h
#define planner_h 

#include <string.h>				// needed for memset in clear_vector()

//#define MC_LINE mc_line
#define MC_LINE mc_aline

/*
 *	Most of these factors are the result of a lot of tweaking. 
 *	Change at your peril.
 */
#define MC_BUFFER_SIZE 8		// sub-move buffer pool (255 max)
#define MC_BUFFERS_NEEDED 4		// worst case write buffers needed for aline

#define MIN_LINE_LENGTH 0.01	// mm - smallest complete line it can handle
#define MIN_SEGMENT_TIME 10000	// microseconds - 10 ms. works well
#define MIN_SEGMENT_LENGTH 0.03	// accel/decel segments
#define MM_PER_ARC_SEGMENT 0.03	// set to produce ~10 ms segments

#define _mc_bump(a) ((a<MC_BUFFER_SIZE-1)?(a+1):0)	// buffer incr & wrap
#define _steps(x,a) round(a * CFG(x).steps_per_unit)

// ritorno is Italian for return - it returns only if an error occurred
uint8_t ritcode;	// defined once globally for ritorno
#define ritorno(a) if((ritcode=a) != TG_OK) { return(ritcode); }

#define clear_vector(a) memset(a,0,sizeof(a)) // used in motion_control.c & gcode.c
#define uSec(a)	(round(a * ONE_MINUTE_OF_MICROSECONDS))

/*
 * Global Scope Functions
 */

void mc_init(void);
//uint8_t mc_move_dispatcher(void);
uint8_t mc_move_dispatcher(uint8_t kill);

uint8_t mc_test_write_buffer(uint8_t count);
uint8_t mc_queue_write_buffer(uint8_t move_type);
uint8_t mc_end_run_buffer(void);
void mc_unget_write_buffer(void);
struct mcBuffer * mc_get_write_buffer(void); 
struct mcBuffer * mc_get_run_buffer(void);
struct mcBuffer * mc_get_prev_buffer(void);

uint8_t mc_isbusy(void);
uint8_t mc_set_position(double x, double y, double z, double a);
uint8_t mc_async_stop(void);
uint8_t mc_async_start(void);
uint8_t mc_async_end(void);

uint8_t mc_queued_stop(void);
uint8_t mc_queued_start(void);
uint8_t mc_queued_end(void);

uint8_t mc_dwell(double seconds);
uint8_t mc_line(double x, double y, double z, double a, double minutes);
uint8_t mc_aline(double x, double y, double z, double a, double minutes);
uint8_t mc_arc(double x, double y, double z, double a,
			   double i, double j, double k, 
			   double theta, double radius, 
		   	   double angular_travel, double linear_travel, 
		   	   uint8_t axis_1, uint8_t axis_2, uint8_t axis_linear,
			   double minutes);

uint8_t mc_go_home_cycle(void);

void mc_unit_tests(void);

#endif
