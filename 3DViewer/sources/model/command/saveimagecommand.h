#ifndef SAVEIMAGECOMMAND_H
#define SAVEIMAGECOMMAND_H

#include "command/command.h"
#include "qfiledialog.h"
#include "view/scene.h"
class SaveImageCommand : public Command {
 public:
  SaveImageCommand(Scene *scene, const QString &save_dir)
      : scene_(scene), save_dir_(save_dir) {}

  bool execute() {
    QString strfilter = "";
    QFileDialog dialog;
    QImage img = scene_->grabFramebuffer();

    QString file_name = dialog.getSaveFileName(
        scene_, QObject::tr("save"), save_dir_ + "/image.jpg",
        QObject::tr("*.bmp *.jpg"), &strfilter);

    if (!file_name.isEmpty()) {
      if (strfilter.contains("jpg"))
        img.save(file_name, "JPG");
      else
        img.save(file_name, "BMP");
    }

    return true;
  }

 private:
  Scene *scene_;
  QString save_dir_;
};

#endif  // SAVEIMAGECOMMAND_H
