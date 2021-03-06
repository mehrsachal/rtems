/*
 *  This file contains the basic algorithms for all assembly code used
 *  in an specific CPU port of RTEMS.  These algorithms must be implemented
 *  in assembly language
 *
 *  NOTE:  This port uses a C file with inline assembler instructions
 *
 *  Authors: Ralf Corsepius (corsepiu@faw.uni-ulm.de) and
 *           Bernd Becker (becker@faw.uni-ulm.de)
 *
 *  COPYRIGHT (c) 1997-1998, FAW Ulm, Germany
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 *  COPYRIGHT (c) 1998.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

/*
 *  This is supposed to be an assembly file.  This means that system.h
 *  and cpu.h should not be included in a "real" cpu_asm file.  An
 *  implementation in assembly should include "cpu_asm.h"
 */

#include <rtems/system.h>
#include <rtems/score/cpu.h>
#include <rtems/score/isr.h>
#include <rtems/score/threaddispatch.h>
#include <rtems/score/sh.h>
#include <rtems/score/ispsh7750.h>
#include <rtems/score/iosh7750.h>
#include <rtems/score/sh4_regs.h>
#include <rtems/score/sh_io.h>

unsigned long *_old_stack_ptr;

register unsigned long  *stack_ptr __asm__ ("r15");

/*
 *  This routine provides the RTEMS interrupt management.
 */

void __ISR_Handler( uint32_t   vector)
{
  ISR_Level level;

  _ISR_Local_disable( level );

   _Thread_Dispatch_disable();

  if ( _ISR_Nest_level == 0 )
    {
      /* Install irq stack */
      _old_stack_ptr = stack_ptr;
      stack_ptr = _CPU_Interrupt_stack_high;
    }

  _ISR_Nest_level++;

  _ISR_Local_enable( level );

  /* call isp */
  if ( _ISR_Vector_table[ vector])
    (*_ISR_Vector_table[ vector ])( vector );

  _ISR_Local_disable( level );

  _Thread_Dispatch_enable( _Per_CPU_Get() );

  _ISR_Nest_level--;

  if ( _ISR_Nest_level == 0 )
    /* restore old stack pointer */
    stack_ptr = _old_stack_ptr;

  _ISR_Local_enable( level );

  if ( _ISR_Nest_level )
    return;

  if ( !_Thread_Dispatch_is_enabled() ) {
    return;
  }

  if ( _Thread_Dispatch_necessary ) {
    _Thread_Dispatch();
  }
}
