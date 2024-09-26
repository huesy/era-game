#include <stdio.h>
#include <stdlib.h>

int main() {

    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int *array = (int *)malloc(n * sizeof(int));

    // Check if memory allocation succeeded.
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // Populate the array with values.
    for (int i = 0; i < n; i++) {
        array[i] = i + 1; // Assign values 1 to n.
    }

    // Print the array values.
    for (int i = 0; i < n; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }

    // Free the allocated memory.
    free(array);

    return 0;
}