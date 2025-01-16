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


#pragma once

namespace Lrya {
int SetSSHAuthorizedKeys(const std::string& keys);
} // namespace Lrya
