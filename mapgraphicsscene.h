#ifndef MAPGRAPHICSSCENE_H
#define MAPGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <map>
#include "hubmodule.h"
#include "object.h"

class MapGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapGraphicsScene(QObject *parent = 0);
    
signals:
    
public slots:

public:
    // Draw some object
    void drawObject(Object *object);

};

#endif // MAPGRAPHICSSCENE_H
