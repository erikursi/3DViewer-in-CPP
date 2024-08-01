#ifndef PROCESSOBJFILECOMMAND_H
#define PROCESSOBJFILECOMMAND_H
#include "command/convertobjdatacommand.h"
#include "command/macrocommand.h"
#include "command/parseobjfilecommand.h"
#include "command/selectfilecommad.h"
#include "model/scenemodel.h"

class ProcessOBJFileCommand : public MacroCommand {
 public:
  explicit ProcessOBJFileCommand(SceneModel *scene_model,
                                 bool with_select = true) {
    if (with_select) {
      commands_.push_back(
          new SelectFileCommand(scene_model, FileFormat::model));
    }
    commands_.push_back(new ParseOBJFileCommand(scene_model));
    commands_.push_back(new ConvertOBJDataCommand(scene_model));
    for (auto command : commands_) {
      addCommand(command);
    }
  }

  ~ProcessOBJFileCommand() {
    for (auto command : commands_) {
      delete command;
    }
  }

 private:
  QVector<Command *> commands_;
};

#endif  // PROCESSOBJFILECOMMAND_H
