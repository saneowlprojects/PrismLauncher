// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QPainter>

class BackgroundWidget : public QWidget {
    Q_OBJECT

public:
    explicit BackgroundWidget(QWidget* parent = nullptr);

    void addWallpaper(const QString& path);
    void startRotation(int intervalMs = 15000);
    void stopRotation();

    void setCrossfadeDuration(int ms);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool event(QEvent* event) override;

private slots:
    void rotateWallpaper();
    void advanceFade();

private:
    void loadNextWallpaper();
    QPixmap scaleToSize(const QPixmap& source, const QSize& size) const;

    QList<QString> m_wallpapers;
    int m_currentIndex = 0;

    QPixmap m_currentPixmap;
    QPixmap m_nextPixmap;
    QPixmap m_originalCurrent;
    QPixmap m_originalNext;

    double m_fadeProgress = 0.0;
    bool m_isFading = false;

    int m_crossfadeDuration = 1500;
    int m_crossfadeSteps = 60;

    QTimer* m_rotationTimer = nullptr;
    QTimer* m_fadeTimer = nullptr;
};
