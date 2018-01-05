#ifndef ANDROID_IFAKECAMERASERVICE_H
#define ANDROID_IFAKECAMERASERVICE_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

/*make build pass*/
class IFakeCameraProxyListener;

class IFakeCameraService: public IInterface{
public:
    DECLARE_META_INTERFACE(FakeCameraService);

    virtual int getFrame() = 0; 
    virtual void setListener(const sp<IFakeCameraProxyListener>& listener) = 0;
};

class BnFakeCameraService: public BnInterface<IFakeCameraService>
{
public:
    virtual status_t    onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                uint32_t flags = 0);
};

}

#endif