#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include "Commands/commands.h"

int main() {
    Particle *particle = newParticle();

    // Gets user commands and executes for this single particle
    execCommand(particle);

    // Preventing memory leaks
    freeAll(particle);
    free(particle);
    return 0;
}