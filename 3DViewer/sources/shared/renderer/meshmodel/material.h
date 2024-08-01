#ifndef MATERIAL_H
#define MATERIAL_H

#include "qimage.h"
#include "qopengltexture.h"
#include "qvectornd.h"
typedef struct MaterialComponents {
  QVector3D diffuse_color = QVector3D(0.8, 0.8, 0.8);
  QVector3D ambient_color = QVector3D(0.2, 0.2, 0.2);
  QVector3D specular_color = QVector3D(1.0, 1.0, 1.0);
  float shininess_exponent = 16.0f;
} MaterialComponents;

typedef struct Material {
  QString name = "empty";
  QImage map_kd;
  QImage map_ka;
  QImage map_ks;
  QImage map_bump;
  float dissolve = 1.0;
  float bump_multiplier = 1.0;
  bool has_diffuse_texture = false;
  bool has_ambient_texture = false;
  bool has_specular_texture = false;
  bool has_bump_texture = false;
  MaterialComponents components;
  QOpenGLTexture *diffuse_sampler = nullptr;
  QOpenGLTexture *ambient_sampler = nullptr;
  QOpenGLTexture *specular_sampler = nullptr;
  QOpenGLTexture *bump_sampler = nullptr;
  bool use_base_color = true;

  const MaterialComponents &GetMaterialComponents() const { return components; }
} Material;

#endif  // MATERIAL_H
