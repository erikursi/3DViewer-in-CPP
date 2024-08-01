#ifndef VIEWSETTINGS_H
#define VIEWSETTINGS_H

#include <QColor>

#include "consts.h"
#include "qvectornd.h"
class ViewSettings {
 public:
  ViewSettings() : window({}) {}
  typedef struct lables_settings {
    QColor lbl_bg_color = default_bg_color;
    QColor lbl_edge_color = default_edge_color;
    QColor lbl_vertex_color = default_vertex_color;
    QColor lbl_face_color = default_face_color;
    QColor lbl_normal_color = default_normal_color;
    QColor lbl_light_color = default_light_color;
  } lbl_settings;

  typedef struct spinboxes_settings {
    int default_point_size = 1;
    int default_edge_width = 1;
    int point_size = default_point_size;
    int edge_width = default_edge_width;
  } spbx_settings;

  typedef struct radiobuttons_settings {
    bool is_solid_edge = false;
    bool is_dashed_edge = false;
    bool is_hide_edge = true;
    bool is_circle_vertex = false;
    bool is_square_vertex = false;
    bool is_hide_vertex = true;
    bool is_display_face = true;
    bool is_hide_face = false;
    bool is_ortho_projection = true;
    bool is_perspective_projection = false;
  } rdb_settings;

  typedef struct checkboxes_settings {
    bool is_light_mode = default_light_mode;
    bool is_texture_mode = default_texture_mode;
    bool is_materials_mode = default_materials_mode;
    bool is_normals_mode = default_normals_mode;
    bool is_neon_mode = default_neon_mode;
  } cbx_settings;

  typedef struct slider_settings {
    int default_translation_pos = 0;
    int x_slider_pos = default_translation_pos;
    int y_slider_pos = default_translation_pos;
    int z_slider_pos = default_translation_pos;

    int default_scale_pos = 100;
    int default_normals_scale_pos = 5;
    int default_intensity_pos = 10;
    int scale_pos = default_scale_pos;
    int normals_scale_pos = default_normals_scale_pos;
    int intensity_pos = default_intensity_pos;

    int ambient_intensity_pos = default_intensity_pos;
    int diffuse_intensity_pos = default_intensity_pos;
    int specular_intensity_pos = default_intensity_pos;
  } sld_settings;

  typedef struct dials_settings {
    int default_rotation_pos = 0;
    int x_dial_pos = default_rotation_pos;
    int y_dial_pos = default_rotation_pos;
    int z_dial_pos = default_rotation_pos;
  } dls_settings;

  typedef struct scene_settings {
    QColor bg_color = default_bg_color;
    QString baseshader_vert = default_baseshader_vert;
    QString baseshader_frag = default_baseshader_frag;
    QString lightshader_vert = default_lightshader_vert;
    QString lightshader_frag = default_lightshader_frag;
    QString normalsshader_vert = default_normalsshader_vert;
    QString normalsshader_frag = default_normalsshader_frag;

  } s_settings;

  typedef struct window_settings {
    sld_settings sliders = {};
    cbx_settings checkboxes{};
    rdb_settings radiobuttons = {};
    lbl_settings lables = {};
    dls_settings dials = {};
    spbx_settings spinboxes = {};
    s_settings scene = {};
  } w_settings;

  typedef struct dialog_settings {
    QString load_dir = default_load_dir;
    QString save_dir = default_save_dir;
  } d_settings;

  w_settings window;

  d_settings dialog;
};

#endif  // VIEWSETTINGS_H
