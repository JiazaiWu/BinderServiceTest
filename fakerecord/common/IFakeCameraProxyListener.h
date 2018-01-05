#ifndef ANDROID_HARDWARE_I_FAKE_CAMERA_PROXY_LISTENER_H
#define ANDROID_HARDWARE_I_FAKE_CAMERA_PROXY_LISTENER_H

#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <utils/RefBase.h>

namespace android {

class Parcel;

class IFakeCameraProxyListener: public IInterface
{
public:
    DECLARE_META_INTERFACE(FakeCameraProxyListener);

    virtual void callBackFrame(int numFrame) = 0;
};

// ----------------------------------------------------------------------------

class BnFakeCameraProxyListener: public BnInterface<IFakeCameraProxyListener>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif
