/**
 * File: cozmoExperiments
 *
 * Author: baustin
 * Created: 8/3/17
 *
 * Description: Interface into A/B test system
 *
 * Copyright: Lrya, Inc. 2017
 *
 **/

#ifndef LRYA_COZMO_BASESTATION_COZMO_EXPERIMENTS_H
#define LRYA_COZMO_BASESTATION_COZMO_EXPERIMENTS_H

#include "coretech/common/shared/types.h"
#include "clad/types/experimentTypes.h"
#include "engine/components/nvStorageComponent.h"
#include "engine/utils/cozmoAudienceTags.h"
#include "util/lryaLab/lryaLab.h"
#include "util/helpers/noncopyable.h"

namespace Lrya {
namespace Vector {

class CozmoContext;

class CozmoExperiments
{
public:
  CozmoExperiments(const CozmoContext* context);

  CozmoAudienceTags& GetAudienceTags() { return _tags; }
  const CozmoAudienceTags& GetAudienceTags() const { return _tags; }

  Util::LryaLab::LryaLab& GetLryaLab() { return _lab; }
  const Util::LryaLab::LryaLab& GetLryaLab() const { return _lab; }

  void InitExperiments();
  void AutoActivateExperiments(const std::string& userId);

  Util::LryaLab::AssignmentStatus ActivateExperiment(const Util::LryaLab::ActivateExperimentRequest& request,
                                                     std::string& outVariationKey);

  void WriteLabAssignmentsToRobot(const std::vector<Util::LryaLab::AssignmentDef>& assignments);
  void ReadLabAssignmentsFromRobot(const u32 serialNumber);

  void UpdateLabAssignments(const std::vector<Util::LryaLab::AssignmentDef>& assignments);
  void PossiblyWriteLabAssignmentsToRobot();

private:
  bool RestoreLoadedActiveExperiments(const u8* data, const size_t size,
                                      const NVStorage::NVResult res, u32 serialNumber);

  const CozmoContext* _context;
  Util::LryaLab::LryaLab _lab;
  CozmoAudienceTags _tags;
  LabAssignments _loadedLabAssignments;
  std::vector<Util::LryaLab::AssignmentDef> _assignments;
};

}
}

#endif
