#include "Spline.h"

#include "ReparamUtils.h"

#include "MathLib.h"

constexpr int s_reparamSegmentNum = 25;
constexpr int closestPointNumIterations = 20;

inline glm::dvec3 BezierPos(const glm::dvec3& p0, const glm::dvec3& t0, const glm::dvec3& p1, const glm::dvec3& t1, double a)
{
    const double a2 = a * a, a3 = a2 * a;
    return ((2 * a3) - (3 * a2) + 1) * p0 + (a3 - (2 * a2) + a) * t0 + (a3 - a2) * t1 + ((-2 * a3) + (3 * a2)) * p1;
}

inline glm::dvec3 BezierDeriv(const glm::dvec3& p0, const glm::dvec3& t0, const glm::dvec3& p1, const glm::dvec3& t1, double a)
{
    const glm::dvec3 c = (6. * p0) + (3. * t0) + (3. * t1) - (6. * p1);
    const glm::dvec3 d = (-6. * p0) - (4. * t0) - (2. * t1) + (6. * p1);
    const glm::dvec3 e = t0;
    return c * a * a + d * a + e;
};

Spline::Spline()
{
    m_bezierPoints = {
        // NIGHT CITY TRACK (it has no elevation, no roll)
        { glm::dvec3(4665.0, 59665.0, 0.0), glm::dvec3(6440.9, 64848.0, 0.0), 0.0 },
        { glm::dvec3(28765.0, 116565.0, 0.0), glm::dvec3(88663.0, -12393.1, 0.0), 0.0 },
        { glm::dvec3(33480.0, 6885.0, 0.0), glm::dvec3(73631.6, -28296.2, 0.0), 0.0 },
        { glm::dvec3(62045.0, -25690.0, 0.0), glm::dvec3(77955.6, -27156.6, 0.0), 0.0 },
        { glm::dvec3(52280.0, -69975.0, 0.0), glm::dvec3(-95032.1, 4658.5, 0.0), 0.0 },
        { glm::dvec3(6260.0, -35315.0, 0.0), glm::dvec3(-7062.5, 59686.1, 0.0), 0.0 },
        { glm::dvec3(14725.0, 15065.0, 0.0), glm::dvec3(-10248.0, 67859.6, 0.0), 0.0 },
        { glm::dvec3(-24075.0, 30843.5, 0.0), glm::dvec3(-8498.8, 43823.5, 0.0), 0.0 },
    };

    double prevSegmentDist = 0;
    for (int segmentIndex = 0; segmentIndex < m_bezierPoints.size(); ++segmentIndex) {
        BezierPoint& b0 = m_bezierPoints[segmentIndex];
        BezierPoint& b1 = m_bezierPoints[(segmentIndex + 1) % m_bezierPoints.size()];

        auto calcLength = [&b0, &b1](double param) {
            glm::dvec3 C1 = ((b0.p - b1.p) * 2. + b0.t + b1.t) * 3.;
            glm::dvec3 C2 = (b1.p - b0.p) * 6. - b0.t * 4. - b1.t * 2.;
            glm::dvec3 C3 = b0.t;
            static const glm::dvec2 LegendreGaussCoefficients[5] = {
                { 0.0, 0.5688889 }, { -0.5384693, 0.47862867 }, { 0.5384693, 0.47862867 },
                { -0.90617985, 0.23692688 }, { 0.90617985, 0.23692688 }
            };

            double length = 0.0f;
            const double halfParam = param * 0.5f;
            for (int i = 0; i < 5; ++i) {
                auto& coeff = LegendreGaussCoefficients[i];
                const double Alpha = halfParam * (1.0f + coeff.x);
                const glm::dvec3 deriv = ((C1 * Alpha + C2) * Alpha + C3);
                length += glm::length(deriv) * coeff.y;
            }

            return length * halfParam;
        };

        for (int reparamIndex = 0; reparamIndex < s_reparamSegmentNum; ++reparamIndex) {
            double param = double(reparamIndex) / s_reparamSegmentNum;
            auto lenCurrent = calcLength(param);
            m_reparamTable.push_back({ segmentIndex + param, prevSegmentDist + lenCurrent });
        }

        prevSegmentDist += calcLength(1.0);
    }

    m_reparamTable.push_back({ (double)m_bezierPoints.size(), prevSegmentDist });
    m_splineLength = prevSegmentDist;

    /*for (int i = 0; i < m_reparamTable.size(); ++i) {
        auto& r = m_reparamTable[i];
        if (i % 6 == 0)
            printf("\n");
        printf("vec2(%.2f, %.0f), ", r.key, r.distance);
    }

    printf("\nm_reparamTable %d", m_reparamTable.size());

    exit(0);*/
}

