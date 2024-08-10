#ifndef SCENEMODEL_H
#define SCENEMODEL_H
#include <QApplication>
#include <QObject>
#include <QOpenGLTexture>

#include "qimage.h"
#include "qsettings.h"
#include "renderer/raw_data/obj_data.h"
#include "shared/renderer/configuration/config.h"
#include "shared/renderer/meshmodel/meshmodel.h"
#include "viewsettings.h"

#define COLORS "colors"
#define TYPES "types"
#define INFO "info"
#define MODES "modes"
#define POSITIONS "positions"
#define MAPPED_VALUES "mappedValues"
#define SIZES "sizes"
#define FILES "files"
#define DIRECTORIES "directories"

class SceneModel : public QObject {
  Q_OBJECT
 public:
  SceneModel(const QString &settings_path = settings_file_name,
             QObject *parent = nullptr)
      : QObject(parent),
        scale_(default_scale),
        normals_scale_(default_normals_scale),
        ambient_intensity_(default_intensity),
        diffuse_intensity_(default_intensity),
        specular_intensity_(default_intensity),
        projection_type_(default_projection_type),
        file_path_(default_file_path),
        texture_path_(default_file_path),
        material_path_(default_file_path),
        settings_path_(settings_path),
        load_dir_(default_load_dir),
        save_dir_(default_save_dir),
        config_(nullptr),
        view_settings_({}),
        aspect_ratio_(default_widget_size.x() / default_widget_size.y())

  {
    RestoreSettings();
    auto model_matrix = translation_matrix_ * rotation_matrix_ * scale_matrix_;
    config_ =
        new Config(view_settings_, model_matrix, normals_scale_,
                   ambient_intensity_, diffuse_intensity_, specular_intensity_);
    SetProjectionMatrix();
  }

  ~SceneModel() {
    DeleteTexture(config_->Texture());

    for (auto &texture : created_textures_) {
      DeleteTexture(texture);
    }

    delete config_;
  }

  void DeleteTexture(QOpenGLTexture *texture) {
    if (texture) {
      if (texture->isCreated()) texture->destroy();
      delete texture;
    }
  }

  void SetTexture(QOpenGLTexture *texture) {
    DeleteTexture(config_->Texture());
    config_->SetTexture(texture);
  }

  void PrepareTexture(QOpenGLTexture *texture, const QImage &image) {
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
    texture->setData(image.mirrored());
  }

  void SetTexturePath(const QString &texture_path) {
    texture_path_ = texture_path;
    if (texture_path.isEmpty()) {
      texture_path_ = default_file_path;
    }
  }

  void SetMaterialPath(const QString &material_path) {
    material_path_ = material_path;
    if (material_path.isEmpty()) {
      material_path_ = default_file_path;
    }
  }
  void SetFilePath(const QString &str) {
    file_path_ = str.isEmpty() ? default_file_path : str;
  }
  const QString &TexturePath() { return texture_path_; }
  const QString &FilePath() { return file_path_; }
  const QString &MaterialPath() { return material_path_; }
  QString FileName() {
    QFileInfo file_info(file_path_);
    return file_info.fileName();
  }

  Config *GetConfig() const { return config_; }
  void SetVertexColor(QColor color) {
    config_->SetVertexColor(color);
    view_settings_.window.lables.lbl_vertex_color = color;
  }
  void SetLightColor(QColor color) {
    config_->SetLightColor(color);
    view_settings_.window.lables.lbl_light_color = color;
  }
  void SetEdgeColor(QColor color) {
    config_->SetEdgeColor(color);
    view_settings_.window.lables.lbl_edge_color = color;
  }
  void SetFaceColor(QColor color) {
    config_->SetFaceColor(color);
    view_settings_.window.lables.lbl_face_color = color;
  }
  void SetBgColor(QColor color) {
    config_->SetBgColor(color);
    view_settings_.window.lables.lbl_bg_color = color;
  }

