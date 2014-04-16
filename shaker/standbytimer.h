#ifndef STANDBYTIMER_H
#define STANDBYTIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

/*----------------------*/
/* STANDBY TIMER CONFIG */
/*----------------------*/

#ifndef STANDBY_TIMER_SEC
#define STANDBY_TIMER_SEC	240
#endif

#ifndef STANDBY_VOLT_READ_CHAN
#define STANDBY_VOLT_READ_CHAN 1
#endif

// we share the timer with shake_detect!
// so this is not necessary:
/*
#ifndef STANDBY_8BIT_TIMER_REG
#define STANDBY_8BIT_TIMER_REG 		TCCR1
#define STANDBY_8BIT_TIMER_REG_MASK	(1<<CS13) | (1<<CS12)	// Prescaler 2048
#define STANDBY_8BIT_TIMER_PRESCALER 2048
#endif
*/

#ifndef STANDBY_INTERRUPT_REG
#define STANDBY_INTERRUPT_REG 		TIMSK
#define STANDBY_INTERRUPT_REG_MASK 	(1 << TOIE1)	// Timer/Counter1 Overflow Interrupt Enable
#endif

#ifndef STANDBY_INTERRUPT_HANDLER
#define STANDBY_INTERRUPT_HANDLER TIMER1_OVF_vect	// Timer/Counter1 Overflow
#endif

/*-------------------------*/
/* STANDBY TIMER CALLBACKS */
/*-------------------------*/

void (*stationCheckCallback)(uint32_t avgInputVolt);  // intermediate check if the cube is on a loading station
void (*standbyTimerCallback)(uint32_t avgInputVolt);  // entered standby

/*-------------------------*/
/* STANDBY TIMER FUNCTIONS */
/*-------------------------*/

void standby_timer_init();

void standby_timer_reset();

#endif // STANDBYTIMER_H
