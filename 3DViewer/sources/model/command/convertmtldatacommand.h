#ifndef CONVERTMTLDATACOMMAND_H
#define CONVERTMTLDATACOMMAND_H

#include <QFile>

#include "command/command.h"
#include "model/scenemodel.h"
#include "qfileinfo.h"

class ConvertMTLDataCommand : public Command {
 public:
  explicit ConvertMTLDataCommand(SceneModel *scene_model)
      : scene_model_(scene_model) {}
  bool execute() override {
    QFile material_path(scene_model_->MaterialPath());
    if (!material_path.open(QIODevice::ReadOnly | QIODevice::Text))
      return false;

    QFileInfo file_info(material_path);
    QString path = file_info.path();
    QTextStream in(&material_path);
    QVector<Material> materials;
    Material cur_material;
    bool textures = false;
    while (!in.atEnd()) {
      QString line = in.readLine().trimmed();
      QStringList parts = line.split(" ", Qt::SkipEmptyParts);
      if (parts.isEmpty()) continue;

      QString keyword = parts[0].toLower();
      if (keyword == "newmtl") {
        if (cur_material.name != "") {
          materials.push_back(cur_material);
          cur_material = {};
        }
        cur_material.name = parts[1];
      } else if (keyword == "ns") {
        cur_material.components.shininess_exponent = parts[1].toFloat();
      } else if (keyword == "ka") {
        cur_material.components.ambient_color = QVector3D(
            parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
      } else if (keyword == "kd") {
        cur_material.components.diffuse_color = QVector3D(
            parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
      } else if (keyword == "ks") {
        cur_material.components.specular_color = QVector3D(
            parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
      } else if (keyword == "map_kd") {
        QString filename = parts[1];
        QImage texture_image(path + "/" + filename);
        if (!texture_image.isNull()) {
          cur_material.map_kd = texture_image;
          cur_material.has_diffuse_texture = true;
          textures = true;
        } else {
          // qDebug() << "Failed to load texture " << path + filename;
        }
      } else if (keyword == "map_bump" || keyword == "bump") {
        QStringList bump_parts = line.split(" ", Qt::SkipEmptyParts);
        float bump_multiplier = 1.0;  // Default value
        if (bump_parts.size() > 2 && bump_parts[1] == "-bm") {
          bump_multiplier = bump_parts[2].toFloat();
        }
        QString filename = bump_parts.last();
        QImage texture_image(path + "/" + filename);
        if (!texture_image.isNull()) {
          cur_material.bump_multiplier = bump_multiplier;
          cur_material.map_bump = texture_image;
          cur_material.has_bump_texture = true;
          textures = true;
        } else {
          // qDebug() << "Failed to load texture " << path + filename;
        }
      } else if (keyword == "map_ka") {
        QString filename = parts[1];
        QImage texture_image(path + "/" + filename);
        if (!texture_image.isNull()) {
          cur_material.map_ka = texture_image;
          cur_material.has_ambient_texture = true;
          textures = true;
        } else {
          // qDebug() << "Failed to load texture " << path + filename;
        }
      } else if (keyword == "map_ks") {
        QString filename = parts[1];
        QImage texture_image(path + "/" + filename);
        if (!texture_image.isNull()) {
          cur_material.map_ks = texture_image;
          cur_material.has_specular_texture = true;
          textures = true;
        } else {
          // qDebug() << "Failed to load texture " << path + filename;
        }
      } else if (keyword == "d ") {
        QStringList d_parts = line.split(" ", Qt::SkipEmptyParts);
        if (d_parts.size() > 1) {
          cur_material.dissolve = d_parts[1].toFloat();
        }
      }
    }

    if (cur_material.name != "") {
      materials.push_back(cur_material);
    }

    material_path.close();
    ConvertMaterials(materials);
    scene_model_->AddMaterials(file_info.fileName(), materials, textures);
    return true;
  }

  void ConvertMaterials(QVector<Material> &materials) {
    for (auto &material : materials) {
      if (material.has_diffuse_texture) {
        QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->create();
        scene_model_->PrepareTexture(texture, material.map_kd);
        material.diffuse_sampler = texture;
      }
      if (material.has_ambient_texture) {
        QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->create();
        scene_model_->PrepareTexture(texture, material.map_ka);
        material.ambient_sampler = texture;
      }
      if (material.has_specular_texture) {
        QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->create();
        scene_model_->PrepareTexture(texture, material.map_ks);
        material.specular_sampler = texture;
      }
      if (material.has_bump_texture) {
        QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->create();
        scene_model_->PrepareTexture(texture, material.map_bump);
        material.bump_sampler = texture;
      }
    }
  }

 private:
  SceneModel *scene_model_;
};

#endif  // CONVERTMTLDATACOMMAND_H