  void SetNormalColor(QColor color) {
    config_->SetNormalColor(color);
    view_settings_.window.lables.lbl_normal_color = color;
  }
  void SetEdgeWidth(unsigned int width) {
    config_->SetEdgeWidth(width);
    view_settings_.window.spinboxes.edge_width = width;
  }
  void SetVertexSize(unsigned int size) {
    config_->SetVertexSize(size);
    view_settings_.window.spinboxes.point_size = size;
  }
  void SetVertexType(int type) {
    config_->SetVertexType(type);
    switch (type) {
      case CIRCLE:
        view_settings_.window.radiobuttons.is_circle_vertex = true;
        view_settings_.window.radiobuttons.is_square_vertex = false;
        view_settings_.window.radiobuttons.is_hide_vertex = false;
        break;
      case SQUARE:
        view_settings_.window.radiobuttons.is_circle_vertex = false;
        view_settings_.window.radiobuttons.is_square_vertex = true;
        view_settings_.window.radiobuttons.is_hide_vertex = false;
        break;
      default:
        view_settings_.window.radiobuttons.is_circle_vertex = false;
        view_settings_.window.radiobuttons.is_square_vertex = false;
        view_settings_.window.radiobuttons.is_hide_vertex = true;
        break;
    }
  }
  void SetEdgeType(int type) {
    config_->SetEdgeType(type);
    switch (type) {
      case DASHED:
        view_settings_.window.radiobuttons.is_dashed_edge = true;
        view_settings_.window.radiobuttons.is_solid_edge = false;
        view_settings_.window.radiobuttons.is_hide_edge = false;
        break;
      case SOLID:
        view_settings_.window.radiobuttons.is_dashed_edge = false;
        view_settings_.window.radiobuttons.is_solid_edge = true;
        view_settings_.window.radiobuttons.is_hide_edge = false;
        break;
      default:
        view_settings_.window.radiobuttons.is_dashed_edge = false;
        view_settings_.window.radiobuttons.is_solid_edge = false;
        view_settings_.window.radiobuttons.is_hide_edge = true;
        break;
    }
  }

  void SetFaceType(int type) {
    config_->SetFaceType(type);
    switch (type) {
      case ON:
        view_settings_.window.radiobuttons.is_display_face = true;
        view_settings_.window.radiobuttons.is_hide_face = false;
        break;
      default:
        view_settings_.window.radiobuttons.is_display_face = false;
        view_settings_.window.radiobuttons.is_hide_face = true;
        break;
    }
  }

  void SetProjectionType(int type) {
    projection_type_ = type;
    SetProjectionMatrix();
  }

  void SetStepX(double step) {
    view_settings_.window.sliders.x_slider_pos = step;
    double mapped_step =
        MapValue(step, -500, 500, -mesh_model_.ModelSize() * 1.5,
                 mesh_model_.ModelSize() * 1.5);
    translation_vector_.setX(mapped_step);
    UpdateViewMatrix();
    UpdateTranslationMatrix();
    SetModelMatrix();
  }

  void SetStepY(double step) {
    view_settings_.window.sliders.y_slider_pos = step;
    double mapped_step =
        MapValue(step, -500, 500, -mesh_model_.ModelSize() * 1.5,
                 mesh_model_.ModelSize() * 1.5);
    translation_vector_.setY(mapped_step);
    UpdateViewMatrix();
    UpdateTranslationMatrix();
    SetModelMatrix();
  }

  void SetStepZ(double step) {
    view_settings_.window.sliders.z_slider_pos = step;
    double mapped_step =
        MapValue(step, -500, 500, -mesh_model_.ModelSize() * 1.5,
                 mesh_model_.ModelSize() * 1.5);
    translation_vector_.setZ(mapped_step);
    UpdateTranslationMatrix();
    SetModelMatrix();
  }

  void ResetModelGeometry() {
    scale_ = default_scale;
    UpdateScaleMatrix();
    normals_scale_ = default_normals_scale;
    UpdateNormalsScale();
    ambient_intensity_ = default_intensity;
    config_->SetAmbientIntensity(ambient_intensity_);
    diffuse_intensity_ = default_intensity;
    config_->SetDiffuseIntensity(diffuse_intensity_);
    specular_intensity_ = default_intensity;
    config_->SetSpecularIntensity(specular_intensity_);
    SetTranslationVector({});
    SetRotationVector({});
  }
  void SetDirectories(const QString &load_dir, const QString &save_dir) {
    load_dir_ = load_dir;
    view_settings_.dialog.load_dir = load_dir_;
    save_dir_ = save_dir;
    view_settings_.dialog.save_dir = save_dir_;
  }

