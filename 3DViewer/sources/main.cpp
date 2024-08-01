#include <QApplication>

#include "controller/mainviewcontroller.h"
#include "view/mainview.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setApplicationVersion(QT_VERSION_STR);
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);
  QIcon icon(default_icon);
  QApplication::setWindowIcon(icon);

  MainView view;
  view.setFocus();
  SceneModel scene_model(settings_file_name);
  MainViewController controller(&view, &scene_model);
  view.show();
  return a.exec();
}
