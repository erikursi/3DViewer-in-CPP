#include "mainview.h"

MainView::MainView(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainView),
      dialog(new SettingsDialog(this)) {
  ui->setupUi(this);

  QFile file(":/styles/violet.qss");
  QString content = "";
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    // qDebug() << "Не удалось открыть файл";
  } else {
    QTextStream in(&file);
    content = in.readAll();
    file.close();
    qApp->setStyleSheet(content);
  }
  // DoubleSpinBoxes
  QList<QDoubleSpinBox *> allDSpinBoxes =
      MainView::findChildren<QDoubleSpinBox *>();
  for (int i = 0; i < allDSpinBoxes.length(); i++) {
    allDSpinBoxes[i]->setButtonSymbols(QAbstractSpinBox::NoButtons);
  }

  this->setCentralWidget(ui->frame_Window);
  this->showMaximized();
}

void MainView::SetupConnections() {
  // buttons
  connect(ui->btn_SelectFile, &QPushButton::clicked, this,
          &::MainView::onSelectObjectButtonClicked);
  connect(ui->btn_ReadMaterials, &QPushButton::clicked, this,
          &::MainView::onSelectMaterialButtonClicked);
  connect(ui->btn_LoadTextures, &QPushButton::clicked, this,
          &::MainView::onLoadTexturesClicked);
  connect(ui->btn_Settings, &QPushButton::clicked, this,
          &::MainView::onSettingsButtonClicked);

  connect(ui->btn_Reset, &QPushButton::clicked, this,
          &::MainView::onResetButtonClicked);

  connect(ui->btn_GIF, &QAbstractButton::clicked, this,
          []() { Mediator::getInstance().Notify("SaveGifClicked"); });

  connect(ui->btn_Save, &QAbstractButton::clicked, this,
          []() { Mediator::getInstance().Notify("SaveImageClicked"); });

  // scene
  QObject::connect(ui->scene, &QOpenGLWidget::resized, this, []() {
    Mediator::getInstance().Notify("AspectRatioChanged");
  });

  // dialog
  QObject::connect(dialog, &SettingsDialog::ChooseLoadDirSignal, this,
                   [](const QString &dir) {
                     Mediator::getInstance().Notify("ChooseLoadDirClicked",
                                                    QVariant::fromValue(dir));
                   });
  QObject::connect(dialog, &SettingsDialog::ChooseSaveDirSignal, this,
                   [](const QString &dir) {
                     Mediator::getInstance().Notify("ChooseSaveDirClicked",
                                                    QVariant::fromValue(dir));
                   });

  // colors
  connect(ui->btn_BgColor, &QPushButton::clicked, this,
          &::MainView::onBgColorButtonClicked);
  connect(ui->btn_VertexColor, &QPushButton::clicked, this,
          &::MainView::onVertexColorButtonClicked);
  connect(ui->btn_EdgeColor, &QPushButton::clicked, this,
          &::MainView::onEdgeColorButtonClicked);

  connect(ui->btn_FaceColor, &QPushButton::clicked, this,
          &::MainView::onFaceColorButtonClicked);
  connect(ui->btn_NormalColor, &QPushButton::clicked, this,
          &::MainView::onNormalColorButtonClicked);

  connect(ui->btn_LightColor, &QPushButton::clicked, this,
          &::MainView::onLightColorButtonClicked);

  // sizes
  connect(ui->spbx_EdgeSize, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainView::onEdgeSizeValueChanged);
  connect(ui->spbx_VertexSize, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainView::onVertexSizeValueChanged);

  // positions
  connect(ui->hsld_xPos, &QSlider::valueChanged, this,
          &MainView::xPosSliderMoved);
  connect(ui->hsld_yPos, &QSlider::valueChanged, this,
          &MainView::yPosSliderMoved);
  connect(ui->hsld_zPos, &QSlider::valueChanged, this,
          &MainView::zPosSliderMoved);

  connect(ui->vsld_Scale, &QAbstractSlider::valueChanged, this,
          &MainView::scaleSliderMoved);
  connect(ui->hsld_NormalsScale, &QAbstractSlider::valueChanged, this,
          &MainView::normalsScaleSliderMoved);
  connect(ui->vsld_AmbientScale, &QAbstractSlider::valueChanged, this,
          &MainView::ambientSliderMoved);
  connect(ui->vsld_DiffuseScale, &QAbstractSlider::valueChanged, this,
          &MainView::diffuseSliderMoved);
  connect(ui->vsld_SpecularScale, &QAbstractSlider::valueChanged, this,
          &MainView::specularSliderMoved);

  connect(ui->dial_XRot, &QDial::valueChanged, this,
          &MainView::xRotationChanged);
  connect(ui->dial_YRot, &QDial::valueChanged, this,
          &MainView::yRotationChanged);
  connect(ui->dial_ZRot, &QDial::valueChanged, this,
          &MainView::zRotationChanged);

  // types
  connect(ui->rdbtn_eTypeDashed, &QRadioButton::clicked, this,
          &MainView::onEdgeTypeChanged);
  connect(ui->rdbtn_eTypeSolid, &QRadioButton::clicked, this,
          &MainView::onEdgeTypeChanged);
  connect(ui->rdbtn_eTypeNoEdge, &QRadioButton::clicked, this,
          &MainView::onEdgeTypeChanged);

  connect(ui->rdbtn_fTypeDisplay, &QRadioButton::clicked, this,
          &MainView::onFaceTypeChanged);
  connect(ui->rdbtn_fTypeNoFace, &QRadioButton::clicked, this,
          &MainView::onFaceTypeChanged);

  connect(ui->rdbtn_vTypeCircle, &QRadioButton::clicked, this,
          &MainView::onVertexTypeChanged);
  connect(ui->rdbtn_vTypeSquare, &QRadioButton::clicked, this,
          &MainView::onVertexTypeChanged);
  connect(ui->rdbtn_vTypeNoVertex, &QRadioButton::clicked, this,
          &MainView::onVertexTypeChanged);

  connect(ui->rdbtn_pTypeParallel, &QRadioButton::clicked, this,
          &MainView::onProjectionTypeChanged);
  connect(ui->rdbtn_pTypeCentral, &QRadioButton::clicked, this,
          &MainView::onProjectionTypeChanged);

  // check_boxes
  QObject::connect(ui->cbx_Light, &QCheckBox::stateChanged, this,
                   &::MainView::onLightCheckBoxClicked);
  QObject::connect(ui->cbx_Texture, &QCheckBox::stateChanged, this,
                   &::MainView::onTextureCheckBoxClicked);
  QObject::connect(ui->cbx_Material, &QCheckBox::stateChanged, this,
                   &::MainView::onMaterialCheckBoxClicked);
  QObject::connect(ui->cbx_Normals, &QCheckBox::stateChanged, this,
                   &::MainView::onNormalsCheckBoxClicked);
  QObject::connect(ui->cbx_Neon, &QCheckBox::stateChanged, this,
                   &::MainView::onNeonCheckBoxClicked);
}

void MainView::RestoreSettings(const ViewSettings &view_settings,
                               const Config *config) {
  view_settings_ = view_settings;

  // colors
  QColor bg_color = view_settings_.window.lables.lbl_bg_color;
  ui->lbl_BgColor->setStyleSheet(
      "background-color: " + bg_color.name(QColor::HexArgb) + ";");

  QColor edge_color = view_settings_.window.lables.lbl_edge_color;
  ui->lbl_EdgeColor->setStyleSheet(
      "background-color: " + edge_color.name(QColor::HexArgb) + ";");

  QColor vertex_color = view_settings_.window.lables.lbl_vertex_color;
  ui->lbl_VertexColor->setStyleSheet(
      "background-color: " + vertex_color.name(QColor::HexArgb) + ";");

  QColor face_color = view_settings_.window.lables.lbl_face_color;
  ui->lbl_FaceColor->setStyleSheet(
      "background-color: " + face_color.name(QColor::HexArgb) + ";");

  QColor normal_color = view_settings_.window.lables.lbl_normal_color;
  ui->lbl_NormalColor->setStyleSheet(
      "background-color: " + normal_color.name(QColor::HexArgb) + ";");
  QColor light_color = view_settings_.window.lables.lbl_light_color;
  ui->lbl_LightColor->setStyleSheet(
      "background-color: " + light_color.name(QColor::HexArgb) + ";");

  // sizes
  int edge_width = view_settings_.window.spinboxes.edge_width;
  ui->spbx_EdgeSize->setValue(edge_width);
  int point_size = view_settings_.window.spinboxes.point_size;
  ui->spbx_VertexSize->setValue(point_size);

  //  sliders
  int x_slider_pos = view_settings_.window.sliders.x_slider_pos;
  ResetSlider(ui->hsld_xPos, x_slider_pos);
  int y_slider_pos = view_settings_.window.sliders.y_slider_pos;
  ResetSlider(ui->hsld_yPos, y_slider_pos);
  int z_slider_pos = view_settings_.window.sliders.z_slider_pos;
  ResetSlider(ui->hsld_zPos, z_slider_pos);

  int scale_slider_pos = view_settings_.window.sliders.scale_pos;
  ResetSlider(ui->vsld_Scale, scale_slider_pos);
  int normals_scale_slider_pos =
      view_settings_.window.sliders.normals_scale_pos;
  ResetSlider(ui->hsld_NormalsScale, normals_scale_slider_pos);
  int ambient_slider_pos = view_settings_.window.sliders.ambient_intensity_pos;
  ResetSlider(ui->vsld_AmbientScale, ambient_slider_pos);
  int diffuse_slider_pos = view_settings_.window.sliders.diffuse_intensity_pos;
  ResetSlider(ui->vsld_DiffuseScale, diffuse_slider_pos);
  int specular_slider_pos =
      view_settings_.window.sliders.specular_intensity_pos;
  ResetSlider(ui->vsld_SpecularScale, specular_slider_pos);

  // dials
  int x_dial_pos = view_settings_.window.dials.x_dial_pos;
  ResetSlider(ui->dial_XRot, x_dial_pos);
  int y_dial_pos = view_settings_.window.dials.y_dial_pos;
  ResetSlider(ui->dial_YRot, y_dial_pos);
  int z_dial_pos = view_settings_.window.dials.z_dial_pos;
  ResetSlider(ui->dial_ZRot, z_dial_pos);

  // radiobuttons
  bool is_solid_edge = view_settings_.window.radiobuttons.is_solid_edge;
  ui->rdbtn_eTypeSolid->setChecked(is_solid_edge);
  bool is_dashed_edge = view_settings_.window.radiobuttons.is_dashed_edge;
  ui->rdbtn_eTypeDashed->setChecked(is_dashed_edge);
  bool is_hide_edge = view_settings_.window.radiobuttons.is_hide_edge;
  ui->rdbtn_eTypeNoEdge->setChecked(is_hide_edge);
  bool is_circle_vertex = view_settings_.window.radiobuttons.is_circle_vertex;
  ui->rdbtn_vTypeCircle->setChecked(is_circle_vertex);
  bool is_square_vertex = view_settings_.window.radiobuttons.is_square_vertex;
  ui->rdbtn_vTypeSquare->setChecked(is_square_vertex);
  bool is_hide_vertex = view_settings_.window.radiobuttons.is_hide_vertex;
  ui->rdbtn_vTypeNoVertex->setChecked(is_hide_vertex);
  bool is_display_face = view_settings_.window.radiobuttons.is_display_face;
  ui->rdbtn_fTypeDisplay->setChecked(is_display_face);
  bool is_hide_face = view_settings_.window.radiobuttons.is_hide_face;
  ui->rdbtn_fTypeNoFace->setChecked(is_hide_face);
  bool is_central_projection =
      view_settings_.window.radiobuttons.is_perspective_projection;
  ui->rdbtn_pTypeCentral->setChecked(is_central_projection);
  bool is_parallel_projection =
      view_settings_.window.radiobuttons.is_ortho_projection;
  ui->rdbtn_pTypeParallel->setChecked(is_parallel_projection);

  // modes
  bool is_light_mode = view_settings_.window.checkboxes.is_light_mode;
  ui->cbx_Light->setChecked(is_light_mode);
  bool is_texture_mode = view_settings_.window.checkboxes.is_texture_mode;
  ui->cbx_Texture->setChecked(is_texture_mode);
  bool is_materials_mode = view_settings_.window.checkboxes.is_materials_mode;
  ui->cbx_Material->setChecked(is_materials_mode);
  bool is_normals_mode = view_settings_.window.checkboxes.is_normals_mode;
  ui->cbx_Normals->setChecked(is_normals_mode);
  bool is_neon_mode = view_settings_.window.checkboxes.is_neon_mode;
  ui->cbx_Neon->setChecked(is_neon_mode);

  ui->scene->SetConfig(config);
  ui->scene->SetShaderPrograms(view_settings_.window.scene.baseshader_vert,
                               view_settings_.window.scene.baseshader_frag,
                               view_settings_.window.scene.lightshader_vert,
                               view_settings_.window.scene.lightshader_frag,
                               view_settings_.window.scene.normalsshader_vert,
                               view_settings_.window.scene.normalsshader_frag);

  dialog->UpdateDirectories(view_settings_.dialog.load_dir,
                            view_settings_.dialog.save_dir);
}