double Spline::KeyToDistance(double key) const
{
    auto interp = GetInterpData<ReparamPoint, double, &ReparamPoint::key>(m_reparamTable, key);
    return interp.isValid() ? interp.GetValue<&ReparamPoint::distance>() : 0.0;
}

double Spline::DistanceToKey(double dist) const
{
    auto interp = GetInterpData<ReparamPoint, double, &ReparamPoint::distance>(m_reparamTable, dist);
    return interp.isValid() ? interp.GetValue<&ReparamPoint::key>() : 0.0;
}

Spline::BerierInterp Spline::GetInterpAtKey(double splineKey) const
{
    if (m_bezierPoints.size() == 0)
        return BerierInterp(nullptr, 0, 0, 0.0);

    splineKey = glm::clamp(splineKey, 0., (double)m_bezierPoints.size());
    int segmentIndex = (int)splineKey;
    if (segmentIndex == m_bezierPoints.size()) {
        return BerierInterp(m_bezierPoints.data(), 0, 0, 0.0);
    }

    return BerierInterp(m_bezierPoints.data(),
        segmentIndex, (segmentIndex + 1) % m_bezierPoints.size(),
        splineKey - (double)segmentIndex);
}

glm::dvec3 Spline::BerierInterp::getPos() const { return BezierPos(points[i0].p, points[i0].t, points[i1].p, points[i1].t, param); }
glm::dvec3 Spline::BerierInterp::getDeriv() const { return BezierDeriv(points[i0].p, points[i0].t, points[i1].p, points[i1].t, param); }
void Spline::BerierInterp::getFrame(glm::dvec3& forward, glm::dvec3& right, glm::dvec3& up) const
{
    const glm::dvec3 worldZ(0, 0, 1);
    const glm::dvec3 forw = glm::normalize(getDeriv());
    const glm::dvec3 baseX = glm::normalize(glm::cross(worldZ, forw));
    const glm::dvec3 baseY = glm::cross(forw, baseX);

    const double hermiteParam = glm::smoothstep(0., 1., glm::fract(param));
    const double roll = glm::mix(points[i0].roll, points[i1].roll, hermiteParam);
    const double c = cos(roll), s = sin(roll);
    const glm::dvec3 x = (c * baseX) - (s * baseY);
    const glm::dvec3 y = (c * baseY) + (s * baseX);

    forward = forw;
    right = x;
    up = y;
}

void ClosestPoint(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, const glm::dvec3& d,
    const glm::dvec3& WorldPos, double& outParam, double& outMinDistSquared);

double Spline::GetKeyClosestToPosition(const glm::dvec3& worldPos, int segmentIndexPrev) const
{
    double bestDistanceSq = INFINITY;
    double bestKey = 0.0;

    const int numSegmemts = (int)m_bezierPoints.size();

    auto findClosest = [&](int segmentIndex) {
        const BezierPoint& b0 = m_bezierPoints[segmentIndex];
        const BezierPoint& b1 = m_bezierPoints[(segmentIndex + 1) % m_bezierPoints.size()];

        double localDistSq, localParam;
        ClosestPoint(b0.p, b0.p + b0.t / 3., b1.p - b1.t / 3., b1.p, worldPos, localParam, localDistSq);

        if (localDistSq < bestDistanceSq) {
            bestDistanceSq = localDistSq, bestKey = segmentIndex + localParam;
        }
    };

    if (segmentIndexPrev == INT_MAX) {
        for (int attemptIndex = 0; attemptIndex < numSegmemts; ++attemptIndex)
            findClosest(correctModulo(segmentIndexPrev + attemptIndex, numSegmemts));

    } else {
        for (int attemptIndex = -1; attemptIndex <= 1; ++attemptIndex)
            findClosest(correctModulo(segmentIndexPrev + attemptIndex, numSegmemts));
    }

    return bestKey;
}

// The code below is based on
// Cubic bezier distance by gleboneloner
// https://www.shadertoy.com/view/7lsBW2
// Thank you for this magic!

