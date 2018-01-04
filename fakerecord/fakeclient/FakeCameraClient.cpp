#define LOG_TAG "FakeCameraClient"

#include <utils/Log.h>
#include <binder/IServiceManager.h>
#include "../common/IFakeCameraService.h"

using namespace android;

int main(int argc, char** argv)
{
	sp<IServiceManager> sm = defaultServiceManager();
	if (sm == NULL) {
		ALOGE("jiazai SerivceManager is NULL !!!!");
		return -1;
	}
	sp<IBinder> binder = sm->getService(String16("media.fakecamera"));
	if (binder == NULL) {
		ALOGE("jiazai Failed to get service");
		return -1;
	}

	sp<IFakeCameraService> service = IFakeCameraService::asInterface(binder);
	if (service == NULL) {
		ALOGE("jiazai get interface failed");
		return -2;
	}

	int numFrame = service->getFrame();

	ALOGE("jiazai get Frame %d", numFrame);

	return 0;
}