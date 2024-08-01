#include "scene.h"

Scene::~Scene() {
  delete shader_program_;
  delete light_shader_program_;
  delete normals_shader_program_;
  delete vertices_buffer_;
  delete normals_buffer_;
  delete uvs_buffer_;
  delete vao_base_;
  delete vao_light_;
  delete vao_normal_vectors_;
}

Scene::Scene(QWidget *parent)
    : QOpenGLWidget{parent},
      model_count_(0),
      config_(nullptr),
      mesh_model_(MeshModel{}),
      shader_program_(new QOpenGLShaderProgram()),
      light_shader_program_(new QOpenGLShaderProgram()),
      normals_shader_program_(new QOpenGLShaderProgram()),
      vao_base_(new QOpenGLVertexArrayObject()),
      vao_light_(new QOpenGLVertexArrayObject()),
      vao_normal_vectors_(new QOpenGLVertexArrayObject()),
      vertices_buffer_(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer)),
      normals_buffer_(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer)),
      uvs_buffer_(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer)) {}

std::string glErrorToString(GLenum error) {
  switch (error) {
    case GL_NO_ERROR:
      return "No error";
    case GL_INVALID_ENUM:
      return "Invalid enum";
    case GL_INVALID_VALUE:
      return "Invalid value";
    case GL_INVALID_OPERATION:
      return "Invalid operation";
    case GL_STACK_OVERFLOW:
      return "Stack overflow";
    case GL_STACK_UNDERFLOW:
      return "Stack underflow";
    case GL_OUT_OF_MEMORY:
      return "Out of memory";
    default:
      return "Unknown error";
  }
}

void checkGLError(const char *file, int line) {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    qDebug() << "OpenGL error (" << glErrorToString(error) << ") at " << file
             << ":" << line;
  }
}

void Scene::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  CreateVaos();
  CreateBuffers();
}

void Scene::CreateVaos() {
  vao_base_->create();
  vao_light_->create();
  vao_normal_vectors_->create();
}

bool Scene::CreateLightShaderProgram(const QString &lightshader_vert,
                                     const QString &lightshader_frag) {
  // light program
  light_shader_program_->create();
  light_shader_program_->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                 lightshader_vert);
  light_shader_program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 lightshader_frag);

  if (!light_shader_program_->link()) {
    return false;
  }

  vao_light_->bind();
  // save data buffers attributes for light program
  light_shader_program_->bind();

  int light_pos_location = light_shader_program_->attributeLocation("position");
  int light_normal_location =
      light_shader_program_->attributeLocation("normal");
  int light_uv_location = light_shader_program_->attributeLocation("uv");

  vertices_buffer_->bind();
  light_shader_program_->enableAttributeArray(light_pos_location);
  light_shader_program_->setAttributeBuffer(light_pos_location, GL_FLOAT,
                                            sizeof(QVector4D), 3,
                                            sizeof(QVector4D) * 2);
  vertices_buffer_->release();

  normals_buffer_->bind();
  light_shader_program_->enableAttributeArray(light_normal_location);
  light_shader_program_->setAttributeBuffer(light_normal_location, GL_FLOAT, 0,
                                            3, sizeof(QVector3D));

  normals_buffer_->release();
  uvs_buffer_->bind();
  light_shader_program_->enableAttributeArray(light_uv_location);
  light_shader_program_->setAttributeBuffer(light_uv_location, GL_FLOAT, 0, 2,
                                            sizeof(QVector2D));
  uvs_buffer_->release();
  light_shader_program_->release();
  vao_light_->release();
  return true;
}

void Scene::CreateBuffers() {
  // data buffers on gpu
  vertices_buffer_->create();
  vertices_buffer_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
  normals_buffer_->create();
  normals_buffer_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
  uvs_buffer_->create();
  uvs_buffer_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
}

bool Scene::SetShaderPrograms(const QString &baseshader_vert,
                              const QString &baseshader_frag,
                              const QString &lightshader_vert,
                              const QString &lightshader_frag,
                              const QString &normalsshader_vert,
                              const QString &normalsshader_frag) {
  return CreateBaseShaderProgram(baseshader_vert, baseshader_frag) &&
         CreateLightShaderProgram(lightshader_vert, lightshader_frag) &&
         CreateNormalsShaderProgram(normalsshader_vert, normalsshader_frag);
}

