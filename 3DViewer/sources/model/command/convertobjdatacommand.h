#ifndef CONVERTOBJDATACOMMAND_H
#define CONVERTOBJDATACOMMAND_H

#include <QVector3D>
#include <vector>

#include "command/command.h"
#include "model/scenemodel.h"
#include "qfileinfo.h"
#include "shared/renderer/raw_data/obj_data.h"

class ConvertOBJDataCommand : public Command {
 public:
  explicit ConvertOBJDataCommand(SceneModel *scene_model)
      : scene_model_(scene_model) {}

  bool execute() override {
    data_ = scene_model_->getRawData();
    LoadDataFromModel();
    CreateMeshModel();
    return true;
  }

  double dcos(float angle_in_degrees) {
    return std::cos(angle_in_degrees * M_PI / 180.0);
  }

  QVector3D ComputeNormal(const QVector3D &v1, const QVector3D &v2,
                          const QVector3D &v3) {
    QVector3D edge1 = v2 - v1;
    QVector3D edge2 = v3 - v1;
    return QVector3D::crossProduct(edge1, edge2).normalized();
  }

  // for smooth faces [use with treshold = dcos(30)]
  std::vector<QVector3D> GenVertexNormals() {
    std::vector<QVector3D> normals(data_.Vertices().size(), {0, 0, 0});

    for (const auto &face : data_.Faces()) {
      size_t v_count = face.VertexCount();
      const auto &v_indices = face.VertexIndices();
      // Skip faces with less than 3 vertices
      if (v_count < 3) {
        QVector3D default_normal = {0.0f, 0.0f, 1.0f};
        for (size_t idx : v_indices) {
          normals[idx] = default_normal;
        }
        continue;
      }

      // Compute normals for each triangle formed by the face's vertices
      for (size_t i = 1; i < v_count - 1; ++i) {
        auto v_idx0 = v_indices[0];
        auto v_idx1 = v_indices[i];
        auto v_idx2 = v_indices[i + 1];
        QVector3D norm =
            ComputeNormal(data_.GetVertex(v_idx0), data_.GetVertex(v_idx1),
                          data_.GetVertex(v_idx2));

        // Add the computed normal to each vertex of the current triangle
        normals[v_idx0] += norm;
        normals[v_idx1] += norm;
        normals[v_idx2] += norm;
      }
    }
    return normals;
  }

  std::vector<QVector3D> GenFaceNormals() {
    std::vector<QVector3D> face_normals;

    for (const auto &face : data_.Faces()) {
      size_t v_count = face.VertexCount();
      const auto &v_indices = face.VertexIndices();
      // Skip faces with less than 3 vertices
      if (v_count < 3) {
        QVector3D default_normal = {0.0f, 0.0f, 1.0f};
        face_normals.push_back(default_normal);
        continue;
      }

      // Compute normals for each triangle formed by the face's vertices

      auto v_idx0 = v_indices[0];
      auto v_idx1 = v_indices[1];
      auto v_idx2 = v_indices[2];
      QVector3D norm =
          ComputeNormal(data_.GetVertex(v_idx0), data_.GetVertex(v_idx1),
                        data_.GetVertex(v_idx2));
      // Add the computed normal to each vertex of the current triangle
      face_normals.push_back(norm);
    }

    return face_normals;
  }
  // Compute normals for each triangle face
  std::vector<QVector3D> ComputeTriangleNormals(
      const std::vector<QVector3D> &vertices,
      const std::vector<unsigned int> &indices) {
    std::vector<QVector3D> normals;

    for (size_t i = 0; i < indices.size(); i += 3) {
      QVector3D normal = ComputeNormal(vertices[0], vertices[1], vertices[2]);

      normals.push_back(normal);
      normals.push_back(normal);
      normals.push_back(normal);
    }

    return normals;
  }

  double GetModelSize() {
    const auto &min = data_.GetMin();
    const auto &max = data_.GetMax();
    double model_depth = fabs(min.z() - max.z());
    double model_width = fabs(min.x() - max.x());
    double model_height = fabs(min.y() - max.y());
    return std::max<double>({model_width, model_height, model_depth});
  }

