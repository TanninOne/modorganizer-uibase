#ifndef TASKPROGRESSMANAGER_H
#define TASKPROGRESSMANAGER_H

#include <QObject>
#include <QMutexLocker>
#include <QTimer>
#include <QTime>
#include <map>
#include <Windows.h>
#include <shobjidl.h>
#include "dllimport.h"

namespace MOBase {

class QDLLEXPORT TaskProgressManager : QObject
{

  Q_OBJECT

public:

  static TaskProgressManager &instance();

  void forgetMe(quint32 id);
  void updateProgress(quint32 id, qint64 value, qint64 max);

  quint32 getId();

private slots:
  bool tryCreateTaskbar();
private:

  TaskProgressManager();
  void showProgress();

private:

  std::map<quint32, std::pair<QTime, qint64> > m_Percentages;
  QMutex m_Mutex;
  quint32 m_NextId;
  QTimer m_CreateTimer;
  int m_CreateTries;

  HWND m_WinId;

  ITaskbarList3 *m_Taskbar;
};

}

#endif // TASKPROGRESSMANAGER_H
