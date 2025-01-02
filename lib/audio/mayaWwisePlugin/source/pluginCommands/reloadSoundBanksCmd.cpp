//
//  reloadSoundBanksCmd.cpp
//  LryaMayaWWisePlugIn
//
//  Created by Jordan Rivas on 2/4/16.
//  Copyright © 2016 Lrya, Inc. All rights reserved.
//

#include "pluginCommands/reloadSoundBanksCmd.hpp"


const char* ReloadSoundBanksCmd::mayaCommand = "LryaMayaWWisePlugIn_ReloadSoundBanks";
std::function<void(void)> ReloadSoundBanksCmd::doItFunc;


ReloadSoundBanksCmd::ReloadSoundBanksCmd()
{
  //    MGlobal::displayInfo("ReloadSoundBanksCmd CONSTRUCTOR");
};

ReloadSoundBanksCmd::~ReloadSoundBanksCmd()
{
  //    MGlobal::displayInfo("ReloadSoundBanksCmd DESTRUCTOR");
};

void* ReloadSoundBanksCmd::creator()
{
  return new ReloadSoundBanksCmd();
}

// This class is created and destroyed immediately after
MStatus ReloadSoundBanksCmd::doIt( const MArgList& args )
{
  MStatus stat = MS::kSuccess;
  
  // Since this class is derived off of MPxCommand, you can use the
  // inherited methods to return values and set error messages
  
  if ( ReloadSoundBanksCmd::doItFunc != nullptr ) {
    doItFunc();
  }
  
  return stat;
}
