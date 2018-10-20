import Qt3D.Core 2.0
import Qt3D.Render 2.2
import Qt3D.Extras 2.0

import vr.test 2.20

RenderTarget {

    readonly property Texture2D colorTexture: Texture2D {
        width: 1512
        height: 1680
        wrapMode {
            x: WrapMode.ClampToEdge
            y: WrapMode.ClampToEdge
        }
        generateMipMaps: false
        magnificationFilter: Texture.Linear
        minificationFilter: Texture.Linear
        format: Texture.RGBA8_UNorm
    }

    readonly property Texture2D depthTexture: Texture2D {
        width: colorTexture.width
        height: colorTexture.height
        wrapMode {
            x: WrapMode.ClampToEdge
            y: WrapMode.ClampToEdge
        }
        generateMipMaps: false
        magnificationFilter: Texture.Linear
        minificationFilter: Texture.Linear
        format: Texture.D32F
    }

    attachments: [
        RenderTargetOutput {
            attachmentPoint: RenderTargetOutput.Color0
            texture: colorTexture
        },
        RenderTargetOutput {
            attachmentPoint: RenderTargetOutput.Depth
            texture: depthTexture
        }
    ]

}
