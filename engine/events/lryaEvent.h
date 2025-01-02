/**
 * File: lryaEvent.h
 *
 * Author: Lee Crippen
 * Created: 07/30/15
 *
 * Description: Events that contain a simple type and a templatized data parameter.
 *
 * Copyright: Lrya, Inc. 2015
 *
 * COZMO_PUBLIC_HEADER
 **/

#ifndef LRYA_COZMO_EVENT_H
#define LRYA_COZMO_EVENT_H

#include <stdint.h>
#include <utility>
#include <memory>


namespace Lrya {
namespace Vector {

template <typename DataType>
class LryaEvent
{
public:
  // In addition to the class being templated, this constructor is templated.
  // This allows for 'perfect forwarding' where the constructor either uses
  // a standard lvalue version or the c++11 rvalue reference version. Magic!
  template <typename FwdType>
  LryaEvent(double time, uint32_t type, FwdType&& newData)
  : _currentTime(time)
  , _myType(type)
  , _data( std::make_shared<DataType>(std::forward<FwdType>(newData)) )
  { }

  template <typename FwdType>
  LryaEvent(uint32_t type, FwdType&& newData)
  : _currentTime(0.0)
  , _myType(type)
  , _data( std::make_shared<DataType>(std::forward<FwdType>(newData)) )
  { }

  double GetCurrentTime() const { return _currentTime; }
  uint32_t GetType() const { return _myType; }
  const DataType& GetData() const { return *_data; }
  
protected:

  double _currentTime;
  uint32_t _myType;
  std::shared_ptr<DataType> _data;
  
}; // class Event


} // namespace Vector
} // namespace Lrya

#endif //  LRYA_COZMO_EVENT_H
