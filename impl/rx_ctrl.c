#include "impl/rx_structs.h"
#include "rx/rx.h"
#include "utils/list.h"

RX_API(int, rx_ctrl_init(struct rx_ctrl* ctrl, RX_PAINT paint, RX_NOTIFY notify))
{
    RX_ASSERT((NULL != ctrl), "");
    RX_ASSERT((NULL != paint), "");
    //RX_ASSERT((NULL != notify), "");  //  notify ¿ÉÒÔÎªNULL
    struct rx_ctrl_impl* impl = (struct rx_ctrl_impl*)ctrl;
    memset(impl, 0, sizeof(struct rx_ctrl_impl));

    impl->elem = NULL;
    impl->paint = paint;
    impl->notify = notify;

    return 0;
}

RX_API(struct rx_ctrl*, rx_ctrl_nextnibr(struct rx_ctrl* ctrl))
{
    RX_ASSERT((NULL != ctrl), "");
    struct rx_ctrl_impl* impl = (struct rx_ctrl_impl*)ctrl;
    struct rx_elem* elemnibr = rx_elem_nextnibr(&(impl->elem));
    if (NULL == elemnibr) {
        return NULL;
    }

    struct rx_elem_impl* elemnibrimpl = (struct rx_elem_impl*)elemnibr;
    return elemnibrimpl->ctrl;
}

RX_API(struct rx_ctrl*, rx_ctrl_prevnibr(struct rx_ctrl* ctrl))
{
    RX_ASSERT((NULL != ctrl), "");
    struct rx_ctrl_impl* impl = (struct rx_ctrl_impl*)ctrl;
    struct rx_elem* elemnibr = rx_elem_prevnibr(&(impl->elem));
    if (NULL == elemnibr) {
        return NULL;
    }

    struct rx_elem_impl* elemnibrimpl = (struct rx_elem_impl*)elemnibr;
    return elemnibrimpl->ctrl;
}

RX_API(struct rx_ctrl*, rx_ctrl_parent(struct rx_ctrl* ctrl))
{
    RX_ASSERT((NULL != ctrl), "");
    struct rx_ctrl_impl* impl = (struct rx_ctrl_impl*)ctrl;
    struct rx_elem* elemparent = rx_elem_parent(&(impl->elem));
    if (NULL == elemparent) {
        return NULL;
    }

    struct rx_elem_impl* elemparentimpl = (struct rx_elem_impl*)elemparent;
    return elemparentimpl->ctrl;
}

RX_API(void, rx_ctrl_nibr_addbefore(struct rx_ctrl* ctrl, struct rx_ctrl* newitem))
{
}

RX_API(void, rx_ctrl_nibr_addbehind(struct rx_ctrl* ctrl, struct rx_ctrl* newitem))
{
}

RX_API(struct rx_elem*, rx_ctrl_elem(struct rx_ctrl* ctrl))
{
    RX_ASSERT((NULL != ctrl), "");
    struct rx_ctrl_impl* impl = (struct rx_ctrl_impl*)ctrl;
    return (struct rx_elem*)(impl->elem);
}

RX_API(int, rx_ctrl_pos(struct rx_ctrl* ctrl, struct rx_rect* rect))
{
    RX_ASSERT((NULL != ctrl), "");
    RX_ASSERT((NULL != rect), "");

    struct rx_ctrl_impl* impl = (struct rx_ctrl_impl*)ctrl;
    struct rx_elem_impl* elemimpl = (struct rx_elem_impl*)(impl->elem);
    *rect = elemimpl->rect;
    return RX_OK;
}
