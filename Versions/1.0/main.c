#include <stdio.h>
#include <stdlib.h>
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

char * read_input();
p_dyn_int empty_dyn_int();
p_dyn_int sized_dyn_int(int size);
void free_dyn_int(p_dyn_int dyn_int);
p_dyn_int realloc_dyn_int(p_dyn_int dyn_int, int new_size);
p_dyn_int duplicate_dyn_int(p_dyn_int din_int);
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
    p_dyn_int dyn_int = malloc(sizeof(struct dynamic_int));
    dyn_int->size = 0;
    dyn_int->value = NULL;
    dyn_int->negative = 0;
    return dyn_int;
}

//crea y devuelve un int dinámico del tamaño especificado
p_dyn_int sized_dyn_int(int size){
    p_dyn_int dyn_int = malloc(sizeof(struct dynamic_int));
    dyn_int->size = size;
    dyn_int->negative = 0;
    dyn_int->value = malloc(size * sizeof(BYTE));

    //inicializar a 0
    for(int i = 0; i < size; i++)
        dyn_int->value[i] = 0;

    return dyn_int;
}

//libera un int dinámico por completo
void free_dyn_int(p_dyn_int dyn_int){
    free(dyn_int->value);
    free(dyn_int);
}

//guarda los valores en un nuevo int dinámico y cambia el tamaño
p_dyn_int realloc_dyn_int(p_dyn_int dyn_int, int new_size){

    //saltar si son del mismo tamaño
    if(dyn_int->size != new_size) {

        unsigned char * value = malloc(new_size * sizeof(BYTE));

        for (int i = 0; i < dyn_int->size && i < new_size; i++)
            value[i] = dyn_int->value[i];

        //rellenar con 0 si es más grande
        for(int i = dyn_int->size; i < new_size; i++)
            value[i] = 0;

        dyn_int->size = new_size;
        free(dyn_int->value);
        dyn_int->value = value;
    }

    return dyn_int;
}

//duplica el int dinámico
p_dyn_int duplicate_dyn_int(p_dyn_int din_int){
    p_dyn_int new = sized_dyn_int(din_int->size);
    new->negative = din_int->negative;

    for(int i = 0; i < din_int->size; i++){
        new->value[i] = din_int->value[i];
    }

    return new;
}

//copia el int dinámico from a to
p_dyn_int copy_dyn_int(p_dyn_int to, p_dyn_int from){

    //reasignar memoria
    to->size = from->size;
    to->negative = from->negative;
    free(to->value);
    to->value = malloc(from->size * sizeof(BYTE));

    //rellenar array
    for(int i = 0; i < from->size; i++){
        to->value[i] = from->value[i];
    }

    return to;
}

//básicamente, multiplica o divide por 256, pero es más rápido porque simplemente cambia las cifras
p_dyn_int byte_shift(p_dyn_int dyn_int, int shift){

    //terminar si size es 0
    if(dyn_int->size == 0)
        return dyn_int;

    p_dyn_int aux = sized_dyn_int(dyn_int->size + shift);
    aux->negative = dyn_int->negative;

    int i;

    if (shift < 0)
        i = 0;
    else
        i = shift;

    for(; i < aux->size; i++){
        aux->value[i] = dyn_int->value[i-shift];
    }

    copy_dyn_int(dyn_int, aux);
    free_dyn_int(aux);

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

    //comparar tamaños
    if(first->size < second->size)
        return -1;
    if(first->size > second->size)
        return 1;

    //comparar dígitos
    for(int i = first->size -1; i >= 0; i--) {
        if(first->value[i] < second->value[i])
            return -1;
        if(first->value[i] > second->value[i])
            return 1;
    }

    //si lo demás falla, son iguales
    return 0;
}

