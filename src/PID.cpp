#include "PID.hpp"

// Constructor
template <typename T>
PID<T>::PID(T Kp, T Ki, T Kd, T outputMin, T outputMax)
    : Kp(Kp), Ki(Ki), Kd(Kd), prevError(0), integral(0), outputMin(outputMin), outputMax(outputMax)
{
    lastTime = millis();
}

// Compute function
template <typename T>
template <typename U>
T PID<T>::compute(U setpoint, U measured)
{
    unsigned long now = millis();
    T dt = (now - lastTime) / 1000.0; // Chuyển millis -> giây
    lastTime = now;

    T error = static_cast<T>(setpoint - measured);
    integral += error * dt;
    T derivative = (error - prevError) / dt;
    prevError = error;

    T output = Kp * error + Ki * integral + Kd * derivative;

    // Giới hạn đầu ra
    if (output > outputMax)
        output = outputMax;
    else if (output < outputMin)
        output = outputMin;

    return output;
}

// Set tunings
template <typename T>
void PID<T>::setTunings(T Kp, T Ki, T Kd)
{
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

// Set output limits
template <typename T>
void PID<T>::setOutputLimits(T min, T max)
{
    outputMin = min;
    outputMax = max;
}

// Reset function
template <typename T>
void PID<T>::reset()
{
    prevError = 0;
    integral = 0;
    lastTime = millis();
}
