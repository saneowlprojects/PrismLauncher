// SPDX-License-Identifier: GPL-3.0-only
#include "BlurWidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

BlurWidget::BlurWidget(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
}

void BlurWidget::setBlurRadius(int radius)
{
    m_blurRadius = radius;
    update();
}

void BlurWidget::setOverlayColor(const QColor& color)
{
    m_overlayColor = color;
    update();
}

void BlurWidget::setBorderRadius(int radius)
{
    m_borderRadius = radius;
    update();
}

void BlurWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    update();
}

QPixmap BlurWidget::applyBlur(const QPixmap& source, int radius) const
{
    if (source.isNull() || radius <= 0)
        return source;

    // Scale down then back up for blur effect (Qt workaround)
    qreal scale = qMax(0.05, 1.0 - radius / 120.0);
    QSize smallSize = QSize(source.width() * scale, source.height() * scale);
    if (smallSize.isEmpty())
        return source;
    QPixmap small = source.scaled(smallSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return small.scaled(source.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void BlurWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw clipped pill shape
    QPainterPath clipPath;
    clipPath.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
    painter.setClipPath(clipPath);

    // Grab and blur the content behind this widget
    if (parentWidget()) {
        QPoint parentPos = mapTo(parentWidget(), QPoint(0, 0));
        QPixmap bgPixmap = parentWidget()->grab(QRect(parentPos, size()));
        QPixmap blurred = applyBlur(bgPixmap, m_blurRadius);
        painter.drawPixmap(rect(), blurred);
    }

    // Draw overlay color
    painter.fillRect(rect(), m_overlayColor);
}
