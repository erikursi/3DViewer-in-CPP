#ifndef MAINVIEW_H
#define MAINVIEW_H
#include <qstring.h>

#include <QApplication>
#include <QColorDialog>
#include <QMainWindow>
#include <QMouseEvent>
#include <QSlider>
#include <QStyleOptionSlider>
#include <QWidget>

#include "./ui_mainview.h"
#include "settingsdialog.h"
#include "shared/mediator/mediator.h"
#include "shared/viewsettings.h"
#include "view/scene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainView;
}
QT_END_NAMESPACE

class MainView : public QMainWindow {
  Q_OBJECT

 public:
  MainView(QWidget *parent = nullptr);
  ~MainView();
  void SetConfig(const Config *config);
  void SetData(const QString &file_path, const QString &vertex_count,
               const QString &face_count, const MeshModel &mesh_model);
  void SetDirectories(const QString &load_dir, const QString &save_dir);
  void LoadSettings(const ViewSettings &view_settings, const Config *config);
  void UpdateScene();
  Scene *GetScene();
  SettingsDialog *GetDialog();
  ViewSettings view_settings_;

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  void onEdgeSizeValueChanged(int value);
  void onVertexSizeValueChanged(int value);
  void xPosSliderMoved(int value);
  void yPosSliderMoved(int value);
  void zPosSliderMoved(int value);
  void xRotationChanged(int value);
  void yRotationChanged(int value);
  void zRotationChanged(int value);
  void scaleSliderMoved(int value);
  void normalsScaleSliderMoved(int value);
  void ambientSliderMoved(int value);
  void diffuseSliderMoved(int value);
  void specularSliderMoved(int value);
  void onLoadTexturesClicked();
  void onResetButtonClicked();

  void SetupConnections();
  void onSelectObjectButtonClicked();
  void onSelectMaterialButtonClicked();
  void onSettingsButtonClicked();
  void onLightColorButtonClicked();
  void onBgColorButtonClicked();
  void onEdgeColorButtonClicked();
  void onVertexColorButtonClicked();
  void onNormalColorButtonClicked();
  void onFaceColorButtonClicked();
  void onEdgeTypeChanged();
  void onVertexTypeChanged();
  void onFaceTypeChanged();
  void onProjectionTypeChanged();
  void onLightCheckBoxClicked(int value);
  void onTextureCheckBoxClicked(int value);
  void onMaterialCheckBoxClicked(int value);
  void onNormalsCheckBoxClicked(int value);
  void onNeonCheckBoxClicked(int value);

  // methods for saving and restoring settings
  void RestoreSettings(const ViewSettings &view_settings, const Config *config);
  void ResetSlider(QAbstractSlider *slider, int pos);

 private:
  Ui::MainView *ui;
  SettingsDialog *dialog;
};
#endif  // MAINVIEW_H
