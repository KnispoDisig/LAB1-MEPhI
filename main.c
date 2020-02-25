#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct ComplexDig {
    double re, im;
} ComplexDig;

typedef struct Array {
    int byte, count;
    void* ptr;
} Array;

double realZero = 0.0;
ComplexDig complexZero = {0.0, 0.0};

ComplexDig *create_complex_digit(double re, double im);
ComplexDig *compl_sum(ComplexDig a, ComplexDig b);
ComplexDig *compl_sub(ComplexDig a, ComplexDig b);
ComplexDig *compl_mult(ComplexDig a, ComplexDig b);
ComplexDig *compl_div(ComplexDig a, ComplexDig b);
ComplexDig *compl_power(ComplexDig a, int n);
ComplexDig *compl_conj(ComplexDig a);
double compl_abs(ComplexDig a);
void compl_print(ComplexDig* a);
Array* CreateArray(int count, int byte, void* zero_value);
void print_array(Array* array);
void SetArrayElem(void* array, int index, int byte, void* value);
Array* concatenateArrays(Array* array1, Array* array2);
void fillComplexArray(Array* array);
void fillRealArray(Array* array);
void sortRealArray(Array* array);
Array* realMap(double* (*func)(double*), Array* array);
double* realSquare(double* elem);
Array* realWhere(double (*func)(double*), Array* array);
Array* complexWhere(ComplexDig (*func)(ComplexDig*), Array* array);


ComplexDig *create_complex_digit(double re, double im) {
    ComplexDig *temp = (ComplexDig*) malloc(sizeof(ComplexDig));
    temp->re = re;
    temp->im = im;

    return temp;
}

ComplexDig *compl_sum(ComplexDig a, ComplexDig b) {
    ComplexDig *temp = (ComplexDig*) malloc(sizeof(ComplexDig));
    temp->re = a.re + b.re;
    temp->im = a.im + b.im;

    return temp;
}

ComplexDig *compl_sub(ComplexDig a, ComplexDig b) {
    ComplexDig *temp = (ComplexDig*) malloc(sizeof(ComplexDig));
    temp->re = a.re - b.re;
    temp->im = a.im - b.im;

    return temp;
}

ComplexDig *compl_mult(ComplexDig a, ComplexDig b) {
    ComplexDig *temp = (ComplexDig*) malloc(sizeof(ComplexDig));
    temp->re = a.re * b.re - a.im * b.im;
    temp->im = a.im * b.re + a.re * b.im;

    return temp;
}

ComplexDig *compl_div(ComplexDig a, ComplexDig b) {
    ComplexDig *temp = (ComplexDig*) malloc(sizeof(ComplexDig));
    temp->re = (a.re * b.re + a.im * b.im) / (b.re * b.re + b.im * b.im);
    temp->im = (a.im * b.re - a.re * b.im) / (b.re * b.re + b.im * b.im);

    return temp;
}

ComplexDig *compl_power(ComplexDig a, int n) {
    int i;
    ComplexDig *temp = (ComplexDig*) malloc(sizeof(ComplexDig));
    temp->re = a.re;
    temp->im = a.im;
    for (i = 0; i < n; ++i) {
        temp = compl_mult(*temp, *temp);
    }

    return temp;
}

ComplexDig *compl_conj(ComplexDig a) {
    ComplexDig *temp = (ComplexDig*) malloc(sizeof(ComplexDig));
    temp->re = a.re;
    temp->im = (-1) * a.im;

    return temp;
}

double compl_abs(ComplexDig a) {
    return sqrt(a.im * a.im + a.re * a.re);
}

void compl_print(ComplexDig* a) {
    printf("%4f + i * %4f\n", a->re, a->im);
}

Array* CreateArray(int count, int byte, void* zero_value) {
    int i;
    Array* arr = malloc(sizeof(Array));

    arr->byte = byte;
    arr->count = count;

    if (byte == 8) {
        arr->ptr = (double*) malloc(count * byte);
    } else if (byte == 16) {
        arr->ptr = (ComplexDig*) malloc(count * byte);
    } else {
        return NULL;
    }

    for (i = 0; i < count; ++i) {
        memcpy(arr->ptr + i, zero_value, byte);
    }

    return arr;
}