  void SetScale(double step) {
    view_settings_.window.sliders.scale_pos = step;
    scale_ = step / default_scale_multiplier;
    UpdateScaleMatrix();
  }

  void SetNormalsScale(double step) {
    view_settings_.window.sliders.normals_scale_pos = step;
    normals_scale_ = step / default_normals_scale_multiplier;
    UpdateNormalsScale();
  }

  void SetAmbientIntensity(double step) {
    view_settings_.window.sliders.ambient_intensity_pos = step;
    ambient_intensity_ = step / default_intensity_multiplier;
    config_->SetAmbientIntensity(ambient_intensity_);
  }

  void SetDiffuseIntensity(double step) {
    view_settings_.window.sliders.diffuse_intensity_pos = step;
    diffuse_intensity_ = step / default_intensity_multiplier;
    config_->SetDiffuseIntensity(diffuse_intensity_);
  }

  void SetSpecularIntensity(double step) {
    view_settings_.window.sliders.specular_intensity_pos = step;
    specular_intensity_ = step / default_intensity_multiplier;
    config_->SetSpecularIntensity(specular_intensity_);
  }

  void SetAngleX(double angle) {
    view_settings_.window.dials.x_dial_pos = angle;
    rotation_vector_.setX(angle);
    UpdateRotationMatrix();
  }
  void SetAngleY(double angle) {
    view_settings_.window.dials.y_dial_pos = angle;

    rotation_vector_.setY(angle);
    UpdateRotationMatrix();
  }
  void SetAngleZ(double angle) {
    view_settings_.window.dials.z_dial_pos = angle;
    rotation_vector_.setZ(angle);
    UpdateRotationMatrix();
  }
  void SetRotationVector(const QVector3D &v) {
    SetAngleX(v.x());
    SetAngleY(v.y());
    SetAngleZ(v.z());
  }
  void SetTranslationVector(const QVector3D &v) {
    SetStepX(v.x());
    SetStepY(v.y());
    SetStepZ(v.z());
  }
  void ConfigureViewMatrix() {
    QVector3D camera_position = QVector3D(0, 0, mesh_model_.ModelSize() * 2);
    config_->SetCameraPosition(camera_position);
    UpdateViewMatrix();
  }

  void SetOrthoProjection(double left, double right, double bottom, double top,
                          double near_plane, double far_plane) {
    ortho_matrix_.setToIdentity();
    ortho_matrix_.ortho(left, right, bottom, top, near_plane, far_plane);
  }

