#define LOG_TAG "FakeCameraService"

#include <utils/Log.h>
#include <utils/Errors.h>
#include <binder/IMemory.h>
#include <binder/IPCThreadState.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include "FakeCameraService.h"

namespace android {

FakeCameraService::FakeCameraService():BnFakeCameraService(),
	numFrame(0),
	mStart(false)
{
	ALOGE("jiazai create FakeCameraService");
	mStart = true;
	startAddFrame();
}

void FakeCameraService::startAddFrame() {
	t = std::thread([](FakeCameraService* odj){odj->addAndshowFrame();}, this);
}

void FakeCameraService::stopFrame() {
	mStart = false;
}

void FakeCameraService::setListener(const sp<IFakeCameraProxyListener>& listener) {
	ALOGE("jiazai setListener");
	std::lock_guard<std::mutex>  temp_lock(mLock);
	mListener = listener;
}


int FakeCameraService::getFrame() {
    std::lock_guard<std::mutex>  temp_lock(mLock);
    ALOGE("jiazai getFrame %d", numFrame);
    return numFrame;
}

void FakeCameraService::addAndshowFrame() {
	int res;
	sp<IFakeCameraProxyListener> listener;

	while (mStart) {
		mLock.lock();
		res = numFrame;
		if (mListener != nullptr) {
			ALOGE("jiazai server use listener");
			mListener->callBackFrame(res);
		}
		numFrame++;
		mLock.unlock();
		ALOGE("jiazai numFrame %d", res);
		std::this_thread::sleep_for(std::chrono::seconds(1));

	}
}


status_t FakeCameraService::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
        uint32_t flags)
{
    return BnFakeCameraService::onTransact(code, data, reply, flags);
}


}// namespace android

using namespace android;

int main(int argc, char** argv)
{

	strcpy(argv[0], FakeCameraService::getServiceName());
	/*see BinderService.h */
	FakeCameraService::publishAndJoinThreadPool();
}