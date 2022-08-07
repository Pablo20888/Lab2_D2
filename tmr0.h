/* 
 * File: tmr0.h
 * Author: Pablo
 * Comments: Prototipo de funciones para configuración de prescaler y recargo de 
 * valor de retraso del TMR0 (PIC16F887)
 * Revision history:  
 */
 
#ifndef TMR0_H
#define	TMR0_H

#include <xc.h>
#include <stdint.h>

#define _tmr0_value 207 // Tiempo = 100 mS

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void tmr0_init(uint8_t prescaler);


void tmr0_reload(void);

#endif	/* TMR0_H */

