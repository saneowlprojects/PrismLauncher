// SPDX-License-Identifier: GPL-3.0-only
#include "PillTheme.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QPalette>

PillTheme::PillTheme() {
    m_palette = QPalette();
    m_palette.setColor(QPalette::Window, QColor(25, 25, 30, 200));
    m_palette.setColor(QPalette::WindowText, Qt::white);
    m_palette.setColor(QPalette::Base, QColor(0, 0, 0, 0));
    m_palette.setColor(QPalette::AlternateBase, QColor(255, 255, 255, 10));
    m_palette.setColor(QPalette::ToolTipBase, QColor(40, 40, 45, 200));
    m_palette.setColor(QPalette::ToolTipText, Qt::white);
    m_palette.setColor(QPalette::Text, Qt::white);
    m_palette.setColor(QPalette::Button, QColor(255, 255, 255, 30));
    m_palette.setColor(QPalette::ButtonText, Qt::white);
    m_palette.setColor(QPalette::BrightText, Qt::red);
    m_palette.setColor(QPalette::Link, QColor(42, 130, 218));
    m_palette.setColor(QPalette::Highlight, QColor(255, 255, 255, 60));
    m_palette.setColor(QPalette::HighlightedText, Qt::white);

    m_styleSheet = R"(
/* Global Typography */
* {
    font-family: "SF Pro Rounded", "SF Pro Display", -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
}

/* Global Transparency */
QMainWindow, QWidget#centralWidget, QScrollArea, QSplitter, QStackedWidget {
    background: transparent !important;
    border: none;
}

/* Floating Pill Navbar */
QToolBar#mainToolBar {
    background-color: rgba(0, 0, 0, 80);
    border: 1px solid rgba(255, 255, 255, 40);
    border-radius: 25px;
    margin-top: 20px;
    padding: 4px 12px;
    spacing: 12px;
}

QToolBar#mainToolBar QToolButton {
    background: transparent;
    color: rgba(255, 255, 255, 180);
    font-size: 14px;
    font-weight: 500;
    padding: 8px 16px;
    border: none;
}

QToolBar#mainToolBar QToolButton:hover {
    color: white;
    background: rgba(255, 255, 255, 20);
    border-radius: 18px;
}

QToolBar#mainToolBar QToolButton#playButton {
    background-color: white;
    color: black;
    border-radius: 18px;
}

/* Hero Section */
#heroBadge {
    background-color: rgba(255, 255, 255, 30);
    color: white;
    border: 1px solid rgba(255, 255, 255, 40);
    border-radius: 6px;
    padding: 5px 12px;
    font-size: 11px;
    font-weight: 700;
    text-transform: uppercase;
}

#heroTitle {
    color: white;
    font-size: 110px;
    font-weight: 800;
    letter-spacing: -3px;
    background: transparent;
    margin: 5px 0;
}

#heroDescription {
    color: rgba(255, 255, 255, 210);
    font-size: 18px;
    font-weight: 400;
    line-height: 1.4;
    background: transparent;
    max-width: 650px;
}

#heroButton {
    background-color: white;
    color: black;
    border: none;
    border-radius: 28px;
    padding: 15px 40px;
    font-size: 18px;
    font-weight: 700;
}

#heroMoreButton {
    background-color: rgba(255, 255, 255, 25);
    color: white;
    border: 1px solid rgba(255, 255, 255, 45);
    border-radius: 28px;
    padding: 15px 40px;
    font-size: 18px;
    font-weight: 700;
}

/* Instance Grid Fidelity */
InstanceView {
    background: transparent !important;
    border: none;
    padding-left: 80px !important;
    padding-right: 80px !important;
}

#otherInstancesLabel {
    color: white;
    font-size: 30px;
    font-weight: 700;
    margin-top: 45px;
    margin-bottom: 20px;
    margin-left: 80px !important;
}

/* Scrollbars */
QScrollBar:vertical {
    border: none;
    background: transparent;
    width: 6px;
}
QScrollBar::handle:vertical {
    background: rgba(255, 255, 255, 60);
    border-radius: 3px;
}

/* Menus */
QMenu {
    background-color: rgba(30, 30, 35, 240);
    border: 1px solid rgba(255, 255, 255, 40);
    border-radius: 12px;
    color: white;
    padding: 6px;
}
QMenu::item {
    padding: 8px 18px;
    border-radius: 8px;
}
QMenu::item:selected {
    background-color: rgba(255, 255, 255, 50);
}
)";
}

QString PillTheme::appStyleSheet() {
    return m_styleSheet;
}

QPalette PillTheme::colorScheme() {
    return m_palette;
}
