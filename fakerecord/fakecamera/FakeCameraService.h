#ifndef ANDROID_FAKE_CAMERA_SERVICE_H
#define ANDROID_FAKE_CAMERA_SERVICE_H

#include <binder/BinderService.h>
#include "../common/IFakeCameraService.h"
#include "../common/IFakeCameraProxyListener.h"
#include <mutex>
#include <thread>

namespace android {

class FakeCameraService : public BinderService<FakeCameraService>, public BnFakeCameraService
{
    friend class BinderService<FakeCameraService>;    // for MediaCodecService()
public:
    FakeCameraService();
    virtual ~FakeCameraService() { }
    virtual void onFirstRef() { }

    static const char*  getServiceName() { return "media.fakecamera"; }

    virtual int    getFrame();
    virtual void   stopFrame();
    virtual void   setListener(const sp<IFakeCameraProxyListener>& listener);

    virtual status_t    onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                uint32_t flags);

private:
	virtual void   startAddFrame();
	virtual void   addAndshowFrame();
    std::mutex     mLock;
    std::thread    t;
    bool           mStart;
    int            numFrame;
    sp<IFakeCameraProxyListener>  mListener;
};
}

#endif