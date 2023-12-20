#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"
#define BYTE unsigned char
#define BYTE_VALUES 0x100

//estructura básica de un int dinámico
typedef struct dynamic_int{
    BYTE * value;
    int size;
    int negative;
}* p_dyn_int;

void normalize_size(p_dyn_int dyn_int);
char * read_input();
p_dyn_int empty_dyn_int();
p_dyn_int sized_dyn_int(int size);
void free_dyn_int(p_dyn_int dyn_int);
p_dyn_int realloc_dyn_int(p_dyn_int dyn_int, int new_size);
p_dyn_int duplicate_dyn_int(p_dyn_int dyn_int);
p_dyn_int copy_dyn_int(p_dyn_int to, p_dyn_int from);
p_dyn_int byte_shift(p_dyn_int dyn_int, int shift);
int compare(p_dyn_int first, p_dyn_int second);
int compare_abs(p_dyn_int first, p_dyn_int second);
p_dyn_int int_to_dyn_int(long long int value);
long long int dyn_int_to_int(p_dyn_int dyn_int);
p_dyn_int assign_dyn_int(p_dyn_int dyn_int, long long int value);
p_dyn_int negate(p_dyn_int dyn_int);
p_dyn_int absolute(p_dyn_int dyn_int);
p_dyn_int sum(p_dyn_int to, p_dyn_int from);
p_dyn_int sum_int(p_dyn_int dyn_Int, long long int value);
p_dyn_int subtraction(p_dyn_int to, p_dyn_int from);
p_dyn_int subtraction_int(p_dyn_int dyn_int, long long int value);
p_dyn_int multiplication(p_dyn_int to, p_dyn_int from);
p_dyn_int multiplication_int(p_dyn_int dyn_int, long long int value);
p_dyn_int division(p_dyn_int to, p_dyn_int from);
p_dyn_int division_int(p_dyn_int dyn_int, long long int value);
p_dyn_int modulo(p_dyn_int to, p_dyn_int from);
p_dyn_int modulo_int(p_dyn_int dyn_int, long long int value);
char * dyn_int_to_string(p_dyn_int dyn_int);
p_dyn_int string_to_dyn_int(char *string);
p_dyn_int print_dyn_int(p_dyn_int dyn_int);

//elimina los "ceros a la izquierda" que puede tener un numero al operar
void normalize_size(p_dyn_int dyn_int){
    int i;

    //empezar en bloques de 8 bytes
    for(i = dyn_int->size - 9; i >= 0 && *(unsigned long long int *)(dyn_int->value + i) == 0; i-= 8);

    //seguir byte a byte
    for(i+=8; i >= 0 && dyn_int->value[i] == 0; i--);

    dyn_int->size = i + 1;
}

char * read_input(){
    char * string = malloc(1);
    char * aux;
    int i, j;

    string[0] = (char) getchar();
    for(i = 1; string[i - 1] != '\n' && string[i - 1] != ' '; i++){
        aux = malloc(i + 1);
        for(j = 0; j < i; j++)
            aux[j] = string[j];
        free(string);
        string = aux;
        string[i] = (char) getchar();
    }

    string[i - 1] = '\0';

    return string;
}

//crea y devuelve un int dinámico vacío
p_dyn_int empty_dyn_int(){
    p_dyn_int dyn_int = malloc(16); //sizeof(struct dynamic_int)
    dyn_int->size = 0;
    dyn_int->value = NULL;
    dyn_int->negative = 0;
    return dyn_int;
}

//crea y devuelve un int dinámico del tamaño especificado
p_dyn_int sized_dyn_int(int size){
    p_dyn_int dyn_int = malloc(16); //sizeof(struct dynamic_int)
    int i;
    dyn_int->size = size;
    dyn_int->negative = 0;
    dyn_int->value = malloc(size);

    //inicializar a 0 por bloques
    for(i = 0; i < size - 7; i+= 8)
        *(unsigned long long int *)(dyn_int->value + i) = 0;

    //terminar byte a byte
    for(;i < size; i++)
        dyn_int->value[i] = 0;

    return dyn_int;
}

