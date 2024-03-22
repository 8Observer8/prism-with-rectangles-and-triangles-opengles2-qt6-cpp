#include <QtGui/QSurfaceFormat>

#include "opengl_window.h"

OpenGLWindow::OpenGLWindow()
    : m_texture(QOpenGLTexture::Target::Target2D)
{
    setTitle("OpenGL ES 2.0, Qt6, C++");
    resize(400, 400);

    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(8);
    setFormat(surfaceFormat);
}

void OpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.5f, 1.f);
    glEnable(GL_DEPTH_TEST);

    m_program.create();
    m_program.addShaderFromSourceFile(QOpenGLShader::ShaderTypeBit::Vertex,
        ":/assets/shaders/texture.vert");
    m_program.addShaderFromSourceFile(QOpenGLShader::ShaderTypeBit::Fragment,
        ":/assets/shaders/texture.frag");
    m_program.link();
    m_program.bind();

    m_aPositionLocation = m_program.attributeLocation("aPosition");
    m_aNormalLocation = m_program.attributeLocation("aNormal");
    m_aTexCoordLocation = m_program.attributeLocation("aTexCoord");
    m_uMvpMatrixLocation = m_program.uniformLocation("uMvpMatrix");
    m_uNormalMatrixLocation = m_program.uniformLocation("uNormalMatrix");

    m_viewMatrix.lookAt(
        QVector3D(0.75f, 1.75f, 2.25f), // eye position
        QVector3D(0.f, 0.f, 0.f),       // center
        QVector3D(0.f, 1.f, 0.f));      // up

    m_texture.create();
    m_texture.setData(QImage(":/assets/textures/texture.png"));
    m_texture.setMinMagFilters(QOpenGLTexture::Filter::Linear,
        QOpenGLTexture::Filter::Linear);
    m_texture.setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);

    initVertexBuffers();
}

void OpenGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.bind();
    m_texture.bind();

    m_vertPosBuffer.bind();
    m_program.setAttributeBuffer(m_aPositionLocation, GL_FLOAT, 0, 3);
    m_program.enableAttributeArray(m_aPositionLocation);

    m_normalBuffer.bind();
    m_program.setAttributeBuffer(m_aNormalLocation, GL_FLOAT, 0, 3);
    m_program.enableAttributeArray(m_aNormalLocation);

    m_texCoordBuffer.bind();
    m_program.setAttributeBuffer(m_aTexCoordLocation, GL_FLOAT, 0, 2);
    m_program.enableAttributeArray(m_aTexCoordLocation);

    pushMatrix(m_modelMatrix);
    {
        m_modelMatrix.translate(0.f, 0.f, 0.f);
        // Top
        draw(Triangle, QVector3D(0.f, 0.5f, 0.f), QVector3D(1.f, 1.f, 1.f), QVector3D(1.f, 0.f, 0.f), -90.f);
        draw(Triangle, QVector3D(0.f, 0.5f, 0.f), QVector3D(1.f, 1.f, 1.f), QVector3D(1.f, 0.f, 0.f), -90.f, QVector3D(0.f, 1.f, 0.f), -60.f);
        draw(Triangle, QVector3D(0.f, 0.5f, 0.f), QVector3D(1.f, 1.f, 1.f), QVector3D(1.f, 0.f, 0.f), -90.f, QVector3D(0.f, 1.f, 0.f), -120.f);
        draw(Triangle, QVector3D(0.f, 0.5f, 0.f), QVector3D(1.f, 1.f, 1.f), QVector3D(1.f, 0.f, 0.f), -90.f, QVector3D(0.f, 1.f, 0.f), -180.f);
        draw(Triangle, QVector3D(0.f, 0.5f, 0.f), QVector3D(1.f, 1.f, 1.f), QVector3D(1.f, 0.f, 0.f), -90.f, QVector3D(0.f, 1.f, 0.f), -240.f);
        draw(Triangle, QVector3D(0.f, 0.5f, 0.f), QVector3D(1.f, 1.f, 1.f), QVector3D(1.f, 0.f, 0.f), -90.f, QVector3D(0.f, 1.f, 0.f), -300.f);
        // Side
        draw(Rect, QVector3D(0.f, 0.f, 0.866f), QVector3D(1.f, 1.f, 1.f), QVector3D(0.f, 1.f, 0.f), 0.f);
        draw(Rect, QVector3D(0.f, 0.f, 0.866f), QVector3D(1.f, 1.f, 1.f), QVector3D(0.f, 1.f, 0.f), 60.f);
        draw(Rect, QVector3D(0.f, 0.f, 0.866f), QVector3D(1.f, 1.f, 1.f), QVector3D(0.f, 1.f, 0.f), 120.f);
        draw(Rect, QVector3D(0.f, 0.f, 0.866f), QVector3D(1.f, 1.f, 1.f), QVector3D(0.f, 1.f, 0.f), 180.f);
        draw(Rect, QVector3D(0.f, 0.f, 0.866f), QVector3D(1.f, 1.f, 1.f), QVector3D(0.f, 1.f, 0.f), 240.f);
        draw(Rect, QVector3D(0.f, 0.f, 0.866f), QVector3D(1.f, 1.f, 1.f), QVector3D(0.f, 1.f, 0.f), 300.f);
    }
    m_modelMatrix = popMatrix();
}

