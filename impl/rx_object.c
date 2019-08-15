#include "rx/rx.h"
#include "impl/rx_structs.h"


RX_API(int, rx_object_init(struct rx_object* obj))
{
    struct rx_object_impl* impl = (struct rx_object_impl*)obj;
    memset(obj, 0, sizeof(struct rx_object_impl));
    return RX_OK;
}