//libera un int dinámico por completo
void free_dyn_int(p_dyn_int dyn_int){
    free(dyn_int->value);
    free(dyn_int);
}

//guarda los valores en un nuevo array y cambia el tamaño
p_dyn_int realloc_dyn_int(p_dyn_int dyn_int, int new_size){
    int i, min_size = new_size;
    unsigned char * value;

    //saltar si son del mismo tamaño
    if(dyn_int->size < new_size) {

        //get min size
        if(dyn_int->size < min_size)
            min_size = dyn_int->size;

        value = malloc(new_size);

        //rellenar valores en bloques de 8 bytes
        for (i = 0; i < min_size - 7; i+=8)
            *(unsigned long long int *)(value + i) = *(unsigned long long int *)(dyn_int->value + i);

        //terminar byte a byte
        for(; i < min_size; i++)
            value[i] = dyn_int->value[i];

        //rellenar con 0 en bloques de 8
        for(; i < new_size - 7; i+=8)
            *(unsigned long long int *)(value + i) = 0;

        //terminar de rellenar byte a byte
        for(; i < new_size; i++)
            value[i] = 0;

        dyn_int->size = new_size;
        free(dyn_int->value);
        dyn_int->value = value;
    }else{
        //si el tamaño nuevo es menor simplemente cambiar size
        dyn_int->size = new_size;
    }

    return dyn_int;
}

//duplica el int dinámico
p_dyn_int duplicate_dyn_int(p_dyn_int dyn_int){
    p_dyn_int new = sized_dyn_int(dyn_int->size);
    int i;
    new->negative = dyn_int->negative;

    //duplicar en bloques de 8 bytes
    for(i = 0; i < dyn_int->size - 7; i+= 8){
        *(unsigned long long *)(new->value + i) = *(unsigned long long int *)(dyn_int->value + i);
    }

    //terminar byte a byte
    for(; i < dyn_int->size; i++)
        new->value[i] = dyn_int->value[i];

    return new;
}

//copia el int dinámico from a to
p_dyn_int copy_dyn_int(p_dyn_int to, p_dyn_int from){
    int i;

    //reasignar memoria
    realloc_dyn_int(to, from->size);
    to->negative = from->negative;

    //rellenar array en bloques de 8 bytes
    for(i = 0; i < from->size - 7; i +=8){
        *(unsigned long long int *)(to->value + i) = *(unsigned long long int *)(from->value + i);
    }

    //rellenar array en byte a byte
    for(; i < from->size; i++){
        to->value[i] = from->value[i];
    }

    return to;
}

//básicamente, multiplica o divide por 256, pero es más rápido porque simplemente cambia las cifras
p_dyn_int byte_shift(p_dyn_int dyn_int, int shift){

    if(!(dyn_int->size && shift))
        return dyn_int;

    int i, size = dyn_int->size + shift;
    unsigned char * aux = malloc(size);


    //rellenar con 0 en bloques de 8 bytes
    for(i = 0; i < shift - 7; i+= 8)
        *(unsigned long long int *)(aux + i) = 0;

    //rellenar con 0 byte a byte
    for(; i < shift; i++)
        aux[i] = 0;

    //copiar el array en bloques de 8 bytes
    for(; i < size - 7; i+=8){
        *(unsigned long long int *)(aux + i) = *(unsigned long long int *)(dyn_int->value + i - shift);
    }

    //copiar lo que queda del array byte a byte
    for(;i < size; i++){
        aux[i] = dyn_int->value[i - shift];
    }

    //liberar memoria
    free(dyn_int->value);
    dyn_int->value = aux;
    dyn_int->size = size;

    return dyn_int;
}

