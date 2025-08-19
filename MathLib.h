#pragma once

inline int correctModulo(int i, int n) { return (i % n + n) % n; }

template <typename T>
inline T normalizeRange(T inMin, T inMax, T v)
{
    const T d = inMax - inMin;
    return (d == (T)0) ? (v >= inMax ? (T)1 : (T)0) : (v - inMin) / d;
}

template <typename T>
inline T normalizeRangeClamped(T inMin, T inMax, T v)
{
    T r = normalizeRange(inMin, inMax, v);
    return r < 0 ? 0 : r > 1 ? 1
                             : r;
}

template <typename T>
inline T remapRangeClamped(T inMin, T inMax, T outMin, T outMax, T v)
{
    const T r = normalizeRangeClamped(inMin, inMax, v);
    return outMin * (1 - r) + outMax * r;
}