#include "boardpanel.h"
#include <QPainter>
#include <QResizeEvent>

BoardPanel::BoardPanel(QWidget *parent)
    : QWidget(parent)
{
    //setStyleSheet("background-color: white;");
    qDebug() << "Creating boardPanel...";
    computeBoard();
}

void BoardPanel::updateBoardState(const QMap<int, QList<int> > &tilesState)
{
    for(Tile* tile : tiles){
        if(tilesState.contains(tile->tileID())){
            tile->setPlanes(tilesState[tile->tileID()]);
        }
    }
    update();
}

void BoardPanel::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    QPen pen;
    pen.setWidth(1);
    painter.setPen(pen);

    painter.setRenderHint(QPainter::Antialiasing);

    for(Tile* tile : tiles){
        tile->draw(painter);
    }
}

void BoardPanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    computeBoard();
}

void BoardPanel::computeBoard()
{
    QMap<int ,QList<int>> currentState;
    for(Tile* tile : qAsConst(tiles)){
        currentState.insert(tile->tileID(),tile->getplanes());
    }

    qDeleteAll(tiles);
    tiles.clear();
    nextTileID = 1;

    const int w = width();
    const int h = height();
    center = QPoint(w/2,h/2);
    const int CELL_SIZE = cellSize;

    //黄色起点
    addRectTile(center.x() - 17*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::yellow);
    addRectTile(center.x() - 13*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::yellow);
    addRectTile(center.x() - 17*CELL_SIZE, center.y() - 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::yellow);
    addRectTile(center.x() - 13*CELL_SIZE, center.y() - 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::yellow);

    //蓝色起点
    addRectTile(center.x() + 9*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::blue);
    addRectTile(center.x() + 13*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::blue);
    addRectTile(center.x() + 9*CELL_SIZE, center.y() - 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::blue);
    addRectTile(center.x() + 13*CELL_SIZE, center.y() - 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::blue);

    //绿色起点
    addRectTile(center.x() - 17*CELL_SIZE, center.y() + 9*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::green);
    addRectTile(center.x() - 13*CELL_SIZE, center.y() + 9*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::green);
    addRectTile(center.x() - 17*CELL_SIZE, center.y() + 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::green);
    addRectTile(center.x() - 13*CELL_SIZE, center.y() + 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::green);

    //红色起点
    addRectTile(center.x() + 9*CELL_SIZE, center.y() + 9*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::red);
    addRectTile(center.x() + 13*CELL_SIZE, center.y() + 9*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::red);
    addRectTile(center.x() + 9*CELL_SIZE, center.y() + 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::red);
    addRectTile(center.x() + 13*CELL_SIZE, center.y() + 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*4, Qt::red);

    //起点三角形
    QVector<QPoint> trianglePoints17;
    trianglePoints17 << QPoint(center.x() - 17*CELL_SIZE, center.y() - 9*CELL_SIZE)
                   << QPoint(center.x() - 13*CELL_SIZE, center.y() - 9*CELL_SIZE)
                   << QPoint(center.x() - 17*CELL_SIZE, center.y() - 5*CELL_SIZE);
    addTriangle(trianglePoints17, Qt::yellow);

    QVector<QPoint> trianglePoints18;
    trianglePoints18 << QPoint(center.x() +5*CELL_SIZE, center.y() - 17*CELL_SIZE)
                    << QPoint(center.x() +9*CELL_SIZE, center.y() - 17*CELL_SIZE)
                    << QPoint(center.x() +9*CELL_SIZE, center.y() - 13*CELL_SIZE);
    addTriangle(trianglePoints18, Qt::blue);

    QVector<QPoint> trianglePoints19;
    trianglePoints19 << QPoint(center.x() +13*CELL_SIZE, center.y() +9*CELL_SIZE)
                    << QPoint(center.x() +17*CELL_SIZE, center.y() +5*CELL_SIZE)
                    << QPoint(center.x() +17*CELL_SIZE, center.y() +9*CELL_SIZE);
    addTriangle(trianglePoints19, Qt::red);

    QVector<QPoint> trianglePoints20;
    trianglePoints20 << QPoint(center.x() -9*CELL_SIZE, center.y() +13*CELL_SIZE)
                    << QPoint(center.x() -9*CELL_SIZE, center.y() +17*CELL_SIZE)
                    << QPoint(center.x() -5*CELL_SIZE, center.y() +17*CELL_SIZE);
    addTriangle(trianglePoints20, Qt::green);

    //外圈部分21
    QVector<QPoint> trianglePoints21;
    trianglePoints21 << QPoint(center.x() -17*CELL_SIZE, center.y() -5*CELL_SIZE)
                     << QPoint(center.x() -13*CELL_SIZE, center.y() -5*CELL_SIZE)
                     << QPoint(center.x() -13*CELL_SIZE, center.y() -9*CELL_SIZE);
    addTriangle(trianglePoints21, Qt::green);
    addRectTile(center.x() - 13*CELL_SIZE, center.y() - 9*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::yellow);
    addRectTile(center.x() - 11*CELL_SIZE, center.y() - 9*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::blue);

    //24
    QVector<QPoint> trianglePoints24;
    trianglePoints24 << QPoint(center.x() -9*CELL_SIZE, center.y() -9*CELL_SIZE)
                     << QPoint(center.x() -9*CELL_SIZE, center.y() -5*CELL_SIZE)
                     << QPoint(center.x() -5*CELL_SIZE, center.y() -5*CELL_SIZE);
    addTriangle(trianglePoints24, Qt::red);
    //25
    QVector<QPoint> trianglePoints25;
    trianglePoints25 << QPoint(center.x() -9*CELL_SIZE, center.y() -9*CELL_SIZE)
                     << QPoint(center.x() -5*CELL_SIZE, center.y() -9*CELL_SIZE)
                     << QPoint(center.x() -5*CELL_SIZE, center.y() -5*CELL_SIZE);
    addTriangle(trianglePoints25, Qt::green);
    addRectTile(center.x() - 9*CELL_SIZE, center.y() - 11*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::yellow);
    addRectTile(center.x() - 9*CELL_SIZE, center.y() - 13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::blue);

    //28
    QVector<QPoint> trianglePoints28;
    trianglePoints28 << QPoint(center.x() -5*CELL_SIZE, center.y() -17*CELL_SIZE)
                     << QPoint(center.x() -5*CELL_SIZE, center.y() -13*CELL_SIZE)
                     << QPoint(center.x() -9*CELL_SIZE, center.y() -13*CELL_SIZE);
    addTriangle(trianglePoints28, Qt::red);

    addRectTile(center.x() - 5*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::green);
    addRectTile(center.x() - 3*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::yellow);
    addRectTile(center.x() - 1*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::blue);
    addRectTile(center.x() + 1*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::red);
    addRectTile(center.x() + 3*CELL_SIZE, center.y() - 17*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::green);

    //34
    QVector<QPoint> trianglePoints34;
    trianglePoints34 << QPoint(center.x() +5*CELL_SIZE, center.y() -17*CELL_SIZE)
                     << QPoint(center.x() +5*CELL_SIZE, center.y() -13*CELL_SIZE)
                     << QPoint(center.x() +9*CELL_SIZE, center.y() -13*CELL_SIZE);
    addTriangle(trianglePoints34, Qt::yellow);
    addRectTile(center.x() +5*CELL_SIZE, center.y() -13*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::blue);
    addRectTile(center.x() +5*CELL_SIZE, center.y() -11*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::red);

    //37
    QVector<QPoint> trianglePoints37;
    trianglePoints37 << QPoint(center.x() +9*CELL_SIZE, center.y() -9*CELL_SIZE)
                     << QPoint(center.x() +5*CELL_SIZE, center.y() -9*CELL_SIZE)
                     << QPoint(center.x() +5*CELL_SIZE, center.y() -5*CELL_SIZE);
    addTriangle(trianglePoints37, Qt::green);
    //38
    QVector<QPoint> trianglePoints38;
    trianglePoints38 << QPoint(center.x() +9*CELL_SIZE, center.y() -9*CELL_SIZE)
                     << QPoint(center.x() +9*CELL_SIZE, center.y() -5*CELL_SIZE)
                     << QPoint(center.x() +5*CELL_SIZE, center.y() -5*CELL_SIZE);
    addTriangle(trianglePoints38, Qt::yellow);
    addRectTile(center.x() +9*CELL_SIZE, center.y() -9*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::blue);
    addRectTile(center.x() +11*CELL_SIZE, center.y() -9*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::red);

    //41
    QVector<QPoint> trianglePoints41;
    trianglePoints41 << QPoint(center.x() +17*CELL_SIZE, center.y() -5*CELL_SIZE)
                     << QPoint(center.x() +13*CELL_SIZE, center.y() -5*CELL_SIZE)
                     << QPoint(center.x() +13*CELL_SIZE, center.y() -9*CELL_SIZE);
    addTriangle(trianglePoints41, Qt::green);

    addRectTile(center.x() +13*CELL_SIZE, center.y() - 5*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::yellow);
    addRectTile(center.x() +13*CELL_SIZE, center.y() - 3*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::blue);
    addRectTile(center.x() +13*CELL_SIZE, center.y() - 1*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::red);
    addRectTile(center.x() +13*CELL_SIZE, center.y() + 1*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::green);
    addRectTile(center.x() +13*CELL_SIZE, center.y() + 3*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::yellow);

    //47
    QVector<QPoint> trianglePoints47;
    trianglePoints47 << QPoint(center.x() +17*CELL_SIZE, center.y() +5*CELL_SIZE)
                     << QPoint(center.x() +13*CELL_SIZE, center.y() +5*CELL_SIZE)
                     << QPoint(center.x() +13*CELL_SIZE, center.y() +9*CELL_SIZE);
    addTriangle(trianglePoints47, Qt::blue);
    addRectTile(center.x() +11*CELL_SIZE, center.y() +5*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::red);
    addRectTile(center.x() +9*CELL_SIZE, center.y() +5*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::green);

    //50
    QVector<QPoint> trianglePoints50;
    trianglePoints50 << QPoint(center.x() +9*CELL_SIZE, center.y() +9*CELL_SIZE)
                     << QPoint(center.x() +9*CELL_SIZE, center.y() +5*CELL_SIZE)
                     << QPoint(center.x() +5*CELL_SIZE, center.y() +5*CELL_SIZE);
    addTriangle(trianglePoints50, Qt::yellow);
    //51
    QVector<QPoint> trianglePoints51;
    trianglePoints51 << QPoint(center.x() +9*CELL_SIZE, center.y() +9*CELL_SIZE)
                     << QPoint(center.x() +5*CELL_SIZE, center.y() +9*CELL_SIZE)
                     << QPoint(center.x() +5*CELL_SIZE, center.y() +5*CELL_SIZE);
    addTriangle(trianglePoints51, Qt::blue);
    addRectTile(center.x() +5*CELL_SIZE, center.y() +9*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::red);
    addRectTile(center.x() +5*CELL_SIZE, center.y() +11*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::green);

    //54
    QVector<QPoint> trianglePoints54;
    trianglePoints54 << QPoint(center.x() +5*CELL_SIZE, center.y() +17*CELL_SIZE)
                     << QPoint(center.x() +5*CELL_SIZE, center.y() +13*CELL_SIZE)
                     << QPoint(center.x() +9*CELL_SIZE, center.y() +13*CELL_SIZE);
    addTriangle(trianglePoints54, Qt::yellow);

    addRectTile(center.x() +3*CELL_SIZE, center.y() +13*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::blue);
    addRectTile(center.x() +1*CELL_SIZE, center.y() +13*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::red);
    addRectTile(center.x() -1*CELL_SIZE, center.y() +13*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::green);
    addRectTile(center.x() -3*CELL_SIZE, center.y() +13*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::yellow);
    addRectTile(center.x() -5*CELL_SIZE, center.y() +13*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::blue);

    //60
    QVector<QPoint> trianglePoints60;
    trianglePoints60 << QPoint(center.x() -5*CELL_SIZE, center.y() +17*CELL_SIZE)
                     << QPoint(center.x() -5*CELL_SIZE, center.y() +13*CELL_SIZE)
                     << QPoint(center.x() -9*CELL_SIZE, center.y() +13*CELL_SIZE);
    addTriangle(trianglePoints60, Qt::red);
    addRectTile(center.x() -9*CELL_SIZE, center.y() +11*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::green);
    addRectTile(center.x() -9*CELL_SIZE, center.y() +9*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::yellow);

    //63
    QVector<QPoint> trianglePoints63;
    trianglePoints63 << QPoint(center.x() -9*CELL_SIZE, center.y() +9*CELL_SIZE)
                     << QPoint(center.x() -5*CELL_SIZE, center.y() +9*CELL_SIZE)
                     << QPoint(center.x() -5*CELL_SIZE, center.y() +5*CELL_SIZE);
    addTriangle(trianglePoints63, Qt::blue);
    //64
    QVector<QPoint> trianglePoints64;
    trianglePoints64 << QPoint(center.x() -9*CELL_SIZE, center.y() +9*CELL_SIZE)
                     << QPoint(center.x() -9*CELL_SIZE, center.y() +5*CELL_SIZE)
                     << QPoint(center.x() -5*CELL_SIZE, center.y() +5*CELL_SIZE);
    addTriangle(trianglePoints64, Qt::red);
    addRectTile(center.x() -11*CELL_SIZE, center.y() +5*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::green);
    addRectTile(center.x() -13*CELL_SIZE, center.y() +5*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*4, Qt::yellow);

    //67
    QVector<QPoint> trianglePoints67;
    trianglePoints67 << QPoint(center.x() -17*CELL_SIZE, center.y() +5*CELL_SIZE)
                     << QPoint(center.x() -13*CELL_SIZE, center.y() +5*CELL_SIZE)
                     << QPoint(center.x() -13*CELL_SIZE, center.y() +9*CELL_SIZE);
    addTriangle(trianglePoints67, Qt::blue);

    addRectTile(center.x() -17*CELL_SIZE, center.y() +3*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::red);
    addRectTile(center.x() -17*CELL_SIZE, center.y() +1*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::green);
    addRectTile(center.x() -17*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::yellow);
    addRectTile(center.x() -17*CELL_SIZE, center.y() -3*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::blue);
    addRectTile(center.x() -17*CELL_SIZE, center.y() -5*CELL_SIZE, CELL_SIZE*4, CELL_SIZE*2, Qt::red);

    //黄色路径73
    addRectTile(center.x() -13*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::yellow);
    addRectTile(center.x() -11*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::yellow);
    addRectTile(center.x() -9*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::yellow);
    addRectTile(center.x() -7*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::yellow);
    addRectTile(center.x() -5*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::yellow);

    //78
    QVector<QPoint> trianglePoints78;
    trianglePoints78 << QPoint(center.x() -3*CELL_SIZE, center.y() -3*CELL_SIZE)
                     << QPoint(center.x() -3*CELL_SIZE, center.y() +3*CELL_SIZE)
                     << QPoint(center.x() +0*CELL_SIZE, center.y() -0*CELL_SIZE);
    addTriangle(trianglePoints78, Qt::yellow);

    //蓝色路径79
    addRectTile(center.x() -1*CELL_SIZE, center.y() -13*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::blue);
    addRectTile(center.x() -1*CELL_SIZE, center.y() -11*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::blue);
    addRectTile(center.x() -1*CELL_SIZE, center.y() -9*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::blue);
    addRectTile(center.x() -1*CELL_SIZE, center.y() -7*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::blue);
    addRectTile(center.x() -1*CELL_SIZE, center.y() -5*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::blue);

    //84
    QVector<QPoint> trianglePoints84;
    trianglePoints84 << QPoint(center.x() -0*CELL_SIZE, center.y() -0*CELL_SIZE)
                     << QPoint(center.x() -3*CELL_SIZE, center.y() -3*CELL_SIZE)
                     << QPoint(center.x() +3*CELL_SIZE, center.y() -3*CELL_SIZE);
    addTriangle(trianglePoints84, Qt::blue);

    //红色路径85
    addRectTile(center.x() +11*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::red);
    addRectTile(center.x() +9*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::red);
    addRectTile(center.x() +7*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::red);
    addRectTile(center.x() +5*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::red);
    addRectTile(center.x() +3*CELL_SIZE, center.y() -1*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::red);

    //90
    QVector<QPoint> trianglePoints90;
    trianglePoints90 << QPoint(center.x() +3*CELL_SIZE, center.y() -3*CELL_SIZE)
                     << QPoint(center.x() +3*CELL_SIZE, center.y() +3*CELL_SIZE)
                     << QPoint(center.x() +0*CELL_SIZE, center.y() -0*CELL_SIZE);
    addTriangle(trianglePoints90, Qt::red);

    //绿色路径91
    addRectTile(center.x() -1*CELL_SIZE, center.y() +11*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::green);
    addRectTile(center.x() -1*CELL_SIZE, center.y() +9*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::green);
    addRectTile(center.x() -1*CELL_SIZE, center.y() +7*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::green);
    addRectTile(center.x() -1*CELL_SIZE, center.y() +5*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::green);
    addRectTile(center.x() -1*CELL_SIZE, center.y() +3*CELL_SIZE, CELL_SIZE*2, CELL_SIZE*2, Qt::green);

    //96
    QVector<QPoint> trianglePoints96;
    trianglePoints96 << QPoint(center.x() +3*CELL_SIZE, center.y() +3*CELL_SIZE)
                     << QPoint(center.x() -3*CELL_SIZE, center.y() +3*CELL_SIZE)
                     << QPoint(center.x() +0*CELL_SIZE, center.y() -0*CELL_SIZE);
    addTriangle(trianglePoints96, Qt::green);

    for (Tile* tile : tiles) {
        if (currentState.contains(tile->tileID())) {
            tile->setPlanes(currentState[tile->tileID()]);
        }
    }

    update();
}

