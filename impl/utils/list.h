#ifndef _rx_list_H_
#define _rx_list_H_


struct list_head {
    struct list_head* prev;
    struct list_head* next;
};

static inline void __list_add(struct list_head* n, struct list_head* prev, struct list_head* next)
{
    next->prev = n;
    n->next = next;
    n->prev = prev;
}

static inline void __list_del(struct list_head* prev, struct list_head* next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void list_head_init(struct list_head* head)
{
    head->next = head;
    head->prev = head;
}

static inline int list_empty(const struct list_head* head)
{
    return head->next == head;
}

static inline void list_add(struct list_head* head, struct list_head* n)
{
    __list_add(n, head, head->next);
}

static inline void list_add_tail(struct list_head* head, struct list_head* n)
{
    __list_add(n, head->prev, head);
}

static inline struct list_head* list_del(struct list_head* entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = entry;
    entry->prev = entry;
    return entry;
}

static inline struct list_head* list_replace(struct list_head* entry, struct list_head* n)
{
    n->next = entry->next;
    n->prev = entry->prev;
    entry->next->prev = n;
    entry->prev->next = n;

    entry->next = entry;
    entry->prev = entry;
    return entry;
}

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next, prefetch(pos->next); pos != (head); pos = pos->next, prefetch(pos->next))

/**
 * list_for_each_safe	-	iterate over a list safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop counter.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)

#endif //_rx_list_H_
