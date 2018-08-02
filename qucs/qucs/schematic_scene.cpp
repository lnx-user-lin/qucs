
#include "schematic_scene.h"
#include "schematic.h"

// ---------------------------------------------------
// forward to graphicscene, once it is there.
ElementGraphics* Schematic::itemAt(int x, int y)
{
	qDebug() << "Schematic::itemAt" << x << y;
#if QT_VERSION >= 0x050000
	QPoint p(x, y);
	QGraphicsItem* I=scene()->itemAt(p, QTransform());
	if(ElementGraphics* G=dynamic_cast<ElementGraphics*>(I)){ untested();
		qDebug() << "got something";
		return G;
	}else{ untested();
		qDebug() << "miss";
		return nullptr;
	}
#else
	for(Component *pc = Components->first(); pc != 0; pc = Components->next())
		if(pc->getSelected(x, y))
			return (ElementGraphics*)(pc);

	float Corr = 5.0 / Scale; // size of line select

    for(Painting *pp = Paintings->first(); pp != 0; pp = Paintings->next())
        if(pp->getSelected(x, y, Corr))
            return (ElementGraphics*)(pp);

	incomplete(); // also select the other stuff.
	return nullptr;
#endif
}

#if QT_VERSION >= 0x050000
Component* component(ElementGraphics* e){
	if(!e) return nullptr;
	return component(e->operator->());
}
Wire* wire(ElementGraphics* e){
	if(!e) return nullptr;
	return wire(e->operator->());
}
WireLabel* wireLabel(ElementGraphics* e){
	if(!e) return nullptr;
	return wireLabel(e->operator->());
}
Diagram* diagram(ElementGraphics* e){
	if(!e) return nullptr;
	return diagram(e->operator->());
}
Painting* painting(ElementGraphics* e){
	if(!e) return nullptr;
	return painting(e->operator->());
}
Marker* marker(ElementGraphics* e){
	if(!e) return nullptr;
	return marker(e->operator->());
}
Node* node(ElementGraphics* e){
	if(!e) return nullptr;
	return node(e->operator->());
}
Graph* graph(ElementGraphics* e){
	if(!e) return nullptr;
	return graph(e->operator->());
}

#endif

SchematicScene::SchematicScene(QObject *parent) :
  QGraphicsScene(parent)
{
}

SchematicScene::~SchematicScene()
{
}

void SchematicScene::drawBackground(QPainter *painter, const QRectF &rect)
{
#ifndef USE_SCROLLVIEW
	QGraphicsScene::drawBackground(painter, rect);
	return;

	// Draw origin when visible
	if(rect.contains(QPointF(0, 0))) {
		painter->drawLine(QLine(-3.0, 0.0, 3.0, 0.0));
		painter->drawLine(QLine(0.0, -3.0, 0.0, 3.0));
	}else{
	}
	/// \todo getter and setter
	int GridX = 10;
	int GridY = 10;

	// Get associated view, assume single view
	Schematic *v = static_cast<Schematic *>(views().at(0));

	// When scaling, adjust visible grid spacing
	float scale = v->Scale;
	if(scale < 1) {
		if(scale > 0.5) {
			GridX *= 4;
			GridY *= 4;
		} else {
			GridX *= 16;
			GridY *= 16;
		}
	}
#endif
}