//compara dos int dinámicos y devuelve -1 si el primero es menor, 0 si son iguales o 1 si el primero es mayor
int compare(p_dyn_int first, p_dyn_int second){
    //caso especial siendo ceros
    if(!first->size && !second->size)
        return 0;

    //comparar signos
    if(first->negative && !second->negative)
        return -1;
    if(!first->negative && second->negative)
        return 1;

    return compare_abs(first, second);
}

int compare_abs(p_dyn_int first, p_dyn_int second){
    int i;

    //caso especial siendo ceros
    if(!first->size && !second->size)
        return 0;

    //comparar tamaños
    if(first->size < second->size)
        return -1;
    if(first->size > second->size)
        return 1;

    //comparar dígitos en bloques de 8 bytes
    for(i = first->size - 8; i >= 0; i-=8) {
        if(*(unsigned long long int *)(first->value + i) < *(unsigned long long int *)(second->value + i))
            return -1;
        if(*(unsigned long long int *)(first->value + i) > *(unsigned long long int *)(second->value + i))
            return 1;
    }

    //comparar en el final byte a byte
    for(i+=7; i >= 0; i--){
        if(first->value[i] < second->value[i])
            return -1;
        if(first->value[i] > second->value[i])
            return 1;
    }

    //si lo demás falla, son iguales
    return 0;
}

//crea un nuevo int dinámico a partir del valor dado
p_dyn_int int_to_dyn_int(long long int value){
    //asignar memoria
    p_dyn_int dyn_int = sized_dyn_int(8); //sizeof(unsigned long long int)

    if(value < 0){
        value *= -1;
        dyn_int->negative = 1;
    }

    *(unsigned long long int *)(dyn_int->value) = value;

    //reajustar el tamaño real
    normalize_size(dyn_int);

    return dyn_int;
}

//devuelve el valor del dint dinámico en un long long int
long long int dyn_int_to_int(p_dyn_int dyn_int){
    long long int value;

    value = *(long long int *)(dyn_int->value) & 0x7fffffffffffffff;

    if(dyn_int->negative)
        value *= -1;//NOTA: No puede convertir min long long int

    return value;
}

//le asigna a un int dinámico el valor dado
p_dyn_int assign_dyn_int(p_dyn_int dyn_int, long long int value){
    free(dyn_int->value);
    dyn_int->value = malloc(8); //sizeof(unsigned long long int)
    if(value < 0){
        value *= -1;
        dyn_int->negative = 1;
    }else{
        dyn_int->negative = 0;
    }

    *(unsigned long long int *)(dyn_int->value) = value;
    dyn_int->size = 8;
    normalize_size(dyn_int);
    return dyn_int;
}

//niega un valor
p_dyn_int negate(p_dyn_int dyn_int){
    dyn_int->negative = !dyn_int->negative;

    return dyn_int;
}

p_dyn_int absolute(p_dyn_int dyn_int){
    dyn_int->negative = 0;

    return dyn_int;
}


//sum dos valores y los guarda en la dirección del primero
p_dyn_int sum(p_dyn_int to, p_dyn_int from){
    if(to->negative ^ from->negative){
        //caso en el que se suma un positivo y un negativo

        p_dyn_int aux1;
        p_dyn_int aux2;

        if(from->negative) {
            aux1 = duplicate_dyn_int(to);
            aux2 = duplicate_dyn_int(from);
        } else{
            aux1 = duplicate_dyn_int(from);
            aux2 = duplicate_dyn_int(to);
        }

        negate(aux2);
        copy_dyn_int(to, aux1);
        subtraction(to,aux2);

        free_dyn_int(aux1);
        free_dyn_int(aux2);
        return to;
    }

    int i, min, max;
    unsigned char * arr, * max_dir;

    //ajustar tamaño de res
    if(to->size < from->size) {
        min = to->size;
        max = from->size;
        max_dir = from->value;
    }else {
        min = from->size;
        max = to->size;
        max_dir = to->value;
    }

    arr = malloc(max + 1);

    //inicializar a 0
    for(i = 0; i <= max - 7; i += 8)
        *(unsigned long long int *)(arr + i) = 0;

    for(; i <= max; i++)
        arr[i] = 0;

    //sumar valores en bloques de 7 bytes
    for(i = 0; i < min - 7; i+=7)
        *(unsigned long long int *)(arr + i) += ((*(unsigned long long int *)(to->value + i)) & 0x00ffffffffffffff) + ((*(unsigned long long int *)(from->value + i)) & 0x00ffffffffffffff);

    //sumar byte a byte
    for(;i < min; i++)
        *(unsigned short int *)(arr + i) += to->value[i] + from->value[i];

    //sumarle lo que le quede del max
    for(; i < max - 7; i+= 7)
        *(unsigned long long int *)(arr + i) += (*(unsigned long long *)(max_dir + i)) & 0x00ffffffffffffff;

    //sumar byte a byte
    for(; i < max; i++)
        *(unsigned short int *)(arr + i) += max_dir[i];



    //normalizar tamaño
    free(to->value);
    to->value = arr;
    to->size = max + 1;
    normalize_size(to);

    return to;
}

