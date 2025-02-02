
#define RELEASE
// #define FACTORY_TEST 1

#include "whiskeyToF/tof.h"

#include "util/logging/logging.h"

#include <iomanip>
#include <inttypes.h>
#include <unistd.h>
#include <chrono>
#include <csignal>
#include <thread>

#ifdef PRINT_NAMED_ERROR
#undef PRINT_NAMED_ERROR
#endif
#define PRINT_NAMED_ERROR(a, b, ...) printf(a b "\n", ##__VA_ARGS__);

#ifdef PRINT_NAMED_WARNING
#undef PRINT_NAMED_WARNING
#endif
#define PRINT_NAMED_WARNING(a, b, ...) printf(a b "\n", ##__VA_ARGS__);

#ifdef PRINT_NAMED_INFO
#undef PRINT_NAMED_INFO
#endif
#define PRINT_NAMED_INFO(a, b, ...) printf(a b "\n", ##__VA_ARGS__);

using namespace Lrya::Vector;

uint32_t GetTimeStamp(void)
{
  auto currTime = std::chrono::steady_clock::now();
  return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(currTime.time_since_epoch()).count());
}

namespace
{
int shutdown = 0;
}

static void Shutdown(int signum)
{
  printf("shutdown\n");
  shutdown = signum;
}

int main(int argc, char** argv)
{
  signal(SIGTERM, Shutdown);
  signal(SIGINT, Shutdown);
  
  bool pause = false;
  
    printf("8.0. --------> before SetupSensors...\n");
  ToFSensor::getInstance()->SetupSensors([](ToFSensor::CommandResult res)
                                         {
                                           if((int)res < 0)
                                           {
                                             printf("Failed to setup\n");
                                             exit(1);
                                           }
                                         });
    printf("8.1. --------> done SetupSensors...\n");

  if(argc > 1)
  {
    if(argv[1][0] == 'c')
    {
      uint32_t dist = 0;
      float reflectance = 0.f;
      
      if(argc > 2)
      {
        char* end;
        dist = (uint32_t)strtoimax(argv[2], &end, 10);
      }
      
      if(argc > 3)
      {
        char* end;
        reflectance = strtof(argv[3], &end);
      }
    printf("8.0. --------> Calibrating at %u with reflectance %f",
                       dist,
                       reflectance);

      PRINT_NAMED_INFO("ToFTest",
                       "Calibrating at %u with reflectance %f",
                       dist,
                       reflectance);
      
      printf("8.0. --------> before ToFSensor::getInstance()->PerformCalibration ...\n");
      ToFSensor::getInstance()->PerformCalibration(dist, reflectance, nullptr);
      printf("8.1. --------> before ToFSensor::getInstance()->SetupSensors ...\n");
      ToFSensor::getInstance()->SetupSensors(nullptr);
      printf("8.2. --------> done ToFSensor::getInstance()->SetupSensors ...\n");
    }
    else if(argv[1][0] == 'p')
    {
      pause = true;
    }
  }

  printf("8.3. --------> start ToFSensor::getInstance()->StartRanging...\n");
  ToFSensor::getInstance()->StartRanging([](ToFSensor::CommandResult res)
                                         {
                                           if((int)res < 0)
                                           {
                                             printf("Failed to start ranging\n");
                                             exit(1);
                                           }
                                         });
  printf("8.4. --------> done ToFSensor::getInstance()->StartRanging.\n");

  while(shutdown == 0)
  {
    bool isUpdated = false;
    printf("8.4.0 --------> in while...\n");
    RangeDataRaw data = ToFSensor::getInstance()->GetData(isUpdated);

    static uint32_t s = GetTimeStamp();
    if(pause && GetTimeStamp() - s > 3000)
    {
      s = GetTimeStamp();
      static bool b = false;
      printf("8.4.1 --------> GetTimeStamp(): %d\n", s);

      if(b)
      {
        printf("8.4.2.1 --------> ToFSensor::getInstance()->StartRanging()\n");
        printf("STARTING\n");
        ToFSensor::getInstance()->StartRanging([](ToFSensor::CommandResult res)
                                         {
                                           if((int)res < 0)
                                           {
                                             printf("Failed to start ranging\n");
                                             exit(1);
                                           }
                                         });
      }
      else
      {
        printf("8.4.2.2 --------> ToFSensor::getInstance()->StopRanging()\n");
        printf("STOPPING\n");
        ToFSensor::getInstance()->StopRanging([](ToFSensor::CommandResult res)
                                         {
                                           if((int)res < 0)
                                           {
                                             printf("Failed to stop ranging\n");
                                             exit(1);
                                           }
                                         });
      }
      b = !b;
    }

    printf("8.4.3 --------> if(!isUpdated)\n");
    
    if(!isUpdated)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      continue;
    }

    static RangeDataRaw lastValid = data;
      
    std::stringstream ss;
    printf("8.4.4 --------> for(int i = 0; i < 4; i++)\n");

    for(int i = 0; i < 4; i++)
    {
      for(int j = 0; j < 4; j++)
      {
        char status = 0;
        if(data.data[i*4 + j].numObjects > 0)
        {
          status = data.data[i*4 + j].readings[0].status;

          if(data.data[i*4 + j].readings[0].status == 0)
          {
            ss << std::setw(7) << (uint32_t)(data.data[i*4 + j].processedRange_mm);
            lastValid.data[i*4 + j] = data.data[i*4 + j];
          }
          else
          {
            ss << std::setw(7) << (uint32_t)(lastValid.data[i*4 + j].processedRange_mm);
          }
        }
        else
        {
          ss << std::setw(7) << (uint32_t)(lastValid.data[i*4 + j].processedRange_mm);
          status = -1;
        }
        
        ss << "[" << std::setw(2) << (int)status << "]";
      }
      ss << "\n";
    }
    printf("%s\n", ss.str().c_str());    
  }
  printf("8.4.5 --------> before ToFSensor::getInstance()->StopRanging()\n");

  printf("stopping\n");
  ToFSensor::getInstance()->StopRanging([](ToFSensor::CommandResult res)
                                         {
                                           if((int)res < 0)
                                           {
                                             printf("Failed to stop ranging\n");
                                             exit(1);
                                           }
                                         });

  ToFSensor::removeInstance();
  
  return 0;
}