bool Scene::CreateNormalsShaderProgram(
    const QString &normalsshader_vert,
    const QString &normalsshader_frag) {  // base program
  normals_shader_program_->create();
  normals_shader_program_->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                   normalsshader_vert);
  normals_shader_program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                   normalsshader_frag);

  if (!normals_shader_program_->link()) {
    return false;
  }
  vao_normal_vectors_->bind();
  normals_shader_program_->bind();
  vertices_buffer_->bind();

  int vertex_location = normals_shader_program_->attributeLocation("vertex");
  int start_vector_location =
      normals_shader_program_->attributeLocation("start_vector");

  normals_shader_program_->enableAttributeArray(vertex_location);
  normals_shader_program_->setAttributeBuffer(vertex_location, GL_FLOAT, 0, 4,
                                              sizeof(QVector4D));

  normals_shader_program_->enableAttributeArray(start_vector_location);
  normals_shader_program_->setAttributeBuffer(
      start_vector_location, GL_FLOAT, sizeof(QVector4D), 3, sizeof(QVector4D));
  vertices_buffer_->release();
  normals_shader_program_->release();
  vao_normal_vectors_->release();
  return true;
}

bool Scene::CreateBaseShaderProgram(const QString &baseshader_vert,
                                    const QString &baseshader_frag) {
  // base program
  shader_program_->create();
  shader_program_->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           baseshader_vert);
  shader_program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           baseshader_frag);

  if (!shader_program_->link()) {
    return false;
  }
  // save data buffers attributes for base program
  vao_base_->bind();
  shader_program_->bind();
  vertices_buffer_->bind();
  int pos_location = shader_program_->attributeLocation("position");
  int uv_location = shader_program_->attributeLocation("uv");

  shader_program_->enableAttributeArray(pos_location);
  shader_program_->setAttributeBuffer(pos_location, GL_FLOAT, sizeof(QVector4D),
                                      3, sizeof(QVector4D) * 2);
  vertices_buffer_->release();

  uvs_buffer_->bind();
  shader_program_->enableAttributeArray(uv_location);
  shader_program_->setAttributeBuffer(uv_location, GL_FLOAT, 0, 2,
                                      sizeof(QVector2D));
  uvs_buffer_->release();

  shader_program_->release();
  vao_base_->release();
  return true;
}

void Scene::UpdateBuffers() {
  // Update vertices + normal_vectors buffer
  vertices_buffer_->bind();
  vertices_buffer_->allocate(mesh_model_.Vertices().data(),
                             mesh_model_.Vertices().size() * sizeof(QVector4D));
  vertices_buffer_->release();

  // Update normals buffer
  normals_buffer_->bind();
  normals_buffer_->allocate(mesh_model_.Normals().data(),
                            mesh_model_.Normals().size() * sizeof(QVector3D));
  normals_buffer_->release();
  // Update texture coords buffer
  uvs_buffer_->bind();
  uvs_buffer_->allocate(mesh_model_.TextureCoords().data(),
                        mesh_model_.TextureCoords().size() * sizeof(QVector2D));
  uvs_buffer_->release();
}

void Scene::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);

  checkGLError(__FILE__, __LINE__);
}

void Scene::SetConfig(const Config *config) { config_ = config; }

void Scene::paintGL() {
  if (config_ == nullptr) return;

  glClearColor(config_->BgColor().redF(), config_->BgColor().greenF(),
               config_->BgColor().blueF(), 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (model_count_ == 0) return;

  if (config_->LightMode() || config_->NeonMode())
    RenderSceneWithLight();
  else
    RenderBasicScene();

  if (config_->NormalsMode()) {
    vao_normal_vectors_->bind();
    mesh_model_.RenderNormalVectors(normals_shader_program_);
    vao_normal_vectors_->release();
  }
}

void Scene::AddMeshModel(const MeshModel &mesh_model) {
  mesh_model_ = mesh_model;
  model_count_ = 1;
  UpdateBuffers();
  update();
}

void Scene::RemoveMeshModel() {
  mesh_model_.Clear();
  model_count_ = 0;
  update();
}

void Scene::RenderBasicScene() {
  vao_base_->bind();
  mesh_model_.Render(shader_program_);
  vao_base_->release();
}

void Scene::RenderSceneWithLight() {
  vao_light_->bind();
  mesh_model_.Render(light_shader_program_);
  vao_light_->release();
}
