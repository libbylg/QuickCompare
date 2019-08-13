#include "rx/rx.h"
#include "impl/rx_structs.h"
#include "impl/rx_error.h"

RX_API(int, rx_accel_init(struct rx_accel* accel, uintptr_t inst, int id))
{
    RX_ASSERT((NULL != accel), "外部保证有效性");

    struct rx_accel_impl* impl = (struct rx_accel_impl*)accel;
    memset(impl, 0, sizeof(struct rx_accel_impl));
    list_head_init(&(impl->nibrs));
    if (0 != id) {
        impl->accel = (uintptr_t)LoadAccelerators((HINSTANCE)inst, MAKEINTRESOURCE(id));
        if (NULL == impl->accel) {
            return RX_FAIL;
        }
    }

    return RX_OK;
}