MainView::~MainView() {
  delete ui;
  delete dialog;
}

void MainView::closeEvent(QCloseEvent *event) {
  Mediator::getInstance().Notify("SaveSettings");
  QMainWindow::closeEvent(event);
}

void MainView::LoadSettings(const ViewSettings &view_settings,
                            const Config *config) {
  RestoreSettings(view_settings, config);
  SetupConnections();
}

void MainView::SetConfig(const Config *config) { ui->scene->SetConfig(config); }

void MainView::UpdateScene() { ui->scene->update(); }

void MainView::onLoadTexturesClicked() {
  Mediator::getInstance().Notify("SelectTextureClicked");
}

void MainView::onSettingsButtonClicked() {
  dialog->setModal(true);
  int result = dialog->exec();
  if (result == QDialog::Accepted) {
    Mediator::getInstance().Notify(
        "DirectoriesChanged", QVariant::fromValue(dialog->GetDirectories()));
  }
}

void MainView::onSelectObjectButtonClicked() {
  Mediator::getInstance().Notify("SelectModelClicked");
}

void MainView::onSelectMaterialButtonClicked() {
  Mediator::getInstance().Notify("SelectMaterialClicked");
}

void MainView::SetData(const QString &file_path, const QString &vertex_count,
                       const QString &face_count, const MeshModel &mesh_model) {
  ui->lbl_FileName->setText(file_path);
  ui->lbl_NumVertices->setText(vertex_count);
  ui->lbl_NumFaces->setText(face_count);
  ui->scene->AddMeshModel(mesh_model);
}

