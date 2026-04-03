// SPDX-License-Identifier: GPL-3.0-only
#include "BlurWidget.h"

#include <QPainter>
#include <QGraphicsBlurEffect>
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

void BlurWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    // Grab the content behind this widget
    if (parentWidget()) {
        QPoint globalPos = mapToGlobal(QPoint(0, 0));
        QPixmap bgPixmap = parentWidget()->grab(QRect(globalPos - parentWidget()->mapToGlobal(QPoint(0, 0)), size()));

        // Apply blur
        QGraphicsBlurEffect blurEffect;
        blurEffect.setBlurRadius(m_blurRadius);

        QPixmap blurredPixmap(bgPixmap.size());
        blurredPixmap.fill(Qt::transparent);

        QPainter blurPainter(&blurredPixmap);
        blurPainter.setRenderHint(QPainter::SmoothPixmapTransform);
        blurEffect.draw(&blurPainter);

        QPainter clippedPainter(&blurredPixmap);
        clippedPainter.setRenderHint(QPainter::Antialiasing);
        QPainterPath clipPath;
        clipPath.addRoundedRect(blurredPixmap.rect(), m_borderRadius, m_borderRadius);
        clippedPainter.setClipPath(clipPath);
        blurEffect.render(&clippedPainter);

        m_blurredPixmap = blurredPixmap;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw clipped pill shape
    QPainterPath clipPath;
    clipPath.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
    painter.setClipPath(clipPath);

    // Draw blurred background
    if (!m_blurredPixmap.isNull()) {
        painter.drawPixmap(rect(), m_blurredPixmap);
    }

    // Draw overlay color
    painter.fillRect(rect(), m_overlayColor);
}
