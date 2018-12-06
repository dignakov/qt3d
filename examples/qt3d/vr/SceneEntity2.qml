import QtQuick 2.2 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

Entity {

    PhongMaterial {
        id: material
    }

    TorusMesh {
        id: torusMesh
        radius: 0.1
        minorRadius: 0.05
        rings: 100
        slices: 20
    }

    SphereMesh {
        id: sphereMesh
        radius: 0.05
    }

    Transform {
        id: torusTransform
        scale3D: Qt.vector3d(1, 1, 1)
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
        translation: Qt.vector3d(0,0,0)
    }


    Transform {
        id: sphereTransform
        property real userAngle: 0.0
        translation: Qt.vector3d(0.15,0,0.3)
//        matrix: {
//            var m = Qt.matrix4x4();
//            m.translate(Qt.vector3d(0,0,-1))
//            m.rotate(userAngle, Qt.vector3d(0, 0, 1));
//            m.translate(Qt.vector3d(1, 0, 0));
//            return m;
//        }
    }

//    QQ2.NumberAnimation {
//        target: sphereTransform
//        property: "userAngle"
//        duration: 10000
//        from: 0
//        to: 360

//        loops: QQ2.Animation.Infinite
//        running: true
//    }

    Entity {
        id: sphereEntity
        components: [ sphereMesh, material, sphereTransform ]
    }

    Entity {
        id: torusEntity
        components: [ torusMesh, material, torusTransform ]
    }

}
