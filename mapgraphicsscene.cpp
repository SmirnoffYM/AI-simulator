#include "mapgraphicsscene.h"

MapGraphicsScene::MapGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

// Draw all objects
// Each object is a circle colored with getColor()
// If object is movable, it has orientation
// Orientation is indicated by line with inverted circle color
// Line links circle's center and circle's outline
void MapGraphicsScene::drawObject(Object *object)
{
    if (object->getCoords().first >= static_cast<int>(object->getSize() / 2)
            && object->getCoords().second >= static_cast<int>(object->getSize() / 2)
            && object->getSize() > 0
            && object->getCoords().first + static_cast<int>(object->getSize() / 2) <=
            HubModule::modellingSystem->getWorld()->getSize().first * REAL_PIXEL_SIZE
            && object->getCoords().second + static_cast<int>(object->getSize() / 2) <=
            HubModule::modellingSystem->getWorld()->getSize().second * REAL_PIXEL_SIZE) {
        QColor outlineColor(255 - object->getColor().red(),
                            255 - object->getColor().green(),
                            255 - object->getColor().blue());

        int circle_x = (object->getCoords().first -
                        object->getSize() / 2) / REAL_PIXEL_SIZE;
        int circle_y = (object->getCoords().second -
                        object->getSize() / 2) / REAL_PIXEL_SIZE;

        QGraphicsEllipseItem *ellipse = addEllipse(circle_x, circle_y,
                                                   object->getSize() / REAL_PIXEL_SIZE,
                                                   object->getSize() / REAL_PIXEL_SIZE,
                                                   QPen(outlineColor),
                                                   QBrush(QColor(object->getColor().red(),
                                                                 object->getColor().green(),
                                                                 object->getColor().blue())));

        // draw orientation line if object is movable
        if (object->isMovable()) {
            const int accuracy = 16;    //qt's accuracy for degree values
            ellipse->setStartAngle((90 - object->getOrientation()) * accuracy);
            ellipse->setSpanAngle(360 * accuracy - 1);
        }
    }
}
