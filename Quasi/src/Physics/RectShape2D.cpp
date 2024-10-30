#include "RectShape2D.h"

#include "SeperatingAxisSolver.h"

namespace Quasi::Physics2D {
    float RectShape::Inertia() const {
        return (hx * hx + hy * hy) / 3.0f;
    }

    fVector2 RectShape::Corner(bool px, bool py) const {
        return { px ? hx : -hx, py ? hy : -hy };
    }

    fRect2D RectShape::ComputeBoundingBox() const {
        return { { -hx, -hy }, { hx, hy } };
    }

    fVector2 RectShape::NearestPointTo(const fVector2& point) const {
        return Corner(point.x > 0, point.y > 0);
    }

    fVector2 RectShape::FurthestAlong(const fVector2& normal) const {
        const float dx = hx * normal.x, dy = hy * normal.y;
        return Corner(dx > 0, dy > 0);
    }

    fLine2D RectShape::BestEdgeFor(const fVector2& normal) const {
        const float dx = hx * normal.x, dy = hy * normal.y;
        const bool xPerpendicular = std::abs(dx) < std::abs(dy);
        return { Corner(dx > 0, dy > 0), Corner(dx > 0 ^ xPerpendicular, dy > 0 ^ !xPerpendicular) };
    }

    fRange RectShape::ProjectOntoAxis(const fVector2& axis) const {
        const float mean = hx * axis.x, off = hy * axis.y,
                    maxD = std::max(std::abs(mean + off), std::abs(mean - off));
        return { -maxD, maxD };
    }

    fRange RectShape::ProjectOntoOwnAxis(u32 axisID, const fVector2& axis) const {
        const float half = axisID == 0 ? hx : hy;
        return { -half, half };
    }

    bool RectShape::AddSeperatingAxes(SeperatingAxisSolver& sat) const {
        bool success = false;
        success |= sat.CheckAxis({ 1, 0 });
        success |= sat.CheckAxis({ 0, 1 });
        return success;
    }
} // Quasi