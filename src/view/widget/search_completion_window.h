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

#ifndef DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H
#define DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H

#include <QFrame>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>

#include "controller/search_result.h"

namespace dman {

class SearchCompletionWindow : public QFrame {
  Q_OBJECT
 public:
  explicit SearchCompletionWindow(QWidget* parent = nullptr);
  ~SearchCompletionWindow() override;

  void autoResize();

 signals:
  void resultClicked(const QString& app_name, const QString& anchor);
  void searchButtonClicked();

 public slots:
  void goDown();
  void goUp();
  void onEnterPressed();

  void setKeyword(const QString& keyword);
  void setSearchAnchorResult(const SearchAnchorResultList& result);

 private:
  void initConnections();
  void initUI();

  QListView* result_view_ = nullptr;
  QStringListModel* model_ = nullptr;
  QPushButton* search_button_ = nullptr;
  SearchAnchorResultList result_;

 private slots:
  void onResultListClicked(const QModelIndex& index);
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H
