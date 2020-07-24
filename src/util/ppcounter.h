#ifndef LIFE_PPCOUNTER_H
#define LIFE_PPCOUNTER_H

#include <stdlib.h>

// Shamelessly copied from: https://stackoverflow.com/questions/6166337/does-c-support-compile-time-counters

template< size_t n > // This type returns a number through function lookup.
struct cn // The function returns cn<n>.
{ char data[ n + 1 ]; }; // The caller uses (sizeof fn() - 1).

template< typename id, size_t n, size_t acc >
cn< acc > spotted( id, cn< n >, cn< acc > ); // Default fallback case.

/* Evaluate the counter by finding the last defined overload.
   Each function, when defined, alters the lookup sequence for lower-order
   functions. */
#define counter_read( id ) \
( sizeof spotted( id(), cn< 1 >(), cn< \
( sizeof spotted( id(), cn< 2 >(), cn< \
( sizeof spotted( id(), cn< 4 >(), cn< \
( sizeof spotted( id(), cn< 8 >(), cn< \
( sizeof spotted( id(), cn< 16 >(), cn< \
( sizeof spotted( id(), cn< 32 >(), cn< 0 \
                      >() ).data - 1 ) \
                      >() ).data - 1 ) \
                      >() ).data - 1 ) \
                      >() ).data - 1 ) \
                      >() ).data - 1 ) \
                      >() ).data - 1 )
                      
#define counter_inc( id ) \
cn< counter_read( id ) + 1 > \
spotted( id, cn< ( counter_read( id ) + 1 ) & ~ counter_read( id ) >, \
          cn< ( counter_read( id ) + 1 ) & counter_read( id ) > )

struct base_counter {};

#endif //LIFE_PPCOUNTER_H
