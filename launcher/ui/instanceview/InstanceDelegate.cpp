// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "InstanceDelegate.h"
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QTextLayout>
#include <QTextOption>
#include <QtMath>

#include <QIcon>
#include <QTextEdit>
#include "BaseInstance.h"
#include "InstanceList.h"
#include "InstanceView.h"

// Origin: Qt
static void viewItemTextLayout(QTextLayout& textLayout, int lineWidth, qreal& height, qreal& widthUsed)
{
    height = 0;
    widthUsed = 0;
    textLayout.beginLayout();
    QString str = textLayout.text();
    while (true) {
        QTextLine line = textLayout.createLine();
        if (!line.isValid())
            break;
        if (line.textLength() == 0)
            break;
        line.setLineWidth(lineWidth);
        line.setPosition(QPointF(0, height));
        height += line.height();
        widthUsed = qMax(widthUsed, line.naturalTextWidth());
    }
    textLayout.endLayout();
}

ListViewDelegate::ListViewDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void drawSelectionRect(QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect)
{
    if ((option.state & QStyle::State_Selected))
        painter->fillRect(rect, option.palette.brush(QPalette::Highlight));
    else {
        QColor backgroundColor = option.palette.color(QPalette::Window);
        backgroundColor.setAlpha(160);
        painter->fillRect(rect, QBrush(backgroundColor));
    }
}

void drawFocusRect(QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect)
{
    if (!(option.state & QStyle::State_HasFocus))
        return;
    QStyleOptionFocusRect opt;
    opt.direction = option.direction;
    opt.fontMetrics = option.fontMetrics;
    opt.palette = option.palette;
    opt.rect = rect;
    // opt.state           = option.state | QStyle::State_KeyboardFocusChange |
    // QStyle::State_Item;
    auto col = option.state & QStyle::State_Selected ? QPalette::Highlight : QPalette::Base;
    opt.backgroundColor = option.palette.color(col);
    // Apparently some widget styles expect this hint to not be set
    painter->setRenderHint(QPainter::Antialiasing, false);

    QStyle* style = option.widget ? option.widget->style() : QApplication::style();

    style->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, painter, option.widget);

    painter->setRenderHint(QPainter::Antialiasing);
}

// TODO this can be made a lot prettier
void drawProgressOverlay(QPainter* painter, const QStyleOptionViewItem& option, const int value, const int maximum)
{
    if (maximum == 0 || value == maximum) {
        return;
    }

    painter->save();

    qreal percent = (qreal)value / (qreal)maximum;
    QColor color = option.palette.color(QPalette::Dark);
    color.setAlphaF(0.70f);
    painter->setBrush(color);
    painter->setPen(QPen(QBrush(), 0));
    painter->drawPie(option.rect, 90 * 16, -percent * 360 * 16);

    painter->restore();
}

void drawBadges(QPainter* painter, const QStyleOptionViewItem& option, BaseInstance* instance, QIcon::Mode mode, QIcon::State state)
{
    QList<QString> pixmaps;
    if (instance->isRunning()) {
        pixmaps.append("status-running");
    } else if (instance->hasCrashed() || instance->hasVersionBroken()) {
        pixmaps.append("status-bad");
    }
    if (instance->hasUpdateAvailable()) {
        pixmaps.append("checkupdate");
    }

    static const int itemSide = 24;
    static const int spacing = 1;
    const int itemsPerRow = qMax(1, qFloor(double(option.rect.width() + spacing) / double(itemSide + spacing)));
    const int rows = qCeil((double)pixmaps.size() / (double)itemsPerRow);
    QListIterator<QString> it(pixmaps);
    painter->translate(option.rect.topLeft());
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < itemsPerRow; ++x) {
            if (!it.hasNext()) {
                return;
            }
            // FIXME: inject this.
            auto icon = QIcon::fromTheme(it.next());
            QRect badgeRect(option.rect.width() - x * itemSide + qMax(x - 1, 0) * spacing - itemSide,
                            y * itemSide + qMax(y - 1, 0) * spacing, itemSide, itemSide);
            icon.paint(painter, badgeRect, Qt::AlignCenter, mode, state);
        }
    }
    painter->translate(-option.rect.topLeft());
}

static QSize viewItemTextSize(const QStyleOptionViewItem* option)
{
    QStyle* style = option->widget ? option->widget->style() : QApplication::style();
    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    QTextLayout textLayout;
    textLayout.setTextOption(textOption);
    textLayout.setFont(option->font);
    textLayout.setText(option->text);
    const int textMargin = style->pixelMetric(QStyle::PM_FocusFrameHMargin, option, option->widget) + 1;
    QRect bounds(0, 0, 100 - 2 * textMargin, 600);
    qreal height = 0, widthUsed = 0;
    viewItemTextLayout(textLayout, bounds.width(), height, widthUsed);
    const QSize size(qCeil(widthUsed), qCeil(height));
    return QSize(size.width() + 2 * textMargin, size.height());
}

void ListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    QRect cardRect = opt.rect.adjusted(10, 10, -10, -10);
    bool selected = (opt.state & QStyle::State_Selected);
    bool hovered = (opt.state & QStyle::State_MouseOver);

    // Rounded path for the high-fidelity card
    QPainterPath path;
    path.addRoundedRect(cardRect, 20, 20);
    
    // Draw the actual image (icon) scaled to fill the poster
    QRect iconRect = cardRect;
    QPixmap pixmap = opt.icon.pixmap(512, 512); 
    if (!pixmap.isNull()) {
        painter->setClipPath(path);
        painter->drawPixmap(iconRect, pixmap.scaled(iconRect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        painter->setClipping(false);
    } else {
        painter->fillPath(path, QColor(255, 255, 255, 20));
    }

    // Selection/Focus Effect (Premium white border and glow)
    if (selected) {
        painter->setPen(QPen(Qt::white, 6));
        painter->drawPath(path);

        // Title overlay for selected item — clipped to card rounded corners
        painter->save();
        painter->setClipPath(path);
        painter->setBrush(QColor(0, 0, 0, 180));
        painter->setPen(Qt::NoPen);
        QRect titleRect(cardRect.left(), cardRect.bottom() - 50, cardRect.width(), 50);
        painter->drawRect(titleRect);
        painter->restore();

        painter->setPen(Qt::white);
        QFont font = opt.font;
        font.setPixelSize(18);
        font.setWeight(QFont::Bold);
        painter->setFont(font);
        painter->drawText(titleRect.adjusted(20, 0, -20, 0), Qt::AlignVCenter | Qt::AlignLeft, opt.text);
    } else if (hovered) {
        painter->setPen(QPen(QColor(255, 255, 255, 120), 2));
        painter->drawPath(path);
    } else {
        painter->setPen(QPen(QColor(255, 255, 255, 30), 1));
        painter->drawPath(path);
    }

    // Progress Overlay if needed
    drawProgressOverlay(painter, opt, index.data(InstanceViewRoles::ProgressValueRole).toInt(),
                        index.data(InstanceViewRoles::ProgressMaximumRole).toInt());

    painter->restore();
}

QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem& option, [[maybe_unused]] const QModelIndex& index) const
{
    // High-fidelity posters are 280x160 (roughly 16:9) + margins
    return QSize(300, 180);
}

class NoReturnTextEdit : public QTextEdit {
    Q_OBJECT
   public:
    explicit NoReturnTextEdit(QWidget* parent) : QTextEdit(parent)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    }
    bool event(QEvent* event) override
    {
        auto eventType = event->type();
        if (eventType == QEvent::KeyPress || eventType == QEvent::KeyRelease) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            auto key = keyEvent->key();
            if ((key == Qt::Key_Return || key == Qt::Key_Enter) && eventType == QEvent::KeyPress) {
                emit editingDone();
                return true;
            }
            if (key == Qt::Key_Tab) {
                return true;
            }
        }
        return QTextEdit::event(event);
    }
   signals:
    void editingDone();
};

void ListViewDelegate::updateEditorGeometry(QWidget* editor,
                                            const QStyleOptionViewItem& option,
                                            [[maybe_unused]] const QModelIndex& index) const
{
    QRect cardRect = option.rect.adjusted(10, 10, -10, -10);
    QRect titleRect(cardRect.left(), cardRect.bottom() - 50, cardRect.width(), 50);
    editor->setGeometry(titleRect);
}

void ListViewDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    auto text = index.data(Qt::EditRole).toString();
    QTextEdit* realEditor = qobject_cast<NoReturnTextEdit*>(editor);
    realEditor->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    realEditor->append(text);
    realEditor->selectAll();
    realEditor->document()->clearUndoRedoStacks();
}

void ListViewDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QTextEdit* realEditor = qobject_cast<NoReturnTextEdit*>(editor);
    QString text = realEditor->toPlainText();
    text.replace(QChar('\n'), QChar(' '));
    text = text.trimmed();
    // Prevent instance names longer than 128 chars
    text.truncate(128);
    if (text.size() != 0) {
        emit textChanged(model->data(index).toString(), text);
        model->setData(index, text);
    }
}

QWidget* ListViewDelegate::createEditor(QWidget* parent,
                                        [[maybe_unused]] const QStyleOptionViewItem& option,
                                        [[maybe_unused]] const QModelIndex& index) const
{
    auto editor = new NoReturnTextEdit(parent);
    connect(editor, &NoReturnTextEdit::editingDone, this, &ListViewDelegate::editingDone);
    return editor;
}

void ListViewDelegate::editingDone()
{
    NoReturnTextEdit* editor = qobject_cast<NoReturnTextEdit*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

#include "InstanceDelegate.moc"
