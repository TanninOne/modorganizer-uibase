/*
Mod Organizer shared UI functionality

Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "questionboxmemory.h"
#include "ui_questionboxmemory.h"

#include <functional>
#include <QMessageBox>
#include <QPushButton>
#include <QMutexLocker>


namespace MOBase {

QSettings *QuestionBoxMemory::s_SettingFile = NULL;
QMutex QuestionBoxMemory::s_SettingsMutex;

QuestionBoxMemory::QuestionBoxMemory(QWidget *parent, const QString &title, const QString &text,
                                     const QDialogButtonBox::StandardButtons buttons, QDialogButtonBox::StandardButton defaultButton)
  : QDialog(parent), ui(new Ui::QuestionBoxMemory)
{
  ui->setupUi(this);

  this->setWindowTitle(title);

  QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxQuestion);
  ui->iconLabel->setPixmap(icon.pixmap(128));
  ui->messageLabel->setText(text);
  ui->buttonBox->setStandardButtons(buttons);
  if (defaultButton != QDialogButtonBox::NoButton) {
    ui->buttonBox->button(defaultButton)->setDefault(true);
  }
  connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}


QuestionBoxMemory::~QuestionBoxMemory()
{
  delete ui;
}


void QuestionBoxMemory::init(const QString &fileName)
{
  QMutexLocker locker(&s_SettingsMutex);
  if (s_SettingFile == NULL) {
    s_SettingFile = new QSettings(fileName, QSettings::IniFormat);
    atexit(&QuestionBoxMemory::cleanup);
  }
}

void QuestionBoxMemory::cleanup()
{
  QMutexLocker locker(&s_SettingsMutex);
  s_SettingFile->sync();
  delete s_SettingFile;
}

void QuestionBoxMemory::buttonClicked(QAbstractButton *button)
{
  m_Button = ui->buttonBox->standardButton(button);
}

QDialogButtonBox::StandardButton QuestionBoxMemory::query(QWidget *parent, const QString &name,
    const QString &title, const QString &text, QDialogButtonBox::StandardButtons buttons,
    QDialogButtonBox::StandardButton defaultButton)
{
  QMutexLocker locker(&s_SettingsMutex);
  if (s_SettingFile->contains(QString("DialogChoices/") + name)) {
    return static_cast<QDialogButtonBox::StandardButton>(s_SettingFile->value(QString("DialogChoices/") + name).toInt());
  } else {
    QuestionBoxMemory dialog(parent, title, text, buttons, defaultButton);
    dialog.exec();
    if (dialog.ui->rememberCheckBox->isChecked()) {
      s_SettingFile->setValue(QString("DialogChoices/") + name, dialog.m_Button);
    }
    return dialog.m_Button;
  }
}

}

