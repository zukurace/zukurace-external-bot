#pragma once

#include "RaceCommon.h"
#include "Spline.h"

#include <vector>

// RaceBotAI, line follower

struct TelemetryPoint {
    float splineKey, speed, offsetY;
};

class RaceBotAI {
    Spline m_spline;
    std::vector<TelemetryPoint> m_telemetry;
    float m_startFactor = 1.f;

public:
    void reset() { m_startFactor = 1.f; }

public:
    RaceBotAI();
    CarInput Calculate(const CarData& carData);
};
