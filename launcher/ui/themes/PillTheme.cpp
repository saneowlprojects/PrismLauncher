// SPDX-License-Identifier: GPL-3.0-only
#include "PillTheme.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QPalette>

PillTheme::PillTheme() {
    m_palette = QPalette();
    m_palette.setColor(QPalette::Window, QColor(10, 10, 10, 255));
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
/* =====================================================
   GLOBAL TYPOGRAPHY - SF Pro Rounded
   ===================================================== */
* {
    font-family: "SF Pro Rounded", "SF Pro Display", -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
}

/* =====================================================
   GLOBAL TRANSPARENCY
   ===================================================== */
QMainWindow, QWidget#centralWidget, QScrollArea, QSplitter, QStackedWidget {
    background: transparent !important;
    border: none;
}

/* =====================================================
    FLOATING PILL NAVBAR
    ===================================================== */
QWidget#pillNavBar {
    background-color: rgba(60, 55, 45, 153);
    border: none;
    border-radius: 9999px;
    padding: 0;
}

QWidget#pillNavBar QPushButton {
    background: transparent;
    color: #D4D4D8;
    font-size: 14px;
    font-weight: 500;
    padding: 8px 24px;
    border: none;
    border-radius: 9999px;
    min-height: 40px;
    line-height: 1.4;
}

QWidget#pillNavBar QPushButton:hover {
    color: #FFFFFF;
}

QWidget#pillNavBar QPushButton:checked {
    background-color: #FFFFFF;
    color: #18181B;
    font-weight: 500;
}

/* =====================================================
    INSTANCE CONTEXT MENU (modern & minimalistic)
    ===================================================== */
QMenu#instanceContextMenu {
    background-color: rgba(40, 37, 32, 245);
    border: 1px solid rgba(255, 255, 255, 30);
    border-radius: 12px;
    padding: 6px;
    font-size: 13px;
    min-width: 200px;
}

QMenu#instanceContextMenu::item {
    color: #D4D4D8;
    padding: 8px 16px;
    border-radius: 6px;
    margin: 2px 4px;
}

QMenu#instanceContextMenu::item:selected {
    background-color: rgba(255, 255, 255, 15);
    color: #FFFFFF;
}

QMenu#instanceContextMenu::item:disabled {
    color: rgba(212, 212, 216, 60);
}

QMenu#instanceContextMenu::separator {
    height: 1px;
    background-color: rgba(255, 255, 255, 15);
    margin: 4px 12px;
}

/* =====================================================
    HERO SECTION
    ===================================================== */
QWidget#heroWidget {
    background: transparent;
}

#heroTitle {
    color: white;
    font-size: 96px;
    font-weight: 200;
    letter-spacing: -2px;
    background: transparent;
}

#heroDescription {
    color: rgba(255, 255, 255, 180);
    font-size: 18px;
    font-weight: 400;
    line-height: 1.5;
    background: transparent;
    max-width: 700px;
}

#heroButton {
    background-color: white;
    color: black;
    border: none;
    border-radius: 28px;
    font-size: 16px;
    font-weight: 600;
}

#heroButton:hover {
    background-color: rgba(255, 255, 255, 220);
}

#heroMoreButton {
    background-color: rgba(255, 255, 255, 15);
    color: white;
    border: 1px solid rgba(255, 255, 255, 30);
    border-radius: 28px;
    font-size: 16px;
    font-weight: 600;
    backdrop-filter: blur(10px);
}

#heroMoreButton:hover {
    background-color: rgba(255, 255, 255, 25);
}

/* =====================================================
   OTHER INSTANCES LABEL
   ===================================================== */
#otherInstancesLabel {
    color: white;
    font-size: 30px;
    font-weight: 600;
    margin-top: 24px;
    margin-bottom: 24px;
    margin-left: 80px !important;
}

/* =====================================================
    INSTANCES PAGE HEADER
    ===================================================== */
#instancesHeader {
    color: white;
    font-size: 34px;
    font-weight: 600;
    margin-top: 24px;
    margin-bottom: 16px;
    margin-left: 80px !important;
    margin-right: 80px !important;
}

#homePage, #instancesPage {
    background: transparent;
}

#pageStack {
    background: transparent;
}

/* =====================================================
    INSTANCE CARDS
    ===================================================== */
InstanceView {
    background: transparent !important;
    border: none;
    padding-left: 80px !important;
    padding-right: 80px !important;
    padding-top: 8px !important;
    padding-bottom: 80px !important;
}

/* =====================================================
   SETTINGS PAGE - SIDEBAR
   ===================================================== */
QListView#settingsSidebar {
    background: transparent;
    border: none;
    padding: 16px 12px;
    outline: none;
}

QListView#settingsSidebar::item {
    padding: 12px 16px;
    border-radius: 8px;
    margin-bottom: 4px;
    color: rgba(255, 255, 255, 150);
    font-size: 14px;
    font-weight: 500;
}

QListView#settingsSidebar::item:hover {
    background: rgba(255, 255, 255, 10);
    color: white;
}

QListView#settingsSidebar::item:selected {
    background: rgba(255, 255, 255, 10);
    color: white;
}

QLabel#settingsTitle {
    color: white;
    font-size: 24px;
    font-weight: 300;
    padding: 24px 16px 16px 16px;
}

QPushButton#backButton {
    background: transparent;
    color: rgba(255, 255, 255, 180);
    border: none;
    border-radius: 8px;
    padding: 8px 16px;
    font-size: 14px;
    font-weight: 500;
    text-align: left;
}

