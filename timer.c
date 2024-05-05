#include "timer.h"

#include "LPC17xx.h"

// TODO: Preprocess this entirely
/**
 * Initialize a given `timer` with a clock speed of `clock_speed`.
 * @param timer - The timer to initialize. Use one of TIMER_0, TIMER_1, TIMER_2,
 * TIMER_3.
 * @param clock_speed - The clock speed to set. Use one of
 * TIMER_CLOCK_SPEED_1_1, TIMER_CLOCK_SPEED_1_4, TIMER_CLOCK_SPEED_1_2,
 * TIMER_CLOCK_SPEED_1_8.
 * @return 0 if successful, -1 if the timer is invalid.
 */
char init_timer(unsigned char const timer,
				unsigned char const relative_clock_speed) {
	switch (timer) {
		case TIMER_0:
			// Page 63
			// Set power on
			LPC_SC->PCONP |= 1 << 1;
			// Set clock speed
			LPC_SC->PCLKSEL0 |= relative_clock_speed << 2;
			break;
		case TIMER_1:
			LPC_SC->PCONP |= 1 << 2;
			LPC_SC->PCLKSEL0 |= relative_clock_speed << 4;
			break;
		case TIMER_2:
			LPC_SC->PCONP |= 1 << 22;
			// Page 57
			LPC_SC->PCLKSEL1 |= relative_clock_speed << 12;
			break;
		case TIMER_3:
			LPC_SC->PCONP |= 1 << 23;
			LPC_SC->PCLKSEL1 |= relative_clock_speed << 14;
			break;
		default:
			return -1;
	}

	return 0;
}

/** Convert milliseconds to a match value. This allows you to easily set a timer
 * to a specific time.
 * @param ms - The milliseconds to convert.
 * @param relative_clock_speed - The clock speed to use. Use one of
 * TIMER_CLOCK_SPEED_1_1, TIMER_CLOCK_SPEED_1_4, TIMER_CLOCK_SPEED_1_2,
 * TIMER_CLOCK_SPEED_1_8.
 * @return The match value.
 */
unsigned long ms_to_match_value(unsigned long const ms,
								unsigned char const relative_clock_speed) {
	long clock_speed;

	switch (relative_clock_speed) {
		case TIMER_CLOCK_SPEED_1_1:
			clock_speed = MAX_CPU_CLOCK;
			break;
		case TIMER_CLOCK_SPEED_1_4:
			clock_speed = MAX_CPU_CLOCK / 4;
			break;
		case TIMER_CLOCK_SPEED_1_2:
			clock_speed = MAX_CPU_CLOCK / 2;
			break;
		case TIMER_CLOCK_SPEED_1_8:
			clock_speed = MAX_CPU_CLOCK / 8;
			break;
		default:
			return 0;
	}

	return clock_speed * ((double)ms / (double)1000);
}

#define MR0I 0
#define MR0R 1
/**
 * Register a simple timer that interrupts for a given `timer` every `ms`
 * milliseconds.
 * @param timer - The timer to register. Use one of TIMER_0, TIMER_1, TIMER_2,
 * TIMER_3.
 * @param ms - The milliseconds to interrupt every.
 * @return 0 if successful, -1 if the timer is invalid.
 */
char register_simple_timer_interrupt(unsigned char const timer,
									 unsigned long const ms) {
	init_timer(timer, TIMER_CLOCK_SPEED_1_8);

	switch (timer) {
		case TIMER_0: {
			LPC_TIM0->MR0 = ms_to_match_value(ms, TIMER_CLOCK_SPEED_1_8);
			// Page 496
			// Reset on match (autoreload)
			LPC_TIM0->MCR |= 1 << MR0I;
			LPC_TIM0->MCR |= 1 << MR0R;

			// Enable timer
			NVIC_SetPriority(TIMER0_IRQn, 0);
			NVIC_EnableIRQ(TIMER0_IRQn);

			// Page 494
			LPC_TIM0->TCR |= 1 << 0;
			break;
		}
		case TIMER_1: {
			LPC_TIM1->MR0 = ms_to_match_value(ms, TIMER_CLOCK_SPEED_1_8);

			LPC_TIM1->MCR |= 1 << MR0I;
			LPC_TIM1->MCR |= 1 << MR0R;

			NVIC_SetPriority(TIMER1_IRQn, 0);
			NVIC_EnableIRQ(TIMER1_IRQn);

			LPC_TIM1->TCR |= 1 << 0;
			break;
		}
		case TIMER_2: {
			LPC_TIM2->MR0 = ms_to_match_value(ms, TIMER_CLOCK_SPEED_1_8);

			LPC_TIM2->MCR |= 1 << MR0I;
			LPC_TIM2->MCR |= 1 << MR0R;

			NVIC_SetPriority(TIMER2_IRQn, 0);
			NVIC_EnableIRQ(TIMER2_IRQn);

			LPC_TIM2->TCR |= 1 << 0;
			break;
		}
		case TIMER_3: {
			LPC_TIM3->MR0 = ms_to_match_value(ms, TIMER_CLOCK_SPEED_1_8);

			LPC_TIM3->MCR |= 1 << MR0I;
			LPC_TIM3->MCR |= 1 << MR0R;

			NVIC_SetPriority(TIMER3_IRQn, 0);
			NVIC_EnableIRQ(TIMER3_IRQn);

			LPC_TIM3->TCR |= 1 << 0;
			break;
		}
		default:
			return -1;
	}

	return 0;
}