void MainView::SetDirectories(const QString &load_dir,
                              const QString &save_dir) {
  dialog->UpdateDirectories(load_dir, save_dir);
}

void MainView::onNormalColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    Mediator::getInstance().Notify("NormalColorChanged",
                                   QVariant::fromValue(color));
    ui->lbl_NormalColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}

void MainView::onBgColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    Mediator::getInstance().Notify("BgColorChanged",
                                   QVariant::fromValue(color));
    ui->lbl_BgColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}

void MainView::onLightColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    Mediator::getInstance().Notify("LightColorChanged",
                                   QVariant::fromValue(color));
    ui->lbl_LightColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}

void MainView::onEdgeColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    Mediator::getInstance().Notify("EdgeColorChanged",
                                   QVariant::fromValue(color));
    ui->lbl_EdgeColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}

void MainView::onVertexColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    Mediator::getInstance().Notify("VertexColorChanged",
                                   QVariant::fromValue(color));
    ui->lbl_VertexColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}

void MainView::onFaceColorButtonClicked() {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    Mediator::getInstance().Notify("FaceColorChanged",
                                   QVariant::fromValue(color));
    ui->lbl_FaceColor->setStyleSheet(
        "background-color: " + color.name(QColor::HexArgb) + ";");
  }
}

