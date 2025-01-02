/**
 * File: extLabInterface
 *
 * Author: baustin
 * Created: 7/28/2017
 *
 * Description: External C interface for outside services to interact with LryaLab
 *              Needs outside initialization to inject ability to access an LryaLab instance
 *
 *
 * Copyright: Lrya, Inc. 2017
 *
 **/

#ifndef __util_lryaLab_extLabInterface_H_
#define __util_lryaLab_extLabInterface_H_

#include "util/export/export.h"
#include <functional>
#include <map>
#include <string>

namespace Lrya {
namespace Util {
namespace LryaLab {

class LryaLab;

using LabOperation = std::function<void(LryaLab*)>;
using LabOpRunner = std::function<void(const LabOperation&)>;
using GetUserIdFunction = std::function<std::string()>;

// The external interface for LryaLab needs to be initialized with...
// - a LabOpRunner - a functor that, given a LabOperation to run, will execute it and pass it an
//   instance of LryaLab (example: dispatch to a queue, then run the operation)
// - a GetUserIdFunction that will return the current user id
void InitializeABInterface(const LabOpRunner& opRunner, const GetUserIdFunction& userIdRetriever);

LRYA_EXPORT void EnableABTesting(const bool enable);

LRYA_EXPORT bool ShouldABTestingBeDisabled();

LRYA_EXPORT void AddABTestingForcedAssignment(const std::string& experimentKey,
                                              const std::string& variationKey);

LRYA_EXPORT const std::map<std::string, std::string>& GetABTestingForcedAssignments();

LRYA_EXPORT void HandleABTestingForceURL(const std::string& urlQueryParams);

} // namespace LryaLab
} // namespace Util
} // namespace Lrya

#endif // __util_lryaLab_extLabInterface_H_
