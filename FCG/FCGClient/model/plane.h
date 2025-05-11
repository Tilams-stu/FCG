#ifndef PLANE_H
#define PLANE_H

#include <QString>
#include <QColor>

class Plane
{
public:
    enum Status {
        Normal,
        TakingOff,
        Collided
    };

    Plane(int id, const QColor& color, Status status = Normal);

    int id() const;
    QColor color() const;
    QString statusString() const;
    Status status() const;

    void setStatus(Status newStatus);

private:
    int m_id;
    QColor m_color;
    Status m_status;
};

#endif // PLANE_H
