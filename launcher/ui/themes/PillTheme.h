// SPDX-License-Identifier: GPL-3.0-only
#pragma once
#include <QPalette>
#include <QColor>
#include <QString>
#include "ITheme.h"

class PillTheme : public ITheme {
public:
    PillTheme();
    QString id() override { return "pill"; }
    QString name() override { return "Pill"; }
    QString tooltip() override { return "Modern Apple TV inspired UI with translucent pill navigation."; }
    bool hasStyleSheet() override { return true; }
    QString appStyleSheet() override;
    QString qtTheme() override { return "fusion"; }
    QPalette colorScheme() override;
    double fadeAmount() override { return 0.5; }
    QColor fadeColor() override { return QColor(0, 0, 0); }

private:
    QString m_styleSheet;
    QPalette m_palette;
};
