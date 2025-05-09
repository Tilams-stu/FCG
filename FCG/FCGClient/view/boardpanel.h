#ifndef BOARDPANEL_H
#define BOARDPANEL_H

#include <QWidget>
#include <QPolygon>
#include <QColor>
#include <QList>
#include <QMap>


class BoardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit BoardPanel(QWidget *parent = nullptr);

    void updateBoardState(const QMap<int , QList<int>>& tilesState);
    int getCellSize()const{return cellSize;}
    int cellSize = 20;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    class Tile{
    public:
        Tile(int id,const QPolygon& poly,const QColor& color);

        void draw(QPainter& painter ) const;
        int tileID() const{return m_id;}

        QList<int> getplanes()const{return m_planes;}
        void setPlanes(const QList<int>& planes){ m_planes = planes;}

    private:
        void drawCircle(QPainter& painter) const;
        void drawPlanes(QPainter& painter) const;
        QColor playerColor(int planeID) const;

        int m_id;
        QPolygon m_polygon;
        QColor m_baseColor;
        QList<int> m_planes;
    };

    void computeBoard();
    void addRectTile(int x,int y,int w,int h,const QColor& color);
    void addTriangle(const QVector<QPoint>& points,const QColor& color);

    QList<Tile*> tiles;
    int nextTileID = 1;
    QPoint center;
};

#endif // BOARDPANEL_H
