#include "vrdevice_p.h"
#include <QPluginLoader>
#include <Qt3DRender/qvrdevice.h>
#include <Qt3DCore/qpropertyupdatedchange.h>
//#include <openvr.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

//namespace {

////inline QMatrix4x4 hmdMatrix4x4ToQMatrix(const vr::HmdMatrix44_t &mVr)
////{
////    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], mVr.m[3][0],
////                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], mVr.m[3][1],
////                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], mVr.m[3][2],
////                      mVr.m[0][3], mVr.m[1][3], mVr.m[2][3], mVr.m[3][3]);
////}

////inline QMatrix4x4 hmdMatrix3x4ToQMatrix(const vr::HmdMatrix34_t &mVr)
////{
////    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], 0.0f,
////                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], 0.0f,
////                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], 0.0f,
////                      mVr.m[0][3], mVr.m[1][3], mVr.m[2][3], 1.0f);
////}

//} // anonymous

VRDevice::VRDevice()
    : BackendNode(ReadWrite)
{
}

void VRDevice::initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change)
{

    qDebug() << "[VRDevice::initializeFromPeer()]";
    const auto typedChange = qSharedPointerCast<Qt3DCore::QNodeCreatedChange<QVRDeviceData>>(change);
    const QVRDeviceData &data = typedChange->data;
    m_leftEyeTextureId = data.leftEyeTextureId;
    m_rightEyeTextureId = data.rightEyeTextureId;
    m_nearPlane = data.nearPlane;
    m_farPlane = data.farPlane;

    m_pluginLocation = data.pluginLocation;


    t1_submit = std::chrono::high_resolution_clock::now();
    t2_submit = std::chrono::high_resolution_clock::now();
    t1_update = std::chrono::high_resolution_clock::now();
    t2_update = std::chrono::high_resolution_clock::now();
    t1_render = std::chrono::high_resolution_clock::now();
    t2_render = std::chrono::high_resolution_clock::now();

    //MAKE THIS MAKE SENSE EVENTIALLY
//    m_plugin_location = "/home/dmitri/Code/hmd2_test/build/libglxplugin_test.so";
//    m_plugin_location = "/home/dmitri/Code/plugin_test/bin/libmy_plugin.so";

}

void VRDevice::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e)
{
    switch (e->type()) {
    case Qt3DCore::PropertyUpdated: {
        const auto change = qSharedPointerCast<Qt3DCore::QPropertyUpdatedChange>(e);
        if (change->propertyName() == QByteArrayLiteral("nearPlane")) {
            m_nearPlane = change->value().toFloat();
            //qDebug() << "nearPlane udated" << m_nearPlane;
        } else if (change->propertyName() == QByteArrayLiteral("farPlane")) {
            m_farPlane = change->value().toFloat();
            //qDebug() << "farPlane udated" << m_farPlane;
        } else if (change->propertyName() == QByteArrayLiteral("leftEyeTexture")) {
            m_leftEyeTextureId = change->value().value<Qt3DCore::QNodeId>();
            //qDebug() << "leftEyeTextureId udated" << m_leftEyeTextureId;
        } else if (change->propertyName() == QByteArrayLiteral("rightEyeTexture")) {
            m_rightEyeTextureId = change->value().value<Qt3DCore::QNodeId>();
            //qDebug() << "rightEyeTextureId udated" << m_rightEyeTextureId;
        }
        break;
    }
    }
}

// Called from a Job
void VRDevice::updatePoses()
{
//    qDebug() << "[VRDevice::updatePose()] init: "<<m_vrInitialized;
    if (!m_vrInitialized)
        return;
    count_update+=1;
    t1_update = t2_update;
    t2_update = std::chrono::high_resolution_clock::now();
    float time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_update - t1_update).count()/1000000000.0;
    printf("[VRDevice::updatePose(%d)]: dt: %0.2f\n",count_update, time_span*1000);
    // TO DO: Retrieve Poses
//    int validPoseCount = 0;

    //call plugin for updates, calcualtes everything it need to inside
    m_vrplugin->updateVR();
    m_headPosMatrix = m_vrplugin->getHmdPose();
    qDebug()<<"DEVICE"<<m_headPosMatrix.column(3);
    //TODO: hand tracker poses

    auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
    e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
    e->setPropertyName("headsetViewMatrix");
    e->setValue(QVariant::fromValue(m_headPosMatrix));
    notifyObservers(e);

    {
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
        e->setPropertyName("leftEyeProjectionMatrix");
        e->setValue(QVariant::fromValue(m_leftEyeProjection));
        notifyObservers(e);
    }
    {
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
        e->setPropertyName("rightEyeProjectionMatrix");
        e->setValue(QVariant::fromValue(m_rightEyeProjection));
        notifyObservers(e);
    }
    {
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
        e->setPropertyName("leftViewMatrix");
//        e->setValue(QVariant::fromValue((m_leftEyePosMatrix * m_headPosMatrix)));
        e->setValue(QVariant::fromValue((m_headPosMatrix*m_leftEyePosMatrix)));
//        qDebug()<<"LHM: " << (m_leftEyePosMatrix*m_headPosMatrix);
//        qDebug() << m_leftEyePosMatrix;
        notifyObservers(e);
    }
    {
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
        e->setPropertyName("rightViewMatrix");
//        e->setValue(QVariant::fromValue((m_rightEyePosMatrix * m_headPosMatrix)));
        e->setValue(QVariant::fromValue(( m_headPosMatrix*m_rightEyePosMatrix)));
//        qDebug()<<"RHM: " << (m_headPosMatrix * m_rightEyePosMatrix);
        notifyObservers(e);
    }
}



