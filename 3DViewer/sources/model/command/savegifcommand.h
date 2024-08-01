#ifndef SAVEGIFCOMMAND_H
#define SAVEGIFCOMMAND_H
#include <QObject>

#include "command/command.h"
#include "lib/QtGifImage/gifimage/qgifimage.h"
#include "qfiledialog.h"
#include "view/scene.h"
class SaveGifCommand : public QObject, public Command {
  Q_OBJECT
 public:
  SaveGifCommand(Scene *scene, const QString &save_dir)
      : scene_(scene), save_dir_(save_dir) {}

  bool execute() override {
    auto gif = new QGifImage;
    auto timer = new QTimer(this);
    gif->setDefaultDelay(100);
    gif->setDefaultTransparentColor(QColorConstants::Black);

    connect(timer, &QTimer::timeout, this, [=]() {
      QPixmap pix = QPixmap::fromImage(scene_->grabFramebuffer());
      QPixmap scaledPix = pix.scaled(QSize(640, 480), Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation);
      gif->addFrame(scaledPix.toImage());
      if (gif->frameCount() == 50) {
        timer->stop();
        QString pathGIF = QFileDialog::getSaveFileName(
            scene_, "Save as GIF", save_dir_ + "/image.gif",
            "GIF Image Files (*.gif)");
        if (!pathGIF.isEmpty()) {
          gif->save(pathGIF);
        }
        delete timer;
        delete gif;
        emit EndLoop();
      }
    });

    timer->start(100);
    // Wait for timer completion
    QEventLoop loop;
    connect(this, &::SaveGifCommand::EndLoop, &loop, &QEventLoop::quit);
    loop.exec();
    return true;
  }

 signals:
  void EndLoop();

 private:
  Scene *scene_;
  QString save_dir_;
};
#endif  // SAVEGIFCOMMAND_H
