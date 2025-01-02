/*******************************************************************************************************************************
 *
 *  ConsoleSystem.h
 *  Lrya::Util
 *
 *  Created by Jarrod Hatfield on 4/7/14.
 *  Copyright (c) 2014 Lrya. All rights reserved.
 *
 *  Description:
 *  - The Console System allows users to register variables so that they can be edited remotely via console/terminal
 *  - Users can also register functions with the system to be called remotely via console/terminal
 *  - It handles all of the text parsing from the console
 *
 *******************************************************************************************************************************/

#ifndef LRYAUTIL_CONSOLE_SYSTEM
#define LRYAUTIL_CONSOLE_SYSTEM

#include "util/console/consoleFunction.h"
#include "util/console/consoleVariable.h"
#include "util/console/consoleTypes.h"
#include "util/global/globalDefinitions.h"
#include "util/export/export.h"
#include "util/helpers/includeSstream.h"
#include "util/stringTable/stringID.h"

#include <map>
#include <vector>
#include <string>



#if LRYA_DEV_CHEATS
#define LRYA_CONSOLE_SYSTEM_ENABLED   1
#else
#define LRYA_CONSOLE_SYSTEM_ENABLED   0
#endif


namespace Lrya{ namespace Util {

//******************************************************************************************************************************
struct ConsoleParseResult
{
  std::string response;
};

class IConsoleChannel;
  
//******************************************************************************************************************************
// The system that manages all of our EditableVars.
class ConsoleSystem
{
  //----------------------------------------------------------------------------------------------------------------------------
public:
  ConsoleSystem();
  ~ConsoleSystem();
  
  void FinishInitialization(const char* iniPath);
  
  template <typename T>
  void Register( T& value, const std::string& keystring, const std::string& category );
  void Register( const std::string& keystring, IConsoleVariable* variable );
  void Register( const std::string& keystring, ConsoleFunc function, const char* categoryName, const std::string& args );
  void Register( const std::string& keystring, IConsoleFunction* function );
  void Unregister( const std::string& keystring );

  bool Eval( const char *text, IConsoleChannel& channel );
  
  template< typename T>
  bool GetArgumentValue( const ConsoleFunctionContext& functiondata, const char* argument, T& value );
  
  //----------------------------------------------------------------------------------------------------------------------------
  // Access
public:
  size_t GetNumConsoleVariables() const { return editvars_.size(); }
  size_t GetNumConsoleFunctions() const { return consolefunctions_.size(); }
  IConsoleVariable* FindVariable( const char* name );
  IConsoleFunction* FindFunction( const char* name );
  
  const IConsoleVariable* FindVariable( const char* name ) const
  {
    return const_cast<ConsoleSystem*>(this)->FindVariable(name);
  }
  const IConsoleFunction* FindFunction( const char* name ) const
  {
    return const_cast<ConsoleSystem*>(this)->FindFunction(name);
  }

  void AppendConsoleVariables( std::string& output ) const;
  void AppendConsoleFunctions( std::string& output ) const;
  
  bool ParseConsoleFunctionCall( IConsoleFunction* functor, const char* token, IConsoleChannel& channel );

private:
  bool ParseConsoleVariableText( char* token, IConsoleChannel& channel, bool readonly );
  bool ParseConsoleFunctionText( char* token, IConsoleChannel& channel );

  //----------------------------------------------------------------------------------------------------------------------------
  // Helpers
public:
  void ToLower( char* text );
   
  //----------------------------------------------------------------------------------------------------------------------------
  // EditVar Mapping
public:
  
  typedef std::vector<StringID> VariableIdList;
  typedef std::map<const StringID, IConsoleVariable*> VariableDatabase;

  typedef std::vector<StringID> FunctionIdList;
  typedef std::map<const StringID, IConsoleFunction*> FunctionDatabase;
  
  const VariableIdList& GetVariableIds() const { return varIds_; }
  const VariableDatabase& GetVariableDatabase() const { return editvars_; }
  
  const FunctionIdList& GetFunctionIds() const { return functIds_; }
  const FunctionDatabase& GetFunctionDatabase() const { return consolefunctions_; }
  
private:
  StringID GetSearchKey( const std::string& key ) const;
  
  VariableIdList   varIds_;
  VariableDatabase editvars_;
  
  FunctionIdList   functIds_;
  FunctionDatabase consolefunctions_;
  
  std::vector<IConsoleVariable*> allocatedVariables_;
  std::vector<IConsoleFunction*> allocatedFunctions_;
  
  bool            _isInitializationComplete;
  
