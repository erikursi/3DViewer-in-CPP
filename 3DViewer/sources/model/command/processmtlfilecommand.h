#ifndef PROCESSMTLFILECOMMAND_H
#define PROCESSMTLFILECOMMAND_H
#include "command/convertmtldatacommand.h"
#include "command/macrocommand.h"
#include "command/selectfilecommad.h"
#include "model/scenemodel.h"

class ProcessMTLFileCommand : public MacroCommand {
 public:
  explicit ProcessMTLFileCommand(SceneModel *scene_model,
                                 bool with_select = true) {
    if (with_select) {
      commands_.push_back(
          new SelectFileCommand(scene_model, FileFormat::material));
    }
    commands_.push_back(new ConvertMTLDataCommand(scene_model));
    for (auto command : commands_) {
      addCommand(command);
    }
  }

  ~ProcessMTLFileCommand() {
    for (auto command : commands_) {
      delete command;
    }
  }

 private:
  QVector<Command *> commands_;
};

#endif  // PROCESSMTLFILECOMMAND_H
