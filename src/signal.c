#include "../include/signal.h"
#include "../include/IO.h"
#include <signal.h>
#include <stdlib.h>
#include "../include/shell.h"


void sigint_handler(int a)
{
    printf("SigInt" BLU " asn" CRESET ".\nCode %d\n",a);
    shell_stop();
}

void bind_signals(void)
{
    signal(SIGINT,sigint_handler);
}
