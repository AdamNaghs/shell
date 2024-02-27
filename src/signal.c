#include "../include/signal.h"
#include "../include/IO.h"
#include <signal.h>
#include <stdlib.h>


void sigint_handler(int a)
{
    printf("Exited" BLU " asn" CRESET ".\nCode %d\n",a);
    exit(0);
}

void bind_signals(void)
{
    signal(SIGINT,sigint_handler);
}