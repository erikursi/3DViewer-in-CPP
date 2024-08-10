#ifndef MESHMODEL_H
#define MESHMODEL_H
#include <QArrayData>

#include "qfileinfo.h"
#include "qopenglshaderprogram.h"
#include "shared/renderer/configuration/config.h"
#include "shared/renderer/meshmodel/face.h"

class MeshModel {
 public:
  MeshModel()
      : size_(0),
        vertex_count_(0),
        face_count_(0),
        config_(nullptr),
        program_(nullptr) {}

  MeshModel(const QVector<Face> &faces, const std::vector<QVector4D> &vertices,
            unsigned int vertex_count, unsigned int face_count, double size)
      : size_(size),
        vertex_count_(vertex_count),
        face_count_(face_count),
        faces_(faces),
        vertices_(vertices),
        config_(nullptr),
        program_(nullptr) {}

  void Clear() {
    size_ = vertex_count_ = face_count_ = 0;
    faces_.clear();
    vertices_.clear();
    normals_.clear();
    config_ = nullptr;
    program_ = nullptr;
  }

  void SetTextureCoords(const std::vector<QVector2D> &texture_coords) {
    texture_coords_ = texture_coords;
  }
  void SetNormals(const std::vector<QVector3D> &normals) { normals_ = normals; }

  void FinishRendering() {
    if (config_->TextureMode() && config_->Texture()) {
      config_->Texture()->release();
    }
  }

  static void FinishRenderingGroup(const Material &material) {
    if (material.has_diffuse_texture && material.diffuse_sampler) {
      material.diffuse_sampler->release();
    }
    if (material.has_ambient_texture && material.ambient_sampler) {
      material.ambient_sampler->release();
    }
    if (material.has_specular_texture && material.specular_sampler) {
      material.specular_sampler->release();
    }
    if (material.has_bump_texture && material.bump_sampler) {
      material.bump_sampler->release();
    }
  }

  void RenderFaces(QVector<int>::const_iterator start,
                   QVector<int>::const_iterator end) const {
    for (auto i = start; i != end; i++) {
      const auto &face = faces_[*i];

      // SetLightSettings(face, config_->FaceColor());
      face.RenderFaces();
    }
  }

  void RenderFaces() {
    for (const auto &face : faces_) {
      // SetLightSettings(face, config_->FaceColor());
      program_->setUniformValue("color", config_->FaceColor());
      face.RenderFaces();
    }
  }

  void RenderByMaterial() {
    for (auto it = faces_with_one_material_.begin();
         it != faces_with_one_material_.end(); ++it) {
      const QVector<int> &indices = it.value();
      const auto &material = materials_map_[it.key()];
      SendMaterialDataToShaders(material);
      RenderGroup(indices.begin(), indices.end());
      FinishRenderingGroup(material);
    }
  }

  void RenderEdges(QVector<int>::const_iterator start,
                   QVector<int>::const_iterator end) {
    glLineWidth(config_->EdgeWidth());

    if (config_->EdgeType() == DASHED) {
      glLineStipple(1, 0x3333);
      glEnable(GL_LINE_STIPPLE);
    }

    for (auto i = start; i != end; i++) {
      const auto &face = faces_[*i];

      // SetLightSettings(face, config_->EdgeColor());
      face.RenderEdges();
    }

    if (config_->EdgeType() == DASHED) {
      glDisable(GL_LINE_STIPPLE);
    }
  }

  void RenderEdges() {
    glLineWidth(config_->EdgeWidth());

    if (config_->EdgeType() == DASHED) {
      glLineStipple(1, 0x3333);
      glEnable(GL_LINE_STIPPLE);
    }

    for (const auto &face : faces_) {
      program_->setUniformValue("color", config_->EdgeColor());
      // SetLightSettings(face, config_->EdgeColor());
      face.RenderEdges();
    }

    if (config_->EdgeType() == DASHED) {
      glDisable(GL_LINE_STIPPLE);
    }
  }
  void RenderPoints(QVector<int>::const_iterator start,
                    QVector<int>::const_iterator end) {
    glPointSize(config_->VertexSize());
    // program_->setUniformValue("color", config_->VertexColor());
    if (config_->VertexType() == CIRCLE) {
      glEnable(GL_MULTISAMPLE);
    } else {
      glDisable(GL_MULTISAMPLE);
    }

    for (auto i = start; i != end; i++) {
      const auto &face = faces_[*i];

      // program_->setUniformValue("color", config_->VertexColor());
      // SetLightSettings(face, config_->VertexColor());
      face.RenderPoints();
    }
    if (config_->VertexType() == CIRCLE) glDisable(GL_MULTISAMPLE);
  }

