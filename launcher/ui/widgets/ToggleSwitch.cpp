// SPDX-License-Identifier: GPL-3.0-only
#include "ToggleSwitch.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

ToggleSwitch::ToggleSwitch(QWidget* parent) : QWidget(parent) {
    setFixedHeight(28);
    setFixedWidth(52);
    setCursor(Qt::PointingHandCursor);
    
    m_animation = new QPropertyAnimation(this, "offset", this);
    m_animation->setDuration(200);
    m_animation->setEasingCurve(QEasingCurve::InOutCubic);
}

QSize ToggleSwitch::sizeHint() const {
    return QSize(52, 28);
}

void ToggleSwitch::setChecked(bool checked) {
    if (m_checked == checked) return;
    m_checked = checked;
    
    m_animation->stop();
    m_animation->setStartValue(m_offset);
    m_animation->setEndValue(checked ? 26.0 : 0.0);
    m_animation->start();
    
    emit toggled(checked);
}

void ToggleSwitch::setOffset(qreal offset) {
    m_offset = offset;
    update();
}

void ToggleSwitch::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Track background
    QPainterPath trackPath;
    trackPath.addRoundedRect(2, 2, width() - 4, height() - 4, 12, 12);
    
    QColor trackColor = m_checked ? QColor(255, 255, 255) : QColor(255, 255, 255, 40);
    painter.fillPath(trackPath, trackColor);
    
    // Thumb
    qreal thumbX = 2 + m_offset;
    QPainterPath thumbPath;
    thumbPath.addEllipse(QPointF(thumbX + 10, height() / 2.0), 10, 10);
    
    QColor thumbColor = m_checked ? QColor(0, 0, 0) : QColor(255, 255, 255, 200);
    painter.fillPath(thumbPath, thumbColor);
}

void ToggleSwitch::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setChecked(!m_checked);
        event->accept();
    }
}

void ToggleSwitch::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    if (m_checked) {
        m_offset = 26.0;
    } else {
        m_offset = 0.0;
    }
}
