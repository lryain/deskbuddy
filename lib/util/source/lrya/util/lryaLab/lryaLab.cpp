/**
 * File: lryaLab.cpp
 *
 * Author: chapados
 * Created: 1/25/17
 *
 * Description: Interface to A/B test data for experimental features
 *
 * Copyright: Lrya, Inc. 2017
 *
 **/

#include "util/lryaLab/lryaLab.h"
#include "util/lryaLab/lryaLabAccessors.h"
#include "util/helpers/templateHelpers.h"
#include "util/logging/logging.h"
#include "util/math/numericCast.h"
#include "util/string/stringUtils.h"
#include <chrono>

#define LOG_CHANNEL    "LryaLab"

namespace Lrya {
namespace Util {
namespace LryaLab {

LryaLab::LryaLab()
: _enabled(true)
, _expUserHashFunc(&CalculateExperimentHashBucket)
, _activeAssignmentsUpdatedSignal(new ActiveAssignmentsUpdatedSignalType())
{
}

void LryaLab::Enable(const bool enabled)
{
  _enabled = enabled;
  if (_enabled) {
    LOG_INFO("LryaLab.Enable", "Enabling the lab");
  } else {
    LOG_INFO("LryaLab.Enable", "Disabling the lab");
    _activeAssignments.clear();
    _forceAssignments.clear();
    _overrideAssignments.clear();
  }
}

bool LryaLab::Load(const std::string& jsonContents)
{
  Json::Reader reader;
  Json::Value root;

  bool success = reader.parse(jsonContents, root);
  if (!success) {
    LOG_ERROR("LryaLab.Load", "Invalid json format in LryaLab file");
    return false;
  }

  success = _lryaLabDef.SetFromJSON(root);
  return success;
}

const LryaLabDef& LryaLab::GetLryaLabDefinition() const
{
  return _lryaLabDef;
}

const Experiment* LryaLab::FindExperiment(const std::string& experimentKey) const
{
  return Lrya::Util::LryaLab::FindExperiment(&_lryaLabDef, experimentKey);
}

AssignmentDef* LryaLab::FindAssignment(std::vector<AssignmentDef>& assignmentList,
                                       const std::string& experimentKey,
                                       const std::string& userId)
{
  auto search =
  std::find_if(assignmentList.begin(), assignmentList.end(),
               [&experimentKey, &userId](const AssignmentDef& def) {
                 return (   (def.GetExperiment_key() == experimentKey)
                         && (def.GetUser_id() == userId));
               });
  if (search == assignmentList.end()) {
    return nullptr;
  } else {
    AssignmentDef* def = &(*search);
    return def;
  }
}


bool LryaLab::IsActiveExperiment(const std::string& experimentKey,
                                 const std::string& userId) const
{
  // Drop const for lookup - safe since we are not exposing the mutable ptr result
  LryaLab* mutableThis = const_cast<LryaLab*>(this);
  AssignmentDef* def = mutableThis->FindAssignment(mutableThis->_activeAssignments,
                                                   experimentKey,
                                                   userId);
  if (nullptr == def) {
    def = mutableThis->FindAssignment(mutableThis->_forceAssignments,
                                      experimentKey,
                                      userId);
  }
  return (nullptr != def);
}

uint32_t LryaLab::GetSecondsSinceEpoch() const
{
  using namespace std::chrono;
  system_clock::time_point nowTp {system_clock::now()};
  seconds nowSec = duration_cast<seconds>(nowTp.time_since_epoch());
  uint32_t now = Lrya::Util::numeric_cast<uint32_t>(nowSec.count());

  return now;
}


AssignmentStatus LryaLab::ActivateExperiment(const std::string& experimentKey,
                                             const std::string& userId,
                                             const std::vector<std::string>& audienceTags,
                                             std::string& outVariationKey)
{
  uint32_t now = GetSecondsSinceEpoch();
  AssignmentStatus status = ActivateExperiment(experimentKey, userId,
                                               audienceTags,
                                               now,
                                               outVariationKey);
  return status;
}

AssignmentStatus LryaLab::ActivateExperiment(const std::string& experimentKey,
                                             const std::string& userId,
                                             const std::vector<std::string>& audienceTags,
                                             const uint32_t epochSec,
                                             std::string& outVariationKey)
{
  AssignmentStatus status = ActivateExperimentInternal(experimentKey, userId,
                                                       audienceTags,
                                                       epochSec,
                                                       outVariationKey);

  AssignmentDef assignment{experimentKey, userId, outVariationKey};
  ReportExperimentAssignmentResult(assignment, status);

  return status;
}

AssignmentStatus LryaLab::RestoreActiveExperiment(const std::string& experimentKey,
                                                  const std::string& userId,
                                                  const std::string& variationKey,
                                                  const uint32_t epochSec)
{
  const Experiment* experiment = FindExperiment(experimentKey);
  if (nullptr == experiment) {
    return AssignmentStatus::ExperimentNotFound;
  }

  const ExperimentVariation* variation = FindVariation(experiment, variationKey);
  if (nullptr == variation) {
    return AssignmentStatus::VariantNotFound;
  }

  // Check for "active" experiment based on timestamp
  if (!IsExperimentRunning(experiment, epochSec)) {
    return AssignmentStatus::ExperimentNotRunning;
  }

  if (!IsEnabled()) {
    return AssignmentStatus::Unassigned;
  }

  AssignmentDef def{experimentKey, userId, variationKey};
  AssignExperimentVariation(def);

  return AssignmentStatus::Assigned;
}


size_t LryaLab::AutoActivateExperimentsForUser(const std::string& userId,
                                               const std::vector<std::string>& audienceTags)
{
  size_t count = 0;
  for (const Experiment& e : _lryaLabDef.GetExperiments()) {
    if (e.GetActivation_mode() == ActivationMode::automatic) {
      std::string variationKey;
      AssignmentStatus status = ActivateExperiment(e.GetKey(),
                                                   userId,
                                                   audienceTags,
                                                   variationKey);
      LOG_INFO("LryaLab.AutoActivateExperimentsForUser",
               "%s : %s : %s",
               e.GetKey().c_str(), variationKey.c_str(), EnumToString(status));
      ++count;
    }
  }
  return count;
}

std::vector<std::string> LryaLab::GetKnownAudienceTags() const
{
  return Lrya::Util::LryaLab::GetKnownAudienceTags(_lryaLabDef);
}

AssignmentStatus LryaLab::ActivateExperimentInternal(const std::string& experimentKey,
                                                     const std::string& userId,
                                                     const std::vector<std::string>& audienceTags,
                                                     const uint32_t epochSec,
                                                     std::string& outVariationKey)
{
  outVariationKey = "";
  const Experiment* experiment = FindExperiment(experimentKey);
  if (nullptr == experiment) {
    return AssignmentStatus::ExperimentNotFound;
  }

  // Check for force assignment
  // TODO-BRC: Also need to check that variant is valid?
  {
    AssignmentDef* def = FindAssignment(_forceAssignments, experimentKey, userId);
    if (nullptr != def) {
      outVariationKey = def->GetVariation_key();
      return AssignmentStatus::ForceAssigned;
    }
  }

  // Check for "active" experiment based on timestamp
  uint32_t now = epochSec;
  if (!IsExperimentRunning(experiment, now)) {
    return AssignmentStatus::ExperimentNotRunning;
  }

  // If we already have an assignment restored from our profile, use that
  AssignmentDef* existingAssignment = FindAssignment(_activeAssignments, experimentKey, userId);
  if (nullptr != existingAssignment) {
    outVariationKey = existingAssignment->GetVariation_key();
    return AssignmentStatus::Assigned;
  }

  // if audience_ids were specified in the experiment,
  // check that the specified audienceTags match the set defined in the experiment
  if (!IsMatchingAudience(experiment, audienceTags)) {
    return AssignmentStatus::AudienceMismatch;
  }

  // check override assignment
  AssignmentDef* overrideAssignment = FindAssignment(_overrideAssignments, experimentKey, userId);
  if (nullptr != overrideAssignment) {
    outVariationKey = overrideAssignment->GetVariation_key();
    return AssignmentStatus::OverrideAssigned;
  }

  // If we made it this far, the user is either unassigned or assigned
  AssignmentStatus status = AssignmentStatus::Unassigned;
  if (!IsEnabled()) {
    LOG_INFO("LryaLab.ActivateExperiment.LabIsDisabled", "");
    return status;
  }

  const uint8_t userHashBucket = CalculateExperimentHashBucket(experimentKey, userId);
  LOG_INFO("LryaLab.ActivateExperiment.bucket",
           "%s : %u",
            experimentKey.c_str(), userHashBucket);
  const ExperimentVariation* variation = GetExperimentVariation(experiment, userHashBucket);


  if (nullptr != variation) {
    outVariationKey = variation->GetKey();
    LOG_INFO("LryaLab.ActivateExperiment",
             "%s : %u : %s",
             experiment->GetKey().c_str(),
             userHashBucket,
             outVariationKey.c_str());

    AssignmentDef assignment{experimentKey, userId, variation->GetKey()};
    AssignExperimentVariation(assignment);
    status = AssignmentStatus::Assigned;
  }

  return status;
}

void LryaLab::ReportExperimentAssignmentResult(const AssignmentDef& assignment,
                                               const AssignmentStatus status)
{
  const std::string& experimentKey = assignment.GetExperiment_key();

  std::string statusString = EnumToString(status);
  std::transform(statusString.begin(), statusString.end(), statusString.begin(), ::tolower);

  const Metadata& meta = _lryaLabDef.GetMeta();
  std::string versionString = meta.GetProject_id() +
                              "-" + std::to_string(meta.GetVersion()) +
                              "." + std::to_string(meta.GetRevision());

  // Invalid input, log event and bail
  if ((status == AssignmentStatus::Invalid) ||
      (status == AssignmentStatus::ExperimentNotFound) ||
      (status == AssignmentStatus::VariantNotFound)){
    std::string statusInfo = versionString + ":" + statusString;
    LOG_WARNING("LryaLab.experiment.invalid",
                "%s:%s:%s - %s",
                experimentKey.c_str(),
                assignment.GetVariation_key().c_str(),
                assignment.GetUser_id().c_str(),
                statusInfo.c_str());
    return;
  }

  // If we've reached this point, there were no errors.
  // Generate an `assigned` or `unassigned` event

  const Experiment* experiment = FindExperiment(experimentKey);
  const ExperimentVariation* variation = FindVariation(experiment, assignment.GetVariation_key());

  // create `info` string: '<version>:<experiment_name>:<variant_name>:<status>'
  std::string info = versionString +
                     ":" + experiment->GetKey() +
                     ":" + ((nullptr != variation) ? variation->GetKey() : "") +
                     ":" + statusString;

  if ((status == AssignmentStatus::AudienceMismatch) ||
      (status == AssignmentStatus::ExperimentNotRunning) ||
      (status == AssignmentStatus::Unassigned)){
    // assignment criteria not met
    Lrya::Util::sInfo("experiment.unassigned", {
      { "$user", assignment.GetUser_id().c_str() },
      { DGROUP , assignment.GetVariation_key().c_str() },
      { DDATA  , info.c_str() }
    }, experimentKey.c_str());

    LOG_DEBUG("LryaLab.experiment.unassigned",
              "%s:%s:%s - %s",
              experimentKey.c_str(),
              assignment.GetVariation_key().c_str(),
              assignment.GetUser_id().c_str(),
              info.c_str());
  } else {
    Lrya::Util::sInfo("experiment.assigned", {
      { "$user", assignment.GetUser_id().c_str() },
      { DGROUP , variation->GetKey().c_str() },
      { DDATA  , info.c_str() }
    }, experimentKey.c_str());

    LOG_DEBUG("LryaLab.experiment.assigned",
              "%s:%s:%s - %s",
              experimentKey.c_str(),
              variation->GetKey().c_str(),
              assignment.GetUser_id().c_str(),
              info.c_str());
  }
}

void LryaLab::AssignExperimentVariation(const AssignmentDef& assignment)
{
  AssignmentDef* def = FindAssignment(_activeAssignments,
                                      assignment.GetExperiment_key(),
                                      assignment.GetUser_id());
  if (nullptr != def) {
    def->SetVariation_key(assignment.GetVariation_key());
  } else {
    _activeAssignments.push_back(assignment);
  }
  if (_activeAssignmentsUpdatedSignal) {
    _activeAssignmentsUpdatedSignal->emit(_activeAssignments);
  }
}

AssignmentStatus LryaLab::ForceActivateExperiment(const std::string& experimentKey,
                                                  const std::string& userId,
                                                  const std::string& variationKey)
{
  const Experiment* experiment = FindExperiment(experimentKey);
  if (nullptr == experiment) {
    return AssignmentStatus::ExperimentNotFound;
  }

  const ExperimentVariation* variation = FindVariation(experiment, variationKey);
  if (nullptr == variation) {
    return AssignmentStatus::VariantNotFound;
  }

  if (!IsEnabled()) {
    return AssignmentStatus::Unassigned;
  }

  AssignmentDef* assignment = FindAssignment(_forceAssignments, experimentKey, userId);
  if (nullptr == assignment) {
    AssignmentDef def{experimentKey, userId, variationKey};
    _forceAssignments.emplace_back(std::move(def));
    assignment = &_forceAssignments.back();
  } else {
    if (assignment->GetVariation_key() != variationKey) {
      // changed
      assignment->SetVariation_key(variationKey);
    }
  }

  LOG_INFO("LryaLab.ForceActivate", "ForceAssigned: %s -> %s",
                   experiment->GetKey().c_str(), variation->GetKey().c_str());

  return AssignmentStatus::ForceAssigned;
}

bool LryaLab::OverrideExperimentVariation(const std::string& experimentKey,
                                          const std::string& userId,
                                          const std::string& variationKey)
{
  const Experiment* experiment = FindExperiment(experimentKey);
  if (nullptr == experiment) {
    return false;
  }

  const ExperimentVariation* variation = FindVariation(experiment, variationKey);
  if (nullptr == variation) {
    return false;
  }

  if (!IsEnabled()) {
    return false;
  }

  if (IsActiveExperiment(experimentKey, userId)) {
    return false;
  }

  const auto& search =
  std::find_if(_overrideAssignments.begin(), _overrideAssignments.end(),
               [&experimentKey, &userId](const AssignmentDef &a){
                 return ((a.GetExperiment_key() == experimentKey) && (a.GetUser_id() == userId));
               });

  if (search == _overrideAssignments.end()) {
    // add entry
    AssignmentDef def{experimentKey, userId, variationKey};
    _overrideAssignments.emplace_back(std::move(def));
  } else {
    AssignmentDef& def = *search;
    def.SetVariation_key(variationKey);
  }

  return true;
}

} // namespace LryaLab
} // namespace Util
} // namespace Lrya
