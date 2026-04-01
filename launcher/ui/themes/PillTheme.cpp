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
    background-color: rgba(60, 60, 65, 120);
    border-radius: 20px;
    margin-top: 40px;
    margin-left: auto;
    margin-right: auto;
    padding: 8px 16px;
    spacing: 20px;
    border: 1px solid rgba(255, 255, 255, 30);
    max-width: 600px; /* Force it to look pill-shaped and not stretch entirely */
}

/* On Mac, the toolbar will be pushed down. We want it floating. */
QToolBar#mainToolBar::separator {
    background-color: rgba(255, 255, 255, 30);
    width: 1px;
    margin: 4px 10px;
}

QToolBar#mainToolBar QToolButton {
    background: transparent;
    color: rgba(255, 255, 255, 200);
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    font-size: 14px;
    font-weight: 500;
    padding: 8px 16px;
    border-radius: 12px;
}

QToolBar#mainToolBar QToolButton:hover {
    background-color: rgba(255, 255, 255, 40);
    color: white;
}

QToolBar#mainToolBar QToolButton:checked, QToolBar#mainToolBar QToolButton:pressed {
    background-color: rgba(255, 255, 255, 255);
    color: black;
}

/* Menus */
QMenu {
    background-color: rgba(40, 40, 45, 240);
    border-radius: 12px;
    border: 1px solid rgba(255, 255, 255, 20);
    color: white;
    padding: 6px;
}
QMenu::item {
    padding: 6px 16px;
    border-radius: 8px;
}
QMenu::item:selected {
    background-color: rgba(255, 255, 255, 40);
    color: white;
}

/* Instance View / Cards */
QListView {
    background: transparent;
    border: none;
    outline: none;
    padding: 20px;
}

QListView::item {
    background-color: rgba(255, 255, 255, 10);
    border-radius: 12px;
    margin: 10px;
    padding: 10px;
}

QListView::item:selected {
    background-color: rgba(255, 255, 255, 40);
    border: 2px solid rgba(255, 255, 255, 100);
}

QListView::item:hover {
    background-color: rgba(255, 255, 255, 25);
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
