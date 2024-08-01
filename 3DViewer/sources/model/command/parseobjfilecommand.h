#ifndef PARSEOBJFILECOMMAND_H
#define PARSEOBJFILECOMMAND_H

#include <QFile>
#include <QString>
#include <QTextStream>

#include "command/command.h"
#include "scenemodel.h"
#include "shared/renderer/raw_data/obj_data.h"

class ParseOBJFileCommand : public Command {
 public:
  explicit ParseOBJFileCommand(SceneModel *model) : model_(model), data_({}) {}

  ~ParseOBJFileCommand() = default;

  bool execute() {
    QFile file(model_->FilePath());
    qDebug() << model_->FilePath();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    unsigned int offset = 0;
    QTextStream in(&file);
    QString mtllib = "";
    QString cur_material = "";
    while (!in.atEnd()) {
      QString line = in.readLine().trimmed();
      if (line.startsWith("v ")) {
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        QVector3D vertex = QVector3D(parts[1].toDouble(), parts[2].toDouble(),
                                     parts[3].toDouble());
        data_.AddVertex(vertex);
      } else if (line.startsWith("vn ")) {
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        QVector3D normal = QVector3D(parts[1].toDouble(), parts[2].toDouble(),
                                     parts[3].toDouble());
        data_.AddNormal(normal);
      } else if (line.startsWith("vt ")) {
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        QVector2D texture_coords =
            QVector2D(parts[1].toDouble(), parts[2].toDouble());
        data_.AddTextureCoords(texture_coords);
      } else if (line.startsWith("usemtl")) {
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.size() > 1) {
          cur_material = parts[1];
        }
      } else if (line.startsWith("mtllib")) {
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.size() > 1) {
          mtllib = parts[1];
        }
      } else if (line.startsWith("f ")) {
        Face face = {};
        face.SetOffset(offset);
        face.SetMaterialName(cur_material);
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        QVector<unsigned int> polygon;

        for (int i = 1; i < parts.size(); ++i) {
          QStringList indices = parts[i].split("/");
          if (indices.size() >= 1) {
            unsigned int vertex_index = indices[0].toUInt();
            face.AddVertexIndex(vertex_index);
          }
          if (indices.size() >= 2 && indices[1] != "") {
            unsigned int texture_coords_index = indices[1].toUInt();
            face.AddTextureCoordsIndex(texture_coords_index);
          }

          if (indices.size() >= 3) {
            unsigned int normal_index = indices[2].toUInt();
            face.AddNormalIndex(normal_index);
          }
        }

        data_.AddFace(face);
        offset += face.VertexCount();
      }
    }
    data_.SetMaterialsLib(mtllib);
    data_.CenterVertices();
    model_->AddRawData(data_);
    return true;
  }

 private:
  SceneModel *model_;
  OBJ_Data data_;
};

#endif  // PARSEOBJFILECOMMAND_H