int SolveQuartic(double a, double b, double c, double d, double e, glm::dvec4& roots)
{
    b /= a, c /= a, d /= a, e /= a;

    double bb = b * b;
    double p = (8.0 * c - 3.0 * bb) / 8.0;
    double q = (8.0 * d - 4.0 * c * b + bb * b) / 8.0;
    double r = (256.0 * e - 64.0 * d * b + 16.0 * c * bb - 3.0 * bb * bb) / 256.0;
    int n = 0;

    double ra = 2.0 * p;
    double rb = p * p - 4.0 * r;
    double rc = -q * q;

    double ru = ra / 3.0;
    double rp = rb - ra * ru;
    double rq = rc - (rb - 2.0 * ra * ra / 9.0) * ru;

    double lambda_;
    double rh = 0.25 * rq * rq + rp * rp * rp / 27.0;
    if (rh > 0.0) {
        rh = sqrt(rh);
        double ro = -0.5 * rq;
        lambda_ = cbrt(ro - rh) + cbrt(ro + rh) - ru;
    }

    else {
        double rm = sqrt(-rp / 3.0);
        lambda_ = -2.0 * rm * sin(asin(1.5 * rq / (rp * rm)) / 3.0) - ru;
    }

    for (int i = 0; i < 2; i++) {
        double a_2 = ra + lambda_;
        double a_1 = rb + lambda_ * a_2;
        double b_2 = a_2 + lambda_;

        double f = rc + lambda_ * a_1;
        double f1 = a_1 + lambda_ * b_2;

        lambda_ -= f / f1;
    }

    if (lambda_ < 0.0)
        return n;
    double t = sqrt(lambda_);
    double alpha = 2.0 * q / t, beta = lambda_ + ra;

    double u = 0.25 * b;
    t *= 0.5;

    double z = -alpha - beta;
    if (z > 0.0) {
        z = sqrt(z) * 0.5;
        double h = +t - u;
        roots[0] = (h + z);
        roots[1] = (h - z);
        n += 2;
    }

    double w = +alpha - beta;
    if (w > 0.0) {
        w = sqrt(w) * 0.5;
        double h = -t - u;
        roots[2] = (h + w);
        roots[3] = (h - w);
        if (n == 0) {
            roots[0] = roots[2];
            roots[1] = roots[3];
        }
        n += 2;
    }

    return n;
}

void ClosestPoint(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, const glm::dvec3& d,
    const glm::dvec3& WorldPos, double& outParam, double& outMinDistSquared)
{

    double s1 = -1.0;
    glm::dvec3 S1 = 3.0 * (b - c) - a + d;
    double s2 = +1.0;
    glm::dvec3 S2 = 3.0 * (c + a - b - b);
    double H1 = -1.0;
    glm::dvec3 S3 = 3.0 * (b - a);
    double H2 = +1.0;
    glm::dvec3 S4 = a - WorldPos;

    double U1 = 3.0 * glm::dot(S1, S1);
    double U2 = 5.0 * glm::dot(S1, S2);
    double U3 = 4.0 * glm::dot(S1, S3) + 2.0 * glm::dot(S2, S2);
    double U4 = 3.0 * glm::dot(S1, S4) + 3.0 * glm::dot(S2, S3);
    double U5 = 2.0 * glm::dot(S2, S4) + 1.0 * glm::dot(S3, S3);
    double U6 = 1.0 * glm::dot(S3, S4);

    for (int i = 0; i < closestPointNumIterations; ++i) {
        double s3 = (s1 + s2) / 2.0;
        double k = s3 / (1.0 - abs(s3));
        double H3 = k * (k * (k * (k * (U1 * k + U2) + U3) + U4) + U5) + U6;
        (H1 * H3 <= 0.0) ? (s2 = s3, H2 = H3) : (s1 = s3, H1 = H3);
    }

    double params[5], distSquared[5];
    params[0] = (s1 * H2 - s2 * H1) / (H2 - H1);
    params[0] /= 1.0 - abs(params[0]);

    double B1 = U1;
    double B2 = U2 + params[0] * B1;
    double B3 = U3 + params[0] * B2;
    double B4 = U4 + params[0] * B3;
    double B5 = U5 + params[0] * B4;

    glm::dvec4 roots;
    SolveQuartic(B1, B2, B3, B4, B5, roots);

    params[1] = roots.x, params[2] = roots.y, params[3] = roots.z, params[4] = roots.w;

    for (int i = 0; i < 5; ++i) {
        params[i] = glm::clamp(params[i], 0., 1.);
        glm::dvec3 tmp_ = params[i] * (params[i] * (S1 * params[i] + S2) + S3) + S4;
        distSquared[i] = glm::dot(tmp_, tmp_);
    }

    double minDistSquared = distSquared[0];
    double minParam = params[0];

    for (int i = 1; i < 5; ++i) {
        if (distSquared[i] < minDistSquared) {
            minDistSquared = distSquared[i];
            minParam = params[i];
        }
    }

    outMinDistSquared = minDistSquared;
    outParam = minParam;
}
