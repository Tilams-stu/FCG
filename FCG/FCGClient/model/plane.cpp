#include "plane.h"

Plane::Plane(int id, const QColor& color, Status status)
    : m_id(id), m_color(color), m_status(status) {}

int Plane::id() const { return m_id; }
QColor Plane::color() const { return m_color; }

Plane::Status Plane::status() const
{
    return m_status;
}

QString Plane::statusString() const
{
    switch (m_status) {
    case Normal:    return "Normal";
    case TakingOff: return "TakingOff";
    case Collided:  return "Collided";
    default:        return "Unknown";
    }
}

void Plane::setStatus(Plane::Status newStatus)
{
    m_status = newStatus;
}