int compare_abs(p_dyn_int first, p_dyn_int second){
    //comparar tamaños
    if(first->size < second->size)
        return -1;
    if(first->size > second->size)
        return 1;

    //comparar dígitos
    for(int i = first->size -1; i >= 0; i--) {
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
    //get size
    int size = 0;
    long long int aux = value;
    while (aux != 0) {
        aux /= BYTE_VALUES;
        size++;
    }

    //asignar memoria
    p_dyn_int dyn_int = sized_dyn_int(size);
    if(value < 0){
        dyn_int->negative = 1;
        value *= -1;//NOTA: No puede convertir min long long int
    }

    //rellenar array
    for(int i = 0; i < size; i++){
        aux = value % BYTE_VALUES;
        value /= BYTE_VALUES;
        dyn_int->value[i] = aux;
    }

    return dyn_int;
}

//devuelve el valor del dint dinámico en un long long int
long long int dyn_int_to_int(p_dyn_int dyn_int){
    long long int value = 0;

    for(int i = dyn_int->size - 1; i >= 0; i--){
        value *= BYTE_VALUES;
        value += dyn_int->value[i];
    }

    if(dyn_int->negative)
        value *= -1;//NOTA: No puede convertir min long long int

    return value;
}

//le asigna a un int dinámico el valor dado
p_dyn_int assign_dyn_int(p_dyn_int dyn_int, long long int value){
    p_dyn_int aux = int_to_dyn_int(value);
    copy_dyn_int(dyn_int, aux);
    free_dyn_int(aux);
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

    //igualar el tamaño del to si es más pequeño
    if(to->size < from->size)
        realloc_dyn_int(to, from->size);

    int size = from->size;
    int i, carry;


    //sumar valores y con carry
    for(i = 0, carry = 0; i < size; i++){
        to->value[i] += from->value[i] + carry;
        //si el valor es menor al del from + carry, significa que ha habido overflow
        if(to->value[i] < from->value[i] + carry)
            carry=1;
        else
            carry=0;
    }

    //añadir carry hasta que no se pueda si es necesario
    size = to->size;
    for(i = from->size; i < size && carry; i++){
        to->value[i] += carry;
        if(to->value[i])
            //si ocurre to.value, significa que no ha habido overflow, por lo que carry es 0
            carry = 0;
    }


    if(carry) {
        //si todavía hay carry, significa que hay que agrandar el tamaño
        realloc_dyn_int(to, size + 1);
        to->value[size] = carry;

    }
    return to;
}

//ejecuta sum pero con un entero normal en vez de dinámico
p_dyn_int sum_int(p_dyn_int dyn_Int, long long int value){
    p_dyn_int aux = int_to_dyn_int(value);
    sum(dyn_Int, aux);
    free_dyn_int(aux);
    return dyn_Int;
}

//le resta el valor de from a to
p_dyn_int subtraction(p_dyn_int to, p_dyn_int from){

    if(to->negative ^ from->negative){
        p_dyn_int aux = duplicate_dyn_int(from);
        negate(aux);
        sum(to, aux);
        free_dyn_int(aux);

        return to;
    }

    int i, carry, aux;

    if(compare_abs(to, from) < 0){
        negate(to);
        return negate(sum(to, from));
    }

    for(i = 0, carry = 0; i < from->size; i++){
        aux = to->value[i];
        to->value[i] -= from->value[i] + carry;
        if(to->value[i] + carry > aux){
            carry = 1;
        } else{
            carry = 0;
        }
    }

    //seguir hasta que no halla carry, si es que hay
    for(;carry && i < to->size;i++){
        aux = to->value[i];
        to->value[i] -= carry;
        if(to->value[i] + carry < aux){
            carry = 0;
        }
    }

    //get nuevo tamaño
    for(i = to->size; i > 0 && !to->value[i-1]; i--);

    //reajustar si es necesario
    if(to->size != i)
        realloc_dyn_int(to, i);

    return to;
}

//lo mismo que subtraction pero con un entero
p_dyn_int subtraction_int(p_dyn_int dyn_int, long long int value){
    p_dyn_int aux = int_to_dyn_int(value);
    subtraction(dyn_int, aux);
    free_dyn_int(aux);
    return dyn_int;
}

//hace la multiplicación from de to
p_dyn_int multiplication(p_dyn_int to, p_dyn_int from){
    p_dyn_int res = empty_dyn_int();
    p_dyn_int aux = empty_dyn_int();
    long long int value;

    //se le va sumando a res el producto movido el número de pasos necesarios
    for(int i = 0, j; i < from->size; i++){
        for(j = 0; j < to->size; j++){
            value = to->value[j] * from->value[i];
            free_dyn_int(aux);
            aux = int_to_dyn_int(value);
            sum(res, byte_shift(aux, i + j));
        }
    }

    res->negative = to->negative ^ from->negative;
    copy_dyn_int(to, res);
    free_dyn_int(res);
    free_dyn_int(aux);
    return to;
}

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
            while(factor > 0){
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
    copy_dyn_int(to, res);
    free_dyn_int(res);
    free_dyn_int(carry);
    free_dyn_int(aux);
    return to;
}

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
        sum_int(res, char_to_int(string[0]));

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