#include "LPC17xx.h"

/// 100 MHz
#define MAX_CPU_CLOCK 100000000

// Page 57
// Run at 100% clock rate
#define TIMER_CLOCK_SPEED_1_1 0b01
// Run at 1/4 clock rate
#define TIMER_CLOCK_SPEED_1_4 0b00
// Run at 1/2 clock rate
#define TIMER_CLOCK_SPEED_1_2 0b10
// Run at 1/8 clock rate, except for CAN1, CAN2, and CAN filtering when “11”
// selects = 1/6 clock rate
#define TIMER_CLOCK_SPEED_1_8 0b11

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
#define TIMER_3 3

char init_timer(const unsigned char timer,
				const unsigned char relative_clock_speed);

unsigned long ms_to_match_value(const unsigned long ms,
								const unsigned char relative_clock_speed);

char register_simple_timer_interrupt(unsigned char const timer,
									 unsigned long const ms);