void MainView::onLightCheckBoxClicked(int value) {
  Mediator::getInstance().Notify("LightCheckBoxClicked",
                                 QVariant::fromValue(value));
}

void MainView::onTextureCheckBoxClicked(int value) {
  Mediator::getInstance().Notify("TextureCheckBoxClicked",
                                 QVariant::fromValue(value));
}

void MainView::onMaterialCheckBoxClicked(int value) {
  Mediator::getInstance().Notify("MaterialCheckBoxClicked",
                                 QVariant::fromValue(value));
}

void MainView::onNormalsCheckBoxClicked(int value) {
  Mediator::getInstance().Notify("NormalsCheckBoxClicked",
                                 QVariant::fromValue(value));
}

void MainView::onNeonCheckBoxClicked(int value) {
  Mediator::getInstance().Notify("NeonCheckBoxClicked",
                                 QVariant::fromValue(value));
}

void MainView::onEdgeSizeValueChanged(int value) {
  Mediator::getInstance().Notify("EdgeWidthChanged",
                                 QVariant::fromValue(value));
}

void MainView::onVertexSizeValueChanged(int value) {
  Mediator::getInstance().Notify("VertexSizeChanged",
                                 QVariant::fromValue(value));
}

void MainView::xPosSliderMoved(int value) {
  Mediator::getInstance().Notify("XPosSliderMoved", QVariant::fromValue(value));
}

void MainView::yPosSliderMoved(int value) {
  Mediator::getInstance().Notify("YPosSliderMoved", QVariant::fromValue(value));
}

void MainView::zPosSliderMoved(int value) {
  Mediator::getInstance().Notify("ZPosSliderMoved", QVariant::fromValue(value));
}

void MainView::xRotationChanged(int value) {
  Mediator::getInstance().Notify("XAngleChanged", QVariant::fromValue(value));
}

void MainView::yRotationChanged(int value) {
  Mediator::getInstance().Notify("YAngleChanged", QVariant::fromValue(value));
}

void MainView::zRotationChanged(int value) {
  Mediator::getInstance().Notify("ZAngleChanged", QVariant::fromValue(value));
}

void MainView::onFaceTypeChanged() {
  QRadioButton *rdbtn = qobject_cast<QRadioButton *>(sender());
  if (rdbtn == ui->rdbtn_fTypeDisplay) {
    Mediator::getInstance().Notify("FaceTypeChanged", QVariant::fromValue(ON));
  } else if (rdbtn == ui->rdbtn_fTypeNoFace) {
    Mediator::getInstance().Notify("FaceTypeChanged", QVariant::fromValue(OFF));
  }
}

void MainView::onEdgeTypeChanged() {
  QRadioButton *rdbtn = qobject_cast<QRadioButton *>(sender());
  if (rdbtn == ui->rdbtn_eTypeDashed) {
    Mediator::getInstance().Notify("EdgeTypeChanged",
                                   QVariant::fromValue(DASHED));

  } else if (rdbtn == ui->rdbtn_eTypeSolid) {
    Mediator::getInstance().Notify("EdgeTypeChanged",
                                   QVariant::fromValue(SOLID));

  } else if (rdbtn == ui->rdbtn_eTypeNoEdge) {
    Mediator::getInstance().Notify("EdgeTypeChanged", QVariant::fromValue(OFF));
  }
}