void BoardPanel::addRectTile(int x, int y, int w,int h, const QColor &color)
{
    QPolygon poly;
    poly << QPoint(x, y)
         << QPoint(x+w, y)
         << QPoint(x+w, y+h)
         << QPoint(x, y+h);
    tiles.append(new Tile(nextTileID++, poly, color));
}

void BoardPanel::addTriangle(const QVector<QPoint> &points, const QColor &color)
{
    QPolygon poly(points);
    tiles.append(new Tile(nextTileID++, poly, color));
}

BoardPanel::Tile::Tile(int id, const QPolygon &poly, const QColor &color)
    :m_id(id), m_polygon(poly),m_baseColor(color){}

void BoardPanel::Tile::draw(QPainter &painter) const
{
    QPen pen;
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(m_baseColor);
    painter.drawPolygon(m_polygon);

    painter.setPen(Qt::black);
    painter.drawPolygon(m_polygon);

    drawCircle(painter);
    drawPlanes(painter);
}

void BoardPanel::Tile::drawCircle(QPainter &painter) const
{
    double avgX = 0, avgY = 0;
    const int n = m_polygon.size();

    for (const QPoint& p : m_polygon) {
        avgX += p.x();
        avgY += p.y();
    }

    if (n > 0) {
        avgX /= n;
        avgY /= n;
    }

    //QPointF center = m_polygon.boundingRect().center();
    int r = 20/1.15;

    QPen pen;
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(QColor(240,240,240));
    painter.drawEllipse(QPointF(avgX,avgY),r,r);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPointF(avgX,avgY),r,r);
}

