#ifndef LINE_H
#define LINE_H
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>
#include <QPoint>
#include <QWidget>
#include <QMenu>
#include <QAction>
class line : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    line();
    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void merge();

    float x[2];
    float y[2];
    int segments = 0;
    bool heater_line;
    line *previous;
    line *next;
signals:
    void delete_this_line(line *delete_line);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // LINE_H
