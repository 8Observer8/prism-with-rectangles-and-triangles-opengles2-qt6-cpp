#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include <QtGui/QOpenGLFunctions>
#include <QtCore/QStack>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QVector3D>
#include <QtOpenGL/QOpenGLBuffer>
#include <QtOpenGL/QOpenGLShaderProgram>
#include <QtOpenGL/QOpenGLTexture>
#include <QtOpenGL/QOpenGLWindow>

enum ObjectToDraw
{
    Rect,
    Triangle
};

class OpenGLWindow : public QOpenGLWindow, private QOpenGLFunctions
{
public:
    OpenGLWindow();

private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void closeEvent(QCloseEvent *event) override;

    void pushMatrix(const QMatrix4x4 &matrix);
    QMatrix4x4 popMatrix();
    void draw(ObjectToDraw objectToDraw, const QVector3D &p, const QVector3D &s,
        const QVector3D &r, float a, const QVector3D &r2 = QVector3D(0.f, 0.f, 0.f),
        float a2 = 0.f);
    void initVertexBuffers();

private:
    QOpenGLShaderProgram m_program;
    int m_aPositionLocation;
    int m_aNormalLocation;
    int m_aTexCoordLocation;
    int m_uMvpMatrixLocation;
    int m_uNormalMatrixLocation;
    QOpenGLBuffer m_vertPosBuffer;
    QOpenGLBuffer m_normalBuffer;
    QOpenGLBuffer m_texCoordBuffer;
    QOpenGLTexture m_texture;
    QMatrix4x4 m_projMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_modelMatrix;
    QStack<QMatrix4x4> m_matrixStack;
};

#endif // OPENGL_WINDOW_H
