// SPDX-License-Identifier: GPL-3.0-only
#include "BackgroundWidget.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>

BackgroundWidget::BackgroundWidget(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(false);

    m_rotationTimer = new QTimer(this);
    connect(m_rotationTimer, &QTimer::timeout, this, &BackgroundWidget::rotateWallpaper);

    m_fadeTimer = nullptr;
}

void BackgroundWidget::addWallpaper(const QString& path)
{
    if (QFileInfo::exists(path)) {
        m_wallpapers.append(path);
    }
}

void BackgroundWidget::startRotation(int intervalMs)
{
    if (m_wallpapers.isEmpty())
        return;

    m_rotationTimer->start(intervalMs);
    rotateWallpaper();
}

void BackgroundWidget::stopRotation()
{
    m_rotationTimer->stop();
    if (m_fadeTimer) {
        m_fadeTimer->stop();
        m_fadeTimer->deleteLater();
        m_fadeTimer = nullptr;
    }
    m_currentPixmap = QPixmap();
    m_nextPixmap = QPixmap();
    m_isFading = false;
    m_fadeProgress = 0.0;
    update();
}

void BackgroundWidget::setCrossfadeDuration(int ms)
{
    m_crossfadeDuration = ms;
}

void BackgroundWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    if (m_currentPixmap.isNull() && m_nextPixmap.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    if (!m_currentPixmap.isNull()) {
        if (m_isFading)
            painter.setOpacity(1.0 - m_fadeProgress);
        else
            painter.setOpacity(1.0);
        painter.drawPixmap(rect(), m_currentPixmap);
    }

    if (m_isFading && !m_nextPixmap.isNull()) {
        painter.setOpacity(m_fadeProgress);
        painter.drawPixmap(rect(), m_nextPixmap);
    }

    QLinearGradient gradient(rect().topLeft(), rect().bottomLeft());
    gradient.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradient.setColorAt(0.3, QColor(0, 0, 0, 80));
    gradient.setColorAt(0.6, QColor(0, 0, 0, 160));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 220));
    painter.setOpacity(1.0);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillRect(rect(), gradient);
}

void BackgroundWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    // Scale pixmaps to new size
    if (!m_currentPixmap.isNull()) {
        QPixmap orig = m_currentPixmap;
        m_currentPixmap = scaleToSize(orig, size());
    }
    if (!m_nextPixmap.isNull()) {
        QPixmap orig = m_nextPixmap;
        m_nextPixmap = scaleToSize(orig, size());
    }
}

void BackgroundWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    // Only manually resize when not managed by a layout
    if (parentWidget() && !parentWidget()->layout()) {
        setGeometry(parentWidget()->rect());
    }
}

bool BackgroundWidget::event(QEvent* event)
{
    if (event->type() == QEvent::ParentAboutToChange) {
        if (parentWidget()) {
            parentWidget()->removeEventFilter(this);
        }
    } else if (event->type() == QEvent::ParentChange) {
        if (parentWidget() && !parentWidget()->layout()) {
            parentWidget()->installEventFilter(this);
            setGeometry(parentWidget()->rect());
        }
    } else if (event->type() == QEvent::Resize) {
        if (parentWidget() && !parentWidget()->layout()) {
            setGeometry(parentWidget()->rect());
        }
    }
    return QWidget::event(event);
}

void BackgroundWidget::rotateWallpaper()
{
    if (m_isFading)
        return;

    if (m_currentPixmap.isNull()) {
        loadNextWallpaper();
        return;
    }

    if (m_wallpapers.isEmpty())
        return;

    QString path = m_wallpapers[m_currentIndex];
    m_currentIndex = (m_currentIndex + 1) % m_wallpapers.size();

    QPixmap pix;
    if (!pix.load(path))
        return;

    m_nextPixmap = scaleToSize(pix, size());

    m_isFading = true;
    m_fadeProgress = 0.0;

    const int interval = m_crossfadeDuration / m_crossfadeSteps;
    int step = 0;

    m_fadeTimer = new QTimer(this);
    connect(m_fadeTimer, &QTimer::timeout, [this, step]() mutable {
        int s = step + 1;
        m_fadeProgress = static_cast<double>(s) / m_crossfadeSteps;
        update();

        if (s >= m_crossfadeSteps) {
            m_fadeTimer->stop();
            m_fadeTimer->deleteLater();
            m_fadeTimer = nullptr;
            m_isFading = false;
            m_currentPixmap = m_nextPixmap;
            m_nextPixmap = QPixmap();
            m_fadeProgress = 0.0;
            update();
        }
        step = s;
    });
    m_fadeTimer->start(interval);
}

void BackgroundWidget::advanceFade()
{
}

void BackgroundWidget::loadNextWallpaper()
{
    if (m_wallpapers.isEmpty())
        return;

    QString path = m_wallpapers[m_currentIndex];
    m_currentIndex = (m_currentIndex + 1) % m_wallpapers.size();

    QPixmap pix;
    if (!pix.load(path)) {
        qWarning() << "BackgroundWidget: Failed to load wallpaper:" << path;
        return;
    }

    qDebug() << "BackgroundWidget: Loaded wallpaper:" << path << "size:" << pix.size();
    m_currentPixmap = scaleToSize(pix, size());
    update();
}

QPixmap BackgroundWidget::scaleToSize(const QPixmap& source, const QSize& sz) const
{
    if (sz.isEmpty())
        return source;
    return source.scaled(sz, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
}
