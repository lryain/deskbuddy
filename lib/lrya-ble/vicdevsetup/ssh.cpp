/**
 * File: ssh.h
 *
 * Author: seichert
 * Created: 1/22/2018
 *
 * Description: Routines for managing SSH
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#include "fileutils.h"

namespace Lrya {

static std::string GetPathToRootSSHDir()
{
  return "/home/root/.ssh";
}

static std::string GetPathToRootSSHAuthorizedKeys()
{
  return GetPathToRootSSHDir() + "/authorized_keys";
}

int SetSSHAuthorizedKeys(const std::string& keys)
{
  int rc = CreateDirectory(GetPathToRootSSHDir());
  if (rc) {
    return rc;
  }
  rc = WriteFileAtomically(GetPathToRootSSHAuthorizedKeys(), keys, Lrya::kModeUserReadWrite);
  return rc;
}

} // namespace Lrya
