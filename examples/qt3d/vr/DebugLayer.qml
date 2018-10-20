import Qt3D.Core 2.0
import Qt3D.Render 2.2
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import vr.test 2.20

Entity {
    id: root
    property vector2d winSize
    property Texture2D colorTexture
    readonly property Layer layer: Layer {}

    PlaneMesh {
        id: planeMesh
        width: 2.0
        height: 2.0
        meshResolution: Qt.size(2, 2)
    }

    Transform { // We rotate the plane so that it faces us
        id: transform
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
    }

    Material {
        id: material

        parameters: [
            Parameter { name: "winSize"; value : root.winSize },
            Parameter { name: "color"; value : root.colorTexture }
        ]

        effect: Effect {
            techniques : [
                // OpenGL 3.1
                Technique {
                    graphicsApiFilter {api : GraphicsApiFilter.OpenGL; profile : GraphicsApiFilter.CoreProfile; minorVersion : 2; majorVersion : 3 }
                    renderPasses : RenderPass {
                        filterKeys : FilterKey { name : "pass"; value : "debug" }
                        shaderProgram : ShaderProgram {
                            vertexShaderCode:
                            "
                            #version 140

                            in vec4 vertexPosition;
                            uniform mat4 modelMatrix;

                            void main()
                            {
                                gl_Position = modelMatrix * vertexPosition;
                            }
                            "
                            fragmentShaderCode:
                            "
                            #version 140

                            uniform sampler2D color;
                            uniform vec2 winSize;

                            out vec4 fragColor;

                            void main()
                            {
                                vec2 texCoord = gl_FragCoord.xy / winSize;
                                vec4 col = texture(color, texCoord);
                                fragColor = vec4(texCoord, 0.0, 1.0);
                            }
                            "
                        }
                    }
                }]
        }
    }

    components: [
        planeMesh,
        transform,
        material,
        layer
    ]
}