QPushButton#backButton:hover {
    background: rgba(255, 255, 255, 10);
    color: white;
}

/* =====================================================
   SETTINGS CONTENT CARDS
   ===================================================== */
QGroupBox, QWidget[cssClass="card"] {
    background: rgba(255, 255, 255, 5);
    border: 1px solid rgba(255, 255, 255, 10);
    border-radius: 24px;
    padding: 32px;
    margin-top: 24px;
    margin-bottom: 24px;
}

QGroupBox::title {
    color: white;
    font-size: 16px;
    font-weight: 600;
    padding: 0 0 16px 0;
}

QLabel#pageHeader {
    color: white;
    font-size: 60px;
    font-weight: 300;
    letter-spacing: -1px;
}

QLabel#pageSubtitle {
    color: rgba(255, 255, 255, 160);
    font-size: 18px;
    font-weight: 400;
}

/* =====================================================
   FORM CONTROLS
   ===================================================== */
QRadioButton {
    color: rgba(255, 255, 255, 180);
    font-size: 14px;
    font-weight: 400;
    padding: 8px 0;
    spacing: 10px;
}

QRadioButton:hover {
    color: white;
}

QRadioButton::indicator {
    width: 20px;
    height: 20px;
    border-radius: 10px;
    border: 2px solid rgba(255, 255, 255, 30);
    background: transparent;
}

QRadioButton::indicator:hover {
    border-color: rgba(255, 255, 255, 60);
}

QRadioButton::indicator:checked {
    background: white;
    border-color: white;
}

QCheckBox {
    color: rgba(255, 255, 255, 180);
    font-size: 14px;
    font-weight: 400;
    spacing: 12px;
}

QCheckBox:hover {
    color: white;
}

QCheckBox::indicator {
    width: 20px;
    height: 20px;
    border-radius: 6px;
    border: 2px solid rgba(255, 255, 255, 30);
    background: transparent;
}

QCheckBox::indicator:hover {
    border-color: rgba(255, 255, 255, 60);
}

QCheckBox::indicator:checked {
    background: white;
    border-color: white;
}

QLineEdit, QComboBox, QSpinBox {
    background: rgba(255, 255, 255, 5);
    border: 1px solid rgba(255, 255, 255, 20);
    border-radius: 10px;
    padding: 12px 16px;
    color: white;
    font-size: 14px;
    font-weight: 400;
}

QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
    border-color: rgba(255, 255, 255, 40);
    background: rgba(255, 255, 255, 8);
}

QLineEdit::placeholder-text {
    color: rgba(255, 255, 255, 80);
}

QPushButton {
    background: rgba(255, 255, 255, 15);
    color: white;
    border: 1px solid rgba(255, 255, 255, 20);
    border-radius: 10px;
    padding: 12px 24px;
    font-size: 14px;
    font-weight: 600;
}

QPushButton:hover {
    background: rgba(255, 255, 255, 25);
}

QPushButton:pressed {
    background: rgba(255, 255, 255, 30);
}

/* =====================================================
   SCROLLBARS
   ===================================================== */
QScrollBar:vertical {
    border: none;
    background: transparent;
    width: 6px;
}

QScrollBar::handle:vertical {
    background: rgba(255, 255, 255, 40);
    border-radius: 3px;
    min-height: 40px;
}

QScrollBar::handle:vertical:hover {
    background: rgba(255, 255, 255, 60);
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    border: none;
    background: transparent;
    height: 6px;
}

QScrollBar::handle:horizontal {
    background: rgba(255, 255, 255, 40);
    border-radius: 3px;
    min-width: 40px;
}

QScrollBar::handle:horizontal:hover {
    background: rgba(255, 255, 255, 60);
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* =====================================================
   MENUS
   ===================================================== */
QMenu {
    background-color: rgba(20, 20, 25, 240);
    border: 1px solid rgba(255, 255, 255, 20);
    border-radius: 12px;
    color: white;
    padding: 8px;
}

QMenu::item {
    padding: 10px 20px;
    border-radius: 8px;
    color: rgba(255, 255, 255, 180);
    font-size: 14px;
    font-weight: 400;
}

QMenu::item:selected {
    background-color: rgba(255, 255, 255, 15);
    color: white;
}

QMenu::separator {
    height: 1px;
    background: rgba(255, 255, 255, 15);
    margin: 8px 12px;
}

/* =====================================================
   LABELS
   ===================================================== */
QLabel {
    color: rgba(255, 255, 255, 200);
    font-size: 14px;
    font-weight: 400;
}

QLabel[cssClass="muted"] {
    color: rgba(255, 255, 255, 120);
}

QLabel[cssClass="title"] {
    color: white;
    font-size: 30px;
    font-weight: 600;
}

/* =====================================================
   GROUP BOXES
   ===================================================== */
QGroupBox {
    background: rgba(255, 255, 255, 5);
    border: 1px solid rgba(255, 255, 255, 10);
    border-radius: 16px;
    margin-top: 16px;
    padding-top: 16px;
}

QGroupBox::title {
    color: white;
    font-size: 14px;
    font-weight: 600;
    subcontrol-origin: margin;
    left: 16px;
    padding: 0 8px;
}
)";
}

QString PillTheme::appStyleSheet() {
    return m_styleSheet;
}

QPalette PillTheme::colorScheme() {
    return m_palette;
}
