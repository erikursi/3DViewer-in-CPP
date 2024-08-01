#ifndef OBJ_DATA_H
#define OBJ_DATA_H

#include <QMap>
#include <QString>
#include <QVector>

#include "qvectornd.h"
#include "shared/renderer/meshmodel/face.h"

class OBJ_Data {
 public:
  OBJ_Data() {}
  void AddTextureCoords(const QVector2D &uv) {
    texture_coords_.emplace_back(uv);
  }
  void AddVertex(const QVector3D &vertex) {
    if (vertices_.empty()) min_ = max_ = vertex;

    vertices_.push_back(vertex);
    const auto &[x, y, z] = vertex;

    min_.setX(std::min(x, min_.x()));
    min_.setY(std::min(y, min_.y()));
    min_.setZ(std::min(z, min_.z()));
    max_.setX(std::max(x, max_.x()));
    max_.setY(std::max(y, max_.y()));
    max_.setZ(std::max(z, max_.z()));
  }

  void AddMaterialLib(const QString &mtllib) { mtllib_ = mtllib; }
  void AddNormal(const QVector3D &normal) { normals_.push_back(normal); }

  void Clear() {
    mtllib_.clear();
    faces_.clear();
    vertices_.clear();
    normals_.clear();
    texture_coords_.clear();
  }

  void AddFace(const Face &face) { faces_.push_back(face); }

  QVector3D GetMax() const { return max_; }
  QVector3D GetMin() const { return min_; }
  QVector3D GetCenter() const { return (max_ + min_) / 2; }
  QVector3D GetNormal(unsigned int index) const { return normals_[index]; }
  QVector3D GetVertex(unsigned int index) const { return vertices_[index]; }
  QVector2D GetTextureCoords(unsigned int index) const {
    return texture_coords_[index];
  }
  void CenterVertices() {
    QVector3D center = GetCenter();
    std::transform(vertices_.cbegin(), vertices_.cend(), vertices_.begin(),
                   [center](QVector3D v) { return v - center; });
    // for (auto &vertex : vertices_) {
    //   vertex -= center;
    // }
  }

  void SetMaterialsLib(const QString &mtllib) { mtllib_ = mtllib; }
  const QVector<Face> &Faces() const { return faces_; }
  const QVector<QVector3D> &Vertices() const { return vertices_; }
  const QVector<QVector3D> &Normals() const { return normals_; }
  const QVector<QVector2D> &TextureCoords() const { return texture_coords_; }
  const QString &MaterialsLib() const { return mtllib_; }
  int CountFaces() const { return faces_.size(); }
  int CountVertices() const { return vertices_.size(); }

 private:
  QVector3D max_;
  QVector3D min_;
  QVector<QVector3D> vertices_;
  QVector<QVector3D> normals_;
  QVector<QVector2D> texture_coords_;
  QVector<Face> faces_;
  QString mtllib_;
};

#endif  // OBJ_DATA_H
