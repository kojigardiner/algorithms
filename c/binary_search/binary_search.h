#pragma once

#define NELEMS(a) sizeof(a) / sizeof(a[0])

// Uses binary search to find the index of a key in a *sorted* int array a
int index_of(int *a, size_t len, int key);
