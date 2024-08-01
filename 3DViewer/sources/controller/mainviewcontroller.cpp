#include "mainviewcontroller.h"

#include "command/selectdirectorycommand.h"

MainViewController::MainViewController(MainView *View, SceneModel *scene_model)
    : view_(View), scene_model_(scene_model) {
  scene_model_->SetWidgetSizes(view_->GetScene()->width(),
                               view_->GetScene()->height());
  view_->LoadSettings(scene_model_->GetViewSettings(),
                      scene_model_->GetConfig());
  Subscribe();
  auto file = scene_model_->FilePath();
  if (file != "empty") {
    LoadModel(false);
  }

  auto texture = scene_model_->TexturePath();
  if (texture != "empty") {
    LoadTexture(false);
  }

  auto material = scene_model_->MaterialPath();
  if (material != "empty") {
    LoadMaterial(false);
  }
}

void MainViewController::Subscribe() { Mediator::getInstance().Attach(this); }

void MainViewController::Update(const QString &event, const QVariant &value) {
  if (event_handlers_with_data_.find(event) !=
      event_handlers_with_data_.end()) {
    auto func = event_handlers_with_data_[event];
    (this->*func)(value);
  }
}

void MainViewController::Update(const QString &event) {
  if (event_handlers_.find(event) != event_handlers_.end()) {
    auto func = event_handlers_[event];
    (this->*func)();
  }
}

void MainViewController::OnLightColorChanged(const QVariant &value) {
  scene_model_->SetLightColor(value.value<QColor>());
  view_->UpdateScene();
}

void MainViewController::OnFaceColorChanged(const QVariant &value) {
  scene_model_->SetFaceColor(value.value<QColor>());
  view_->UpdateScene();
}

void MainViewController::OnEdgeColorChanged(const QVariant &value) {
  scene_model_->SetEdgeColor(value.value<QColor>());
  view_->UpdateScene();
}

void MainViewController::OnVertexColorChanged(const QVariant &value) {
  scene_model_->SetVertexColor(value.value<QColor>());
  view_->UpdateScene();
}

void MainViewController::OnNormalColorChanged(const QVariant &value) {
  scene_model_->SetNormalColor(value.value<QColor>());
  view_->UpdateScene();
}

void MainViewController::OnBgColorChanged(const QVariant &value) {
  scene_model_->SetBgColor(value.value<QColor>());
  view_->UpdateScene();
}

void MainViewController::OnEdgeTypeChanged(const QVariant &value) {
  scene_model_->SetEdgeType(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnVertexTypeChanged(const QVariant &value) {
  scene_model_->SetVertexType(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnFaceTypeChanged(const QVariant &value) {
  scene_model_->SetFaceType(value.value<int>());
  view_->UpdateScene();
}
void MainViewController::OnProjectionTypeChanged(const QVariant &value) {
  scene_model_->SetProjectionType(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnEdgeWidthChanged(const QVariant &value) {
  scene_model_->SetEdgeWidth(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnVertexSizeChanged(const QVariant &value) {
  scene_model_->SetVertexSize(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnSelectModelClicked() { LoadModel(true); }

void MainViewController::LoadModel(bool with_select) {
  Command *command = new ProcessOBJFileCommand(scene_model_, with_select);
  if (command->execute()) {
    view_->SetData(scene_model_->FileName(), scene_model_->VertexCount(),
                   scene_model_->GetEdges(), scene_model_->getMeshModel());
    view_->UpdateScene();
  }
  delete command;
}

void MainViewController::LoadTexture(bool with_select) {
  Command *command = new ProcessTextureFileCommand(scene_model_, with_select);
  if (command->execute()) {
    view_->UpdateScene();
  }
  delete command;
}

void MainViewController::LoadMaterial(bool with_select) {
  Command *command = new ProcessMTLFileCommand(scene_model_, with_select);

  if (command->execute()) {
    view_->UpdateScene();
  }
  delete command;
}

void MainViewController::OnSelectMaterialClicked() { LoadMaterial(true); }

void MainViewController::OnSelectTextureClicked() { LoadTexture(true); }

void MainViewController::OnSaveGifClicked() {
  SaveGifCommand *command =
      new SaveGifCommand(view_->GetScene(), scene_model_->SaveDir());
  command->execute();
  delete command;
}

void MainViewController::OnSaveImageClicked() {
  SaveImageCommand *command =
      new SaveImageCommand(view_->GetScene(), scene_model_->SaveDir());
  command->execute();
  delete command;
}

void MainViewController::OnResetClicked() {
  scene_model_->ResetModelGeometry();
  view_->UpdateScene();
}

void MainViewController::OnXPosSliderMoved(const QVariant &value) {
  scene_model_->SetStepX(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnYPosSliderMoved(const QVariant &value) {
  scene_model_->SetStepY(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnZPosSliderMoved(const QVariant &value) {
  scene_model_->SetStepZ(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnXAngleChanged(const QVariant &value) {
  scene_model_->SetAngleX(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnYAngleChanged(const QVariant &value) {
  scene_model_->SetAngleY(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnZAngleChanged(const QVariant &value) {
  scene_model_->SetAngleZ(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnScaleChanged(const QVariant &value) {
  scene_model_->SetScale(value.value<double>());
  view_->UpdateScene();
}

void MainViewController::OnNormalsScaleChanged(const QVariant &value) {
  scene_model_->SetNormalsScale(value.value<double>());
  view_->UpdateScene();
}

void MainViewController::OnLightCheckBoxClicked(const QVariant &value) {
  scene_model_->SetLightMode(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnTextureCheckBoxClicked(const QVariant &value) {
  scene_model_->SetTextureMode(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnMaterialCheckBoxClicked(const QVariant &value) {
  scene_model_->SetMaterialMode(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnNormalsCheckBoxClicked(const QVariant &value) {
  scene_model_->SetNormalsMode(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnNeonCheckBoxClicked(const QVariant &value) {
  scene_model_->SetNeonMode(value.value<int>());
  view_->UpdateScene();
}

void MainViewController::OnDiffuseIntensityChanged(const QVariant &value) {
  scene_model_->SetDiffuseIntensity(value.value<double>());
  view_->UpdateScene();
}

void MainViewController::OnAmbientIntensityChanged(const QVariant &value) {
  scene_model_->SetAmbientIntensity(value.value<double>());
  view_->UpdateScene();
}

void MainViewController::OnSpecularIntensityChanged(const QVariant &value) {
  scene_model_->SetSpecularIntensity(value.value<double>());
  view_->UpdateScene();
}

void MainViewController::OnSaveSettings() { scene_model_->SaveSettings(); }

void MainViewController::OnAspectRatioChanged() {
  scene_model_->SetWidgetSizes(view_->GetScene()->width(),
                               view_->GetScene()->height());
  view_->UpdateScene();
}

void MainViewController::OnDirectoriesChanged(const QVariant &value) {
  QPair<QString, QString> pair = value.value<QPair<QString, QString>>();
  const auto &[load_dir, save_dir] = pair;
  scene_model_->SetDirectories(load_dir, save_dir);
  view_->SetDirectories(load_dir, save_dir);
}

void MainViewController::OnChooseLoadDirClicked(const QVariant &value) {
  Command *command = new SelectDirectoryCommand(
      view_->GetDialog(), DirectoryFormat::load, value.value<QString>());
  command->execute();
  delete command;
}

void MainViewController::OnChooseSaveDirClicked(const QVariant &value) {
  Command *command = new SelectDirectoryCommand(
      view_->GetDialog(), DirectoryFormat::save, value.value<QString>());
  command->execute();
  delete command;
}