  //----------------------------------------------------------------------------------------------------------------------------
public:
  static ConsoleSystem& Instance();
};

//------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void ConsoleSystem::Register( T& value, const std::string& keystring, const std::string& category )
{
  // The IConsoleVariable will register itself within its constructor.
  IConsoleVariable* var = new ConsoleVar<T>( value, keystring.c_str(), category.c_str(), false );
  allocatedVariables_.push_back( var );
}

//------------------------------------------------------------------------------------------------------------------------------
template< typename T>
inline bool ConsoleSystem::GetArgumentValue( const ConsoleFunctionContext& functiondata, const char* argument, T& value )
{
  IConsoleFunction* function = FindFunction( functiondata.function );
  if ( function != NULL )
  {
    const IConsoleFunctionArg* arg = function->FindArgument( argument );
    if ( arg != NULL )
    {
      return arg->GetArgumentValue( value );
    }
  }
  
  return false;
}

} // namespace Lrya
} //namespace Util


#if LRYA_CONSOLE_SYSTEM_ENABLED
  #define LRYA_CONSOLE_SYSTEM_INIT(iniPath)  Lrya::Util::ConsoleSystem::Instance().FinishInitialization(iniPath);
#else
  #define LRYA_CONSOLE_SYSTEM_INIT(iniPath)
#endif

// Exported C Interface (for Unity/C#)
// Note: These must match up with the externs in ConsoleVarMenu.cs

LRYA_C_BEGIN

#pragma mark - Console Variables
/*
 *  Console Variables
 */
LRYA_EXPORT uint32_t NativeLryaUtilConsoleGetVarCount() LRYA_VISIBLE;
LRYA_EXPORT uint32_t NativeLryaUtilConsoleGetMaxVarNameLen() LRYA_VISIBLE;
LRYA_EXPORT uint32_t NativeLryaUtilConsoleGetMaxCategoryNameLen() LRYA_VISIBLE;

// Variable descriptions
LRYA_EXPORT uint8_t NativeLryaUtilConsoleGetVar(int varIndex,
                                                int* ioNameLength, const char** outName,
                                                int* ioCategoryNameLength, const char** outCategoryName,
                                                double* outMinValue, double* outMaxValue,
                                                uint8_t* outIsToggleable, uint8_t* outIsIntType, uint8_t* outIsSigned) LRYA_VISIBLE;

// Value Getters
LRYA_EXPORT uint32_t NativeLryaUtilConsoleGetVarValueString(const char* varName, char* outBuffer, uint32_t outBufferLen) LRYA_VISIBLE;
LRYA_EXPORT double   NativeLryaUtilConsoleGetVarValueAsDouble(const char* varName) LRYA_VISIBLE;
LRYA_EXPORT int64_t  NativeLryaUtilConsoleGetVarValueAsInt64(const char* varName) LRYA_VISIBLE;
LRYA_EXPORT uint64_t NativeLryaUtilConsoleGetVarValueAsUInt64(const char* varName) LRYA_VISIBLE;

// Value Setters
LRYA_EXPORT void     NativeLryaUtilConsoleToggleValue(const char* varName) LRYA_VISIBLE;
LRYA_EXPORT void     NativeLryaUtilConsoleSetValueWithString(const char* varName, const char* inString) LRYA_VISIBLE;

// Value Defaults
LRYA_EXPORT void     NativeLryaUtilConsoleResetAllToDefault() LRYA_VISIBLE;
LRYA_EXPORT void     NativeLryaUtilConsoleResetValueToDefault(const char* varName) LRYA_VISIBLE;
LRYA_EXPORT uint32_t NativeLryaUtilConsoleIsDefaultValue(const char* varName) LRYA_VISIBLE;

// Forced load and save
LRYA_EXPORT void     NativeLryaUtilConsoleLoadVars() LRYA_VISIBLE;
LRYA_EXPORT void     NativeLryaUtilConsoleSaveVars() LRYA_VISIBLE;


#pragma mark - Console Functions
/*
 *  Console Functions
 */
LRYA_EXPORT uint32_t NativeLryaUtilConsoleGetFunctionCount() LRYA_VISIBLE;
LRYA_EXPORT uint32_t NativeLryaUtilConsoleGetMaxFunctionNameLen() LRYA_VISIBLE;
LRYA_EXPORT uint32_t NativeLryaUtilConsoleGetMaxFunctionSignatureLen() LRYA_VISIBLE;

// Function descriptions
LRYA_EXPORT uint8_t NativeLryaUtilConsoleGetFunction(int varIndex,
                                                     int* outNameLength,      const char** outName,
                                                     int* outCategoryLength,  const char** outCategory,
                                                     int* outSignatureLength, const char** outSignature) LRYA_VISIBLE;
// Call function
LRYA_EXPORT uint32_t NativeLryaUtilConsoleCallFunction(const char* funcName, const char* funcArgs, uint32_t outTextLength, char* outText) LRYA_VISIBLE;

LRYA_C_END


#endif // LRYAUTIL_CONSOLE_SYSTEM
