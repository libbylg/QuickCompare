#include "impl/rx_structs.h"
#include "rx/rx.h"
#include "utils/list.h"

RX_API(int, rx_elem_init(struct rx_elem* elem, struct rx_ctrl* ctrl))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    memset(impl, 0, sizeof(struct rx_elem_impl));
    impl->parent = NULL;
    list_head_init(&(impl->nibrs));
    list_head_init(&(impl->childs));
    impl->childscount = 0;
    impl->rect.left = 0;
    impl->rect.top = 0;
    impl->rect.right = 0;
    impl->rect.bottom = 0;
    impl->ctrl = ctrl;
    return RX_OK;
}

RX_API(struct rx_ctrl*, rx_elem_ctrl(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    return impl->ctrl;
}

RX_API(struct rx_ctrl*, rx_elem_ctrl_set(struct rx_elem* elem, struct rx_ctrl* ctrl))
{
    RX_ASSERT((NULL != elem), "");
    RX_ASSERT((NULL != ctrl), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    struct rx_ctrl* old = impl->ctrl;
    impl->ctrl = ctrl;
    return old;
}

RX_API(struct rx_elem*, rx_elem_nextnibr(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    return impl->nibrs.next;
}

RX_API(struct rx_elem*, rx_elem_prevnibr(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    return impl->nibrs.prev;
}

RX_API(struct rx_elem*, rx_elem_parent(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    return (struct rx_elem*)(impl->parent);
}

RX_API(struct rx_elem*, rx_elem_firstchild(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    if (impl->childs.next == impl->childs.prev) {
        return NULL;
    }

    return impl->childs.next;
}

RX_API(int, rx_elem_childscount(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    return impl->childscount;
}

RX_API(struct rx_elem*, rx_elem_lastchild(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    if (impl->childs.next == impl->childs.prev) {
        return NULL;
    }

    return impl->childs.prev;
}

RX_API(struct rx_elem*, rx_elem_root(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* root = (struct rx_elem_impl*)(elem);
    while (NULL != root->parent) {
        root = root->parent;
    }

    return root;
}

RX_API(void, rx_elem_nibr_addbefore(struct rx_elem* elem, struct rx_elem* newitem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    struct rx_elem_impl* newimpl = (struct rx_elem_impl*)(newitem);
    list_add_tail(&(impl->nibrs), &(newimpl->nibrs));
    newimpl->parent = impl->parent;
    if (NULL != newimpl->parent) {
        newimpl->parent->childscount++;
    }
}

RX_API(void, rx_elem_nibr_addbehind(struct rx_elem* elem, struct rx_elem* newitem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    struct rx_elem_impl* newimpl = (struct rx_elem_impl*)(newitem);
    list_add(&(impl->nibrs), &(newimpl->nibrs));
    newimpl->parent = impl->parent;
    if (NULL != newimpl->parent) {
        newimpl->parent->childscount++;
    }
}

RX_API(void, rx_elem_child_addtail(struct rx_elem* elem, struct rx_elem* newitem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    struct rx_elem_impl* newimpl = (struct rx_elem_impl*)(newitem);
    list_add_tail(&(impl->childs), &(newimpl->nibrs));
    newimpl->parent = impl;
    impl->childscount++;
}

RX_API(void, rx_elem_child_addhead(struct rx_elem* elem, struct rx_elem* newitem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    struct rx_elem_impl* newimpl = (struct rx_elem_impl*)(newitem);
    list_add(&(impl->childs), &(newimpl->nibrs));
    newimpl->parent = impl;
    impl->childscount++;
}

RX_API(struct rx_elem*, rx_elem_split(struct rx_elem* elem))
{
    RX_ASSERT((NULL != elem), "");
    struct rx_elem_impl* impl = (struct rx_elem_impl*)(elem);
    list_del(&(impl->nibrs));
    if (NULL != impl->parent) {
        impl->parent->childscount--;
    }

    return elem;
}