  void SetPerspectiveProjection(double near_plane, double far_plane) {
    double field_of_view = 45.f;
    perspective_matrix_.setToIdentity();
    perspective_matrix_.perspective(field_of_view, aspect_ratio_, near_plane,
                                    far_plane);
  }
  void ConfigureProjectionMatrix() {
    double near_plane = 0.001;
    double far_plane = near_plane + mesh_model_.ModelSize() * 100;

    // Set up view volume
    double box_size = mesh_model_.ModelSize() * 0.6;

    double left = -box_size;
    double right = box_size;
    double bottom = -box_size;
    double top = box_size;

    if (aspect_ratio_ < 1.0) {
      bottom /= aspect_ratio_;
      top /= aspect_ratio_;
    } else {
      left *= aspect_ratio_;
      right *= aspect_ratio_;
    }

    SetOrthoProjection(left, right, bottom, top, near_plane, far_plane);
    SetPerspectiveProjection(near_plane, far_plane);
    SetProjectionMatrix();
  }
  void SetModelMatrix() {
    config_->SetModelMatrix(translation_matrix_ * rotation_matrix_ *
                            scale_matrix_);
  }
  void SetWidgetSizes(float width, float height) {
    aspect_ratio_ = width / height;
    if (mesh_model_.ModelSize() > 0) {
      ConfigureProjectionMatrix();
    }
  }
  void SetProjectionMatrix() {
    switch (projection_type_) {
      case ON:
        view_settings_.window.radiobuttons.is_ortho_projection = false;
        view_settings_.window.radiobuttons.is_perspective_projection = true;
        config_->SetProjectionMatrix(perspective_matrix_);
        break;
      default:
        view_settings_.window.radiobuttons.is_ortho_projection = true;
        view_settings_.window.radiobuttons.is_perspective_projection = false;
        config_->SetProjectionMatrix(ortho_matrix_);
        break;
    }
  }
  void UpdateTranslationMatrix() {
    translation_matrix_.setToIdentity();
    translation_matrix_.translate(translation_vector_);
    SetModelMatrix();
  }
  void UpdateRotationMatrix() {
    rotation_matrix_.setToIdentity();
    const auto &[x, y, z] = rotation_vector_;
    rotation_matrix_.rotate(x, QVector3D(1, 0, 0));
    rotation_matrix_.rotate(y, QVector3D(0, 1, 0));
    rotation_matrix_.rotate(z, QVector3D(0, 0, 1));
    SetModelMatrix();
  }
  void UpdateScaleMatrix() {
    scale_matrix_.setToIdentity();
    scale_matrix_.scale(scale_);
    SetModelMatrix();
  }
  void UpdateViewMatrix() {
    QMatrix4x4 view_matrix;
    view_matrix.setToIdentity();
    view_matrix.lookAt(config_->CameraPosition(), {0, 0, 0}, {0, 1, 0});
    config_->SetViewMatrix(view_matrix);
  }
  void SetLightMode(int mode) {
    config_->SetLightMode(mode);
    view_settings_.window.checkboxes.is_light_mode = mode;
  }
  void SetMaterialMode(int mode) {
    config_->SetMaterialMode(mode);
    view_settings_.window.checkboxes.is_materials_mode = mode;
  }
  void SetNormalsMode(int mode) {
    config_->SetNormalsMode(mode);
    view_settings_.window.checkboxes.is_normals_mode = mode;
  }
  void SetNeonMode(int mode) {
    config_->SetNeonMode(mode);
    view_settings_.window.checkboxes.is_neon_mode = mode;
  }

  void SetTextureMode(int mode) {
    config_->SetTextureMode(mode);
    view_settings_.window.checkboxes.is_texture_mode = mode;
  }

  void SetAmbientLight(double value) { config_->SetAmbientLight(value); }
  void SetDiffuseLight(double value) { config_->SetDiffuseLight(value); }
  void SetSpecularLight(double value) { config_->SetSpecularLight(value); }

  QString LoadDir() { return load_dir_; }
  QString SaveDir() { return save_dir_; }
  const MeshModel &getMeshModel() { return mesh_model_; }
  const OBJ_Data &getRawData() { return raw_data_; }
  void AddRawData(const OBJ_Data &raw_data) { raw_data_ = raw_data; }
  void AddMeshModel(const MeshModel &mesh_model) {
    mesh_model_ = mesh_model;
    ConfigureViewMatrix();
    ConfigureProjectionMatrix();
    ConfigureLight();
    mesh_model_.SetSettings(config_);
  }

  void ConfigureLight() {
    config_->SetLightPosition(QVector3D(0, 0, mesh_model_.ModelSize() * 10));
  }