//todo hacer caso rápido
//ejecuta sum pero con un entero normal en vez de dinámico
p_dyn_int sum_int(p_dyn_int dyn_Int, long long int value){
    p_dyn_int aux = int_to_dyn_int(value);
    sum(dyn_Int, aux);
    free_dyn_int(aux);
    return dyn_Int;
}

//le resta el valor de from a to
p_dyn_int subtraction(p_dyn_int to, p_dyn_int from){

    //caso en el que uno de ellos es negativo
    if(to->negative ^ from->negative){
        p_dyn_int aux = duplicate_dyn_int(from);
        negate(aux);
        sum(to, aux);
        free_dyn_int(aux);

        return to;
    }

    //caso en el que el resultado de la resta vaya a ser negativo
    if(compare_abs(to, from) < 0){
        negate(to);
        return negate(sum(to, from));
    }

    int i;
    unsigned char * arr = malloc(to->size + 1); //el +1 solo está para que no escriba fuera de memoria, pero no se usa

    //inicializar a 0
    for(i = 0; i <= to->size - 7; i += 8)
        *(unsigned long long int *)(arr + i) = 0;

    for(; i <= to->size; i++)
        arr[i] = 0;


    //ir restando en bloques de 7 en 7 bytes
    for(i = 0; i < from->size - 7; i+= 7)
        *(unsigned long long int *)(arr + i) = (*(unsigned long long int *)(to->value + i) & 0x00ffffffffffffff) - ((*(unsigned long long int *)(from->value + i)) & 0x00ffffffffffffff)- (arr[i] != 0);// originalmente !!arr[i]

    //terminar restando byte a byte
    for(; i < from->size; i++)
        *(unsigned short int *)(arr + i) = to->value[i] - from->value[i] - (arr[i] != 0);// originalmente !!arr[i]

    //terminar igualando menos el posible carry
    for(; i < to->size - 7; i+= 7)
        *(unsigned long long int *)(arr + i) = (*(unsigned long long int *)(to->value + i) & 0x00ffffffffffffff) - (arr[i] != 0);// originalmente !!arr[i]

    for(; i < to->size; i++)
        *(unsigned short int *)(arr + i) = to->value[i] - (arr[i] != 0);// originalmente !!arr[i]

    //cambiar el array de to
    free(to->value);
    to->value = arr;


    //reajustar tamaño
    normalize_size(to);

    return to;
}

//todo hacer caso rápido
//lo mismo que subtraction pero con un entero
p_dyn_int subtraction_int(p_dyn_int dyn_int, long long int value){
    p_dyn_int aux = int_to_dyn_int(value);
    subtraction(dyn_int, aux);
    free_dyn_int(aux);
    return dyn_int;
}