/* ### HERE ###
    if(!QLibrary::isLibrary(PLUGIN_LOC)){
        printf("COULD NOT LOAD LIBRARY\n");
        return 1;
    }
    QPluginLoader pluginLoader(PLUGIN_LOC);
    QObject *plugin = pluginLoader.instance();

    if(!plugin){
        printf("COULD NOT INSTANCIATE PLUGIN\n");
        return 1;
    }

    VR::Plugin::IVRDeviceImplementation* vrDevice;
    vrDevice = qobject_cast<VR::Plugin::IVRDeviceImplementation*>(plugin);
    if(!vrDevice){
        printf("INVALID INTERFACE IMPLEMENTED BY PLUGIN \n");
        return 1;
    }
*/



// Called by Render Thread
void VRDevice::initializeVR()
{

    qDebug() << "[VRDevice::intializeVR]";
    m_vrInitialized = false;

    //load and initialize plugin
    if(!m_vrplugin_ready){
        if(!QLibrary::isLibrary(m_pluginLocation)){
            qDebug()<<"[VRDevice::intializeVR] Could not find plugin";
            m_vrInitialized = false;
            return;
        }
        QPluginLoader pluginLoader(m_pluginLocation);
        QObject *plugin_tmp = pluginLoader.instance();
        if(!plugin_tmp){
            qDebug()<<"[VRDevice::intializeVR] Could not load plugin";
            m_vrInitialized = false;
            return;
        }

        m_vrplugin = qobject_cast<VR::Plugin::IVRDeviceImplementation *>(plugin_tmp);
        if(!m_vrplugin){
            qDebug()<<"[VRDevice::intializeVR] Could not cast plugin";
            m_vrInitialized = false;
            return;
        }
        m_vrplugin_ready = true;
    }
    if(!m_vrplugin_ready){
        qDebug()<<"[VRDevice::intializeVR] should not be reachable !!!";
        m_vrInitialized = false;
        return;
    }
    if(m_vrplugin->initializeVR() != 0){
        qDebug()<<"[VRDevice::intializeVR] Could not initialize VRDevice";
        m_vrInitialized = false;
        return;
    }

    //read in values from plugin
    m_leftEyeProjection = m_vrplugin->getEyeProjection(VR::eyeLeft);
    m_rightEyeProjection = m_vrplugin->getEyeProjection(VR::eyeRight);
    m_leftEyePosMatrix = m_vrplugin->getEyePose(VR::eyeLeft);
    m_rightEyePosMatrix = m_vrplugin->getEyePose(VR::eyeRight);

    m_vrInitialized = true;
    qDebug() << "[VRDevice::intializeVR] SUCCESS";
}

// Called by RenderThread
void VRDevice::submitVR(uint leftEyeTextureId, uint rightEyeTextureId)
{
//    qDebug() << "[VRDevice::submitVR]";

    count_submit+=1;
    t1_submit = t2_submit;
    t2_submit = std::chrono::high_resolution_clock::now();
    float time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_submit - t1_submit).count()/1000000000.0;
    printf("[VRDevice::submitVR(%d)]: dt: %0.2f\n",count_submit, time_span*1000);
    m_vrplugin->submitVR(leftEyeTextureId, rightEyeTextureId);
}

void VRDevice::setLeftEyeProjection(QMatrix4x4 mat){
    m_leftEyeProjection = mat;
}

void VRDevice::setRightEyeProjection(QMatrix4x4 mat){
    m_rightEyeProjection = mat;
}

void VRDevice::setLeftEyePose(QMatrix4x4 mat){
    m_leftEyePosMatrix = mat;
}

void VRDevice::setRightEyePose(QMatrix4x4 mat){
    m_rightEyePosMatrix = mat;
}

void VRDevice::setHeadPose(QMatrix4x4 mat){
    m_headPosMatrix= mat;
}

VRDevice::~VRDevice(){
    if(m_vrInitialized){
        qDebug() << "[VRDevice::~VRDevice]";
        m_vrplugin->shutdownVR();
//        m_vrinfo.destroyVRDevice(m_vrplugin);
//        m_vrinfo.unloadLib();
        m_vrInitialized = false; // becasue the above gets called a bunch...
    }
}

} // Render
} // Qt3DRender

QT_END_NAMESPACE
