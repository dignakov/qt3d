#ifndef QTESTVRDEVICE_H
#define QTESTVRDEVICE_H

#include <Qt3DRender/qvrdevice.h>
#include <Qt3DCore/qnode.h>


//class QTestVRDevice : public Qt3DRender::QVRDevice
//{
//    Q_OBJECT
//public:
////    using QVRDevice::QVRDevice; //same constructor for now
//    QTestVRDevice(Qt3DCore::QNode *parent = nullptr)
//        : Qt3DRender::QVRDevice(parent)
//    {qDebug()<<"no longer virtual?";}
//    ~QTestVRDevice();
//};


class QTestVRDevice : public Qt3DRender::QVRDevice
{
    Q_OBJECT
public:
    explicit QTestVRDevice(Qt3DCore::QNode *parent = nullptr)
    : Qt3DRender::QVRDevice(parent)
    {}

    ~QTestVRDevice(){}


//protected:
//    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;

};
#endif // QT3DRENDER_QOPENVRDEVICE_H