/* //experimento algoritmo rápido de multiplicación
typedef struct vector{
    unsigned char * vector;
    int size;
}* pv;

pv ini_vector(int size){
    int i;
    pv res = malloc(sizeof(struct vector));
    res->size = size;
    res->vector = malloc(size);

    for(i = 0; i < size - 7; i+=8)
        *(unsigned long long *)(res->vector + i) = 0;

    for(; i < size; i++)
        res->vector[i] = 0;

    return res;
}

//to siempre debería ser el mayor de los dos, incluso con el shift
pv shifted_sum(pv to, pv from, int shift){
    int i;
    unsigned char * arr = malloc(to->size);

    //copiar valores hasta shift
    for(i = 0; i < shift-7; i+=8)
        *(unsigned long long int *)(arr + i) = *(unsigned long long int *)(to->vector + i);
    for(; i < shift; i++)
        arr[i] = to->vector[i];

    //sumar valores
    for(; i < from->size + shift - 7; i+=8)
        *(unsigned long long int *)(arr + i) += ((*(unsigned long long int *)(to->vector + i)) & 0xffffffffffffff) + ((*(unsigned long long int *)(from->vector + i)) & 0xffffffffffffff);
    for(; i < from->size + shift; i++)
        *(unsigned short int *)(arr + i) += to->vector[i] + from->vector[i];


    //terminar de copiar el resultado sumando de 7 en 7 bytes por si acaso hay carry
    for(; i < to->size - 7; i+=7)
        *(unsigned long long int *)(arr + i) += (*(unsigned long long int *)(to->vector + i)) & 0xffffffffffffff;
    for(; i < to->size - 1; i++)
        *(unsigned short int *)(arr + i) += to->vector[i];

    arr[i] += to->vector[i];

    //liberar y reasignar memoria
    free(to->vector);
    to->vector = arr;

    return to;
}

//ir sumando recursivamente los productos ac * 10^2 + ((a + b)(c + d) - ac - bd) * 10 + bd
//las entradas serán del mismo tamaño
pv karatsuba(pv res, pv upper, pv lower){
    pv a = malloc(sizeof(struct vector)),b = malloc(sizeof(struct vector)),c = malloc(sizeof(struct vector)),d = malloc(sizeof(struct vector))
            , ops1, ops2, ac, bd, abcd;
    if(upper->size <= 4){
        int i;
        unsigned long long int mask = 0;
        //get mask
        for(i = 0; i < upper->size; i++){
            mask <<= 8;
            mask |= 0xff;
        }
        //caso base
        *(unsigned long long int *)(res->vector) += ((*(int *)(upper->vector)) & mask) * ((*(int *)(lower->vector)) & mask);
    }else{
        //caso general
        //separar upper
        a->vector = upper->vector;
        a->size = upper->size / 2;
        b->vector = a->vector + a->size;
        b->size = upper->size - a->size;

        //separar lower
        c->vector = lower->vector;
        c->size = lower->size / 2;
        d->vector = c->vector + c->size;
        d->size = lower->size - c->size;

        //get size de intermedios
        ops1 = ini_vector(b->size + 1);
        ops2 = ini_vector(d->size + 1);
        ac = ini_vector(a->size + c->size);
        bd = ini_vector(b->size + d->size);
        abcd = ini_vector(ops1->size + ops2->size);

        //get ops1
        shifted_sum(ops1, a, 0);
        shifted_sum(ops1, b, 0);

        //get ops2
        shifted_sum(ops2, c, 0);
        shifted_sum(ops2, d, 0);

        //get abdc
        karatsuba(abcd, ops1, ops2);

        //get ac y ab
        karatsuba(ac, a, c);
        karatsuba(bd, b, d);

        //sumar resultados con shift adecuado
        shifted_sum(res, bd, 0);
        shifted_sum(res, abcd, b->size);
        shifted_sum(res, ac, b->size + d->size);

        free(ops1->vector);
        free(ops2->vector);
        free(ac->vector);
        free(bd->vector);
        free(abcd->vector);

        free(ops1);
        free(ops2);
        free(ac);
        free(bd);
        free(abcd);
    }

    free(a);
    free(b);
    free(c);
    free(d);

    return res;
}

//hace la multiplicación from de to (utilizando multiplicación por karatsuba)
p_dyn_int multiplication(p_dyn_int to, p_dyn_int from){
    int max = to->size, i;
    if(max < from->size)
        max = from->size;

    max = ((int)log2(max)) + 2;
    max = (int)pow(2, max);

    pv res = ini_vector(max * 2);
    pv upper = ini_vector(max), lower = ini_vector(max);

    //copiar valores en grupos de 8
    for(i = 0; i < to->size - 7; i+=8)
        *(unsigned long long int *)(upper->vector + i) = *(unsigned long long int *)(to->value + i);
    for(;i < to->size; i++)
        upper->vector[i] = to->value[i];

    for(i = 0; i < from->size - 7; i+=8)
        *(unsigned long long int *)(lower->vector + i) = *(unsigned long long int *)(from->value + i);
    for(; i < to->size; i++)
        lower->vector[i] = from->value[i];


    karatsuba(res, upper, lower);

    free(to->value);
    to->value = res->vector;
    to->size = res->size;

    normalize_size(to);

    to->negative = to->negative ^ from->negative;
    free(upper->vector);
    free(lower->vector);
    free(upper);
    free(lower);
    return to;
}*/

