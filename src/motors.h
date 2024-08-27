#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "main.h"

void setup_motors();
void set_global_thrust();
void set_correction();
void get_total_thrust();
void run_motors();
#endif
