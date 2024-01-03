#include <sse_core.h>

static void sse_heap_min_heapify(sse_heap_t *heap, sse_uint_t i);
static void sse_heap_min_build(sse_heap_t *heap);

sse_heap_t *
sse_heap_create(sse_uint_t capacity, sse_pool_t *pool, sse_log_t *log)
{
    sse_heap_t      *heap;

    if (capacity == 0) {
        return SSE_NULL;
    }

    heap = (sse_heap_t *)sse_pool_alloc(pool, sizeof(sse_heap_t));
    if (!heap) {
        return SSE_NULL;
    }

    heap->items = (sse_heap_item_t *)sse_pool_calloc(pool, capacity * sizeof(sse_heap_item_t));
    if (heap->items == SSE_NULL) {
        return SSE_NULL; 
    }

    heap->size = 0;
    heap->capacity = capacity;
    heap->pool = pool;
    heap->log = log;

    return heap;
}

static void
sse_heap_min_heapify(sse_heap_t *heap, sse_uint_t i)
{
    sse_uint_t      left, right, smallest;
    float           temp_score;
    void            *temp_data;

    left = sse_heap_left(i);
    right = sse_heap_right(i);

    if (left < heap->size && heap->items[left].score < heap->items[i].score) {
        smallest = left;
    }
    else {
        smallest = i;
    }

    if (right < heap->size && heap->items[right].score < heap->items[smallest].score) {
        smallest = right;
    }

    if (smallest != i) {
        /* swap */
        temp_score = heap->items[i].score;
        temp_data = heap->items[i].data;
        heap->items[i].score = heap->items[smallest].score;
        heap->items[i].data = heap->items[smallest].data;
        heap->items[smallest].score = temp_score;
        heap->items[smallest].data = temp_data;

        sse_heap_min_heapify(heap, smallest);
    }
}

static void
sse_heap_min_build(sse_heap_t *heap)
{
    sse_int_t       i;

    if (heap->size < 2) {
        return;
    }

    for (i = (heap->size - 2) / 2; i >= 0; --i) {
        sse_heap_min_heapify(heap, i);
    }
}

void 
sse_heap_min_sort(sse_heap_t *heap)
{
    sse_uint_t      i, size;
    float           temp_score;
    void            *temp_data;

    size = heap->size;

    sse_heap_min_build(heap);

    if (heap->size < 2) {
        return;
    }

    for (i = heap->size - 1; i >= 1; --i) {
        /* swap */
        temp_score = heap->items[i].score;
        temp_data = heap->items[i].data;
        heap->items[i].score = heap->items[0].score;
        heap->items[i].data = heap->items[0].data;
        heap->items[0].score = temp_score;
        heap->items[0].data = temp_data;
        --heap->size;
        sse_heap_min_heapify(heap, 0);
    }
    heap->size = size;
}

void 
sse_heap_min_insert(sse_heap_t *heap, float score, void *data)
{
    if (heap->size < heap->capacity) {
        heap->items[heap->size].score = score;
        heap->items[heap->size++].data = data;
    }
    else {
        sse_heap_min_build(heap);
        if (heap->items[0].score < score) {
            heap->items[0].score = score;
            heap->items[0].data = data;
            sse_heap_min_heapify(heap, 0);
        }
    }
}
