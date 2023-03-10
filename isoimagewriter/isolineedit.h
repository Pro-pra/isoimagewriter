/* 
 * SPDX-FileCopyrightText: 2022 Enes Albay <albayenes@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef ISOLINEEDIT_H
#define ISOLINEEDIT_H

#include <QLineEdit>
#include <QEvent>

class IsoLineEdit : public QLineEdit
{
    Q_OBJECT

public:
  IsoLineEdit(QWidget *parent = 0);
  ~IsoLineEdit() = default;

   bool eventFilter(QObject* object, QEvent* event) override
   {
    if(object == this && event->type() == QEvent::MouseButtonRelease) {
        emit clicked();
        return true;
    }
    return false;
}

signals:
  void clicked();
};

#endif
