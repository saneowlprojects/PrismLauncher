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
    m_palette.setColor(QPalette::Base, QColor(40, 40, 45, 150));
    m_palette.setColor(QPalette::AlternateBase, QColor(50, 50, 55, 150));
    m_palette.setColor(QPalette::ToolTipBase, QColor(40, 40, 45, 200));
    m_palette.setColor(QPalette::ToolTipText, Qt::white);
    m_palette.setColor(QPalette::Text, Qt::white);
    m_palette.setColor(QPalette::Button, QColor(60, 60, 65, 150));
    m_palette.setColor(QPalette::ButtonText, Qt::white);
    m_palette.setColor(QPalette::BrightText, Qt::red);
    m_palette.setColor(QPalette::Link, QColor(42, 130, 218));
    m_palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    m_palette.setColor(QPalette::HighlightedText, Qt::white);

    m_styleSheet = R"(
/* Pill CSS */
QMainWindow, QWidget#centralWidget, QScrollArea, QSplitter {
    background: transparent;
}

/* The floating pill nav bar (assuming mainToolBar) */
QToolBar#mainToolBar {
    background-color: rgba(30, 30, 35, 180);
    border-radius: 30px;
    margin-top: 50px;
    margin-bottom: 20px;
    padding: 6px 12px;
    spacing: 10px;
    border: 1px solid rgba(255, 255, 255, 30);
}

/* Specific centering for the toolbar if it's in a layout */
QWidget#centralWidget > QToolBar#mainToolBar {
    margin-left: 100px;
    margin-right: 100px;
}

QToolBar#mainToolBar::separator {
    background-color: rgba(255, 255, 255, 40);
    width: 1px;
    margin: 10px 12px;
}

QToolBar#mainToolBar QToolButton {
    background: transparent;
    color: rgba(255, 255, 255, 180);
    font-family: -apple-system, BlinkMacSystemFont, "SF Pro Display", "Helvetica Neue", Arial, sans-serif;
    font-size: 16px;
    font-weight: 600;
    padding: 12px 28px;
    border-radius: 24px;
}

QToolBar#mainToolBar QToolButton:hover {
    background-color: rgba(255, 255, 255, 40);
    color: white;
}

QToolBar#mainToolBar QToolButton:checked, QToolBar#mainToolBar QToolButton:pressed {
    background-color: white;
    color: black;
}

/* Menus */
QMenu {
    background-color: rgba(25, 25, 30, 250);
    border-radius: 16px;
    border: 1px solid rgba(255, 255, 255, 30);
    color: white;
    padding: 8px;
}
QMenu::item {
    padding: 8px 20px;
    border-radius: 10px;
}
QMenu::item:selected {
    background-color: rgba(255, 255, 255, 50);
    color: white;
}

/* Instance View / Cards */
QListView {
    background-color: transparent !important;
    background: transparent !important;
    border: none;
    outline: none;
    padding: 40px;
}

/* Grid drawing is handled by InstanceDelegate, so minimal CSS here */
QListView::item {
    margin-right: 20px;
    margin-bottom: 20px;
}

/* Hero Section */
QWidget#heroWidget {
    background: qlineargradient(x1:0, y1:0, x2:0.6, y2:0, stop:0 rgba(0,0,0,160), stop:1 transparent);
}

QLabel#heroTitle {
    color: white;
    font-size: 110px;
    font-weight: 800;
    margin-bottom: 0px;
    letter-spacing: -2px;
    background: transparent;
}

QLabel#heroSubtitle {
    color: rgba(255, 255, 255, 220);
    font-size: 36px;
    font-weight: 500;
    margin-bottom: 30px;
    background: transparent;
}

QPushButton#heroButton {
    background-color: white;
    color: black;
    border-radius: 12px;
    padding: 18px 45px;
    font-weight: 700;
    font-size: 22px;
    min-width: 160px;
}

QPushButton#heroButton:hover {
    background-color: rgba(255, 255, 255, 0.9);
}

QLabel#otherInstancesLabel {
    color: white;
    font-size: 32px;
    font-weight: 700;
    margin-left: 60px;
    margin-top: 40px;
    margin-bottom: 20px;
}

/* Other toolbars to hide */
QToolBar#instanceToolBar, QToolBar#newsToolBar {
    max-height: 0px;
    padding: 0px;
    margin: 0px;
    border: none;
    background: transparent;
}

QToolBar#instanceToolBar *, QToolBar#newsToolBar * {
    max-height: 0px;
    color: transparent;
    background: transparent;
    border: none;
    padding: 0px;
    margin: 0px;
}
)";
}

QString PillTheme::appStyleSheet() {
    return m_styleSheet;
}

QPalette PillTheme::colorScheme() {
    return m_palette;
}