  void RenderPoints() {
    glPointSize(config_->VertexSize());
    // program_->setUniformValue("color", config_->VertexColor());
    if (config_->VertexType() == CIRCLE) {
      glEnable(GL_MULTISAMPLE);
    } else {
      glDisable(GL_MULTISAMPLE);
    }

    for (const auto &face : faces_) {
      program_->setUniformValue("color", config_->VertexColor());
      // SetLightSettings(face, config_->VertexColor());
      face.RenderPoints();
    }
    if (config_->VertexType() == CIRCLE) glDisable(GL_MULTISAMPLE);
  }

  void Render(QOpenGLShaderProgram *shader_program) {
    if (shader_program == nullptr) return;
    program_ = shader_program;

    if (vertex_count_ == 0) return;

    program_->bind();

    PreparePositionInfo();
    PrepareLightInfo();
    PrepareTextureInfo();

    if (PrepareMaterialInfo()) {
      RenderByMaterial();
    } else {
      RenderAll();
    }

    FinishRendering();

    program_->release();
  }

  void RenderNormalVectors(QOpenGLShaderProgram *shader_program) {
    if (shader_program == nullptr) return;
    program_ = shader_program;
    if (vertex_count_ == 0) return;
    program_->bind();
    program_->setUniformValue("view_matrix", config_->ViewMatrix());
    program_->setUniformValue("projection_matrix", config_->ProjectionMatrix());
    program_->setUniformValue("scale", config_->NormalsScale());
    program_->setUniformValue("model_matrix", config_->ModelMatrix());
    program_->setUniformValue("color", config_->NormalColor());
    glLineWidth(config_->NormalWidth());
    glDrawArrays(GL_LINES, 0, vertices_.size());
    program_->release();
  }
  void RenderGroup(QVector<int>::const_iterator start,
                   QVector<int>::const_iterator end) {  // vertices
    if (config_->FaceType()) {
      RenderFaces(start, end);
    }

    if (config_->EdgeType()) {
      RenderEdges(start, end);
    }

    // vertices
    if (config_->VertexType()) {
      RenderPoints(start, end);
    }
  }

  void RenderAll() {
    if (config_->FaceType()) {
      RenderFaces();
    }

    if (config_->EdgeType()) {
      RenderEdges();
    }

    if (config_->VertexType()) {
      RenderPoints();
    }
  }

  // void SetLightSettings(const Face &cur_face, QVector3D base_color)
  // {
  //     if (config_->MaterialMode() && !materials_map_.empty()) {
  //         const auto &cur_m = materials_map_[cur_face.MaterialName()];

  //         if (cur_m.has_textures) {
  //             cur_m.diffuse_sampler->bind();
  //             glActiveTexture(GL_TEXTURE0);
  //         }

  //     } else {
  //         program_->setUniformValue("color", base_color);
  //     }
  // }

  void SendMaterialComponentsToShaders(QVector3D diffuse, QVector3D ambient,
                                       QVector3D specular, float shininess) {
    program_->setUniformValue("material.diffuse_color", diffuse);
    program_->setUniformValue("material.ambient_color", ambient);
    program_->setUniformValue("material.specular_color", specular);
    program_->setUniformValue("material.shininess", shininess);
  }

  void LoadMaterials() {
    const auto &materials = config_->GetMaterialLib(mtllib_);
    if (!materials.empty()) {
      for (const auto &material : materials) {
        materials_map_[material.name] = material;
      }
      int i = 0;
      for (auto &face : faces_) {
        auto &name = face.MaterialName();
        if (name == "") {
          faces_with_one_material_["empty"] += i++;
        } else {
          faces_with_one_material_[name] += i++;
        }
      }
    }
  }

