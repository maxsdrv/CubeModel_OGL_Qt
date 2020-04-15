#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent), m_texture(0),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer) {}

Widget::~Widget() {}

void Widget::initializeGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  initShaders();
  initCube(1.0f);
}

void Widget::resizeGL(int w, int h) {
  float aspect = w / (float)h;

  m_projectionMatrix.setToIdentity();
  m_projectionMatrix.perspective(45, aspect, 0.1f, 10.0f);
}

void Widget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 viewMatrix;
  viewMatrix.setToIdentity(); //make it single
  viewMatrix.translate(0.0, 0.0, -5.0); // move the camera back to 5.0
  // modelViewMatrix.rotate(45, 0.0, 0.0, 0.0);
  viewMatrix.rotate(m_rotation);

  m_texture->bind(0);

  QMatrix4x4 modelMatrix;
  modelMatrix.setToIdentity();

  m_program.bind();

  m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
  m_program.setUniformValue("u_viewMatrix", viewMatrix);
  m_program.setUniformValue("u_modelMatrix", modelMatrix);
  m_program.setUniformValue("u_texture", 0);
  m_program.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
  m_program.setUniformValue("u_lightPower", 5.0f);
  m_arrayBuffer.bind();

  int offset = 0;

  int vertLoc = m_program.attributeLocation("a_position");
  m_program.enableAttributeArray(vertLoc);
  m_program.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

  offset += sizeof(QVector3D);

  int texLoc = m_program.attributeLocation("a_texcoord");
  m_program.enableAttributeArray(texLoc);
  m_program.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

  offset += sizeof (QVector2D);

  int normLoc = m_program.attributeLocation("a_normal");
  m_program.enableAttributeArray(normLoc);
  m_program.setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof (VertexData));

  m_indexBuffer.bind();

  glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
}

void Widget::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    m_mousePosition = QVector2D(event->localPos());
  }
  event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() != Qt::LeftButton)
    return;

  QVector2D diff = QVector2D(event->localPos()) -
                   m_mousePosition; //create a delta(difference) between
                                    //the current location
  m_mousePosition = QVector2D(event->localPos()); // save current location

  float angle = diff.length() / 2.0;

  QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);

  m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;
  update();
}

void Widget::initShaders() {
  if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/vshader.vsh"))
    close();
  if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/fshader.fsh"))
    close();
  if (m_program.link())
    close();
}

void Widget::initCube(float width) {
  float width_div_2 = width / 2.0f;
  ;
  QVector<VertexData> vertexes;
  // axis Z
  vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                             QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                             QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                             QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                             QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
  // axis X

  vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                             QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                             QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                             QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                             QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

  // axis Y
  vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                             QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                             QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                             QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                             QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

  // axis -Z
  vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                             QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                             QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                             QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
  vertexes.append(
      VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                 QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

  // axis -X
  vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                             QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                             QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                             QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
  vertexes.append(
      VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                 QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

  // axis -Y
  vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                             QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(
      VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                 QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                             QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
  vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                             QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

  QVector<GLuint> indexes;
  for (int i = 0; i < 24; i += 4) {
    indexes.append(i + 0);
    indexes.append(i + 1);
    indexes.append(i + 2);
    indexes.append(i + 2);
    indexes.append(i + 1);
    indexes.append(i + 3);
  }
  m_arrayBuffer.create();
  m_arrayBuffer.bind();
  m_arrayBuffer.allocate(vertexes.constData(),
                         vertexes.size() * sizeof(VertexData));
  m_arrayBuffer.release();

  m_indexBuffer.create();
  m_indexBuffer.bind();
  m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
  m_indexBuffer.release();

  m_texture = new QOpenGLTexture(QImage(":/cube2.png").mirrored());

  // Set nearest filtering mode for texture minification
  m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

  // Set bilinear filtering mode for texture magnification
  m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

  // Wrap texture coordinates by repeating
  // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
  m_texture->setWrapMode(QOpenGLTexture::Repeat);
}
