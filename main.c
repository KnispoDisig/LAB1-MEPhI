#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

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
int complex_equals(ComplexDig, ComplexDig);
double compl_abs(ComplexDig a);
void compl_print(ComplexDig* a);
Array* createArray(int count, int byte, void* zero_value);
void print_array(Array* array);
void setArrayElem(void* array, int index, int byte, void* value);
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

int complex_equals(ComplexDig a, ComplexDig b) {
    if (a.im == b.im && a.re == b.re) {
        return 1;
    } else {
        return 0;
    }
}

double compl_abs(ComplexDig a) {
    return sqrt(a.im * a.im + a.re * a.re);
}

void compl_print(ComplexDig* a) {
    printf("%4f + i * %4f\n", a->re, a->im);
}

Array* createArray(int count, int byte, void* zero_value) { //создание массива
    int i;
    Array* arr = malloc(sizeof(Array));

    arr->byte = byte;
    arr->count = count;

    if (byte == 8) {                                     //проверка типа
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

void print_array(Array* array) {  //вывод элементов массива в консоль
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

void setArrayElem(void* array, int index, int byte, void* value) {  //установка элемента value на место index в массиве array
    memcpy(array + index * byte, value, byte);
}

Array* concatenateArrays(Array* array1, Array* array2) {
    Array* arr;

    if (array1->byte == 8 && array2->byte == 8) {
        arr = createArray(array1->count + array2->count, array1->byte, &realZero);
    } else if (array1->byte == 16 && array2->byte == 16) {
        arr = createArray(array1->count + array2->count, array1->byte, &complexZero);
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
            setArrayElem(array->ptr, i, 8, &realElem);
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
            setArrayElem(array->ptr, i, 16, &complexElem);
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
        temp = createArray(array->count, array->byte, &realZero);
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
        temp = createArray(array->count, array->byte, &complexZero);
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
        temp = createArray(array->count, array->byte, &realZero);
        for (i = 0; i < temp->count * temp->byte; i += temp->byte) {
            *((double*) (temp->ptr + i)) = func((double*) (array->ptr + i));
        }
    } else {return NULL;}

    return temp;
}

Array* complexWhere(ComplexDig (*func)(ComplexDig*), Array* array) {
    int i;
    Array* temp;

    if (array->byte == 16) {
        temp = createArray(array->count, array->byte, &complexZero);
        for (i = 0; i < temp->count * temp->byte; i += temp->byte) {
            *((ComplexDig*) (temp->ptr + i)) = func((ComplexDig*) (array->ptr + i));
        }
    } else {return NULL;}

    return temp;
}

double moreThanNull(double *num) {
    double val = *num;
    if (val > 0) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}

ComplexDig imagineMoreThanNull(ComplexDig* num) {
    ComplexDig val;
    val.im = 0.0f;
    if (num->im > 0) {
        val.re = 1.0f;
    } else {
        val.re = 0.0f;
    }

    return val;
}

void testSortReal() {
    int i, j, err = 0;
    Array* arr = createArray(10, 8, &realZero);

    for (i = 0; i < 100; i++) {
        for (j = 0; j < arr->count * arr->byte; j += arr->byte) {
            *((double*) (arr + j)) = rand() % 300 - 150;
        }
        sortRealArray(arr);
        for (j = 0; j < (arr->count - 1) * arr->byte; j += arr->byte) {
            if (*((double*) (arr + j)) > *((double*) (arr + j + 1))) {
                err += 1;
                break;
            }
        }

        if (err) {
            printf("%d - ERROR\n", i);
        } else {
            printf("%d - OK\n", i);
        }
    }

    free(arr);
}

void testComplexProperties() {
    ComplexDig a, b, result;
    int i;

    srand(time(NULL));

    printf("Testing summa:\n");
    for (i = 0; i < 100; i++) {
        a.re = rand() % 1000;
        a.im = rand() % 1000;
        b.re = rand() % 1000;
        b.im = rand() % 1000;

        result.re = a.re + b.re;
        result.im = a.im + b.im;

        if (complex_equals(result, *compl_sum(a, b))) {
            printf("%d - OK\n", i);
        } else {
            printf("%d - WRONG\n", i);
        }
    }

    printf("Testing multyplying:\n");
    for (i = 0; i < 100; i++) {
        a.re = rand() % 1000;
        a.im = rand() % 1000;
        b.re = rand() % 1000;
        b.im = rand() % 1000;

        result.re = (a.re * b.re - a.im * b.im);
        result.im = (a.im * b.re + a.re * b.im);

        if (complex_equals(result, *compl_mult(a, b))) {
            printf("%d - OK\n", i);
        } else {
            printf("%d - WRONG\n", i);
        }
    }
}

int main() {
    int req, size;
    Array* arr = NULL;
    srand(time(NULL));

    do {
        printf("Enter 1 to create array with real numbers\n");
        printf("Enter 2 to create array with complex numbers\n");
        printf("Enter 3 to print array\n");
        printf("Enter 4 to concatenate 2 arrays\n");
        printf("Enter 5 to sort entered array\n");
        printf("Enter 6 to square the array's elements (function 'map')\n");
        printf("Enter 7 to mark real numbers more than 0\n");
        printf("Enter 8 to mark complex number with imagine part more than 0\n");
        printf("Enter 9 to run tests\n");
        printf("Enter 0 for exit\n\n");

        scanf("%d", &req);

        if (req == 1 || req == 2) {
            printf("Enter an array's size: \n");
            scanf("%d", &size);

            if (req == 1) {
                arr = createArray(size, 8, &realZero);
                fillRealArray(arr);
            } else {
                arr = createArray(size, 16, &complexZero);
                fillComplexArray(arr);
            }
        }

        if (req == 3) {
            if (arr == NULL) {
                printf("The array doesn't exist!\n");
            } else {
                print_array(arr);
            }
        }

        if (req == 4) {
            Array *arr1, *arr2, *concatArray;
            printf("Enter 2 arrays for their concatenating...\n");

            printf("Size of 1st array: ");
            scanf("%d", &size);
            arr1 = createArray(size, 8, &realZero);
            fillRealArray(arr1);

            printf("Size of 2nd array: ");
            scanf("%d", &size);
            arr2 = createArray(size, 8, &realZero);
            fillRealArray(arr2);

            concatArray = concatenateArrays(arr1, arr2);
            print_array(concatArray);

            free(arr1);         //освобождение памяти
            free(arr2);
            free(concatArray);
        }

        if (req == 5) {
            if (arr == NULL) {
                printf("The array doesn't exist!\n");
            } else if (arr->byte == 8) {
                sortRealArray(arr);
                print_array(arr);
            } else if (arr->byte == 16) {
                sortComplexArray(arr);
                print_array(arr);
            }
        }

        if (req == 6) {
            Array* mapedArray = NULL;

            if (arr == NULL) {
                printf("The array doesn't exist!\n");
            } else if (arr->byte == 8) {
                arr = realMap(realSquare, arr);
                print_array(arr);
            } else {
                mapedArray = complexMap(complexSquare, arr);
                print_array(mapedArray);
            }

            free(mapedArray);
        }

        if (req == 7) {
            Array* whereArray = NULL;

            if (arr == NULL) {
                printf("The array doesn't exist!\n");
            } else if (arr->byte == 8) {
                whereArray = realWhere(moreThanNull, arr);
                print_array(whereArray);
            } else {
                printf("The array is not real!\n");
            }

            free(whereArray);
        }

        if (req == 8) {
            Array* whereArray = NULL;

            if (arr == NULL) {
                printf("The array doesn't exist!\n");
            } else if (arr->byte == 16) {
                whereArray = complexWhere(imagineMoreThanNull, arr);
                print_array(whereArray);
            } else {
                printf("The array is not complex!\n");
            }

            free(whereArray);
        }

        if (req == 9) {
            printf("Sort testing...\n");
            testSortReal();
            printf("\n");
            testComplexProperties();
        }

    } while (req != 0);

    free(arr);
    return 0;
}
