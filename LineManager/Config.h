#pragma once
#include <string>
#include <iostream>

/** Line Manager **/
const bool SKIP_LIMIT_CHECK = false;
const double S_PER_TIMESLOT = 0.001 * 100;

/** Grid **/
const double VOLTAGE = 230;
const double MIN_VOLTAGE = VOLTAGE * 0.9;
const double MAX_VOLTAGE = VOLTAGE * 1.1;

/** Grid State Solvers **/
const int THREADLIMIT = 4;
const double TERMINATION_NORM = 1e-10;
const int MAX_ITERATIONS = 5000;

/** Spice **/
const std::string SPICE_STEP = "50";
const std::string SPICE_STOP = "200";
const std::string SPICE_START = "0";
const std::string SPICE_RAMP_FACTOR = "100000";

/** Visualization **/
const bool SHOW_PLOTS = true;
const bool SAVE_PLOTS = true;
const bool PLOT_TIMESLOT = false;
const std::string PLOT_FOLDER = "../plots";

