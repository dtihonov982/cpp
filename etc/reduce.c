#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

void *work(void *task);

struct task_t {
    int* arr;
    size_t size;
    long long result;
};

int* arr_create(size_t size) {
    int *arr = malloc(size * sizeof(int));
    for (size_t i = 1; i <= size; ++i) {
        arr[i] = i;
    }
    return arr;
}

struct task_t *tasks_create(int *arr, size_t arr_size, size_t tasks_count) {
    struct task_t *tasks = malloc(tasks_count * sizeof(struct task_t));
    assert(arr_size % tasks_count == 0);
    size_t task_size = arr_size / tasks_count;
    for (size_t i = 0; i < tasks_count; ++i) {
        tasks[i].arr = arr;
        tasks[i].size = task_size;
        arr += task_size;
    }
    return tasks;
}


int main() {
    size_t arr_size = 1000000;
    int *arr = arr_create(arr_size);

    size_t tasks_count = 10;
    struct task_t *tasks = tasks_create(arr, arr_size, tasks_count);


    //threads creation
    size_t threads_count = tasks_count;
    pthread_t threads[threads_count];
    for (size_t i = 0; i < threads_count; ++i) {
        int res = pthread_create(&threads[i], NULL, work, (void*) &tasks[i]);
        if (res) {
            printf("Thread %d creation error.\n", i);
            goto clear;
        }
    }
        
    //waiting until work is done
    for (size_t i = 0; i < threads_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    //collecting results
    long long result = 0;
    for (size_t i = 0; i < tasks_count; ++i) {
        result += tasks[i].result;
    }

    printf("Result %ld.\n", result);
clear:
    free(arr);
    free(tasks);
    exit(0);
}

void *work(void *task) {
    struct task_t *task_ = (struct task_t*) task;
    int *ptr = task_->arr;
    size_t sz = task_->size;
    long long result = 0;

    for (size_t i = 0; i < sz; ++i) {
        result += *ptr;
        ++ptr;
    }

    task_->result = result;
}
