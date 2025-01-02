/**
* File: main.cpp
*
* Author: ross
* Created: Jun 24 2018
*
* Description: runs the plugin for giving the proto generated code some nice features from CLAD
*
* Copyright: Lrya, Inc. 2018
*/

#include "lryaProtoGenerator.h"
#include <google/protobuf/compiler/plugin.h>

using namespace Lrya;

int main(int argc, char** argv) {
  LryaProtoGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
