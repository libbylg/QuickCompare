#ifndef __rx_loop_H_
#define __rx_loop_H_

#include "rx/rx_types.h"

#define RX_LOOP_SIZE (12)

struct rx_loop {
    uint8_t unknown[RX_LOOP_SIZE];
};

#endif//__rx_loop_H_
