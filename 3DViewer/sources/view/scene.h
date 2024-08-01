#ifndef SCENE_H
#define SCENE_H

#include <QDebug>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>

#include "shared/renderer/configuration/config.h"
#include "shared/renderer/meshmodel/meshmodel.h"

class Scene : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  Scene(QWidget *parent = nullptr);
  ~Scene();
  void AddMeshModel(const MeshModel &mesh_model);
  void RemoveMeshModel();
  void SetConfig(const Config *config);
  void RenderBasicScene();
  void RenderSceneWithLight();
  bool SetShaderPrograms(const QString &baseshader_vert,
                         const QString &baseshader_frag,
                         const QString &lightshader_vert,
                         const QString &lightshader_frag,
                         const QString &normalsshader_vert,
                         const QString &normalsshader_frag);

 private:
  void CreateVaos();
  void CreateBuffers();
  void UpdateBuffers();

  bool CreateLightShaderProgram(const QString &lightshader_vert,
                                const QString &lightshader_frag);
  bool CreateBaseShaderProgram(const QString &baseshader_vert,
                               const QString &baseshader_frag);
  bool CreateNormalsShaderProgram(const QString &normalsshader_vert,
                                  const QString &normalsshader_frag);

  // ogl functions
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

 private:
  unsigned int model_count_;
  const Config *config_;
  MeshModel mesh_model_;
  QOpenGLShaderProgram *shader_program_;
  QOpenGLShaderProgram *light_shader_program_;
  QOpenGLShaderProgram *normals_shader_program_;
  QOpenGLVertexArrayObject *vao_base_;
  QOpenGLVertexArrayObject *vao_light_;
  QOpenGLVertexArrayObject *vao_normal_vectors_;
  QOpenGLBuffer *vertices_buffer_;
  QOpenGLBuffer *normals_buffer_;
  QOpenGLBuffer *uvs_buffer_;
};

#endif  // SCENE_H
