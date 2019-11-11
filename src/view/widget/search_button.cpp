/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "view/widget/search_button.h"
#include "base/utils.h"

#include <QHBoxLayout>
#include <QStylePainter>
#include <QMouseEvent>

#include <DStyleHelper>
#include <DApplicationHelper>

#include <DIconButton>
#include <DLog>

DWIDGET_USE_NAMESPACE

namespace dman {

SearchButton::SearchButton(QWidget* parent)
    : DBlurEffectWidget(parent)
    , m_bHover(false)
{
    if (DGuiApplicationHelper::DarkType == DGuiApplicationHelper::instance()->themeType()) {
        setBackgroundRole(QPalette::Background);
        setAutoFillBackground(false);
    }
    else {
        setBackgroundRole(QPalette::Dark);
        setAutoFillBackground(true);
    }

    m_textLabel = new DLabel(this);
    m_textLabel->setBackgroundRole(QPalette::Background);
    m_textLabel->setAutoFillBackground(false);
    m_textLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    DIconButton *iconBtn = new DIconButton(DStyle::SP_IndicatorSearch);
    iconBtn->setBackgroundRole(QPalette::Background);
    iconBtn->setAutoFillBackground(false);
    iconBtn->setFlat(true);
    iconBtn->setFocusPolicy(Qt::NoFocus);
    iconBtn->setAttribute(Qt::WA_TransparentForMouseEvents);

    iconBtn->setIconSize(QSize(20, 20));

    QFont textFont("Source HanSans SC");
    textFont.setPixelSize(14);
    textFont.setWeight(QFont::Medium);

    DPalette paLabel = DApplicationHelper::instance()->palette(m_textLabel);
    paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::WindowText));
    m_textLabel->setPalette(paLabel);
    m_textLabel->setFont(textFont);

    QHBoxLayout *centerLayout = new QHBoxLayout(this);
    centerLayout->setMargin(0);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);

    centerLayout->addSpacing(30);
    centerLayout->addWidget(iconBtn, 0, Qt::AlignVCenter);
    centerLayout->addSpacing(8);
    centerLayout->addWidget(m_textLabel, 0, Qt::AlignCenter);
    centerLayout->addStretch();
    setLayout(centerLayout);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &SearchButton::onThemeChange);
}

SearchButton::~SearchButton() {

}

void SearchButton::setText(QString title) {
    m_textLabel->setText(title);
}

bool SearchButton::isChecked() {
    return m_bHover;
}

void SearchButton::setChecked(bool bChecked) {
    m_bHover = bChecked;
    update();
}

void SearchButton::onThemeChange(DGuiApplicationHelper::ColorType themeType) {
    if (DGuiApplicationHelper::DarkType == themeType) {
        setBackgroundRole(QPalette::Background);
        setAutoFillBackground(false);
    }
    else {
        setBackgroundRole(QPalette::Dark);
        setAutoFillBackground(true);
    }
}

void SearchButton::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event)

    QStylePainter painter(this);
    QPainterPath path;
    path.addRect(this->rect());

    if (m_bHover) {
        DPalette pa = ExApplicationHelper::instance()->palette(this);
        DStyleHelper styleHelper;
        QColor fillColor = pa.color(DPalette::Highlight);
        painter.fillPath(path, QBrush(fillColor));
    }
    else {
        DPalette pa = ExApplicationHelper::instance()->palette(this);
        DStyleHelper styleHelper;
        QColor fillColor = pa.color(DPalette::FrameBorder);
        painter.fillPath(path, QBrush(fillColor));
    }

    DPalette paLabel = ExApplicationHelper::instance()->palette(this);
    paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::Text));
    m_textLabel->setPalette(paLabel);
}

void SearchButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void SearchButton::mouseReleaseEvent(QMouseEvent *event)
{
    DWidget::mouseReleaseEvent(event);

    emit pressed();
}

void SearchButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    emit entered();
    m_bHover = true;
    update();
}

void SearchButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    m_bHover = false;
    update();
}

}  // namespace dman
