/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "view/widget/title_bar.h"

#include <QDebug>
#include <QTimer>

#include "view/theme_manager.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/search_edit.h"

namespace dman {

TitleBar::TitleBar(QWidget* parent) : QFrame(parent) {
  this->setObjectName("TitleBar");
  this->initUI();
  this->initConnections();
}

TitleBar::~TitleBar() {
}

void TitleBar::setCompletionWindow(SearchCompletionWindow* completion_window) {
  completion_window_ = completion_window;
  connect(search_edit_, &SearchEdit::upKeyPressed,
          completion_window_, &SearchCompletionWindow::goUp);
  connect(search_edit_, &SearchEdit::downKeyPressed,
          completion_window_, &SearchCompletionWindow::goDown);
}

bool TitleBar::backButtonVisible() const {
  return back_btn_->isVisible();
}

void TitleBar::setBackButtonVisible(bool visible) {
  back_btn_->setVisible(visible);
}

void TitleBar::initConnections() {
  connect(search_edit_, &SearchEdit::textChanged,
          this, &TitleBar::onSearchTextChanged);
  connect(back_btn_, &Dtk::Widget::DImageButton::clicked,
          this, &TitleBar::backButtonClicked);

  connect(search_edit_, &SearchEdit::focusOut,
          this, &TitleBar::onSearchEditFocusOut);
}

void TitleBar::initUI() {
  QLabel* app_icon = new QLabel();
  app_icon->setObjectName("AppIcon");
//  app_icon->setPixmap(QIcon(kImageDeepinManual).pixmap(24, 24));
  app_icon->setFixedSize(24, 24);

  back_btn_ = new Dtk::Widget::DImageButton();
  back_btn_->setObjectName("BackButton");
  back_btn_->setFixedSize(24, 24);
  back_btn_->hide();

  QHBoxLayout* left_layout = new QHBoxLayout();
  left_layout->setSpacing(0);
  left_layout->setContentsMargins(0, 0, 0, 0);
  left_layout->addWidget(app_icon);
  left_layout->addSpacing(10);
  left_layout->addWidget(back_btn_);
  QFrame* left_buttons = new QFrame();
  left_buttons->setContentsMargins(0, 0, 0, 0);
  left_buttons->setLayout(left_layout);

  search_edit_ = new SearchEdit();
  search_edit_->setObjectName("SearchEdit");
  search_edit_->setFixedSize(242, 26);
  search_edit_->setPlaceHolder(tr("Search"));

  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->addWidget(left_buttons);
  main_layout->addWidget(search_edit_, 1, Qt::AlignCenter);
  main_layout->addSpacing(48);
  this->setLayout(main_layout);

  ThemeManager::instance()->registerWidget(this);
}

void TitleBar::onSearchEditFocusOut() {
  QTimer::singleShot(50, [=]() {
    this->completion_window_->hide();
  });
}

void TitleBar::onSearchTextChanged() {
  const QString text = search_edit_->text();
  emit this->searchTextChanged(text);

  if (text.size() > 1 && completion_window_ != nullptr) {
    // Do real search.

    completion_window_->show();
    // Move to bottom of search edit.
    completion_window_->move(this->parentWidget()->rect().width() / 2 - 80,
                             40 - 4);
    completion_window_->setFocusPolicy(Qt::StrongFocus);
    completion_window_->raise();
    qDebug() << "size:" << completion_window_->geometry();
  }
}

}  // namespace dman