void print_array(Array* array) {
    int i;

    if (array == NULL) {
        printf("Sorry but array doesn't exist :(\n");
    } else if (array->byte == 8) {
        for (i = 0; i < array->count; ++i) {
            printf("(%f) ", *((double*) (array->ptr + i * array->byte)));
        }
    } else if (array->byte == 16) {
        for (i = 0; i < array->count; ++i) {
            printf("(%f + i * (%f)) ", ((ComplexDig*) (array->ptr + i * array->byte))->re, ((ComplexDig*) (array->ptr + i * array->byte))->im);
        }
    }
    printf("\n");
}

void SetArrayElem(void* array, int index, int byte, void* value) {
    memcpy(array + index * byte, value, byte);
}

Array* concatenateArrays(Array* array1, Array* array2) {
    Array* arr;

    if (array1->byte == 8 && array2->byte == 8) {
        arr = CreateArray(array1->count + array2->count, array1->byte, &realZero);
    } else if (array1->byte == 16 && array2->byte == 16) {
        arr = CreateArray(array1->count + array2->count, array1->byte, &complexZero);
    } else {
        printf("Concatenation is impossible");
        return NULL;
    }
    memcpy(arr->ptr, array1->ptr, array1->count * array1->byte);
    memcpy(arr->ptr + array1->count * array1->byte, array2->ptr, array2->count * array2->byte);

    return arr;
}

void fillRealArray(Array* array) {
    int i;
    double realElem;

    if (array->byte == 8) {
        for (i = 0; i < array->count; ++i) {
            printf("%d number: ", i + 1);
            scanf("%lf", &realElem);
            printf("\n");
            SetArrayElem(array->ptr, i, 8, &realElem);
        }
    } else {printf("The array is not real\n");}
}

void fillComplexArray(Array* array) {
    int i;
    ComplexDig complexElem;
    printf("Example for complex number 2.1 + i * 8.4 == 2.1 8.4\n");

    if (array->byte == 16) {
        for (i = 0; i < array->count; ++i) {
            printf("%d number - ", i + 1);
            scanf("%lf %lf", &complexElem.re, &complexElem.im);
            printf("\n");
            SetArrayElem(array->ptr, i, 16, &complexElem);
        }
    } else {printf("The array is not complex\n");}
}

void sortRealArray(Array* array) {
    int i, j;
    double temp;

    if (array->byte == 8) {
        for (i = 0; i < array->count - 1; ++i) {
            for (j = i + 1; j < array->count; ++j) {
                if (*((double*) (array->ptr + i * array->byte)) > *((double*) (array->ptr + j * array->byte))) {
                    temp = *((double*) (array->ptr + i * array->byte));
                    *((double*) (array->ptr + i * array->byte)) = *((double*) (array->ptr + j * array->byte));
                    *((double*) (array->ptr + j * array->byte)) = temp;
                }
            }
        }
    }
}

void sortComplexArray(Array* array) {
    int i, j;
    ComplexDig temp;

    if (array->byte == 16) {
        for (i = 0; i < array->count - 1; ++i) {
            for (j = i + 1; j < array->count; ++j) {
                if (compl_abs(*((ComplexDig*) (array->ptr + i * array->byte))) > compl_abs(*((ComplexDig*) (array->ptr + j * array->byte)))) {
                    temp = *((ComplexDig*) (array->ptr + i * array->byte));
                    *((ComplexDig*) (array->ptr + i * array->byte)) = *((ComplexDig*) (array->ptr + j * array->byte));
                    *((ComplexDig*) (array->ptr + j * array->byte)) = temp;
                }
            }
        }
    }
}

Array* realMap(double* (*func)(double*), Array* array) {
    int i;
    Array* temp;

    if (array->byte == 8) {
        temp = CreateArray(array->count, array->byte, &realZero);
        for (i = 0; i < temp->count * temp->byte; i += temp->byte) {
            *((double*) (temp->ptr + i)) = *func(array->ptr + i);
        }
    } else {return NULL;}

    return temp;
}

Array* complexMap(ComplexDig* (*func)(ComplexDig*), Array* array) {
    int i;
    Array* temp;

    if (array->byte == 16) {
        temp = CreateArray(array->count, array->byte, &complexZero);
        for (i = 0; i < temp->count * temp->byte; i += temp->byte) {
            *((ComplexDig*) (temp->ptr + i)) = *func(array->ptr + i);
        }
    } else {return NULL;}

    return temp;
}

