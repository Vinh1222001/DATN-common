#pragma once
#ifndef PID_HPP
#define PID_HPP

#include <Arduino.h>

template <typename T>
class PID
{
private:
    T Kp, Ki, Kd;           // Hệ số PID
    T prevError;            // Sai số trước đó
    T integral;             // Thành phần tích phân
    T outputMin, outputMax; // Giới hạn đầu ra
    unsigned long lastTime; // Thời gian trước đó

public:
    PID(T Kp, T Ki, T Kd, T outputMin = -255, T outputMax = 255);

    template <typename U>
    T compute(U setpoint, U measured);

    void setTunings(T Kp, T Ki, T Kd);
    void setOutputLimits(T min, T max);
    void reset();
};

#endif
