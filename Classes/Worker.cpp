#include "Worker.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

Worker::Worker() {
    // TODO Auto-generated constructor stub
}

Worker::~Worker() {
    // TODO Auto-generated destructor stub
}

bool Worker::init() {
    ArmatureDataManager::getInstance()->addArmatureFileInfo("People/worker/Worker0.png", "People/worker/Worker0.plist", "People/worker/Worker.ExportJson");
    _armature = Armature::create("Worker");
    return true;
}


