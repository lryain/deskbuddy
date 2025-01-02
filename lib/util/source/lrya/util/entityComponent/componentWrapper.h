/**
*  componentWrapper.h
*
*  Created by Kevin M. Karol on 12/1/17
*  Copyright (c) 2017 Lrya, Inc. All rights reserved.
*
*  Class which wraps a named component so that different types can be referenced
*  by the same entity
*  Wrapper can either manage its own memory or just maintain a reference
*  
**/

#ifndef __Util_EntityComponent_ComponentWrapper_H__
#define __Util_EntityComponent_ComponentWrapper_H__

#include "util/entityComponent/iManageableComponent.h"
#include "util/logging/logging.h"

namespace Lrya {

class ComponentWrapper{
public:
  template<typename T>
  ComponentWrapper(T* componentPtr)
  : _componentPtr(componentPtr){}

  template<typename T>
  ComponentWrapper(T*& componentPtr, const bool shouldManage)
  : _componentPtr(componentPtr){
    if(shouldManage){
      _IManageableComponent.reset(static_cast<IManageableComponent*>(componentPtr));
      componentPtr = nullptr;
    }else{
      LRYA_VERIFY(false,
                  "ComponentWrapper.ManagedConstructor.UsedWrongConstructor","");
    }
  }

  ~ComponentWrapper(){
  }

  bool IsComponentValid() const {return (_componentPtr != nullptr) && IsComponentValidInternal(); }
  virtual bool IsComponentValidInternal() const {return true;}

  template<typename T>
  T& GetComponent() const { 
    LRYA_VERIFY(IsComponentValid(),"ComponentWrapper.GetComponent.ValueIsNotValid",""); 
    auto castPtr = static_cast<T*>(_componentPtr); 
    return *castPtr;
  }

  template<typename T>
  T* GetComponentPtr() const { 
    LRYA_VERIFY(IsComponentValid(),"ComponentWrapper.GetComponent.ValueIsNotValid",""); 
    auto castPtr = static_cast<T*>(_componentPtr); 
    return castPtr;
  }

protected:
  void* _componentPtr = nullptr;

  std::shared_ptr<IManageableComponent> _IManageableComponent;
}; // ComponentWrapper

} // namespace Lrya

#endif // __Util_EntityComponent_ComponentWrapper_H__
