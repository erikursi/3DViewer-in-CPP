#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "consts.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT
 public:
  SettingsDialog(QWidget *parent = 0,
                 const QString &load_dir = default_load_dir,
                 const QString &save_dir = default_save_dir);
  ~SettingsDialog();
  void UpdateDirectories(const QString &load_dir, const QString &save_dir);
  void SaveClicked();
  void CancelClicked();
  void SetLoadDirLabelText(const QString &load_dir);
  void SetSaveDirLabelText(const QString &save_dir);
  QString LoadDir() { return load_dir_; }
  QString SaveDir() { return save_dir_; }
  QPair<QString, QString> GetDirectories() { return {load_dir_, save_dir_}; }
  void ChooseLoadDirClicked();
  void ChooseSaveDirClicked();

 signals:
  void ChooseLoadDirSignal(const QString &dir);
  void ChooseSaveDirSignal(const QString &dir);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  void ConfigureButtonBox();
  Ui::SettingsDialog *ui;
  QString load_dir_;
  QString save_dir_;
};

#endif  // SETTINGSDIALOG_H
