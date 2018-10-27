
import QtQuick 2.2 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0


import vr.test 2.20

Entity {
    id: testSceneRoot

    Camera {
        id: camera
        projectionMatrix: Qt.matrix4x4(0.03,0,0,0,0,0.03,0,0,0,0,-0.002,-1,0,0,0,1)
//        projectionMatrix: Qt.matrix4x4(0.5,0,0,0,0,0.5,0,0,0,0,-0.002,-1,0,0,0,1)

    }

    OrbitCameraController {
        camera: camera
    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                clearColor: Qt.rgba(0, 0.5, 1, 1)
                camera: camera
            }
        },
        InputSettings { }
    ]


//    VRDevice {
//        id: vrDeviceInstance
//        nearPlane: 0.1
//        farPlane: 1000.0
//        leftEyeTexture: leftEyeRenderTarget.colorTexture
//        rightEyeTexture: rightEyeRenderTarget.colorTexture
//    }
//    EyeRenderTarget { id: leftEyeRenderTarget }
//    EyeRenderTarget { id: rightEyeRenderTarget }

    SceneEntity2 { }

}
