/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "quniformvalue_p.h"

#include <Qt3DRender/private/graphicscontext_p.h>
#include <Qt3DRender/private/texture_p.h>

#include <Qt3DCore/private/qframeallocator_p.h>

#include <QOpenGLShaderProgram>
#include <QDebug>
#include <QColor>
#include <QQuaternion>
#include <Qt3DRender/private/renderlogging_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {
namespace Render {

QUniformValue::QUniformValue()
{
}

QUniformValue *QUniformValue::fromVariant(const QVariant &v, Qt3DCore::QFrameAllocator *allocator)
{
    QUniformValue *u = allocator->allocate<QUniformValue>();
    u->m_var = v;
    return u;
}

bool QUniformValue::operator ==(const QUniformValue &other)
{
    return (other.m_var == m_var);
}

bool QUniformValue::operator !=(const QUniformValue &other)
{
    return !operator ==(other);
}

void QUniformValue::apply(GraphicsContext *ctx, const ShaderUniform &shaderDescription) const
{
    ctx->bindUniform(m_var, shaderDescription);
}

ShaderParameterPack::~ShaderParameterPack()
{
    m_uniforms.clear();
}

void ShaderParameterPack::setUniform(const QString &glslName, const QUniformValue *val)
{
    m_uniforms.insert(glslName, val);
}

void ShaderParameterPack::setTexture(const QString &glslName, const Qt3DCore::QNodeId &texId)
{
    for (int t=0; t<m_textures.size(); ++t) {
        if (m_textures[t].glslName != glslName) {
            continue;
        }

        m_textures[t].texId = texId;
        return;
    }

    m_textures.append(NamedTexture(glslName, texId));
}

// Contains Uniform Block Index and QNodeId of the ShaderData (UBO)
void ShaderParameterPack::setUniformBuffer(const BlockToUBO &blockToUBO)
{
    m_uniformBuffers.append(blockToUBO);
}

void ShaderParameterPack::setShaderStorageBuffer(const BlockToSSBO &blockToSSBO)
{
    m_shaderStorageBuffers.push_back(blockToSSBO);
}

void TextureUniform::apply(GraphicsContext *ctx, const ShaderUniform &description) const
{
    // We assume that the texture has been successfully bound and attache to a texture unit
    if (m_textureUnit != -1) {
        ctx->bindUniform(m_textureUnit, description);
#if defined(QT3D_RENDER_ASPECT_OPENGL_DEBUG)
        int err = ctx->openGLContext()->functions()->glGetError();
        if (err) {
            qCWarning(Render::Backend, "Error %d after setting uniform \"%s\" at location %d",
                      err, qUtf8Printable(description.m_name), description.m_location);
        }
#endif
    } else {
        qCWarning(Render::Backend, "Invalid texture unit supplied for \"%s\"",
                  qUtf8Printable(description.m_name));
    }
}

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE
