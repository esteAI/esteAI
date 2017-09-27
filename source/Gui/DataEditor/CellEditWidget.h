#pragma once

#include <QTextEdit>
#include <QVector2D>

#include "Model/Definitions.h"
#include "Model/Entities/Descriptions.h"

class CellEditWidget
	: public QTextEdit
{
    Q_OBJECT
public:
    explicit CellEditWidget(QWidget *parent = 0);

    void updateCell (CellDescription const& cell);
    void requestUpdate ();

protected:
    void keyPressEvent (QKeyEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseDoubleClickEvent (QMouseEvent* e);
    void wheelEvent (QWheelEvent* e);

private:

    void updateDisplay ();

    qreal generateNumberFromFormattedString (QString s);
    QString generateFormattedRealString (QString s);
    QString generateFormattedRealString (qreal r);
    QString generateFormattedCellFunctionString (Enums::CellFunction::Type type);

	CellDescription _cell;
};

