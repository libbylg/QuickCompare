#ifndef __array_H_
#define __array_H_

#include "rx/rx_types.h"

typedef void(RX_CALL* RX_COPY)(struct array_trait* trait, void* pos, uintptr_t data, uintptr_t ctx);
struct array_trait {
    int esize; //  ��λ���ݳ���
    int inc; //  ��λ���ݳ���
    RX_COPY copy;
};

#define ARRAY_DYNAMIC (0x00000000)
#define ARRAY_FIXSIZE (0x00000001)
struct array {
    int cap; //  ������
    int count; //  ��ǰ����
    uint8_t* data; //  ���ݴ洢��
    struct array_trait* trait; //  ����
};

static inline void array_copy_uintptr(struct array_trait* trait, void* pos, uintptr_t data, uintptr_t ctx)
{
    *((uintptr_t*)pos) = data;
}

static inline void array_copy_uint32(struct array_trait* trait, void* pos, uintptr_t data, uintptr_t ctx)
{
    *((uint32_t*)pos) = (uint32_t)data;
}

static inline void array_copy_uint16(struct array_trait* trait, void* pos, uintptr_t data, uintptr_t ctx)
{
    *((uint16_t*)pos) = (uint16_t)data;
}

static inline void array_copy_uint8(struct array_trait* trait, void* pos, uintptr_t data, uintptr_t ctx)
{
    *((uint8_t*)pos) = (uint8_t)data;
}

static inline int array_init(struct array* array, int init_cap, struct array_trait* trait)
{
    RX_ASSERT(NULL != array, "");
    RX_ASSERT(init_cap > 0, "");
    RX_ASSERT(NULL != trait, "");
    RX_ASSERT(trait->esize > 0, "");
    RX_ASSERT(trait->inc >= 0, "");
    RX_ASSERT(NULL != trait->copy, "");

    //  ����������
    memset(array, 0, sizeof(struct array));

    //  �����С�������ڴ�
    int maxsize = init_cap * trait->esize;
    array->data = (uint8_t*)malloc(maxsize);
    if (NULL == array->data) {
        return -1;
    }

    array->trait = trait;
    array->cap = init_cap;
    array->count = 0;
    return 0;
}

static inline void array_exit(struct array* array)
{
    free(array->data);
}

static inline void* array_indexof(struct array* array, int index)
{
    RX_ASSERT(NULL != array, "");
    if ((index < 0) || (index >= array->count)) {
        return NULL;
    }

    return array->data + (index * array->trait->esize);
}

static inline void* array_update(struct array* array, int index, uintptr_t data, uintptr_t ctx)
{
    RX_ASSERT(NULL != array, "");
    if ((index < 0) || (index >= array->count)) {
        return NULL;
    }

    void* pos = array->data + (index * array->trait->esize);
    array->trait->copy(array->trait, pos, data, ctx);

    return pos;
}

static inline void array_grow(struct array* array)
{
    RX_ASSERT(NULL != array, "");
    if (array->trait->inc == 0) {
        return;
    }

    int newcap = array->cap + array->trait->inc;
    uint8_t* newdata = (uint8_t*)malloc(newcap * array->trait->esize);
    if (NULL == newdata) {
        return;
    }
    uint8_t* olddata = array->data;
    memcpy(newdata, array->data, array->count + array->trait->esize);
    array->data = newdata;
    array->cap = newcap;
}

static inline void* array_append(struct array* array, uintptr_t data, uintptr_t ctx)
{
    RX_ASSERT(NULL != array, "");

    //  ����ռ䲻������ô��Ҫ���·�������
    if (array->count == array->cap) {
        array_grow(array);
        if (array->count == array->cap) {
            return NULL;
        }
    }

    //  ��������
    void* pos = array->data + (array->count * array->trait->esize);
    array->trait->copy(array->trait, pos, data, ctx);
    array->count++;

    //  ���ر������ݵ���ʼλ��
    return pos;
}

static inline void* array_insert(struct array* array, int index, uintptr_t data, uintptr_t ctx)
{
    RX_ASSERT(NULL != array, "");
    if ((index < 0) || (index > array->count)) {
        return NULL;
    }

    //  ����ռ䲻������ô��Ҫ���·�������
    if (array->count == array->cap) {
        array_grow(array);
        if (array->count == array->cap) {
            return NULL;
        }
    }

    //  ��������
    uint8_t* pos = array->data + (array->count * array->trait->esize);
    memmove(pos + array->trait->esize, pos, array->trait->esize); //  TODO ������Զ����ݽ�һ���Ż�
    array->trait->copy(array->trait, pos, data, ctx);
    array->count++;

    //  ���ر������ݵ���ʼλ��
    return pos;
}

static inline int array_remove(struct array* array, int begin, int end)
{
    RX_ASSERT(NULL != array, "");
    if (begin < 0) {
        begin = 0;
    }

    if (begin >= array->count) {
        return -1;
    }

    if ((end < 0) || (end > array->count)) {
        end = array->count;
    }

    if (begin >= end) {
        return -1;
    }

    //  ��������
    if (end == array->count) {
        array->count = begin;
        return 0;

    }

    //  β���������ݣ���ʱ��Ҫ�ƶ�����
    uint8_t* posbegin = array->data + (begin * array->trait->esize);
    uint8_t* posend = array->data + (end * array->trait->esize);
    memmove(posbegin, posend, ((array->count - end) * array->trait->esize)); //  TODO ������Զ����ݽ�һ���Ż�
    array->count -= (end - begin);

    //  ���ر������ݵ���ʼλ��
    return 0;
}

#endif //__array_H_
