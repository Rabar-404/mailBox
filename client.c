#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "smtp.h"


int main () {
    
    smtp_connect("smtp.gmail.com", "587");
    return 0;
}

