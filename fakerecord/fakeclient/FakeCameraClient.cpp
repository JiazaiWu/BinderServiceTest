#define LOG_TAG "FakeCameraClient"

#include <utils/Log.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include "../fakecamera/FakeCameraService.h"
#include <binder/IPCThreadState.h>
#include "FakeCameraClient.h"

namespace android {

FakeCameraClient::ProxyListener::ProxyListener(const sp<FakeCameraClient>& client):
	mClient(client)
{
	ALOGE("jiazai proxy listener cons");
}

void FakeCameraClient::ProxyListener::callBackFrame(int numFrame)
{
	ALOGE("jiazai listener do callback %d", numFrame);
	mClient->callBackFrame(numFrame);
}

FakeCameraClient::FakeCameraClient():
	mFrame(0)
{
	ALOGE("jiazai FakeCameraClient");
	sp<IServiceManager> sm = defaultServiceManager();
    if (sm == NULL) {
        ALOGE("jiazai SerivceManager is NULL !!!!");
        return;
    }
    sp<IBinder> binder = sm->getService(String16("media.fakecamera"));
    if (binder == NULL) {
        ALOGE("jiazai Failed to get service");
        return;
    }

    mServer = IFakeCameraService::asInterface(binder);
    if (mServer == NULL) {
        ALOGE("jiazai get interface failed");
        return;
    }
    t = std::thread([](FakeCameraClient* odj){odj->showFrame();}, this);
}

FakeCameraClient::~FakeCameraClient()
{
	ALOGE("jiazai FakeCameraClient release");
}

void FakeCameraClient::linkToServer()
{
	ALOGE("jiazai strong count %d before linkToServer", getStrongCount());
	mServer->setListener(new ProxyListener	(this));
	ALOGE("jiazai strong count %d after linkToServer", getStrongCount());
}

void FakeCameraClient::callBackFrame(int numFrame)
{
	ALOGE("jiazai client get callback %d", numFrame);
	std::lock_guard<std::mutex>  temp_lock(mLock);
	mFrame = numFrame;
}

void FakeCameraClient::showFrame() {
	while(true) {
		mLock.lock();
		ALOGE("jiazai client frame %d", mFrame);
		mLock.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

}

using namespace android;

int main(int argc, char** argv)
{

	sp<FakeCameraClient> client = new FakeCameraClient();
	
	client->linkToServer();

	/* neccessary for binder transfer.....*/
	ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

	return 0;
}