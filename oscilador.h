/* 
 * File: oscilador.h
 * Author: Pablo
 * Comments: Prototipo de función para configuración de oscilador interno 
 * (PIC16F887)
 * Revision history: 
 */
 
#ifndef OSCILADOR_H
#define	OSCILADOR_H

#include <xc.h>
#include <stdint.h> 

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void int_osc_MHz (uint8_t freq);

#endif	/* OSCILADOR_H */