  float MapValue(float value, float in_min, float in_max, float out_min,
                 float out_max) {
    if (in_min == in_max) {
      return out_min;  // Avoid division by zero
    }
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

  void AddMaterials(const QString &mtllib, const QVector<Material> &materials,
                    bool with_textures) {
    if (!with_textures) {
      config_->AddMaterialLib(mtllib, materials);
      return;
    }
    for (const auto &material : materials) {
      if (material.has_diffuse_texture) {
        created_textures_.push_back(material.diffuse_sampler);
      }
      if (material.has_ambient_texture) {
        created_textures_.push_back(material.ambient_sampler);
      }
      if (material.has_specular_texture) {
        created_textures_.push_back(material.specular_sampler);
      }
      if (material.has_bump_texture) {
        created_textures_.push_back(material.bump_sampler);
      }
    }
    config_->AddMaterialLib(mtllib, materials);
  }
  ViewSettings GetViewSettings() { return view_settings_; }

  void UpdateNormalsScale() { config_->SetNormalsScale(normals_scale_); }

  void RestoreLabels(const QSettings &settings) {
    QString group_name = COLORS "/";
    view_settings_.window.lables.lbl_bg_color = QColor::fromString(
        settings.value(group_name + BG_COLOR, default_bg_color).toString());
    view_settings_.window.lables.lbl_vertex_color = QColor::fromString(
        settings.value(group_name + VERTEX_COLOR, default_vertex_color)
            .toString());
    view_settings_.window.lables.lbl_face_color = QColor::fromString(
        settings.value(group_name + FACE_COLOR, default_face_color).toString());
    view_settings_.window.lables.lbl_edge_color = QColor::fromString(
        settings.value(group_name + EDGE_COLOR, default_edge_color).toString());
    view_settings_.window.lables.lbl_normal_color = QColor::fromString(
        settings.value(group_name + NORMAL_COLOR, default_normal_color)
            .toString());
    view_settings_.window.lables.lbl_light_color = QColor::fromString(
        settings.value(group_name + LIGHT_COLOR, default_light_color)
            .toString());
  }

  void RestoreGeometry(const QSettings &settings) {
    QString group_name = MAPPED_VALUES "/";
    scale_ = settings.value(group_name + SCALE, scale_).toFloat();
    normals_scale_ =
        settings.value(group_name + NORMALS_SCALE, normals_scale_).toFloat();
    ambient_intensity_ =
        settings.value(group_name + AMBIENT_INTENSITY, ambient_intensity_)
            .toFloat();
    diffuse_intensity_ =
        settings.value(group_name + DIFFUSE_INTENSITY, diffuse_intensity_)
            .toFloat();
    specular_intensity_ =
        settings.value(group_name + SPECULAR_INTENSITY, specular_intensity_)
            .toFloat();

    translation_vector_.setX(settings.value(group_name + X_POS, 0).toFloat());
    translation_vector_.setY(settings.value(group_name + Y_POS, 0).toFloat());
    translation_vector_.setZ(settings.value(group_name + Z_POS, 0).toFloat());

    rotation_vector_.setX(settings.value(group_name + X_ANGLE, 0).toFloat());
    rotation_vector_.setY(settings.value(group_name + Y_ANGLE, 0).toFloat());
    rotation_vector_.setZ(settings.value(group_name + Z_ANGLE, 0).toFloat());

    scale_matrix_.setToIdentity();
    scale_matrix_.scale(scale_);
    rotation_matrix_.setToIdentity();
    const auto &[x, y, z] = rotation_vector_;
    rotation_matrix_.rotate(x, QVector3D(1, 0, 0));
    rotation_matrix_.rotate(y, QVector3D(0, 1, 0));
    rotation_matrix_.rotate(z, QVector3D(0, 0, 1));
    translation_matrix_.setToIdentity();
    translation_matrix_.translate(translation_vector_);
  }

  void RestoreSpinboxes(const QSettings &settings) {
    QString group_name = SIZES "/";
    view_settings_.window.spinboxes.edge_width =
        settings
            .value(group_name + EDGE_WIDTH,
                   view_settings_.window.spinboxes.default_edge_width)
            .toInt();
    view_settings_.window.spinboxes.point_size =
        settings
            .value(group_name + POINT_SIZE,
                   view_settings_.window.spinboxes.default_point_size)
            .toInt();
  }

  void RestoreSlidersAndDials(const QSettings &settings) {
    QString group_name = POSITIONS "/";

    view_settings_.window.sliders.scale_pos =
        settings
            .value(group_name + SCALE,
                   view_settings_.window.sliders.default_scale_pos)
            .toInt();
    view_settings_.window.sliders.normals_scale_pos =
        settings
            .value(group_name + NORMALS_SCALE,
                   view_settings_.window.sliders.default_normals_scale_pos)
            .toInt();
    view_settings_.window.sliders.ambient_intensity_pos =
        settings
            .value(group_name + AMBIENT_INTENSITY,
                   view_settings_.window.sliders.default_normals_scale_pos)
            .toInt();

    view_settings_.window.sliders.diffuse_intensity_pos =
        settings
            .value(group_name + DIFFUSE_INTENSITY,
                   view_settings_.window.sliders.default_normals_scale_pos)
            .toInt();
    view_settings_.window.sliders.specular_intensity_pos =
        settings
            .value(group_name + SPECULAR_INTENSITY,
                   view_settings_.window.sliders.default_normals_scale_pos)
            .toInt();

    view_settings_.window.sliders.x_slider_pos =
        settings
            .value(group_name + X_POS,
                   view_settings_.window.sliders.default_translation_pos)
            .toInt();
    view_settings_.window.sliders.y_slider_pos =
        settings
            .value(group_name + Y_POS,
                   view_settings_.window.sliders.default_translation_pos)
            .toInt();
    view_settings_.window.sliders.z_slider_pos =
        settings
            .value(group_name + Z_POS,
                   view_settings_.window.sliders.default_translation_pos)
            .toInt();

    view_settings_.window.dials.x_dial_pos =
        settings
            .value(group_name + X_ANGLE,
                   view_settings_.window.dials.default_rotation_pos)
            .toInt();
    view_settings_.window.dials.y_dial_pos =
        settings
            .value(group_name + Y_ANGLE,
                   view_settings_.window.dials.default_rotation_pos)
            .toInt();
    view_settings_.window.dials.z_dial_pos =
        settings
            .value(group_name + Z_ANGLE,
                   view_settings_.window.dials.default_rotation_pos)
            .toInt();
  }

  void RestoreCheckBoxes(const QSettings &settings) {
    QString group_name = MODES "/";
    view_settings_.window.checkboxes.is_light_mode =
        settings.value(group_name + LIGHT_MODE, default_light_mode).toBool();
    view_settings_.window.checkboxes.is_texture_mode =
        settings.value(group_name + TEXTURE_MODE, default_texture_mode)
            .toBool();
    view_settings_.window.checkboxes.is_materials_mode =
        settings.value(group_name + MATERIALS_MODE, default_materials_mode)
            .toBool();
    view_settings_.window.checkboxes.is_normals_mode =
        settings.value(group_name + NORMALS_MODE, default_normals_mode)
            .toBool();
    view_settings_.window.checkboxes.is_neon_mode =
        settings.value(group_name + NEON_MODE, default_neon_mode).toBool();
  }
  void RestoreRadioButtons(const QSettings &settings) {
    QString group_name = TYPES "/";
    projection_type_ =
        settings.value(group_name + PROJECTION_TYPE, default_projection_type)
            .toInt();
    if (projection_type_ == ON) {
      view_settings_.window.radiobuttons.is_ortho_projection = false;
      view_settings_.window.radiobuttons.is_perspective_projection = true;
    } else {
      view_settings_.window.radiobuttons.is_ortho_projection = true;
      view_settings_.window.radiobuttons.is_perspective_projection = false;
    }

    auto face_type =
        settings.value(group_name + FACE_TYPE, default_face_type).toInt();
    if (face_type == ON) {
      view_settings_.window.radiobuttons.is_display_face = true;
      view_settings_.window.radiobuttons.is_hide_face = false;
    } else {
      view_settings_.window.radiobuttons.is_display_face = false;
      view_settings_.window.radiobuttons.is_hide_face = true;
    }

    auto vertex_type =
        settings.value(group_name + VERTEX_TYPE, default_vertex_type).toInt();
    if (vertex_type == CIRCLE) {
      view_settings_.window.radiobuttons.is_circle_vertex = true;
      view_settings_.window.radiobuttons.is_square_vertex = false;
      view_settings_.window.radiobuttons.is_hide_vertex = false;
    } else if (vertex_type == SQUARE) {
      view_settings_.window.radiobuttons.is_circle_vertex = false;
      view_settings_.window.radiobuttons.is_square_vertex = true;
      view_settings_.window.radiobuttons.is_hide_vertex = false;
    } else {
      view_settings_.window.radiobuttons.is_circle_vertex = false;
      view_settings_.window.radiobuttons.is_square_vertex = false;
      view_settings_.window.radiobuttons.is_hide_vertex = true;
    }
    auto edge_type =
        settings.value(group_name + EDGE_TYPE, default_edge_type).toInt();
    if (edge_type == DASHED) {
      view_settings_.window.radiobuttons.is_dashed_edge = true;
      view_settings_.window.radiobuttons.is_solid_edge = false;
      view_settings_.window.radiobuttons.is_hide_edge = false;
    } else if (edge_type == SOLID) {
      view_settings_.window.radiobuttons.is_dashed_edge = false;
      view_settings_.window.radiobuttons.is_solid_edge = true;
      view_settings_.window.radiobuttons.is_hide_edge = false;
    } else {
      view_settings_.window.radiobuttons.is_dashed_edge = false;
      view_settings_.window.radiobuttons.is_solid_edge = false;
      view_settings_.window.radiobuttons.is_hide_edge = true;
    }
  }

  void RestoreFilePaths(const QSettings &settings) {
    QString group_name = FILES "/";
    file_path_ =
        settings.value(group_name + FILE_NAME, default_file_path).toString();
    texture_path_ =
        settings.value(group_name + TEXTURE_FILE_NAME, default_file_path)
            .toString();
    material_path_ =
        settings.value(group_name + MATERIAL_FILE_NAME, default_file_path)
            .toString();
  }

  void RestoreDirectories(const QSettings &settings) {
    QString group_name = DIRECTORIES "/";
    load_dir_ =
        settings.value(group_name + LOAD_DIR, default_load_dir).toString();
    view_settings_.dialog.load_dir = load_dir_;
    save_dir_ =
        settings.value(group_name + SAVE_DIR, default_save_dir).toString();
    view_settings_.dialog.save_dir = save_dir_;
  }
  void RestoreSettings() {
    auto settings = QSettings(settings_path_, QSettings::IniFormat);

    RestoreLabels(settings);
    RestoreSpinboxes(settings);
    RestoreSlidersAndDials(settings);
    RestoreCheckBoxes(settings);
    RestoreRadioButtons(settings);
    RestoreFilePaths(settings);
    RestoreDirectories(settings);
    RestoreGeometry(settings);
  }

  void SaveSettings() {
    auto settings = QSettings(settings_path_, QSettings::IniFormat);

    // colors
    settings.beginGroup(COLORS);
    QColor bg_color = view_settings_.window.lables.lbl_bg_color;
    settings.setValue(BG_COLOR, bg_color.name(QColor::HexArgb));
    QColor edge_color = view_settings_.window.lables.lbl_edge_color;
    settings.setValue(EDGE_COLOR, edge_color.name(QColor::HexArgb));
    QColor face_color = view_settings_.window.lables.lbl_face_color;
    settings.setValue(FACE_COLOR, face_color.name(QColor::HexArgb));
    QColor vertex_color = view_settings_.window.lables.lbl_vertex_color;
    settings.setValue(VERTEX_COLOR, vertex_color.name(QColor::HexArgb));
    QColor normal_color = view_settings_.window.lables.lbl_normal_color;
    settings.setValue(NORMAL_COLOR, normal_color.name(QColor::HexArgb));
    QColor light_color = view_settings_.window.lables.lbl_light_color;
    settings.setValue(LIGHT_COLOR, light_color.name(QColor::HexArgb));
    settings.endGroup();

    // sizes
    settings.beginGroup(SIZES);
    settings.setValue(EDGE_WIDTH, view_settings_.window.spinboxes.edge_width);
    settings.setValue(POINT_SIZE, view_settings_.window.spinboxes.point_size);
    settings.endGroup();

    // types
    settings.beginGroup(TYPES);
    if (view_settings_.window.radiobuttons.is_perspective_projection) {
      settings.setValue(PROJECTION_TYPE, ON);
    } else {
      settings.setValue(PROJECTION_TYPE, OFF);
    }
    if (view_settings_.window.radiobuttons.is_display_face) {
      settings.setValue(FACE_TYPE, ON);
    } else {
      settings.setValue(FACE_TYPE, OFF);
    }
    if (view_settings_.window.radiobuttons.is_circle_vertex) {
      settings.setValue(VERTEX_TYPE, CIRCLE);
    } else if (view_settings_.window.radiobuttons.is_square_vertex) {
      settings.setValue(VERTEX_TYPE, SQUARE);
    } else {
      settings.setValue(VERTEX_TYPE, OFF);
    }

    if (view_settings_.window.radiobuttons.is_solid_edge) {
      settings.setValue(EDGE_TYPE, SOLID);
    } else if (view_settings_.window.radiobuttons.is_dashed_edge) {
      settings.setValue(EDGE_TYPE, DASHED);
    } else {
      settings.setValue(EDGE_TYPE, OFF);
    }

    settings.endGroup();

    // positions
    settings.beginGroup(POSITIONS);
    settings.setValue(X_POS, view_settings_.window.sliders.x_slider_pos);
    settings.setValue(Y_POS, view_settings_.window.sliders.y_slider_pos);
    settings.setValue(Z_POS, view_settings_.window.sliders.z_slider_pos);

    settings.setValue(X_ANGLE, view_settings_.window.dials.x_dial_pos);
    settings.setValue(Y_ANGLE, view_settings_.window.dials.y_dial_pos);
    settings.setValue(Z_ANGLE, view_settings_.window.dials.z_dial_pos);
    settings.setValue(SCALE, view_settings_.window.sliders.scale_pos);
    settings.setValue(NORMALS_SCALE,
                      view_settings_.window.sliders.normals_scale_pos);

    settings.setValue(AMBIENT_INTENSITY,
                      view_settings_.window.sliders.ambient_intensity_pos);
    settings.setValue(DIFFUSE_INTENSITY,
                      view_settings_.window.sliders.diffuse_intensity_pos);
    settings.setValue(SPECULAR_INTENSITY,
                      view_settings_.window.sliders.specular_intensity_pos);
    settings.endGroup();

    // mapped_values
    settings.beginGroup(MAPPED_VALUES);
    settings.setValue(X_POS, translation_vector_.x());
    settings.setValue(Y_POS, translation_vector_.y());
    settings.setValue(Z_POS, translation_vector_.z());

    settings.setValue(X_ANGLE, rotation_vector_.x());
    settings.setValue(Y_ANGLE, rotation_vector_.y());
    settings.setValue(Z_ANGLE, rotation_vector_.z());
    settings.setValue(SCALE, scale_);
    settings.setValue(NORMALS_SCALE, normals_scale_);
    settings.setValue(AMBIENT_INTENSITY, ambient_intensity_);
    settings.setValue(DIFFUSE_INTENSITY, diffuse_intensity_);
    settings.setValue(SPECULAR_INTENSITY, specular_intensity_);
    settings.endGroup();

    // modes
    settings.beginGroup(MODES);
    settings.setValue(LIGHT_MODE,
                      view_settings_.window.checkboxes.is_light_mode);
    settings.setValue(TEXTURE_MODE,
                      view_settings_.window.checkboxes.is_texture_mode);
    settings.setValue(MATERIALS_MODE,
                      view_settings_.window.checkboxes.is_materials_mode);
    settings.setValue(NORMALS_MODE,
                      view_settings_.window.checkboxes.is_normals_mode);
    settings.setValue(NEON_MODE, view_settings_.window.checkboxes.is_neon_mode);
    settings.endGroup();

    // files
    settings.beginGroup(FILES);
    settings.setValue(FILE_NAME, file_path_);
    settings.setValue(TEXTURE_FILE_NAME, texture_path_);
    settings.setValue(MATERIAL_FILE_NAME, material_path_);
    settings.endGroup();

    // dirs
    settings.beginGroup(DIRECTORIES);
    settings.setValue(LOAD_DIR, load_dir_);
    settings.setValue(SAVE_DIR, save_dir_);
    settings.endGroup();
  }

  QString FaceCount() { return QString::number(mesh_model_.FaceCount()); }
  QString GetEdges() { return QString::number(mesh_model_.EdgeCount()); }
  QString VertexCount() { return QString::number(mesh_model_.VertexCount()); }

 private:
  float scale_;
  float normals_scale_;
  float ambient_intensity_;
  float diffuse_intensity_;
  float specular_intensity_;

  int projection_type_;

  QString file_path_;
  QString texture_path_;
  QString material_path_;
  QString settings_path_;
  QString load_dir_;
  QString save_dir_;

  OBJ_Data raw_data_;
  MeshModel mesh_model_;
  Config *config_;

  ViewSettings view_settings_;

  QVector3D translation_vector_;
  QVector3D rotation_vector_;

  QMatrix4x4 scale_matrix_;
  QMatrix4x4 rotation_matrix_;
  QMatrix4x4 translation_matrix_;

  QMatrix4x4 perspective_matrix_;
  QMatrix4x4 ortho_matrix_;
  float aspect_ratio_;
  QVector<QOpenGLTexture *> created_textures_;
};

#endif  // SCENEMODEL_H