  void CreateMeshModel() {
    int count_vertices = data_.CountVertices();
    int count_faces = data_.CountFaces();
    double model_size = GetModelSize();
    ConvertData();

    MeshModel mesh_model(data_.Faces(), normal_vectors_, count_vertices,
                         count_faces, model_size);
    mesh_model.SetNormals(normals_);
    mesh_model.SetTextureCoords(texture_coords_);

    QFileInfo lib_name(data_.MaterialsLib());
    mesh_model.SetMaterialLib(lib_name.fileName());
    scene_model_->AddMeshModel(mesh_model);
  }

  void ConvertData() {
    auto vertex_it = vertices_.begin();
    auto normal_it = normals_.begin();
    for (; normal_it != normals_.end() && vertex_it != vertices_.end();
         normal_it++, vertex_it++) {
      auto vertex = *vertex_it;
      auto normal = *normal_it;
      normal_vectors_.emplace_back(normal, 1.0);
      normal_vectors_.emplace_back(vertex, 0.0);
    }
  }

  void LoadDataFromModel() {
    for (const auto &_face : data_.Faces()) {
      // vertices from the model
      AddFaceVertices(_face);

      // normals from the model
      if (!data_.Normals().empty()) {
        AddFaceNormals(_face);
      }

      // texture coords from the model
      if (!data_.TextureCoords().empty()) {
        AddFaceTextureCoords(_face);
      }
    }

    // if no normals in obj file
    if (normals_.empty()) {
      CalculateNormals();
    }

    // if no uv in obj file
    if (texture_coords_.empty()) {
      CalculateTextureCoords();
    }
  }

  void CalculateNormals(bool smooth = default_smooth_normals) {
    if (smooth) {
      CalculateVertexNormals();
    } else {
      CalculateFaceNormals();
    }
  }

  void CalculateVertexNormals() {
    // each face vertex has unique normal
    const auto &vertices_normals = GenVertexNormals();
    for (const auto &_face : data_.Faces()) {
      for (const auto &vertex_index : _face.VertexIndices()) {
        normals_.push_back(vertices_normals[vertex_index]);
      }
    }
  }

  void CalculateFaceNormals() {
    // each face vertex has the same normal
    const auto &face_normals = GenFaceNormals();
    size_t i = 0;
    for (const auto &_face : data_.Faces()) {
      for (size_t k = 0; k < _face.VertexCount(); k++) {
        normals_.push_back(face_normals[i]);
      }
      i++;
    }
  }

  void CalculateTextureCoords() {
    const auto &min = data_.GetMin().toVector2D();
    const auto &max = data_.GetMax().toVector2D();
    QVector2D range = max - min;
    for (const auto &vertex : vertices_) {
      CalculateUVFromVertex(vertex, min, range);
    }
  }

  void CalculateUVFromVertex(const QVector3D &vertex, const QVector2D &min,
                             const QVector2D &range) {
    QVector2D uv = (QVector2D(vertex.x(), vertex.y()) - min) / range;
    uv.setY(1.0 - uv.y());  // Flip Y-coordinate
    texture_coords_.push_back(uv);
  }

  void AddFaceTextureCoords(const Face &face) {
    for (const auto &texture_coords_index : face.TextureCoordsIndices()) {
      const auto &uv = data_.GetTextureCoords(texture_coords_index);
      texture_coords_.emplace_back(uv);
    }
    if (face.TextureCoordsIndices().size() < face.VertexCount()) {
      const auto &min = data_.GetMin().toVector2D();
      const auto &max = data_.GetMax().toVector2D();
      QVector2D range = max - min;
      int num = face.VertexCount() - face.TextureCoordsIndices().size();
      for (int i = 0; i < num; i++) {
        CalculateUVFromVertex(data_.GetVertex(i), min, range);
      }
    }
  }

  void AddFaceNormals(const Face &face) {
    for (const auto &normal_index : face.NormalIndices()) {
      const auto &normal = data_.GetNormal(normal_index);
      normals_.emplace_back(normal);
    }
  }

  void AddFaceVertices(const Face &face) {
    for (const auto &vertex_index : face.VertexIndices()) {
      const auto &vertex = data_.GetVertex(vertex_index);
      vertices_.emplace_back(vertex);
    }
  }

 private:
  SceneModel *scene_model_;
  OBJ_Data data_;
  std::vector<QVector3D> vertices_;
  std::vector<QVector3D> normals_;
  std::vector<QVector2D> texture_coords_;
  std::vector<QVector4D> normal_vectors_;
};

#endif  // CONVERTOBJDATACOMMAND_H