double* realSquare(double* elem) {
    double* result = malloc(sizeof(double));
    *result = *elem * *elem;
    return result;
}

ComplexDig* complexSquare(ComplexDig* elem) {
    ComplexDig* result = malloc(sizeof(ComplexDig));
    result = compl_power(*elem, 2);
    return result;
}

Array* realWhere(double (*func)(double*), Array* array) {
    int i;
    Array* temp;

    if (array->byte == 8) {
        temp = CreateArray(array->count, array->byte, &realZero);
        for (i = 0; i < temp->count * temp->byte; i += temp->byte) {
            *((double*) (temp->ptr + i)) = func((double*) (temp->ptr + i));
        }
    } else {return NULL;}

    return temp;
}

Array* complexWhere(ComplexDig (*func)(ComplexDig*), Array* array) {
    int i;
    Array* temp;

    if (array->byte == 16) {
        temp = CreateArray(array->count, array->byte, &complexZero);
        for (i = 0; i < temp->count * temp->byte; i += temp->byte) {
            *((ComplexDig*) (temp->ptr + i)) = func((ComplexDig*) (temp->ptr + i));
        }
    } else {return NULL;}

    return temp;
}

double moreThanNull(double *num) {
    double val = *num;
    if (val > 0) {
        return 56.0;
    } else if (val == 0) {
        return 14.0;
    } else {
        return 167.0;
    }
}

int main() {
    int req, size;
    Array* Arr = NULL;

    do {
        printf("Enter 1 to create array with real numbers\n");
        printf("Enter 2 to create array with complex numbers\n");
        printf("Enter 3 to print array\n");
        printf("Enter 4 to concatenate 2 arrays\n");
        printf("Enter 5 to sort entered array\n");
        printf("Enter 6 to square the array's elements (function 'map')\n");
        printf("Enter 7 to mark numbers more than 0 (if arrays is double)\n");
        printf("Enter 0 for exit\n\n");

        scanf("%d", &req);

        if (req == 1 || req == 2) {
            printf("Enter an array's size: \n");
            scanf("%d", &size);

            if (req == 1) {
                Arr = CreateArray(size, 8, &realZero);
                fillRealArray(Arr);
            } else {
                Arr = CreateArray(size, 16, &complexZero);
                fillComplexArray(Arr);
            }
        }

        if (req == 3) {
            if (Arr == NULL) {
                printf("The array doesn't exist!\n");
            } else {
                print_array(Arr);
            }
        }

        if (req == 4) {
            Array *arr1, *arr2, *concatArray;
            printf("Enter 2 arrays for their concatenating...\n");

            printf("Size of 1st array: ");
            scanf("%d", &size);
            arr1 = CreateArray(size, 8, &realZero);
            fillRealArray(arr1);

            printf("Size of 2nd array: ");
            scanf("%d", &size);
            arr2 = CreateArray(size, 8, &realZero);
            fillRealArray(arr2);

            concatArray = concatenateArrays(arr1, arr2);
            print_array(concatArray);
        }

        if (req == 5) {
            if (Arr == NULL) {
                printf("The array doesn't exist!\n");
            } else if (Arr->byte == 8) {
                sortRealArray(Arr);
                print_array(Arr);
            } else if (Arr->byte == 16) {
                sortComplexArray(Arr);
                print_array(Arr);
            }
        }

        if (req == 6) {
            Array* mapedArray;

            if (Arr == NULL) {
                printf("The array doesn't exist!\n");
            } else if (Arr->byte == 8) {
                mapedArray = realMap(realSquare, Arr);
                print_array(mapedArray);
            } else {
                mapedArray = complexMap(complexSquare, Arr);
                print_array(mapedArray);
            }

            free(mapedArray);
        }

        if (req == 7) {
            Array* whereArray;

            if (Arr == NULL) {
                printf("The array doesn't exist!\n");
            } else if (Arr->byte == 8) {
                whereArray = realWhere(moreThanNull, Arr);
                print_array(whereArray);
            } else {
                printf("The array is not real!\n");
            }
        }

    } while (req != 0);
    return 0;
}