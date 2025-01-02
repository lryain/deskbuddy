/**
 * File: christen.h
 *
 * Author: Paul Aluri, inspired by seichert's mate-christen 
 *         which was inspired by Paul Aluri's example code
 * Created: 4/26/2018
 *
 * Description: Christen the robot with a name if it doesn't have one
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#include <string>

namespace Lrya {
namespace Switchboard {

class Christen {
public:
  static std::string GenerateName();
};

} // Lrya
} // Switchboard