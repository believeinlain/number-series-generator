
#include "stdio.h" // for printing
#include "stdlib.h" // for error handling and atoi
#include "string.h" // for strcmp
#include "pthread.h" // for threading

// generate and print the first n natural numbers
// if n < 0, continue indefinitely
// return the next number in the series
int print_natural_series(int n) {
    int c = n;
    // first natural number
    int num = 0;
    while (c != 0) {
        // display current number
        printf("%d\n", num);

        // find next natural number
        ++num;

        // don't decrement on negative c (so we don't underflow)
        if (c > 0) --c;
    }
    return num;
}

// generate and print the first n prime numbers
// if n < 0, continue indefinitely
// return the next number in the series
int print_prime_series(int n) {
    int c = n;
    // first prime number
    int num = 2;
    while (c != 0) {

        // find next prime number
        int not_prime = 0;

        // printf("%d\n", num);
        int max = num/2;
        for (int i=2; i<=max; ++i)
            if (num%i == 0)
                not_prime = 1;

        // if prime
        if (!not_prime) {
            // display current number
            printf("%d\n", num);
            // don't decrement on negative c (so we don't underflow)
            if (c > 0) --c;
        }
        ++num;
    }
    return num;
}

// generate and print the first n fibonacci numbers
// if n < 0, continue indefinitely
// return the next number in the series
int print_fibonacci_series(int n) {
    int c = n;

    int last = 0;
    int num = 1;

    // display first number
    if (c != 0) {
        printf("%d\n", last);
        --c;
    }
    while (c != 0) {
        // display current number
        printf("%d\n", num);

        // compute the next fibonacci number
        int next = last + num;
        last = num;
        num = next;
        --c;
    }

    return num; 
}

struct thread_args {
    // number of numbers to compute
    int n;
    // pointer to the function we'll call
    int (*series_ptr)(int);
};

void *thread_function(void *argp) {
    struct thread_args *p = (struct thread_args *)argp;

    // call the function pointer passed to thread
    int num = p->series_ptr(p->n);

    printf("Computation completed successfully.\n");
    printf("Next number in series: %d.\n", num);

    return argp;
}

// arguments:
// -n: number of numbers to generate and print
//     if n < 0 continue indefinitely
// -s: series to print
//     'natural' 'prime' or 'fibonacci'
int main(int argc, char *argv[]) {
    int opt;
    char *nvalue = NULL;
    char *s = NULL;
    for (int i=0; i<argc; ++i) {
        if (argv[i][0] == '-') {
            // get nvalue
            if (argv[i][1] == 'n' && i+1 <= argc)
                nvalue = argv[i+1];
            
            // get s
            else if (argv[i][1] == 's' && i+1 <= argc) 
                s = argv[i+1];
        }
    }
    // default arguments
    if (nvalue == NULL) nvalue = "100";
    if (s == NULL) s = "natural";

    // define a struct to collect execution parameters
    struct thread_args p;
    // convert n to an integer
    p.n = atoi(nvalue);

    // name the series we're going to compute
    // and select the desired function
    if (strcmp(s, "natural") == 0) {
        p.series_ptr = &print_natural_series; 
    } else if (strcmp(s, "prime") == 0) {
        p.series_ptr = &print_prime_series; 
    } else if (strcmp(s, "fibonacci") == 0) {
        p.series_ptr = &print_fibonacci_series; 
    } else {
        printf("Please specify a known series");
        exit(EXIT_FAILURE);
    }

    if (p.n<0) printf("Computing %s series indefinitely:\n", s);
    else printf("Computing the first %d %s numbers:\n", p.n, s);

    // create a thread to compute the series
    pthread_t tid;
    pthread_create(&tid, NULL, thread_function, (void * )&p);
    printf("Created new thread, id: %lu\n", tid);

    // end the main thread while leaving the process open
    pthread_exit(NULL);

    return 0;
}