void BoardPanel::Tile::drawPlanes(QPainter &painter) const
{
    //QRectF rect = m_polygon.boundingRect();
    //QPointF center = rect.center();
    double avgX = 0, avgY = 0;
    const int n = m_polygon.size();

    for (const QPoint& p : m_polygon) {
        avgX += p.x();
        avgY += p.y();
    }

    if (n > 0) {
        avgX /= n;
        avgY /= n;
    }

    const int r = 20 / 2;
    QVector<QPointF> offsets;

    switch (m_planes.size()){
    case 2:
        offsets << QPointF(-r,0) << QPointF(r,0);
        break;
    case 3: case 4:
        offsets << QPointF(-r,-r) << QPointF(r,-r)
                << QPointF(-r,r) << QPointF(r,r);
        break;
    default:
        for(int i=0;i< m_planes.size();i++){
            offsets << QPointF(0,0);
        }
    }
    QPen pen;
    pen.setWidth(1);
    painter.setPen(pen);
    for(int i=0;i<m_planes.size();i++){
        QPointF pos = QPointF(avgX,avgY) + offsets[i];
        QColor color = playerColor(m_planes[i]);

        painter.setBrush(color);
        painter.drawEllipse(pos, r, r);

        painter.setPen(Qt::black);
        painter.drawText(QRectF(pos.x()-r,pos.y()-r,2*r,2*r),
                         Qt::AlignCenter,
                         QString::number((m_planes[i]-1)%4 + 1));
    }
}

QColor BoardPanel::Tile::playerColor(int planeID) const
{
    if (planeID >= 1 && planeID <= 4) return Qt::yellow;
    if (planeID >= 5 && planeID <= 8) return Qt::blue;
    if (planeID >= 9 && planeID <= 12) return Qt::green;
    if (planeID >= 13 && planeID <= 16) return Qt::red;
    return Qt::gray;
}


