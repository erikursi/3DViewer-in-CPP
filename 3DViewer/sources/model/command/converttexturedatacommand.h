#ifndef CONVERTTEXTUREDATACOMMAND_H
#define CONVERTTEXTUREDATACOMMAND_H

#include "command/command.h"
#include "scenemodel.h"
class ConvertTextureDataCommand : public Command {
 public:
  explicit ConvertTextureDataCommand(SceneModel *scene_model)
      : scene_model_(scene_model) {}
  bool execute() override {
    QImage image(scene_model_->TexturePath());
    if (image.isNull()) {
      return false;
    }
    image.convertTo(QImage::Format_RGBA8888);
    auto texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->create();
    scene_model_->PrepareTexture(texture, image);
    scene_model_->SetTexture(texture);
    return true;
  }

 private:
  SceneModel *scene_model_;
};
#endif  // CONVERTTEXTUREDATACOMMAND_H