//se asume que dyn_int tiene el tamaño adecuado
p_dyn_int shifted_sum(p_dyn_int dyn_int, unsigned long long int value, int shift){
    int i;

    unsigned char * arr = malloc(dyn_int->size);

    //inicializar a 0
    for(i = 0; i < dyn_int->size - 7; i+=8)
        *(unsigned long long int *)(arr + i) = 0;
    for(;i < dyn_int->size; i++)
        arr[i] = 0;

    //copiar los valores del dyn_int hasta shift
    for(i = 0; i < shift - 7; i += 8)
        *(unsigned long long int *)(arr + i) = *(unsigned long long int *)(dyn_int->value + i);
    for(; i < shift; i++)
        arr[i] = dyn_int->value[i];

    //sumar value
    *(unsigned long long int*)(arr + i) = ((*(unsigned long long int *)(dyn_int->value + i)) & 0xffffffffffffff) + (value & 0xffffffffffffff);
    i+=7;
    *(unsigned short int *)(arr + i) += dyn_int->value[i] + (value >> (8 * 7));
    i++;

    //terminar sumando los valores de dyn int
    for(; i < dyn_int->size - 7; i+= 7)
        *(unsigned long long int *)(arr + i) += (*(unsigned long long int *)(dyn_int->value + i)) & 0xffffffffffffff;
    for(; i < dyn_int->size; i++)
        *(unsigned short int *)(arr + i) += dyn_int->value[i];

    free(dyn_int->value);
    dyn_int->value = arr;

    return dyn_int;
}

//hace la multiplicación from de to
p_dyn_int multiplication(p_dyn_int to, p_dyn_int from){
    p_dyn_int res = sized_dyn_int(to->size + from->size + 8);
    unsigned long long int value;
    int i, j, t, k;

    //se le va sumando a res el producto movido el número de pasos necesarios
    for(i = 0, j = 0; i < from->size - 3; i+=4)
        for(j = 0; j < to->size - 3; j+=4){
            value = (*(unsigned int *)(to->value + j)) * (unsigned long long int)(*(unsigned int *)(from->value + i));
            shifted_sum(res, value, i + j);
        }


    //hacer los 2 grupos medianos que faltan
    for(t = i; t < from->size; t++)
        for(k = 0; k < to->size - 3; k+=4){
            value = from->value[t] * (unsigned long long int)(*(unsigned int *)(to->value + k));
            shifted_sum(res, value, t + k);
        }

    for(t = 0; t < from->size - 3; t+=4)
        for(k = j; k < to->size; k++){
            value = (*(unsigned int *)(from->value + t)) * (unsigned long long int)to->value[k];
            shifted_sum(res, value, t + k);
        }


    //terminar los numeros que faltan
    for(t = i; t < from->size; t++)
        for(k = j; k < to->size; k++){
            value = from->value[t] * (unsigned long long int)to->value[k];
            shifted_sum(res, value, t + k);
        }


    res->negative = to->negative ^ from->negative;
    copy_dyn_int(to, res);
    free_dyn_int(res);
    normalize_size(to);
    return to;
}

