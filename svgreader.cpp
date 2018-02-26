#include "svgreader.h"
#include <QPen>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QStringList>
extern int pix_per_brick;
SvgReader::SvgReader()
{

}

QList<unit *> SvgReader::getElements(const QString filename)
{
    QList<unit *> rectList;

    QDomDocument doc;
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return rectList;


    QDomNodeList gList = doc.elementsByTagName("g");

    for (int i = 0; i < gList.size(); i++) {
        QDomNode gNode = gList.item(i);
        QDomElement rectangle = gNode.firstChildElement("rect");

        if (rectangle.isNull()){
            continue;
        } else {

            unit *rect = new unit();

            rect->setFlag(QGraphicsItem::ItemIsMovable);

            QDomElement gElement = gNode.toElement();
//            rect->xi = rectangle.attribute("x").toInt()/pix_per_brick;
//            rect->yi = rectangle.attribute("y").toInt()/pix_per_brick;
//            rect->length = rectangle.attribute("width").toInt()/pix_per_brick;
//            rect->width = rectangle.attribute("height").toInt()/pix_per_brick;
            rect->xi = rectangle.attribute("x").toInt();
            rect->yi = rectangle.attribute("y").toInt();
            rect->length = rectangle.attribute("width").toInt();
            rect->width = rectangle.attribute("height").toInt();

            QColor fillColor(gElement.attribute("fill", "#ffffff"));
            fillColor.setAlphaF(gElement.attribute("fill-opacity","0").toFloat());
            rect->color = fillColor;


            if(fillColor == QColor(255, 208, 166, 127)){
                rect->type = "merge";
            }else if(fillColor == QColor(1, 96, 177, 127)){
                rect->type = "cycle";
            }else if(fillColor == QColor(255, 184, 184, 127)){
                rect->type = "move";
            }else if(fillColor == QColor(215, 230, 144, 127)){
                rect->type = "dispenser";
            }else if(fillColor == QColor(108, 137, 147, 127)){
                rect->type = "heat";
            }else{
                rect->type = "heat";
            }

            rectList.append(rect);
        }
    }
    file.close();
    return rectList;
}

QRectF SvgReader::getSizes(const QString filename)
{
    QDomDocument doc;
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return QRectF(0,0,200,200);

    QDomNodeList list = doc.elementsByTagName("svg");
    if(list.length() > 0) {
        QDomElement svgElement = list.item(0).toElement();
        QStringList parameters = svgElement.attribute("viewBox").split(" ");
        return QRectF(parameters.at(0).toInt(),
                      parameters.at(1).toInt(),
                      parameters.at(2).toInt(),
                      parameters.at(3).toInt());
    }
    return QRectF(0,0,200,200);
}
