#ifndef CUSTOMMENUSTYLE_H
#define CUSTOMMENUSTYLE_H


#include "qstyleoption.h"
#include <QProxyStyle>
#include <QPainter>

class CustomMenuStyle : public QProxyStyle
{

public:
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override {
        if (element == CE_MenuItem)
        {
            if (const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
            {
                // Если это пункт "Удалить" — красим в красный
                if (menuItem->text == "Удалить")
                {
                    QStyleOptionMenuItem modifiedItem = *menuItem;

                    // Меняем цвет текста
                    modifiedItem.palette.setColor(QPalette::Text, Qt::red);

                    // Если пункт выделен (hover) — делаем светло-красный фон
                    if (menuItem->state & State_Selected)
                    {
                        modifiedItem.palette.setColor(QPalette::Highlight, QColor(255, 235, 238)); // #FFEBEE
                    }

                    QProxyStyle::drawControl(element, &modifiedItem, painter, widget);
                    return;
                }
            }
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }
};

#endif // CUSTOMMENUSTYLE_H
