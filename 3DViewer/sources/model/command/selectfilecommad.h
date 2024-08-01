#ifndef SELECTFILECOMMAND_H
#define SELECTFILECOMMAND_H

#include <QFileDialog>
#include <QVector3D>

#include "command/command.h"
#include "model/scenemodel.h"

class SelectFileCommand : public Command {
 public:
  explicit SelectFileCommand(SceneModel *scene_model, FileFormat format)
      : scene_model_(scene_model), format_(format) {}

  bool execute() override {
    if (format_ == FileFormat::model) {
      return SelectModel();
    } else if (format_ == FileFormat::texture) {
      return SelectTexture();
    } else {
      return SelectMaterial();
    }
  }

  bool SelectModel() {
    QString file_path = QFileDialog::getOpenFileName(
        0, default_model_files_title, scene_model_->LoadDir(),
        default_model_files_format);
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    scene_model_->SetFilePath(file_path);
    return true;
  }

  bool SelectTexture() {
    QString file_path = QFileDialog::getOpenFileName(
        0, default_texture_files_title, scene_model_->LoadDir(),
        default_texture_files_format);
    QImage image(file_path);
    if (image.isNull()) {
      return false;
    }
    scene_model_->SetTexturePath(file_path);
    return true;
  }

  bool SelectMaterial() {
    QString file_path = QFileDialog::getOpenFileName(
        0, default_material_files_title, scene_model_->LoadDir(),
        default_material_files_format);
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    scene_model_->SetMaterialPath(file_path);
    return true;
  }

 private:
  SceneModel *scene_model_;
  FileFormat format_;
};

#endif  // SELECTFILECOMMAND_H
