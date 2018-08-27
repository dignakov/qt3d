/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

//import openvr.test 2.20

Entity {
    id: sceneRoot

    Entity {
        id: leftEyeCamera
        readonly property CameraLens lens: CameraLens { projectionMatrix: vrDeviceInstance.leftEyeProjectionMatrix }
        readonly property Transform transform: Transform { matrix: vrDeviceInstance.leftEyeViewMatrix }
        components: [lens, transform]
    }

    Entity {
        id: rightEyeCamera
        readonly property CameraLens lens: CameraLens { projectionMatrix: vrDeviceInstance.rightEyeProjectionMatrix }
        readonly property Transform transform: Transform { matrix: vrDeviceInstance.rightEyeViewMatrix }
        components: [lens, transform]
    }

    OpenVRDevice {
        id: vrDeviceInstance
        nearPlane : 0.1
        farPlane : 1000.0
        leftEyeTexture: leftEyeRenderTarget.colorTexture
        rightEyeTexture: rightEyeRenderTarget.colorTexture
    }

    EyeRenderTarget { id: leftEyeRenderTarget }
    EyeRenderTarget { id: rightEyeRenderTarget }

    components: [
        RenderSettings {
            activeFrameGraph: VRFrameGraph {
                clearColor: Qt.rgba(0, 0.5, 1, 1)
                leftCamera: leftEyeCamera
                rightCamera: rightEyeCamera
                leftEyeRenderTarget: leftEyeRenderTarget
                rightEyeRenderTarget: rightEyeRenderTarget
                vrDevice: vrDeviceInstance
//                debugLayer: debugLayerEntity.layer
            }
        },
        // Event Source will be set by the Qt3DQuickWindow
        InputSettings { }
    ]

    SceneEntity { }

//    DebugLayer {
//        id: debugLayerEntity
//        colorTexture: leftEyeRenderTarget.colorTexture
//        winSize: Qt.vector2d(1024, 768)
//    }
}
