#pragma once

/* Floor Division:
/ -
T: an integer type.
divisor != 0.
/ -
Divides, rounding toward negative infinity, e.g. floorDivision(-1, 16) = -1. */
template <typename T>
inline T floorDivision(T value, T divisor) {
    T quotient = value / divisor;
    T remainder = value % divisor;

    if (remainder != 0 && ((remainder < 0) != (divisor < 0))) --quotient;

    return quotient;
}

/* Floor Modulus:
/ -
T: an integer type.
divisor > 0.
/ -
Returns value mod divisor ∈ [0, divisor), e.g. floorModulus(-1, 16) = 15. */
template <typename T>
inline T floorModulus(T value, T divisor) {
    T remainder = value % divisor;
    
    if (remainder < 0) remainder += divisor;

    return remainder;
}

/* Smooth Step:
/ -
value: any float, clamped to [0.0f, 1.0f].
/ -
Returns an S-curve ∈ [0.0f, 1.0f] with zero slope at both ends. */
inline float smoothStep(float value) {
    if (value < 0.0f) value = 0.0f;
    else if (value > 1.0f) value = 1.0f;
    return value * value * (3.0f - 2.0f * value);
}
