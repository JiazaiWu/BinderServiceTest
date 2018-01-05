#ifndef ANDROID_FAKE_CAMERA_CLIENT_H
#define ANDROID_FAKE_CAMERA_CLIENT_H

#include <thread>
#include <mutex>
#include "../common/IFakeCameraProxyListener.h"

namespace android {

class FakeCameraClient: public RefBase
{
public:
    FakeCameraClient();
    virtual ~FakeCameraClient();
    virtual void showFrame();
    virtual void linkToServer();
protected:
    class ProxyListener: public BnFakeCameraProxyListener
    {
    public:
        ProxyListener(const sp<FakeCameraClient>& client);
        virtual void callBackFrame(int numFrame);
    private:
        sp<FakeCameraClient> mClient;
    };

    virtual void callBackFrame(int numFrame);

private:
    std::mutex mLock;
    std::thread t;
    int mFrame;
    sp<IFakeCameraService>   mServer;
};

}

#endif