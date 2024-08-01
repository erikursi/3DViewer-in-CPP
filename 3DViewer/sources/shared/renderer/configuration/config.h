#ifndef CONFIG_H
#define CONFIG_H

#include <GL/gl.h>

#include <QObject>

#include "consts.h"
#include "qcolor.h"
#include "qmatrix4x4.h"
#include "qopengltexture.h"
#include "qvectornd.h"
#include "shared/renderer/meshmodel/face.h"
#include "viewsettings.h"

// #define MATERIALSON 2

typedef struct light_settings {
  QVector3D position = {0.0f, 1.0f, 0.0f};
  QVector3D light_color = {1.0f, 1.0f, 1.0f};
  float ambient_intensity = 1.0;
  float diffuse_intensity = 1.0;
  float specular_intensity = 1.0;
} l_settings;

typedef struct scene_settings {
  QColor bg_color = default_bg_color;
  QVector3D camera_position = default_camera_position;
  QMatrix4x4 model_matrix = {};
  QMatrix4x4 view_matrix = {};
  QMatrix4x4 projection_matrix = {};
  float normals_scale = default_normals_scale;
  l_settings light_settings = {};
} s_settings;

typedef struct vertex_settings {
  int type = default_vertex_type;
  double size = default_vertex_size;
  QVector3D color = {default_vertex_color.redF(), default_vertex_color.greenF(),
                     default_vertex_color.blueF()};
} v_settings;

typedef struct normal_settings {
  double width = default_normal_width;
  QVector3D color = {default_normal_color.redF(), default_normal_color.greenF(),
                     default_normal_color.blueF()};
} n_settings;

typedef struct edge_settings {
  int type = default_edge_type;
  double width = default_edge_width;
  QVector3D color = {default_edge_color.redF(), default_edge_color.greenF(),
                     default_edge_color.blueF()};
} e_settings;

typedef struct face_settings {
  int type = default_face_type;
  QVector3D color = {default_face_color.redF(), default_face_color.greenF(),
                     default_face_color.blueF()};
} f_settings;

typedef struct drawing_settings {
  int light_mode = default_light_mode;
  int texture_mode = default_texture_mode;
  int material_mode = default_materials_mode;
  int normals_mode = default_normals_mode;
  int neon_mode = default_neon_mode;
  QMap<QString, QVector<Material>> materials;
  QMap<QString, QVector<QOpenGLTexture *>> material_textures;
  QOpenGLTexture *texture = nullptr;
  v_settings vertex_settings = {};
  e_settings edge_settings = {};
  f_settings face_settings = {};
  n_settings normal_settings = {};
} d_settings;

class Config {
 public:
  Config() : drawing_settings_{}, scene_settings_{} {}

  Config(const ViewSettings &view_settings, const QMatrix4x4 &model_matrix,
         float normals_scale, float ambient, float diffuse, float specular)
      : drawing_settings_{}, scene_settings_{} {
    ConfigureDrawingSettings(view_settings);
    ConfigureSceneSettings(view_settings, model_matrix, normals_scale, ambient,
                           diffuse, specular);
  }
  ~Config() {}

 private:
  void ConfigureSceneSettings(const ViewSettings &view_settings,
                              const QMatrix4x4 &model_matrix,
                              float normals_scale, float ambient, float diffuse,
                              float specular) {
    SetBgColor(view_settings.window.lables.lbl_bg_color);
    SetLightColor(view_settings.window.lables.lbl_light_color);
    SetLightMode(view_settings.window.checkboxes.is_light_mode);
    SetTextureMode(view_settings.window.checkboxes.is_texture_mode);
    SetMaterialMode(view_settings.window.checkboxes.is_materials_mode);
    SetNormalsMode(view_settings.window.checkboxes.is_normals_mode);
    SetNeonMode(view_settings.window.checkboxes.is_neon_mode);
    SetAmbientIntensity(ambient);
    SetDiffuseIntensity(diffuse);
    SetSpecularIntensity(specular);
    SetModelMatrix(model_matrix);
    SetNormalsScale(normals_scale);
  }

  void ConfigureDrawingSettings(const ViewSettings &view_settings) {
    int vertex_type =
        view_settings.window.radiobuttons.is_circle_vertex   ? CIRCLE
        : view_settings.window.radiobuttons.is_square_vertex ? SQUARE
                                                             : OFF;
    SetVertexType(vertex_type);
    SetVertexColor(view_settings.window.lables.lbl_vertex_color);
    SetVertexSize(view_settings.window.spinboxes.point_size);

    int edge_type = view_settings.window.radiobuttons.is_solid_edge    ? SOLID
                    : view_settings.window.radiobuttons.is_dashed_edge ? DASHED
                                                                       : OFF;
    SetEdgeType(edge_type);
    SetEdgeColor(view_settings.window.lables.lbl_edge_color);
    SetEdgeWidth(view_settings.window.spinboxes.edge_width);
    int face_type =
        view_settings.window.radiobuttons.is_display_face ? ON : OFF;
    SetFaceType(face_type);
    SetFaceColor(view_settings.window.lables.lbl_face_color);

    SetNormalColor(view_settings.window.lables.lbl_normal_color);
  }

 public:
  // setters
  // drawing settings

  void SetBgColor(QColor color) { scene_settings_.bg_color = color; }
  void SetVertexColor(QColor color) {
    drawing_settings_.vertex_settings.color = {color.redF(), color.greenF(),
                                               color.blueF()};
  }
  void SetEdgeColor(QColor color) {
    drawing_settings_.edge_settings.color = {color.redF(), color.greenF(),
                                             color.blueF()};
  }
  void SetFaceColor(QColor color) {
    drawing_settings_.face_settings.color = {color.redF(), color.greenF(),
                                             color.blueF()};
  }
  void SetNormalColor(QColor color) {
    drawing_settings_.normal_settings.color = {color.redF(), color.greenF(),
                                               color.blueF()};
  }

