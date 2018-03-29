#include <QPainter>
#include <QWidget>
#ifndef LINE_H
#define LINE_H


/*class line
{
public:
    line();
};*/

class line : public QWidget
{
    Q_OBJECT

public:
    //the constructor is marked explicit so that we don't get any
    // implicit conversion by passing the wrong parameter
    explicit line(); //also it has a default null pointer value, so a linewidget can be created with null parameter (i.e when we don't specify the constructor parenthesis at all!)
    bool mousePressed;
    bool drawStarted;
    int selectedTool;
    //destructor is needed when we construct a object on the heap instead of stack
    // for efficient memory management
    ~line();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);


private:
    //we declare a linewidget pointer object using namespace identifier;
    QPainter painter;
    QPixmap *mPix;
    QLine mLine;
    QRect mRect;

};


#endif // LINE_H
