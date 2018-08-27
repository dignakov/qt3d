import QtQuick 2.2 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.2
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import openvr.test 2.20

Entity {

    SphereMesh {
        id: sphereMesh
        radius: 1
    }

    Entity {
        id: sphereEntity
        property Material material: PhongMaterial {
            diffuse: "red"
        }
        property Transform transform: Transform {
            scale: 0.125
            translation: Qt.vector3d(0, y, 0)
            property real y: 1.5
            QQ2.NumberAnimation on y {
                duration: 1500
                to: 0.25
                from: 1.5
                loops: QQ2.Animation.Infinite
                easing.type: QQ2.Easing.InOutBounce
            }
        }
        components: [ sphereMesh, material, transform ]
    }

    Entity {
        id: sphereEntity2
        property Transform transform: Transform {
            scale: 0.125
            translation: Qt.vector3d(0.0, 1.5, 1.0)
        }
        property Material material: PhongMaterial {
            diffuse: "white"
        }
        components: [ sphereMesh, material, transform ]
    }
    Entity {
        id: sphereEntity3
        property Transform transform: Transform {
            scale: 0.125
            translation: Qt.vector3d(0.0, 1.5, -1.0)
        }
        property Material material: PhongMaterial {
            diffuse: "blue"
        }
        components: [ sphereMesh, material, transform ]
    }

}
