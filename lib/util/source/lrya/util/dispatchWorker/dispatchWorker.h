/**
* File: dispatchWorker
*
* Author: Lee Crippen
* Created: 8/30/16
*
* Description: Helper class for parallelizing tasks. Use by declaring a version of the template
* with the desired number of threads and argument types to be used by the task.
*
*
* Example:
*
* using MyDispatchWorker = Util::DispatchWorker<3, const std::string&>;
* MyDispatchWorker::FunctionType loadFileFunc = [this] (const std::string& filePath)
* {
*   LocalLoadFileFunction(filePath);
* };
* MyDispatchWorker myWorker(loadFileFunc);
* const auto& fileList = { "filePath0", "filePath1", "etc..." };
* for (int i = 0; i < fileList.size(); i++)
* {
*   myWorker.PushJob(fileList[i]);
* }
* myWorker.Process();
*
*
* Copyright: Lrya, inc. 2016
*
*/
#ifndef __Util_DispatchWorker_DispatchWorker_H_
#define __Util_DispatchWorker_DispatchWorker_H_

#include <array>
#include <functional>
#include <mutex>
#include <thread>
#include <tuple>
#include <vector>

namespace Lrya {
namespace Util {

// template arguments are: number of threads, followed by list of arg types that the processing function expects in its tuple
template<std::size_t TCount, typename... Args>
class DispatchWorker
{
public:
  using FunctionType = std::function<void (Args...)>;
  
  DispatchWorker(FunctionType func) : _function(std::move(func)) { }
  
  // Push in a set of arguments to be processed by a worker thread
  template<typename... FuncArgs>
  void PushJob(FuncArgs&&... args);
  
  // Process all the input arguments that have been pushed
  void Process();
  
private:
  using FunctionArgType = std::tuple<Args...>;
  using ArgumentVector = std::vector<FunctionArgType>;

  std::array<std::thread, TCount>                 _workerThreads{};
  FunctionType                                    _function;
  ArgumentVector                                  _argumentList;
  std::mutex                                      _argsListMutex;
  
  // Use the provided function to process the arguments specified by the input iterators
  void DoThreadWork(typename ArgumentVector::iterator start, typename ArgumentVector::iterator end);

  // Tuple unpacking - from http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
  template<int ...>
  struct seq { };

  template<int N, int ...S>
  struct gens : gens<N-1, N-1, S...> { };

  template<int ...S>
  struct gens<0, S...> {
    typedef seq<S...> type;
  };

  template <int... S>
  void Invoke(FunctionArgType& params, seq<S...>)
  {
//     printf("3.1.2.5.5.6.8.2.1. in Invoke\n");fflush(stdout);
    _function(std::forward<typename std::tuple_element<S, FunctionArgType>::type>(std::get<S>(params))...);
//     printf("3.1.2.5.5.6.8.2.2. done Invoke\n");fflush(stdout);
  }

}; // end class DispatchWorker
  
  
  
  
// ---------------------- DispatchWorker Implementation ----------------------

template<std::size_t TCount, typename... Args>
template<typename... FuncArgs>
void DispatchWorker<TCount, Args...>::PushJob(FuncArgs&&... args)
{
  std::lock_guard<std::mutex> lockGuard(_argsListMutex);
  _argumentList.emplace_back(std::forward<FuncArgs>(args)...);
//   printf("3.1.2.5.5.5.1.2. done emplace_back\n");fflush(stdout);
}

template<std::size_t TCount, typename... Args>
void DispatchWorker<TCount, Args...>::Process()
{
  printf("3.1.2.5.5.6.0. in Process()\n");fflush(stdout);
  std::lock_guard<std::mutex> lockGuard(_argsListMutex);
  const std::size_t size = _argumentList.size();
  const std::size_t numTotalThreads = TCount + 1; // Reserve one slot for the calling thread
  const std::size_t countPerThread = size / numTotalThreads;
  std::size_t remainder = (size % numTotalThreads);
  printf("3.1.2.5.5.6.1. _argumentList.begin()\n");fflush(stdout);
  
  typename ArgumentVector::iterator curIter = _argumentList.begin();
  
  // Hand out work to each of the threads we've allocated
  for (std::size_t i = 0; i < TCount; i++)
  {
//     printf("3.1.2.5.5.6.2. for (std::size_t i = 0; i < TCount; i++)\n");fflush(stdout);
    if (0 == countPerThread && 0 == remainder)
    {
      break;
    }
    
    std::size_t sizeForThread = countPerThread;

    if (remainder > 0)
    {
      printf("3.1.2.5.5.6.3. if (remainder > 0)\n");fflush(stdout);
      ++sizeForThread;
      --remainder;
    }
    printf("3.1.2.5.5.6.4. _workerThreads\n");fflush(stdout);
    _workerThreads[i] = std::thread(&DispatchWorker::DoThreadWork, this, curIter, curIter + sizeForThread);
    curIter += sizeForThread;
  }
  printf("3.1.2.5.5.6.5. start DoThreadWork\n");fflush(stdout);
  
  // Now allow the calling thread to do some work too
  DoThreadWork(curIter, _argumentList.end());
  printf("3.1.2.5.5.6.6. done DoThreadWork\n");fflush(stdout);
  
  // Wait for all our threads to be done
  for (std::size_t i = 0; i < TCount; i++)
  {
    if (_workerThreads[i].joinable())
    {
      _workerThreads[i].join();
      _workerThreads[i] = std::thread();
        // printf("3.1.2.5.5.6.7. _workerThreads[i] = std::thread()\n");fflush(stdout);
    }
  }
  printf("3.1.2.5.5.6.8. _argumentList.clear()\n");fflush(stdout);
  
  _argumentList.clear();
}
  
template<std::size_t TCount, typename... Args>
void DispatchWorker<TCount, Args...>::DoThreadWork(typename ArgumentVector::iterator start, typename ArgumentVector::iterator end)
{
//   printf("3.1.2.5.5.6.8.0. in DoThreadWork...\n");fflush(stdout);
  static const auto paramSeq = typename gens<sizeof...(Args)>::type{};
//   printf("3.1.2.5.5.6.8.1. start while\n");fflush(stdout);
  while (start != end)
  {
//     printf("3.1.2.5.5.6.8.2. in while\n");fflush(stdout);
    Invoke(*start++, paramSeq);
  }
//     printf("3.1.2.5.5.6.8.3. done while\n");fflush(stdout);
}

} // end namespace Util
} // end namespace Lrya

#endif //__Util_DispatchWorker_DispatchWorker_H_
