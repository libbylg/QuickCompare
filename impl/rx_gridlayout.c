#include "impl/rx_structs.h"
#include "rx/rx.h"

//display: grid;
//grid-template-columns: 100px 100px 100px;
//grid-template-rows: 100px 100px 100px;

//grid-template-columns: 33.33% 33.33% 33.33%;
//grid-template-rows: 33.33% 33.33% 33.33%;

//grid-template-rows: 100px 100px 100px;

//grid-template-columns: 100px auto 100px auto;

//grid-row-gap: 20px;
//grid-column-gap: 20px;
//grid-gap: 20px 20px;

//grid-auto-flow: column;   //  控制填充方式

//justify-items: start | end | center | stretch;
//align-items: start | end | center | stretch;

//justify-content: start | end | center | stretch | space-around | space-between | space-evenly;
//align-content: start | end | center | stretch | space-around | space-between | space-evenly;

///////
//grid-column-start 属性，
//grid-column-end 属性，
//grid-row-start 属性，
//grid-row-end 属性

//justify-self: start | end | center | stretch;
//align-self: start | end | center | stretch;

static uintptr_t rx_gridlayout_relayout(struct rx_layout* layout, struct rx_cntr* cntr, uintptr_t context)
{
}

RX_API(int, rx_gridlayout_init(struct rx_gridlayout* layout))
{
    RX_ASSERT((NULL != layout), "");
    int ret = rx_layout_init(layout, rx_gridlayout_relayout);
    if (RX_OK != ret) {
        return ret;
    }

    struct rx_gridlayout* impl = (struct rx_gridlayout*)(layout);
    
}
