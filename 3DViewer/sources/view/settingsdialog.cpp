#include "settingsdialog.h"

#include "qfiledialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, const QString &load_dir,
                               const QString &save_dir)
    : QDialog(parent),
      ui(new Ui::SettingsDialog),
      load_dir_(load_dir),
      save_dir_(save_dir) {
  ui->setupUi(this);
  connect(ui->btnbx_Save, &QDialogButtonBox::accepted, this,
          &SettingsDialog::SaveClicked);
  connect(ui->btnbx_Save, &QDialogButtonBox::rejected, this,
          &SettingsDialog::CancelClicked);
  connect(ui->btn_ChooseLoadDir, &QPushButton::clicked, this,
          &SettingsDialog::ChooseLoadDirClicked);
  connect(ui->btn_ChooseSaveDir, &QPushButton::clicked, this,
          &SettingsDialog::ChooseSaveDirClicked);
  ui->txt_SaveDir->setText(save_dir_);
  ui->txt_LoadDir->setText(load_dir_);
  ConfigureButtonBox();
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::ConfigureButtonBox() {
  ui->btnbx_Save->layout()->setSpacing(9);
  ui->btnbx_Save->button(QDialogButtonBox::Save)->setMinimumHeight(50);
  ui->btnbx_Save->button(QDialogButtonBox::Save)->setMinimumWidth(120);
  ui->btnbx_Save->button(QDialogButtonBox::Save)->setMaximumWidth(150);
  ui->btnbx_Save->button(QDialogButtonBox::Cancel)->setMinimumHeight(50);
  ui->btnbx_Save->button(QDialogButtonBox::Cancel)->setMinimumWidth(120);
  ui->btnbx_Save->button(QDialogButtonBox::Cancel)->setMaximumWidth(150);
  ui->btnbx_Save->button(QDialogButtonBox::Save)
      ->setStyleSheet(save_button_style);
  ui->btnbx_Save->button(QDialogButtonBox::Cancel)
      ->setStyleSheet(cancel_button_style);
}

void SettingsDialog::UpdateDirectories(const QString &load_dir,
                                       const QString &save_dir) {
  load_dir_ = load_dir;
  save_dir_ = save_dir;
  SetLoadDirLabelText(load_dir_);
  SetSaveDirLabelText(save_dir_);
}

void SettingsDialog::SaveClicked() {
  load_dir_ = ui->txt_LoadDir->toPlainText();
  save_dir_ = ui->txt_SaveDir->toPlainText();
  QDialog::accept();
}

void SettingsDialog::CancelClicked() {
  ui->txt_LoadDir->setText(load_dir_);
  ui->txt_SaveDir->setText(save_dir_);
  QDialog::reject();
}

void SettingsDialog::ChooseLoadDirClicked() {
  emit ChooseLoadDirSignal(ui->lbl_LoadDir->text());
}

void SettingsDialog::ChooseSaveDirClicked() {
  emit ChooseSaveDirSignal(ui->lbl_SaveDir->text());
}

void SettingsDialog::closeEvent(QCloseEvent *event) {
  CancelClicked();
  QDialog::closeEvent(event);
}

void SettingsDialog::SetLoadDirLabelText(const QString &load_dir) {
  ui->txt_LoadDir->setText(load_dir);
}

void SettingsDialog::SetSaveDirLabelText(const QString &save_dir) {
  ui->txt_SaveDir->setText(save_dir);
}