  void SendMaterialDataToShaders(const Material &material) {
    const auto &[diffuse, ambient, specular, shininess] = material.components;
    SendMaterialComponentsToShaders(diffuse, ambient, specular, shininess);
    program_->setUniformValue("material.dissolve", material.dissolve);
    if (material.has_diffuse_texture && material.diffuse_sampler) {
      glActiveTexture(GL_TEXTURE0);
      material.diffuse_sampler->bind();
      program_->setUniformValue("material.diffuse_sampler", 0);
      program_->setUniformValue("material.has_diffuse_texture", true);
    } else {
      program_->setUniformValue("material.has_diffuse_texture", false);
    }
    if (material.has_ambient_texture && material.ambient_sampler) {
      glActiveTexture(GL_TEXTURE1);
      material.ambient_sampler->bind();
      program_->setUniformValue("material.ambient_sampler", 1);
      program_->setUniformValue("material.has_ambient_texture", true);
    } else {
      program_->setUniformValue("material.has_ambient_texture", false);
    }
    if (material.has_specular_texture && material.specular_sampler) {
      glActiveTexture(GL_TEXTURE2);
      material.specular_sampler->bind();
      program_->setUniformValue("material.specular_sampler", 2);
      program_->setUniformValue("material.has_specular_texture", true);
    } else {
      program_->setUniformValue("material.has_specular_texture", false);
    }
    if (material.has_bump_texture && material.bump_sampler) {
      glActiveTexture(GL_TEXTURE3);
      material.bump_sampler->bind();
      program_->setUniformValue("material.bump_multiplier",
                                material.bump_multiplier);
      program_->setUniformValue("material.bump_sampler", 3);
      program_->setUniformValue("material.has_bump_texture", true);
    } else {
      program_->setUniformValue("material.has_bump_texture", false);
    }
  }

  bool PrepareMaterialInfo() {
    if (materials_map_.empty()) {
      LoadMaterials();
    }
    if (config_->MaterialMode() && !materials_map_.empty()) {
      program_->setUniformValue("material_mode", true);
      return true;
    } else {
      program_->setUniformValue("material_mode", false);
    }
    return false;
  }

  void PrepareTextureInfo() {
    if (config_->TextureMode() && config_->Texture()) {
      glActiveTexture(GL_TEXTURE4);
      config_->Texture()->bind();
      program_->setUniformValue("textureSampler", 4);
      program_->setUniformValue("texture_mode", config_->TextureMode());
    } else {
      program_->setUniformValue("texture_mode", false);
    }
  }

  void PrepareLightInfo() {
    if (config_->LightMode() || config_->NeonMode()) {
      bool neon_mode = config_->NeonMode();
      program_->setUniformValue("neon_mode", neon_mode);
      QVector3D view_pos = config_->CameraPosition();

      const auto &[position, light_c, ambient_i, diffuse_i, specular_i] =
          config_->Light();

      program_->setUniformValue("light_pos", position);
      program_->setUniformValue("light.light_color", light_c);
      program_->setUniformValue("light.ambient_intensity", ambient_i);
      program_->setUniformValue("light.diffuse_intensity", diffuse_i);
      program_->setUniformValue("light.specular_intensity", specular_i);
      program_->setUniformValue("view_pos", view_pos);
    }
  }

  void PreparePositionInfo() {
    program_->setUniformValue("view_matrix", config_->ViewMatrix());
    program_->setUniformValue("projection_matrix", config_->ProjectionMatrix());
    program_->setUniformValue("model_matrix", config_->ModelMatrix());
  }

  void SetSettings(const Config *config) { config_ = config; }
  void SetMaterialLib(const QString &name) { mtllib_ = name; }
  const std::vector<QVector4D> &Vertices() const { return vertices_; }
  const std::vector<QVector3D> &Normals() const { return normals_; }
  const std::vector<QVector2D> &TextureCoords() const {
    return texture_coords_;
  }
  int VertexCount() const { return vertex_count_; }
  int FaceCount() const { return face_count_; }
  double ModelSize() const { return size_; }
  int EdgeCount() const {
    QSet<QPair<int, int>> unique_edges;
    for (const auto &face : faces_) {
      int vertex_count = face.VertexCount();
      if (vertex_count > 1) {
        auto vertex_indices = face.VertexIndices();
        for (int i = 0; i < vertex_count; ++i) {
          int v1 = vertex_indices[i];
          int v2 = vertex_indices[(i + 1) % vertex_count];
          if (v1 > v2) std::swap(v1, v2);
          unique_edges.insert(qMakePair(v1, v2));
        }
      }
    }
    return unique_edges.size();
  }
  const QVector<Face> &Faces() const { return faces_; }
  const QString &MaterialLibrary() const { return mtllib_; }

 private:
  double size_;
  unsigned int vertex_count_;
  unsigned int face_count_;
  QVector<Face> faces_;
  QMap<QString, QVector<int>> faces_with_one_material_;
  std::vector<QVector4D> vertices_;
  const Config *config_;
  QOpenGLShaderProgram *program_;
  std::vector<QVector2D> texture_coords_;
  std::vector<QVector3D> normals_;
  QString mtllib_;
  QMap<QString, Material> materials_map_;
};

#endif  // MESHMODEL_H