  void SetLightColor(QColor color) {
    scene_settings_.light_settings.light_color = {color.redF(), color.greenF(),
                                                  color.blueF()};
  }
  void SetEdgeWidth(unsigned int width) {
    drawing_settings_.edge_settings.width = width;
  }
  void SetVertexSize(unsigned int size) {
    drawing_settings_.vertex_settings.size = size;
  }

  void SetVertexType(int type) {
    drawing_settings_.vertex_settings.type = type;
  }
  void SetEdgeType(int type) { drawing_settings_.edge_settings.type = type; }
  void SetFaceType(int type) { drawing_settings_.face_settings.type = type; }
  void SetLightMode(int mode) { drawing_settings_.light_mode = mode; }
  void SetNeonMode(int mode) { drawing_settings_.neon_mode = mode; }
  void SetNormalsMode(int mode) { drawing_settings_.normals_mode = mode; }
  void SetTextureMode(int mode) { drawing_settings_.texture_mode = mode; }
  void SetMaterialMode(int mode) { drawing_settings_.material_mode = mode; }

  // scene settings

  void SetLightPosition(QVector3D position) {
    scene_settings_.light_settings.position = position;
  }
  void SetProjectionMatrix(const QMatrix4x4 &matrix) {
    scene_settings_.projection_matrix = matrix;
  }
  void SetViewMatrix(const QMatrix4x4 &matrix) {
    scene_settings_.view_matrix = matrix;
  }
  void SetCameraPosition(const QVector3D &camera_position) {
    scene_settings_.camera_position = camera_position;
  }
  void SetModelMatrix(const QMatrix4x4 &matrix) {
    scene_settings_.model_matrix = matrix;
  }
  void SetTexture(QOpenGLTexture *texture) {
    drawing_settings_.texture = texture;
  }

  void SetAmbientLight(double value) {
    scene_settings_.light_settings.ambient_intensity = value;
  }
  void SetDiffuseLight(double value) {
    scene_settings_.light_settings.diffuse_intensity = value;
  }
  void SetSpecularLight(double value) {
    scene_settings_.light_settings.specular_intensity = value;
  }

  void SetNormalsScale(float scale) { scene_settings_.normals_scale = scale; }

  void SetAmbientIntensity(float intensity) {
    scene_settings_.light_settings.ambient_intensity = intensity;
  }
  void SetDiffuseIntensity(float intensity) {
    scene_settings_.light_settings.diffuse_intensity = intensity;
  }
  void SetSpecularIntensity(float intensity) {
    scene_settings_.light_settings.specular_intensity = intensity;
  }

  void AddMaterialLib(const QString &name, const QVector<Material> &materials) {
    drawing_settings_.materials[name] = materials;
  }

  void AddMaterialTexture(const QString &name, QOpenGLTexture *texture) {
    drawing_settings_.material_textures[name].push_back(texture);
  }

  // getters
  // drawing settings
  QColor BgColor() const { return scene_settings_.bg_color; }
  QVector3D VertexColor() const {
    return drawing_settings_.vertex_settings.color;
  }
  QVector3D EdgeColor() const { return drawing_settings_.edge_settings.color; }
  QVector3D FaceColor() const { return drawing_settings_.face_settings.color; }
  QVector3D NormalColor() const {
    return drawing_settings_.normal_settings.color;
  }
  unsigned int EdgeWidth() const {
    return drawing_settings_.edge_settings.width;
  }
  unsigned int VertexSize() const {
    return drawing_settings_.vertex_settings.size;
  }
  unsigned int NormalWidth() const {
    return drawing_settings_.normal_settings.width;
  }
  int VertexType() const { return drawing_settings_.vertex_settings.type; }
  int EdgeType() const { return drawing_settings_.edge_settings.type; }
  int FaceType() const { return drawing_settings_.face_settings.type; }
  QOpenGLTexture *Texture() const { return drawing_settings_.texture; }
  QOpenGLTexture *MaterialTexture() const { return drawing_settings_.texture; }
  int TextureMode() const { return drawing_settings_.texture_mode; }
  int MaterialMode() const { return drawing_settings_.material_mode; }
  int LightMode() const { return drawing_settings_.light_mode; }
  int NormalsMode() const { return drawing_settings_.normals_mode; }
  int NeonMode() const { return drawing_settings_.neon_mode; }

  QVector<QOpenGLTexture *> GetMaterialTextures(const QString &name) const {
    QVector<QOpenGLTexture *> material_textures;
    if (drawing_settings_.material_textures.find(name) !=
        drawing_settings_.material_textures.end()) {
      return material_textures;
    } else {
      return drawing_settings_.material_textures[name];
    }
  }

  QVector<Material> GetMaterialLib(const QString &mtllib_name) const {
    QVector<Material> materials;
    if (drawing_settings_.materials.find(mtllib_name) ==
        drawing_settings_.materials.end()) {
      return materials;
    } else {
      return drawing_settings_.materials[mtllib_name];
    }
  }

  // scene settings
  const QMatrix4x4 &ProjectionMatrix() const {
    return scene_settings_.projection_matrix;
  }
  const QMatrix4x4 &ViewMatrix() const { return scene_settings_.view_matrix; }
  const QMatrix4x4 &ModelMatrix() const { return scene_settings_.model_matrix; }
  QVector3D CameraPosition() const { return scene_settings_.camera_position; }
  light_settings Light() const { return scene_settings_.light_settings; }
  float NormalsScale() const { return scene_settings_.normals_scale; }

 private:
  d_settings drawing_settings_;
  s_settings scene_settings_;

 signals:
};

#endif  // CONFIG_H
