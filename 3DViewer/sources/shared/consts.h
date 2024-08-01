#ifndef CONSTS_H
#define CONSTS_H

#include "qcolor.h"
#include "qdir.h"
#include "qvectornd.h"

// key values for settings
#define PROJECTION_TYPE "projectionType"
#define FACE_TYPE "faceType"
#define VERTEX_TYPE "vertexType"
#define EDGE_TYPE "edgeType"

#define EDGE_WIDTH "facetSize"
#define POINT_SIZE "vertexSize"

#define BG_COLOR "backgroundColor"
#define FACE_COLOR "faceColor"
#define VERTEX_COLOR "vertexColor"
#define EDGE_COLOR "edgeColor"
#define NORMAL_COLOR "normalColor"
#define LIGHT_COLOR "lightColor"

#define X_POS "xPos"
#define Y_POS "yPos"
#define Z_POS "zPos"
#define X_ANGLE "xAngle"
#define Y_ANGLE "yAngle"
#define Z_ANGLE "zAngle"
#define SCALE "scale"
#define NORMALS_SCALE "normalsScale"
#define AMBIENT_INTENSITY "ambientIntensity"
#define DIFFUSE_INTENSITY "diffuseIntensity"
#define SPECULAR_INTENSITY "specularIntensity"

#define TEXTURE_FILE_NAME "textureFileName"
#define MATERIAL_FILE_NAME "materialFileName"
#define FILE_NAME "fileName"
#define FACE_COUNT "faceCount"
#define VERTEX_COUNT "vertexCount"

#define LOAD_DIR "loadDir"
#define SAVE_DIR "saveDir"

#define LIGHT_MODE "lightMode"
#define TEXTURE_MODE "textureMode"
#define MATERIALS_MODE "materialsMode"
#define NORMALS_MODE "normalsMode"
#define NEON_MODE "neonMode"

// modes
#define OFF 0
#define ON 1

#define SOLID 2
#define DASHED 3

#define CIRCLE 4
#define SQUARE 5

// colors
const QColor default_vertex_color = QColor("#ff198dbe");
const QColor default_edge_color = QColor("#ffff8081");
const QColor default_face_color = QColor("#ff31d8f5");
const QColor default_bg_color = QColor("#fff1c8c8");
const QColor default_light_color = QColor("#FFFFFF");
const QColor default_normal_color = QColor("#FFFFFF");

// const QColor default_vertex_color = QColor(255, 128, 129);
// const QColor default_edge_color = QColor(137, 255, 238);
// const QColor default_face_color = QColor(137, 255, 238);
// const QColor default_bg_color = QColor(36, 71, 71);

// scale
const float default_scale_multiplier = 100;
const QVector2D default_scale_range = {1.0f / default_scale_multiplier,
                                       1.0f * default_scale_multiplier};
const float default_scale = 1.0;

// normals_scale
const float default_normals_scale_multiplier = 5.0;
const QVector2D default_normals_scale_range = {
    1.0f / default_normals_scale_multiplier,
    1.0f * default_normals_scale_multiplier};
const double default_normals_scale = 1.0;

// intensity
const float default_intensity_multiplier = 10.0;
const QVector2D default_intensity_range = {1.0f / default_intensity_multiplier,
                                           1.0f * default_intensity_multiplier};
const double default_intensity = 1.0;

// modes
const int default_light_mode = ON;
const int default_texture_mode = OFF;
const int default_materials_mode = OFF;
const int default_normals_mode = OFF;
const int default_neon_mode = OFF;

// types
const int default_vertex_type = OFF;
const int default_edge_type = OFF;
const int default_face_type = ON;
const int default_projection_type = ON;

// sizes

const int default_vertex_size = 1;
const int default_edge_width = 1;
const int default_normal_width = 1;

const QString default_file_path = "empty";
const QVector3D default_camera_position = {0, 0, -1};
const QVector2D default_widget_size = {769, 569};

const QString default_load_dir = QDir::homePath();
const QString default_save_dir = QDir::homePath();

const QString default_model_files_format = "3D Image Files (*.obj)";
const QString default_texture_files_format = "Images (*.png *.jpg)";
const QString default_material_files_format = "3D Image Files (*.mtl)";

const QString default_model_files_title = "Select Model";
const QString default_texture_files_title = "Select Texture";
const QString default_material_files_title = "Select Material";

const QString default_baseshader_vert = ":/shaders/baseshader.vert";
const QString default_baseshader_frag = ":/shaders/baseshader.frag";
const QString default_lightshader_vert = ":/shaders/lightshader.vert";
const QString default_lightshader_frag = ":/shaders/lightshader.frag";
const QString default_normalsshader_vert = ":/shaders/normalsshader.vert";
const QString default_normalsshader_frag = ":/shaders/normalsshader.frag";
const QString default_icon = ":/icons/app.png";

const bool default_smooth_normals = false;
enum class FileFormat { model, texture, material };
enum class DirectoryFormat { load, save };

const QString save_button_style =
    "QPushButton{"
    "background-color: rgb(249, 213, 209);"
    "color: rgb(36, 36, 36);"
    "border-radius: 10px;"
    "font-size: 24px;"
    "}"
    "QPushButton:hover {"
    "background-color: rgb(229, 140, 140);"
    "}"
    "QPushButton:pressed {"
    "background-color: rgb(245, 106, 134);"
    "}";
const QString cancel_button_style =
    "QPushButton{"
    "background-color: rgb(241, 42, 93);"
    "color: rgb(252, 244, 249);"
    "border-radius: 10px;"
    "font-size: 24px;"
    "}"
    "QPushButton:hover {"
    "background-color: rgb(240, 161, 161)"
    "}"
    "QPushButton:pressed {"
    "background-color: rgb(179, 38, 74);"
    "}";
const QString settings_file_name = "myapp.ini";
#endif  // CONSTS_H
