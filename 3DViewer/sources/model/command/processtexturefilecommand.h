#ifndef PROCESSTEXTUREFILECOMMAND_H
#define PROCESSTEXTUREFILECOMMAND_H
#include "command/converttexturedatacommand.h"
#include "command/macrocommand.h"
#include "command/selectfilecommad.h"
#include "model/scenemodel.h"

class ProcessTextureFileCommand : public MacroCommand {
 public:
  explicit ProcessTextureFileCommand(SceneModel *scene_model,
                                     bool with_select = true) {
    if (with_select) {
      commands_.push_back(
          new SelectFileCommand(scene_model, FileFormat::texture));
    }
    commands_.push_back(new ConvertTextureDataCommand(scene_model));
    for (auto command : commands_) {
      addCommand(command);
    }
  }

  ~ProcessTextureFileCommand() {
    for (auto command : commands_) {
      delete command;
    }
  }

 private:
  QVector<Command *> commands_;
};

#endif  // PROCESSTEXTUREFILECOMMAND_H
