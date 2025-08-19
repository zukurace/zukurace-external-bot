#pragma once

#include <glm/glm.hpp>

#include <vector>

// Basic Spline things
// Assume, spline is closed and have coninious derivative (have no sharp edges).

struct BezierPoint {
    glm::dvec3 p, t;
    double roll; // radians
};

// used for KeyToDistance and DistanceToKey
struct ReparamPoint {
    double key, distance;
};


class Spline {
    std::vector<BezierPoint> m_bezierPoints;
    std::vector<ReparamPoint> m_reparamTable;
    double m_splineLength = 0.0;

public:
    struct BerierInterp {
        const BezierPoint* points;
        const int i0, i1;
        const double param;

        BerierInterp(const BezierPoint* _points, int _i0, int _i1, double _param)
            : points(_points)
            , i0(_i0)
            , i1(_i1)
            , param(_param)
        {
        }

        glm::dvec3 getPos() const;
        glm::dvec3 getDeriv() const; // unnormalized tangent
        void getFrame(glm::dvec3& forward, glm::dvec3& right, glm::dvec3& up) const;
        bool isValid() const { return !!points; }
    };

    Spline();

    double KeyToDistance(double key) const;
    double DistanceToKey(double distance) const;
    double GetLength() const { return m_splineLength; }

    BerierInterp GetInterpAtKey(double splineKey) const;

    // if segmentIndexPrev is INT_MAX -> search on entire spline
    // otherwise search among 3 segments (i = segmentIndexPrev) -> (i - 1,  i,  i + 1)
    double GetKeyClosestToPosition(const glm::dvec3& worldPos, int segmentIndexPrev = INT_MAX) const;
};
