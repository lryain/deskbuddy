/*******************************************************************************************************************************
 *
 *  ConsoleFunction.h
 *  Lrya::Util
 *
 *  Created by Jarrod Hatfield on 4/7/14.
 *  Copyright (c) 2014 Lrya. All rights reserved.
 *
 *  Description:
 *  - ConsoleFunction is a function that is registered with the ConsoleInterface so that they can be called remotely via
 *    a console/terminal.
 *
 *******************************************************************************************************************************/

#ifndef LRYAUTIL_CONSOLE_FUNCTION
#define LRYAUTIL_CONSOLE_FUNCTION

#include "util/console/consoleTypes.h"
#include "util/console/consoleArgument.h"
#include "util/helpers/includeSstream.h"
#include "util/stringTable/stringID.h"
#include <string>
#include <functional>
#include <cstring>

namespace Lrya{ namespace Util
{

class ConsoleFunctionArgs;
class IConsoleFunctionArg;
class ConsoleFunctionResponse;


//******************************************************************************************************************************
class IConsoleFunction
{
public:
  typedef enum
  {
    ERROR_OK,
    ERROR_TOO_MANY_ARGS,
    ERROR_NOT_ENOUGH_ARGS,
    ERROR_INVALID_ARG,
  } ERROR_CODE;
  
  //----------------------------------------------------------------------------------------------------------------------------
public:
  using ConsoleStdFunc = std::function<void(ConsoleFunctionContextRef)>;
  
  IConsoleFunction( const std::string& keystring, ConsoleFunc function, const char* categoryName, const std::string& args );
  IConsoleFunction( const std::string& keystring, ConsoleStdFunc&& function, const char* categoryName, const std::string& args );
  ~IConsoleFunction();
  void operator()( const ConsoleFunctionContext& context ) const { if( function_ != nullptr ) { function_( (ConsoleFunctionContextRef)&context ); } }
  
  void Reset();
  ERROR_CODE ParseText( const std::string& text );
  const IConsoleFunctionArg* FindArgument( const std::string& argname ) const;
  
  const std::string& GetSignature() const { return signature_; }
  const std::string& GetID()        const { return id_; }
  const char*        GetCategory()  const { return category_; }
  const std::string  GetFullPathString() const { return ( std::string(category_) + "." + id_ ); }
  
  //----------------------------------------------------------------------------------------------------------------------------
  // Helpers
protected:
  void ParseFunctionSignature( const std::string& args );
  void ParseArgDeclaration( const char* arg );
  
  ConsoleArgType GetArgTypeFromString( const std::string& argstring );
  IConsoleFunctionArg* CreateConsoleArg( ConsoleArgType type, StringID name, bool isOptional );
  
  void ToLower( std::string& text ) const;
  StringID GetSearchKey( const std::string& key ) const;
  
  bool AddArgumentToFunction( IConsoleFunctionArg* arg );
  const IConsoleFunctionArg* FindArgument( StringID argid ) const;
  
  bool VerifyFunctionSignature() const  { return true; }
  bool VerifyFunctionArgs() const;

  //----------------------------------------------------------------------------------------------------------------------------
private:
  std::function<void(ConsoleFunctionContextRef)> function_;
  typedef std::vector<IConsoleFunctionArg*> ArgList;
  ArgList args_;
  std::string signature_;
  std::string id_;
  const char* category_;

  IConsoleFunction();
};

} // namespace Lrya
} //namespace Util

#endif // LRYAUTIL_CONSOLE_FUNCTION