//todo hacer caso rápido
//hace multiplication pero con un int
p_dyn_int multiplication_int(p_dyn_int dyn_int, long long int value){
    p_dyn_int aux = int_to_dyn_int(value);
    multiplication(dyn_int, aux);
    free_dyn_int(aux);
    return dyn_int;
}

//hace la división from de to
p_dyn_int division(p_dyn_int to, p_dyn_int from){
    p_dyn_int res = empty_dyn_int();
    p_dyn_int carry = empty_dyn_int();
    p_dyn_int aux = empty_dyn_int();
    int factor, i, value;

    if(compare_abs(to, from) <= 0){
        if(compare_abs(to, from) < 0)
            //si to es menor que from la división va a dar 0
            assign_dyn_int(res, 0);
        else
            //si to es igual que from, la división va a dar 1
            assign_dyn_int(res, 1);
    } else {
        //get los valores por los que se va a dividir
        i = to->size - 1;
        while (i >= 0) {

            while (compare_abs(carry, from) < 0 && i >= 0) {
                byte_shift(res, 1);
                byte_shift(carry, 1);
                sum_int(carry, to->value[i--]);
            }
            //a este punto carry es mayor o igual que from (a no ser que 'i' sea -1)

            factor = BYTE_VALUES;
            value = 0;
            while(factor > 1){
                factor /= 2;
                value += factor;
                absolute(copy_dyn_int(aux, from));
                multiplication_int(aux, value);
                if(compare(aux, carry) > 0)
                    value -= factor;
            }
            //a este punto value es el resultado de dividir carry entre from
            sum_int(res, value);
            subtraction(carry, multiplication_int(absolute(copy_dyn_int(aux, from)), value));
        }
    }

    res->negative = to->negative ^ from->negative;
    res->negative = res->negative && res->size;
    copy_dyn_int(to, res);
    free_dyn_int(res);
    free_dyn_int(carry);
    free_dyn_int(aux);
    return to;
}

//todo hacer caso rápido
//hace division pero con un int
p_dyn_int division_int(p_dyn_int dyn_int, long long int value){
    p_dyn_int aux = int_to_dyn_int(value);
    division(dyn_int, aux);
    free_dyn_int(aux);
    return dyn_int;
}

//hace el módulo from de to
p_dyn_int modulo(p_dyn_int to, p_dyn_int from){
    p_dyn_int aux = division(duplicate_dyn_int(to), from);
    subtraction(to, multiplication(aux, from));
    free_dyn_int(aux);
    return absolute(to);
}

//todo hacer caso rápido
//hace modulo pero con un int
p_dyn_int modulo_int(p_dyn_int dyn_int, long long int value){
    p_dyn_int aux = int_to_dyn_int(value);
    modulo(dyn_int, aux);
    free_dyn_int(aux);
    return dyn_int;
}

int char_to_int(char letra){
    return (letra - 48) % 10; //el módulo solo está para que no dé cosas muy locas
}

char int_to_char(long long int numero){
    return (char) (numero + 48);
}

char * dyn_int_to_string(p_dyn_int dyn_int){
    char * res = malloc(3 * dyn_int->size + 2);
    p_dyn_int aux = absolute(duplicate_dyn_int(dyn_int));
    p_dyn_int ops = empty_dyn_int();
    int i = 0, j;
    char c;

    //repetir mientras sea mayor que 0
    while(aux->size > 0){
        modulo_int(copy_dyn_int(ops, aux), 10);
        division_int(aux, 10);
        res[i++] = int_to_char(dyn_int_to_int(ops));
    }

    //repetir para quitar los 0 del principio
    while(res[i - 1] == '0')
        i--;

    //añadir negativo si es necesario
    if(dyn_int->negative)
        res[i++] = '-';

    res[i] = '\0';


    //darle la vuelta a res
    for(j = 0; j < i/2; j++){
        c = res[j];
        res[j] = res[i - j - 1];
        res[i - j - 1] = c;
    }

    //caso especial en el que la entrada es 0
    if(dyn_int->size == 0){
        res[0] = '0';
        res[1] = '\0';
    }

    free_dyn_int(ops);
    free_dyn_int(aux);
    return res;
}