void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_projMatrix.setToIdentity();
    m_projMatrix.perspective(50.f, (float)width() / height(), 0.1f, 100.f);
}

void OpenGLWindow::pushMatrix(const QMatrix4x4 &matrix)
{
    m_matrixStack.push(matrix);
}

QMatrix4x4 OpenGLWindow::popMatrix()
{
    QMatrix4x4 matrix = m_matrixStack.top();
    m_matrixStack.pop();
    return matrix;
}

void OpenGLWindow::draw(ObjectToDraw objectToDraw, const QVector3D &p, const QVector3D &s,
    const QVector3D &r, float a, const QVector3D &r2, float a2)
{
    QMatrix4x4 mvpMatrix;
    QMatrix4x4 normalMatrix;

    switch (objectToDraw)
    {
    case ObjectToDraw::Rect:
        pushMatrix(m_modelMatrix);
        {
            m_modelMatrix.rotate(a, r);
            m_modelMatrix.translate(p);
            m_modelMatrix.scale(s);
            mvpMatrix = m_projMatrix * m_viewMatrix * m_modelMatrix;
            m_program.bind();
            m_program.setUniformValue(m_uMvpMatrixLocation, mvpMatrix);
            normalMatrix = m_modelMatrix.inverted();
            normalMatrix = normalMatrix.transposed();
            m_program.setUniformValue(m_uNormalMatrixLocation, normalMatrix);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        m_modelMatrix = popMatrix();
        break;
    case ObjectToDraw::Triangle:
        pushMatrix(m_modelMatrix);
        {
            m_modelMatrix.translate(p);
            m_modelMatrix.rotate(a2, r2);
            m_modelMatrix.rotate(a, r);
            m_modelMatrix.scale(s);
            mvpMatrix = m_projMatrix * m_viewMatrix * m_modelMatrix;
            m_program.bind();
            m_program.setUniformValue(m_uMvpMatrixLocation, mvpMatrix);
            normalMatrix = m_modelMatrix.inverted();
            normalMatrix = normalMatrix.transposed();
            m_program.setUniformValue(m_uNormalMatrixLocation, normalMatrix);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 3);
        }
        m_modelMatrix = popMatrix();
        break;
    }
}

void OpenGLWindow::initVertexBuffers()
{
    float vertPositions[] = {
        -0.5f, -0.5f, 0.f, // Rectangle
        0.5f, -0.5f, 0.f,
        -0.5f, 0.5f, 0.f,
        0.5f, 0.5f, 0.f,
        0.f, 0.f, 0.f, // Triangle
        -0.5f, -0.866f, 0.f,
        0.5f, -0.866f, 0.f
    };
    m_vertPosBuffer.create();
    m_vertPosBuffer.bind();
    m_vertPosBuffer.allocate(vertPositions, sizeof(vertPositions));

    float normals[] = {
        0.f, 0.f, 1.f, // Rectangle
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f, // Triangle
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f
    };
    m_normalBuffer.create();
    m_normalBuffer.bind();
    m_normalBuffer.allocate(normals, sizeof(normals));

    float texCoords[] = {
        0.f, 1.f, // Rectangle
        1.f, 1.f,
        0.f, 0.f,
        1.f, 0.f,
        0.5f, 0.f, // Triangle
        0.f, 1.f,
        1.f, 1.f
    };
    m_texCoordBuffer.create();
    m_texCoordBuffer.bind();
    m_texCoordBuffer.allocate(texCoords, sizeof(texCoords));
}

void OpenGLWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    m_texture.destroy();
}