void MainView::onVertexTypeChanged() {
  QRadioButton *rdbtn = qobject_cast<QRadioButton *>(sender());
  if (rdbtn == ui->rdbtn_vTypeCircle) {
    Mediator::getInstance().Notify("VertexTypeChanged",
                                   QVariant::fromValue(CIRCLE));

  } else if (rdbtn == ui->rdbtn_vTypeSquare) {
    Mediator::getInstance().Notify("VertexTypeChanged",
                                   QVariant::fromValue(SQUARE));

  } else if (rdbtn == ui->rdbtn_vTypeNoVertex) {
    Mediator::getInstance().Notify("VertexTypeChanged",
                                   QVariant::fromValue(OFF));
  }
}

void MainView::onProjectionTypeChanged() {
  QRadioButton *rdbtn = qobject_cast<QRadioButton *>(sender());
  if (rdbtn == ui->rdbtn_pTypeCentral) {
    Mediator::getInstance().Notify("ProjectionTypeChanged",
                                   QVariant::fromValue(ON));
  } else if (rdbtn == ui->rdbtn_pTypeParallel) {
    Mediator::getInstance().Notify("ProjectionTypeChanged",
                                   QVariant::fromValue(OFF));
  }
}

void MainView::scaleSliderMoved(int value) {
  Mediator::getInstance().Notify("ScaleChanged", QVariant::fromValue(value));
}

void MainView::normalsScaleSliderMoved(int value) {
  Mediator::getInstance().Notify("NormalsScaleChanged",
                                 QVariant::fromValue(value));
}

void MainView::ambientSliderMoved(int value) {
  Mediator::getInstance().Notify("AmbientIntensityChanged",
                                 QVariant::fromValue(value));
}

void MainView::diffuseSliderMoved(int value) {
  Mediator::getInstance().Notify("DiffuseIntensityChanged",
                                 QVariant::fromValue(value));
}

void MainView::specularSliderMoved(int value) {
  Mediator::getInstance().Notify("SpecularIntensityChanged",
                                 QVariant::fromValue(value));
}

Scene *MainView::GetScene() { return ui->scene; }

SettingsDialog *MainView::GetDialog() { return dialog; }

void MainView::onResetButtonClicked() {
  ResetSlider(ui->vsld_Scale, view_settings_.window.sliders.default_scale_pos);
  ResetSlider(ui->hsld_NormalsScale,
              view_settings_.window.sliders.default_normals_scale_pos);
  ResetSlider(ui->vsld_AmbientScale,
              view_settings_.window.sliders.default_intensity_pos);
  ResetSlider(ui->vsld_DiffuseScale,
              view_settings_.window.sliders.default_intensity_pos);
  ResetSlider(ui->vsld_SpecularScale,
              view_settings_.window.sliders.default_intensity_pos);

  ResetSlider(ui->hsld_xPos,
              view_settings_.window.sliders.default_translation_pos);
  ResetSlider(ui->hsld_yPos,
              view_settings_.window.sliders.default_translation_pos);
  ResetSlider(ui->hsld_zPos,
              view_settings_.window.sliders.default_translation_pos);

  ResetSlider(ui->dial_XRot, view_settings_.window.dials.default_rotation_pos);
  ResetSlider(ui->dial_YRot, view_settings_.window.dials.default_rotation_pos);
  ResetSlider(ui->dial_ZRot, view_settings_.window.dials.default_rotation_pos);

  Mediator::getInstance().Notify("ResetClicked");
}

void MainView::ResetSlider(QAbstractSlider *slider, int pos) {
  slider->setSliderDown(true);
  slider->setSliderPosition(pos);
  slider->setSliderDown(false);
}