p_dyn_int string_to_dyn_int(char *string){
    p_dyn_int res = empty_dyn_int();
    //ver la primera letra por si es negativo
    int negativo = 0;
    if(string[0] == '-')
        negativo = 1;
    else
        assign_dyn_int(res, char_to_int(string[0]));

    //ir letra a letra asignando valores
    for(int i = 1; string[i] != '\0'; i++){
        multiplication_int(res, 10);
        sum_int(res, char_to_int(string[i]));
    }

    res->negative = negativo;

    return res;
}

p_dyn_int print_dyn_int(p_dyn_int dyn_int){
    char * string;
    printf("\n");
    printf("Pointing to:        %p\n", dyn_int);
    printf("Value at:           %p\n", dyn_int->value);
    printf("Sign:               ");
    if(dyn_int->negative)
        printf("-");
    else
        printf("+");
    printf("\n");
    printf("Size:               %i\n", dyn_int->size);
    printf("Decimal value:      %s\n", string = dyn_int_to_string(dyn_int));
    printf("Hexadecimal values: ");

    for (int i = 0; i < dyn_int->size; i++) {
        if (!(i % 10) && i)
            printf(" -%3i\n                    ", i/10);
        printf("%02X ", dyn_int->value[i]);
    }

    printf("\n");
    free(string);
    return dyn_int;
}


int main() {

    char * input;

    printf("\nIntroduce el primer numero: ");
    input = read_input();
    p_dyn_int t1 = string_to_dyn_int(input);
    free(input);
    print_dyn_int(t1);

    printf("\nIntroduce el segundo numero: ");
    input = read_input();
    p_dyn_int t2 = string_to_dyn_int(input);
    free(input);
    print_dyn_int(t2);

    if(compare(t1, t2) < 0)
        printf("\nEl primer numero es menor que el segundo\n\n");
    else if(compare(t1, t2) > 0)
        printf("\nEl primer numero es mayor que el segundo\n\n");
    else
        printf("\nLos dos numeros son iguales\n\n");


    p_dyn_int res = duplicate_dyn_int(t1);
    printf("\nSu suma es: ");
    print_dyn_int(sum(res, t2));


    copy_dyn_int(res, t1);
    printf("\nSu resta es: ");
    print_dyn_int(subtraction(res, t2));


    copy_dyn_int(res, t1);
    printf("\nSu producto es: ");
    print_dyn_int(multiplication(res, t2));


    copy_dyn_int(res, t1);
    printf("\nSu division es: ");
    print_dyn_int(division(res, t2));


    copy_dyn_int(res, t1);
    printf("\nSu modulo es: ");
    print_dyn_int(modulo(res, t2));


    long long int num;
    printf("\n\n\nQue numero de fibonacci quieres?\n");
    free_dyn_int(res);
    res = string_to_dyn_int(input = read_input());
    free(input);
    num = dyn_int_to_int(res);
    printf("\n\n\n");

    if(num <= 1)
        printf("0");
    else if(num == 2)
        printf("1");
    else {

        assign_dyn_int(t1, 0);
        assign_dyn_int(t2, 1);
        while (num > 1) {
            sum(copy_dyn_int(res,t2), t1);
            copy_dyn_int(t1, t2);
            copy_dyn_int(t2, res);
            num--;
        }

        print_dyn_int(res);
    }

    free_dyn_int(res);
    free_dyn_int(t1);
    free_dyn_int(t2);

    getchar();
    return 0;
}
#pragma clang diagnostic pop