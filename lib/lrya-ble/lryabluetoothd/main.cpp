/**
 * File: main.cpp
 *
 * Author: seichert
 * Created: 1/10/2018
 *
 * Description: main entry point for lrya bluetooth daemon
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#include "include_ev.h"
#include "btstack.h"
#include "device_info.h"
#include "log.h"
#include "agent.h"
#include "robot_name.h"

#include <cutils/properties.h>
#include <unistd.h>

static struct ev_loop* sDefaultLoop = ev_default_loop(EVBACKEND_SELECT);

static void ExitHandler(int status = 0) {
  Lrya::BluetoothStack::UnLoadBtStack();
  _exit(status);
}

static void SignalCallback(struct ev_loop* loop, struct ev_signal* w, int revents)
{
  logi("Exiting for signal %d", w->signum);
  ev_unloop(loop, EVUNLOOP_ALL);
  ExitHandler();
}

static struct ev_signal sIntSig;
static struct ev_signal sTermSig;

std::string GetDesiredAdapterName() {
  char value[PROPERTY_VALUE_MAX] = {0};
  (void) property_get(Lrya::kProductNamePropertyKey.c_str(), value, Lrya::kDefaultProductName.c_str());
  std::string defaultName = std::string(value) + "_" + GetDeviceSerialNumber();

  memset(value, 0, sizeof(value));
  (void) property_get(Lrya::kRobotNamePropertyKey.c_str(), value, defaultName.c_str());
  return std::string(value);
}

int main(int argc, char *argv[]) {
  ev_signal_init(&sIntSig, SignalCallback, SIGINT);
  ev_signal_start(sDefaultLoop, &sIntSig);
  ev_signal_init(&sTermSig, SignalCallback, SIGTERM);
  ev_signal_start(sDefaultLoop, &sTermSig);

  setAndroidLoggingTag("lryabtd");
  setMinLogLevel(property_get_int32("persist.lrya.btd.log_level", kLogLevelInfo));
  if (!Lrya::BluetoothStack::LoadBtStack()) {
    ExitHandler(1);
  }
  if (!Lrya::BluetoothStack::EnableAdapter()) {
    ExitHandler(1);
  }
  std::string adapterName = GetDesiredAdapterName();
  if (!Lrya::BluetoothStack::SetAdapterName(adapterName)) {
    ExitHandler(1);
  }
  if (!Lrya::BluetoothStack::RegisterGattClient()) {
    ExitHandler(1);
  }
  if (!Lrya::BluetoothStack::RegisterGattServer()) {
    ExitHandler(1);
  }
  if (!StartBLEAgent()) {
    ExitHandler(1);
  }
  ev_loop(sDefaultLoop, 0);
  ExitHandler();
  return 0;
}
