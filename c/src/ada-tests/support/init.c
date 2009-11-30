/*
 *  COPYRIGHT (c) 1989-2007
 *  On-Line Applications Research Corporation (OAR).
 *
 *  $Id$
 */

#define __RTEMS_VIOLATE_KERNEL_VISIBILITY__
#include <bsp.h>
#include <stdlib.h>

void *POSIX_Init(
  void *argument
)
{
  extern int gnat_main ( int argc, char **argv, char **envp );

  (void) gnat_main ( 0, 0, 0 );

  exit( 0 );
}

/*
 *  Only for sp04 and sp07
 */

rtems_id tcb_to_id(
  Thread_Control *tcb
)
{
  return tcb->Object.id;
}


/*
 *  By putting this in brackets rather than quotes, we get the search
 *  path and can get this file from ".." in the mptests.
 */

#define CONFIGURE_INIT
#define CONFIGURE_GNAT_RTEMS
#define CONFIGURE_MEMORY_OVERHEAD  (256)

#include <config.h>

uint32_t milliseconds_per_tick(void)
{
  return CONFIGURE_MICROSECONDS_PER_TICK / 1000;
}

uint32_t ticks_per_second(void)
{
  /* XXX HACK -- use public API */
  return TOD_TICKS_PER_SECOND;
}

uint32_t work_space_size(void)
{
  return CONFIGURE_EXECUTIVE_RAM_SIZE;
}

uint32_t is_configured_multiprocessing(void)
{
#if defined(RTEMS_MULTIPROCESSING)
  return 1;
#else
  return 0;
#endif
}

uint32_t get_node(void)
{
  /* XXX HACK -- use public API */
  return _Objects_Local_node;
}
