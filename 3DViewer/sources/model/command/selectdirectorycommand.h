#ifndef SELECTDIRECTORYCOMMAND_H
#define SELECTDIRECTORYCOMMAND_H

#include <QDialog>
#include <QFileDialog>
#include <QVector3D>

#include "command/command.h"
#include "settingsdialog.h"

class SelectDirectoryCommand : public Command {
 public:
  explicit SelectDirectoryCommand(SettingsDialog *dialog,
                                  DirectoryFormat format, const QString &dir)
      : dialog_(dialog),
        format_(format),
        dir_(dir)

  {}

  bool execute() override {
    if (format_ == DirectoryFormat::load) {
      return LoadDirectory();
    } else {
      return SaveDirectory();
    }
  }

  bool LoadDirectory() {
    QString new_dir = QFileDialog::getExistingDirectory(
        0, ("Choose directory"), dir_,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (new_dir.isEmpty()) {
      return false;
    } else
      dialog_->SetLoadDirLabelText(new_dir);

    return true;
  }
  bool SaveDirectory() {
    QString new_dir = QFileDialog::getExistingDirectory(
        0, ("Choose directory"), dir_,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (new_dir.isEmpty()) {
      return false;
    } else
      dialog_->SetSaveDirLabelText(new_dir);
    return true;
  }

 private:
  SettingsDialog *dialog_;
  DirectoryFormat format_;
  QString dir_;
};

#endif  // SELECTDIRECTORYCOMMAND_H
