#ifndef MAINVIEWCONTROLLER_H
#define MAINVIEWCONTROLLER_H

#include <QObject>

#include "command/processmtlfilecommand.h"
#include "command/processobjfilecommand.h"
#include "command/processtexturefilecommand.h"
#include "command/savegifcommand.h"
#include "command/saveimagecommand.h"
// #include "lib/QtGifImage/gifimage/qgifimage.h"
#include "model/scenemodel.h"
#include "view/mainview.h"

class MainViewController : public QObject, Observer {
  Q_OBJECT

 public:
  MainViewController(MainView *View, SceneModel *scene_model);
  ~MainViewController() = default;

 public:
  void OnLightColorChanged(const QVariant &value);
  void OnFaceColorChanged(const QVariant &value);
  void OnVertexColorChanged(const QVariant &value);
  void OnNormalColorChanged(const QVariant &value);
  void OnEdgeColorChanged(const QVariant &value);
  void OnBgColorChanged(const QVariant &value);
  void OnEdgeTypeChanged(const QVariant &value);
  void OnVertexTypeChanged(const QVariant &value);
  void OnFaceTypeChanged(const QVariant &value);
  void OnProjectionTypeChanged(const QVariant &value);
  void OnEdgeWidthChanged(const QVariant &value);
  void OnVertexSizeChanged(const QVariant &value);
  void OnChooseLoadDirClicked(const QVariant &value);
  void OnChooseSaveDirClicked(const QVariant &value);
  void OnSelectMaterialClicked();
  void OnSelectTextureClicked();
  void OnSaveGifClicked();
  void OnSaveImageClicked();
  void OnResetClicked();
  void OnSelectModelClicked();
  void OnXPosSliderMoved(const QVariant &value);
  void OnYPosSliderMoved(const QVariant &value);
  void OnZPosSliderMoved(const QVariant &value);
  void OnXAngleChanged(const QVariant &value);
  void OnYAngleChanged(const QVariant &value);
  void OnZAngleChanged(const QVariant &value);
  void OnScaleChanged(const QVariant &value);
  void OnNormalsScaleChanged(const QVariant &value);
  void OnLightCheckBoxClicked(const QVariant &value);
  void OnTextureCheckBoxClicked(const QVariant &value);
  void OnMaterialCheckBoxClicked(const QVariant &value);
  void OnNormalsCheckBoxClicked(const QVariant &value);
  void OnNeonCheckBoxClicked(const QVariant &value);
  void OnAmbientIntensityChanged(const QVariant &value);
  void OnDiffuseIntensityChanged(const QVariant &value);
  void OnSpecularIntensityChanged(const QVariant &value);
  void OnDirectoriesChanged(const QVariant &value);
  void OnAspectRatioChanged();
  void OnSaveSettings();
  void Subscribe();
  void Update(const QString &event, const QVariant &value) override;
  void Update(const QString &event) override;
  void LoadModel(bool with_select);
  void LoadTexture(bool with_select);
  void LoadMaterial(bool with_select);

 private:
  // Typedef for member function pointer
  using HandlerFunc = void (MainViewController::*)(const QVariant &);
  using HandlerFuncWithoutData = void (MainViewController::*)();

  // Map of event types to handler functions
  std::unordered_map<QString, HandlerFunc> event_handlers_with_data_ = {
      {"LightColorChanged", &MainViewController::OnLightColorChanged},
      {"EdgeColorChanged", &MainViewController::OnEdgeColorChanged},
      {"FaceColorChanged", &MainViewController::OnFaceColorChanged},
      {"VertexColorChanged", &MainViewController::OnVertexColorChanged},
      {"NormalColorChanged", &MainViewController::OnNormalColorChanged},
      {"BgColorChanged", &MainViewController::OnBgColorChanged},
      {"FaceTypeChanged", &MainViewController::OnFaceTypeChanged},
      {"EdgeTypeChanged", &MainViewController::OnEdgeTypeChanged},
      {"VertexTypeChanged", &MainViewController::OnVertexTypeChanged},
      {"ProjectionTypeChanged", &MainViewController::OnProjectionTypeChanged},
      {"EdgeWidthChanged", &MainViewController::OnEdgeWidthChanged},
      {"VertexSizeChanged", &MainViewController::OnVertexSizeChanged},

      {"XPosSliderMoved", &MainViewController::OnXPosSliderMoved},
      {"YPosSliderMoved", &MainViewController::OnYPosSliderMoved},
      {"ZPosSliderMoved", &MainViewController::OnZPosSliderMoved},

      {"XAngleChanged", &MainViewController::OnXAngleChanged},
      {"YAngleChanged", &MainViewController::OnYAngleChanged},
      {"ZAngleChanged", &MainViewController::OnZAngleChanged},

      {"LightCheckBoxClicked", &MainViewController::OnLightCheckBoxClicked},
      {"MaterialCheckBoxClicked",
       &MainViewController::OnMaterialCheckBoxClicked},
      {"TextureCheckBoxClicked", &MainViewController::OnTextureCheckBoxClicked},
      {"NormalsCheckBoxClicked", &MainViewController::OnNormalsCheckBoxClicked},
      {"NeonCheckBoxClicked", &MainViewController::OnNeonCheckBoxClicked},

      {"ScaleChanged", &MainViewController::OnScaleChanged},
      {"NormalsScaleChanged", &MainViewController::OnNormalsScaleChanged},
      {"AmbientIntensityChanged",
       &MainViewController::OnAmbientIntensityChanged},
      {"DiffuseIntensityChanged",
       &MainViewController::OnDiffuseIntensityChanged},
      {"SpecularIntensityChanged",
       &MainViewController::OnSpecularIntensityChanged},
      {"ChooseLoadDirClicked", &MainViewController::OnChooseLoadDirClicked},
      {"ChooseSaveDirClicked", &MainViewController::OnChooseSaveDirClicked},
      {"DirectoriesChanged", &MainViewController::OnDirectoriesChanged}};

  std::unordered_map<QString, HandlerFuncWithoutData> event_handlers_ = {
      {"SaveGifClicked", &MainViewController::OnSaveGifClicked},
      {"ResetClicked", &MainViewController::OnResetClicked},
      {"SaveImageClicked", &MainViewController::OnSaveImageClicked},
      {"SaveSettings", &MainViewController::OnSaveSettings},
      {"AspectRatioChanged", &MainViewController::OnAspectRatioChanged},
      {"SelectModelClicked", &MainViewController::OnSelectModelClicked},
      {"SelectMaterialClicked", &MainViewController::OnSelectMaterialClicked},
      {"SelectTextureClicked", &MainViewController::OnSelectTextureClicked},

  };

  MainView *view_;
  SceneModel *scene_model_;
};

#endif  // MAINVIEWCONTROLLER_H
