#ifndef WIDGET_H
#define WIDGET_H

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

struct VertexData
{
    VertexData()
    {
    }
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n)
    {

    }
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class Widget : public QOpenGLWidget
{
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *event); //pointer on clicking mouse
  void mouseMoveEvent(QMouseEvent *event); //pointer on moving mouse

  void initShaders(); // initialize shaders
  void initCube(float width); //initialize figure


private:
  QMatrix4x4 m_projectionMatrix; //create matrix
  QOpenGLShaderProgram m_program;
  QOpenGLTexture *m_texture; //pointer on texture
  QOpenGLBuffer m_arrayBuffer;
  QOpenGLBuffer m_indexBuffer;
  QVector2D m_mousePosition; //current location mouse when clicking
  QQuaternion m_rotation;

};
#endif // WIDGET_H
