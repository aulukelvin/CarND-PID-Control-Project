#include "PID.h"

/*
 * TODO: Complete the PID class.
 */

PID::PID() {
}

PID::~PID() {
}

void PID::Init(double Kp, double Ki, double Kd) {
    p_error = 0;
    i_error = 0;
    d_error = 0;

    this->Kd = Kd;
    this->Ki = Ki;
    this->Kp = Kp;

    prev_cte = 0;
}

void PID::UpdateError(double cte) {

    p_error = cte;
    d_error = cte - prev_cte;
    i_error *= 0.99;
    i_error += cte;

    prev_cte = cte;
}

double PID::TotalError() {
    return Kp * p_error + Ki * i_error + Kd * d_error;
}

