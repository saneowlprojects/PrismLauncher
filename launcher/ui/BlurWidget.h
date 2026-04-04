// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <QWidget>
#include <QPixmap>
#include <QColor>

class BlurWidget : public QWidget {
    Q_OBJECT

public:
    explicit BlurWidget(QWidget* parent = nullptr);

    void setBlurRadius(int radius);
    void setOverlayColor(const QColor& color);
    void setBorderRadius(int radius);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QPixmap applyBlur(const QPixmap& source, int radius) const;

    int m_blurRadius = 12;
    QColor m_overlayColor = QColor(60, 55, 45, 153);
    int m_borderRadius = 9999;
};
