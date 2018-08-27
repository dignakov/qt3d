import Qt3D.Core 2.0
import Qt3D.Render 2.2
import Qt3D.Extras 2.0
import QtQuick 2.0 as QQ2

import openvr.test 2.20

RenderSurfaceSelector {
    id: root
    property alias leftCamera: leftCameraSelector.camera
    property alias rightCamera: rightCameraSelector.camera
    property alias leftEyeRenderTarget: leftRenderTargetSelector.target
    property alias rightEyeRenderTarget: rightRenderTargetSelector.target
    property color clearColor: "white"
    property alias vrDevice: vrSubmitCommand.device
//    property Layer debugLayer: debugLayerFilter.layers


    Viewport {
        normalizedRect: Qt.rect(0, 0, 1, 1)

        //        // Render Scene in Window for debugging
        //        CameraSelector {
        //            camera: leftCameraSelector.camera
        //        }

        // Render Scene seen from Left eye into FBO
        RenderTargetSelector {
            id: leftRenderTargetSelector
            CameraSelector {
                id: leftCameraSelector

                ClearBuffers {
                    id: clearBufferLeft
                    clearColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
//                    QQ2.ColorAnimation on clearColor { from: "red"; to: "blue"; duration: 500; loops: QQ2.Animation.Infinite }
                    buffers: ClearBuffers.ColorDepthBuffer
                }
            }
        }

        // Render Scene seen from Right eye into FBO
        RenderTargetSelector {
            id: rightRenderTargetSelector
            CameraSelector {
                id: rightCameraSelector

                ClearBuffers {
                    clearColor: clearBufferLeft.clearColor
                    buffers: ClearBuffers.ColorDepthBuffer
                }
            }
        }

//        // DebugLayer
//        LayerFilter {
//            id: debugLayerFilter
//            ClearBuffers {
//                clearColor: "red"
//                buffers: ClearBuffers.ColorDepthBuffer
//                //                NoDraw {}
////                CameraSelector {
////                    camera: leftCameraSelector.camera
//                    RenderPassFilter {
//                        matchAny : FilterKey { name : "pass"; value : "debug" }
//                    }
////                }
//            }
//        }


        // Submit the FBOs to OpenVR
        OpenVRSubmitCommand { id: vrSubmitCommand }
    }
}
