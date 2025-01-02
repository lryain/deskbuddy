/**
 * File: testLryaLab.cpp
 *
 * Author: chapados
 * Created: 1/25/17
 *
 * Test suite for LryaLab (A/B Testing) core framework.
 *
 * Copyright: Lrya, Inc. 2017
 *
 **/

#include "utilUnitTest/tests/testFileHelper.h"
#include "util/fileUtils/fileUtils.h"
#include "util/string/stringUtils.h"

#include "util/lryaLab/lryaLab.h"
#include "util/lryaLab/lryaLabAccessors.h"

#include <sys/stat.h>


namespace Lrya {
namespace Util {
namespace LryaLab {

class LryaLabTest : public FileHelper
{
public:
  LryaLab CreateLryaLabAB50() const;
  const std::string CreateExperimentAB50DataJsonContents() const;

protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }
};

LryaLab LryaLabTest::CreateLryaLabAB50() const
{
  const std::string experimentsJsonContents = CreateExperimentAB50DataJsonContents();

  LryaLab lab;
  (void)lab.Load(experimentsJsonContents);
  return lab;
}

const std::string LryaLabTest::CreateExperimentAB50DataJsonContents() const
{
  return R"json(
  {
    "meta": {
      "project_id": "od",
      "version": 0,
      "revision": 0
    },
    "experiments": [
                    {
                    "key": "report_test_all_control",
                    "version": 0,
                    "activation_mode": "manual",
                    "start_time_utc_iso8601": "",
                    "stop_time_utc_iso8601": "",
                    "pause_time_utc_iso8601": "",
                    "resume_time_utc_iso8601": "",
                    "pop_frac_pct": 100,
                    "variations": [
                                   {
                                   "key": "control",
                                   "pop_frac_pct": 100
                                   },
                                   {
                                   "key": "treatment",
                                   "pop_frac_pct": 0
                                   }
                                   ],
                    "audience_tags": [],
                    "forced_variations": [
                                          { "user_id": "1234", "variation_key": "treatment" },
                                          { "user_id": "5678", "variation_key": "control" }
                                          ]
                    },
                    {
                    "key": "report_test_all_treatment",
                    "version": 0,
                    "activation_mode": "manual",
                    "start_time_utc_iso8601": "",
                    "stop_time_utc_iso8601": "",
                    "pause_time_utc_iso8601": "",
                    "resume_time_utc_iso8601": "",
                    "pop_frac_pct": 100,
                    "variations": [
                                   {
                                   "key": "control",
                                   "pop_frac_pct": 0
                                   },
                                   {
                                   "key": "treatment",
                                   "pop_frac_pct": 100
                                   }
                                   ],
                    "audience_tags": [],
                    "forced_variations": [
                                          { "user_id": "1234", "variation_key": "treatment" },
                                          { "user_id": "5678", "variation_key": "control" }
                                          ]
                    },
                    {
                    "key": "report_test",
                    "version": 0,
                    "activation_mode": "manual",
                    "start_time_utc_iso8601": "",
                    "stop_time_utc_iso8601": "",
                    "pause_time_utc_iso8601": "",
                    "resume_time_utc_iso8601": "",
                    "pop_frac_pct": 50,
                    "variations": [
                                   {
                                   "key": "control",
                                   "pop_frac_pct": 50
                                   },
                                   {
                                   "key": "treatment",
                                   "pop_frac_pct": 50
                                   }
                                   ],
                    "audience_tags": [],
                    "forced_variations": [
                                          { "user_id": "1234", "variation_key": "treatment" },
                                          { "user_id": "5678", "variation_key": "control" }
                                          ]
                    },
                    {
                    "key": "report_test_auto",
                    "version": 0,
                    "activation_mode": "automatic",
                    "start_time_utc_iso8601": "",
                    "stop_time_utc_iso8601": "",
                    "pause_time_utc_iso8601": "",
                    "resume_time_utc_iso8601": "",
                    "pop_frac_pct": 70,
                    "variations": [
                                   {
                                   "key": "control",
                                   "pop_frac_pct": 35
                                   },
                                   {
                                   "key": "treatment",
                                   "pop_frac_pct": 65
                                   }
                                   ],
                    "audience_tags": [],
                    "forced_variations": [
                                          { "user_id": "1234", "variation_key": "treatment" },
                                          { "user_id": "5678", "variation_key": "control" }
                                          ]
                    },
                    {
                    "key": "report_test_first_time_user",
                    "version": 0,
                    "activation_mode": "automatic",
                    "start_time_utc_iso8601": "",
                    "stop_time_utc_iso8601": "",
                    "pause_time_utc_iso8601": "",
                    "resume_time_utc_iso8601": "",
                    "pop_frac_pct": 50,
                    "variations": [
                                   {
                                   "key": "control",
                                   "pop_frac_pct": 50
                                   },
                                   {
                                   "key": "treatment",
                                   "pop_frac_pct": 50
                                   }
                                   ],
                    "audience_tags": [
                                      "first_time_user"
                                      ],
                    "forced_variations": [
                                          { "user_id": "1234", "variation_key": "treatment" },
                                          { "user_id": "5678", "variation_key": "control" }
                                          ]
                    },
                    {
                    "key": "report_test_times",
                    "version": 0,
                    "activation_mode": "automatic",
                    "start_time_utc_iso8601": "2017-04-01T00:00:00Z",
                    "stop_time_utc_iso8601": "2017-04-30T00:00:00Z",
                    "pause_time_utc_iso8601": "2017-04-10T00:00:00Z",
                    "resume_time_utc_iso8601": "2017-04-20T00:00:00Z",
                    "pop_frac_pct": 50,
                    "variations": [
                                   {
                                   "key": "control",
                                   "pop_frac_pct": 50
                                   },
                                   {
                                   "key": "treatment",
                                   "pop_frac_pct": 50
                                   }
                                   ],
                    "audience_tags": [],
                    "forced_variations": [
                                          { "user_id": "1234", "variation_key": "treatment" },
                                          { "user_id": "5678", "variation_key": "control" }
                                          ]
                    }
                    ]
  }
  )json";
}

TEST_F(LryaLabTest, LoadData)
{
  const std::string experimentsJsonContents = CreateExperimentAB50DataJsonContents();

  LryaLab lab;
  bool success = lab.Load(experimentsJsonContents);
  EXPECT_TRUE(success);

  const LryaLabDef& def = lab.GetLryaLabDefinition();
  EXPECT_EQ(6, def.GetExperiments().size());
}

TEST_F(LryaLabTest, ExperimentHashBucket)
{
  uint8_t bucket = CalculateExperimentHashBucket("report_test",
                                                        "user_id_0");
  EXPECT_GE(bucket, 0);
  EXPECT_LT(bucket, 100);
}

TEST_F(LryaLabTest, TestaudienceTagIntersections)
{
  std::vector<std::string> experimentKeys = { "a", "f", "c" };
  std::vector<std::string> audienceTags = { "c", "b", "a", "d", "f", "e" };
  EXPECT_TRUE(AudienceListIsSubsetOfList(experimentKeys, audienceTags));

  std::vector<std::string> mismatchedaudienceTags = { "f", "d", "e", "c" };
  EXPECT_FALSE(AudienceListIsSubsetOfList(experimentKeys, mismatchedaudienceTags));

  // Experiment ids empty
  EXPECT_TRUE(AudienceListIsSubsetOfList(std::vector<std::string>{}, audienceTags));

  // audience ids empty
  EXPECT_FALSE(AudienceListIsSubsetOfList(experimentKeys, std::vector<std::string>{}));
}

TEST_F(LryaLabTest, TestEpochFromUTCDateString)
{
  time_t epochTs;

  epochTs = Util::EpochSecFromIso8601UTCDateString("1970-01-01T00:00:00Z");
  EXPECT_EQ(0, epochTs);

  epochTs = Util::EpochSecFromIso8601UTCDateString("1970-01-01T00:00:01Z");
  EXPECT_EQ(1, epochTs);

  epochTs = Util::EpochSecFromIso8601UTCDateString("1970-01-01T01:00:00Z");
  EXPECT_EQ(3600, epochTs);

  epochTs = Util::EpochSecFromIso8601UTCDateString("2017-02-23T08:27:51Z");
  EXPECT_EQ(1487838471, epochTs);

#ifndef LINUX // below has a different result on linux

  // I've got 99 seconds, but a `time_t` ain't one.
  epochTs = Util::EpochSecFromIso8601UTCDateString("1970-01-01T00:00:99Z");

  // Invalid date returns UINT32_MAX
  EXPECT_EQ(UINT32_MAX, epochTs);
#endif
}

TEST_F(LryaLabTest, GetExperimentVariation)
{
  const std::string experimentsJsonContents = CreateExperimentAB50DataJsonContents();

  LryaLab lab = CreateLryaLabAB50();

  const Experiment* experiment = lab.FindExperiment("report_test");
  EXPECT_NE(nullptr, experiment);

  {
    const ExperimentVariation* variation = GetExperimentVariation(experiment, 0);
    EXPECT_NE(nullptr, variation);
    EXPECT_EQ("control", variation->GetKey());
  }

  {
    const ExperimentVariation* variation = GetExperimentVariation(experiment, 49);
    EXPECT_EQ(nullptr, variation);
  }

  {
    const ExperimentVariation* variation = GetExperimentVariation(experiment, 99);
    EXPECT_EQ(nullptr, variation);
  }

  {
    const ExperimentVariation* variation = GetExperimentVariation(experiment, 50);
    EXPECT_NE(nullptr, variation);
    EXPECT_EQ("treatment", variation->GetKey());
  }
}

TEST_F(LryaLabTest, ActivateExperiment)
{
  LryaLab lab = CreateLryaLabAB50();

  {
    std::string variationKey;
    AssignmentStatus status = lab.ActivateExperiment("report_test",
                                                     "user_id_8",
                                                     std::vector<std::string>(0),
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("treatment", variationKey);
  }

  {
    std::string variationKey;
    AssignmentStatus status = lab.ActivateExperiment("report_test",
                                                     "user_id_0",
                                                     std::vector<std::string>(0),
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("control", variationKey);
  }

  {
    std::string variationKey;
    AssignmentStatus status = lab.ActivateExperiment("report_test",
                                                     "user_id_000000",
                                                     std::vector<std::string>(0),
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Unassigned, status);
  }
}

TEST_F(LryaLabTest, ActivateExperimentAllControl)
{
  // Ensure that an experiment with 0% treatment, 100% control is assigned correctly
  LryaLab lab = CreateLryaLabAB50();

  const Experiment* experiment = lab.FindExperiment("report_test_all_control");
  EXPECT_NE(nullptr, experiment);
  {
    const ExperimentVariation* variation = GetExperimentVariation(experiment, 0);
    EXPECT_NE(nullptr, variation);
    EXPECT_EQ("control", variation->GetKey());
  }

  {
    const ExperimentVariation* variation = GetExperimentVariation(experiment, 99);
    EXPECT_NE(nullptr, variation);
    EXPECT_EQ("control", variation->GetKey());
  }

  {
    std::string variationKey;
    AssignmentStatus status = lab.ActivateExperiment("report_test_all_control",
                                                     "user_id_8",
                                                     std::vector<std::string>(0),
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("control", variationKey);
  }
}

TEST_F(LryaLabTest, ActivateExperimentAllTreatment)
{
  // Ensure that an experiment with 0% control, 100% treatment is assigned correctly
  LryaLab lab = CreateLryaLabAB50();

  const Experiment* experiment = lab.FindExperiment("report_test_all_treatment");
  EXPECT_NE(nullptr, experiment);
  {
    const ExperimentVariation* variation = GetExperimentVariation(experiment, 0);
    EXPECT_NE(nullptr, variation);
    EXPECT_EQ("treatment", variation->GetKey());
  }

  {
    const ExperimentVariation* variation = GetExperimentVariation(experiment, 99);
    EXPECT_NE(nullptr, variation);
    EXPECT_EQ("treatment", variation->GetKey());
  }

  {
    std::string variationKey;
    AssignmentStatus status = lab.ActivateExperiment("report_test_all_treatment",
                                                     "user_id_8",
                                                     {},
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("treatment", variationKey);
  }
}

TEST_F(LryaLabTest, AutoActivateExperiments)
{
  LryaLab lab = CreateLryaLabAB50();

  // Should attempt to activate 3 experiments (activation_mode:automatic)
  size_t attemptCount = lab.AutoActivateExperimentsForUser("user_id_8", {"first_time_user"});
  EXPECT_EQ(3, attemptCount);
}

TEST_F(LryaLabTest, ActivateExperimentWithAudienceFiltering)
{

  {
    // Experiment audienceTags match user audienceTags
    LryaLab lab = CreateLryaLabAB50();
    std::string variationKey;
    std::vector<std::string> audienceTags = {"first_time_user"};
    AssignmentStatus status = lab.ActivateExperiment("report_test_first_time_user",
                                                     "user_id_0",
                                                     audienceTags,
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("treatment", variationKey);
  }

  {
    // Experiment has audienceTags, user set is empty
    LryaLab lab = CreateLryaLabAB50();
    std::string variationKey;
    AssignmentStatus status = lab.ActivateExperiment("report_test_first_time_user",
                                                     "user_id_0",
                                                     std::vector<std::string>(0),
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::AudienceMismatch, status);
  }

  {
    // Experiment audienceTags do not match user audienceTags
    LryaLab lab = CreateLryaLabAB50();
    std::string variationKey;
    std::vector<std::string> audienceTags = {"country_de"};
    AssignmentStatus status = lab.ActivateExperiment("report_test_first_time_user",
                                                     "user_id_0",
                                                     audienceTags,
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::AudienceMismatch, status);
  }

  {
    // Experiment does not have any audienceTags
    LryaLab lab = CreateLryaLabAB50();
    std::string variationKey;
    std::vector<std::string> audienceTags = {"country_de"};
    AssignmentStatus status = lab.ActivateExperiment("report_test",
                                                     "user_id_8",
                                                     audienceTags,
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("treatment", variationKey);
  }
}

TEST_F(LryaLabTest, ActivateExperimentIsRunning) {
  LryaLab lab = CreateLryaLabAB50();

  // Test that time points are accurate
  {
    uint32_t epochSec = Util::EpochSecFromIso8601UTCDateString("2017-04-01T00:00:00Z");

    std::string variationKey;
    AssignmentStatus status;
    status = lab.ActivateExperiment("report_test_times",
                                    "user_id_8",
                                    std::vector<std::string>(0),
                                    epochSec,
                                    variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("treatment", variationKey);
  }
}

TEST_F(LryaLabTest, ActivateExperimentBeforeStarted)
{
  LryaLab lab = CreateLryaLabAB50();

  {
    uint32_t epochSec = Util::EpochSecFromIso8601UTCDateString("2017-03-01T00:00:00Z");

    std::string variationKey;
    AssignmentStatus status;
    status = lab.ActivateExperiment("report_test_times",
                                    "user_id_8",
                                    std::vector<std::string>(0),
                                    epochSec,
                                    variationKey);
    EXPECT_EQ(AssignmentStatus::ExperimentNotRunning, status);
  }
}

TEST_F(LryaLabTest, ActivateExperimentAfterStopped)
{
  LryaLab lab = CreateLryaLabAB50();

  {
    uint32_t epochSec = Util::EpochSecFromIso8601UTCDateString("2017-05-01T00:00:00Z");

    std::string variationKey;
    AssignmentStatus status;
    status = lab.ActivateExperiment("report_test_times",
                                    "user_id_8",
                                    std::vector<std::string>(0),
                                    epochSec,
                                    variationKey);
    EXPECT_EQ(AssignmentStatus::ExperimentNotRunning, status);
  }
}

TEST_F(LryaLabTest, ActivateExperimentWhilePaused)
{
  LryaLab lab = CreateLryaLabAB50();

  {
    uint32_t epochSec = Util::EpochSecFromIso8601UTCDateString("2017-04-11T00:00:00Z");

    std::string variationKey;
    AssignmentStatus status;
    status = lab.ActivateExperiment("report_test_times",
                                    "user_id_8",
                                    std::vector<std::string>(0),
                                    epochSec,
                                    variationKey);
    EXPECT_EQ(AssignmentStatus::ExperimentNotRunning, status);
  }
}

TEST_F(LryaLabTest, ActivateExperimentAfterResume)
{
  LryaLab lab = CreateLryaLabAB50();

  {
    uint32_t epochSec = Util::EpochSecFromIso8601UTCDateString("2017-04-21T00:00:00Z");

    std::string variationKey;
    AssignmentStatus status;
    status = lab.ActivateExperiment("report_test_times",
                                    "user_id_8",
                                    std::vector<std::string>(0),
                                    epochSec,
                                    variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
  }
}

TEST_F(LryaLabTest, ForceActivateExperiment)
{
  LryaLab lab = CreateLryaLabAB50();

  std::string variationKey;
  AssignmentStatus status;
  status = lab.ForceActivateExperiment("report_test",
                                       "user_id_8",
                                       "control");
  EXPECT_EQ(AssignmentStatus::ForceAssigned, status);
}

TEST_F(LryaLabTest, ForceActivateExperimentFailsForInvalidVariant)
{
  LryaLab lab = CreateLryaLabAB50();

  std::string variationKey;
  AssignmentStatus status;
  status = lab.ForceActivateExperiment("report_test",
                                       "user_id_8",
                                       "not_a_valid_variant");
  EXPECT_EQ(AssignmentStatus::VariantNotFound, status);
}

TEST_F(LryaLabTest, ActivateExperimentOverrideAssignment)
{
  LryaLab lab = CreateLryaLabAB50();

  bool willOverride = lab.OverrideExperimentVariation("report_test", "user_id_8", "control");
  EXPECT_TRUE(willOverride);

  std::string variationKey;
  AssignmentStatus status;

  {
    status = lab.ActivateExperiment("report_test",
                                    "user_id_8",
                                    std::vector<std::string>(0),
                                    variationKey);
    EXPECT_EQ(AssignmentStatus::OverrideAssigned, status);
    EXPECT_EQ("control", variationKey);
  }
}

TEST_F(LryaLabTest, ActivateExperimentOverrideAssignmentRequiresValidCriteria)
{
  LryaLab lab = CreateLryaLabAB50();

  bool willOverride = lab.OverrideExperimentVariation("report_test_times", "user_id_8", "control");
  EXPECT_TRUE(willOverride);

  {
    uint32_t epochSec = Util::EpochSecFromIso8601UTCDateString("2017-03-01T00:00:00Z");

    std::string variationKey;
    AssignmentStatus status;
    status = lab.ActivateExperiment("report_test_times",
                                    "user_id_8",
                                    std::vector<std::string>(0),
                                    epochSec,
                                    variationKey);
    EXPECT_EQ(AssignmentStatus::ExperimentNotRunning, status);
  }
}

TEST_F(LryaLabTest, ActivateExperimentFailsToOverrideActiveAssignment)
{
  LryaLab lab = CreateLryaLabAB50();

  {
    // activate experiment
    std::string variationKey;
    AssignmentStatus status;
    status = lab.ActivateExperiment("report_test",
                                    "user_id_8",
                                    std::vector<std::string>(0),
                                    variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("treatment", variationKey);
  }

  // Active Experiment variant can't be overridden
  bool willOverride = lab.OverrideExperimentVariation("report_test", "user_id_8", "control");
  EXPECT_FALSE(willOverride);
}

TEST_F(LryaLabTest, DisableTheLab)
{
  LryaLab lab = CreateLryaLabAB50();
  EXPECT_TRUE(lab.IsEnabled());
  lab.Enable(false);
  EXPECT_FALSE(lab.IsEnabled());

  // Verify that user is unassigned when lab is disabled
  {
    std::string variationKey = "treatment";
    AssignmentStatus status = lab.ActivateExperiment("report_test",
                                                     "user_id_8",
                                                     {},
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Unassigned, status);
    EXPECT_EQ("", variationKey);
  }

  // Verify that override is disabled when lab is disabled
  bool willOverride = lab.OverrideExperimentVariation("report_test", "user_id_8", "control");
  EXPECT_FALSE(willOverride);

  // Verify that force is disabled when lab is disabled
  {
    AssignmentStatus status = lab.ForceActivateExperiment("report_test",
                                                          "user_id_8",
                                                          "treatment");
    EXPECT_EQ(AssignmentStatus::Unassigned, status);

    // Verify that experiment name and variation are evaluated even if lab is disabled when attempting a force
    status = lab.ForceActivateExperiment("invalid",
                                         "user_id_8",
                                         "treatment");
    EXPECT_EQ(AssignmentStatus::ExperimentNotFound, status);
    status = lab.ForceActivateExperiment("report_test",
                                         "user_id_8",
                                         "invalid");
    EXPECT_EQ(AssignmentStatus::VariantNotFound, status);
  }

  // Verify that lab can be re-enabled and experiment assigned
  lab.Enable(true);
  EXPECT_TRUE(lab.IsEnabled());
  {
    std::string variationKey;
    AssignmentStatus status = lab.ActivateExperiment("report_test",
                                                     "user_id_8",
                                                     {},
                                                     variationKey);
    EXPECT_EQ(AssignmentStatus::Assigned, status);
    EXPECT_EQ("treatment", variationKey);
    EXPECT_TRUE(lab.IsActiveExperiment("report_test", "user_id_8"));
  }

  // Verify that lab can be subsequently disabled and experiments are unassigned
  lab.Enable(false);
  EXPECT_FALSE(lab.IsEnabled());
  EXPECT_FALSE(lab.IsActiveExperiment("report_test", "user_id_8"));

}

TEST_F(LryaLabTest, HandleActiveAssignmentUpdate)
{
  size_t assignmentsSize = 0;

  LryaLab lab = CreateLryaLabAB50();
  EXPECT_TRUE(lab.IsEnabled());

  Signal::SmartHandle handle = lab.ActiveAssignmentsUpdatedSignal().ScopedSubscribe(
    [&assignmentsSize](const std::vector<AssignmentDef>& activeAssignments) {
      assignmentsSize = activeAssignments.size();
    });

  std::string variationKey;
  AssignmentStatus status = lab.ActivateExperiment("report_test_all_treatment",
                                                   "user_id_8",
                                                   {},
                                                   variationKey);
  EXPECT_EQ(AssignmentStatus::Assigned, status);
  EXPECT_EQ("treatment", variationKey);
  EXPECT_EQ(1, assignmentsSize);
}

TEST_F(LryaLabTest, RestorePreviousAssignments)
{
  LryaLab lab = CreateLryaLabAB50();
  EXPECT_TRUE(lab.IsEnabled());

  // Simulate previously being in the "control" group for an experiment that changed
  // and put 100% of users in the "treatment" group.  Our user should remain in "control"
  AssignmentStatus status =
    lab.RestoreActiveExperiment("report_test_all_treatment", "user_id_8", "control");
  EXPECT_EQ(AssignmentStatus::Assigned, status);

  std::string variationKey;
  status = lab.ActivateExperiment("report_test_all_treatment",
                                  "user_id_8",
                                  {},
                                  variationKey);
  EXPECT_EQ(AssignmentStatus::Assigned, status);
  EXPECT_EQ("control", variationKey);

  // Test trying to restore an experiment that is no longer running
  uint32_t epochSec = Util::EpochSecFromIso8601UTCDateString("2017-05-01T00:00:00Z");
  status = lab.RestoreActiveExperiment("report_test_times", "user_id_8", "treatment", epochSec);
  EXPECT_EQ(AssignmentStatus::ExperimentNotRunning, status);
}

TEST_F(LryaLabTest, RestoreAssignmentWithAudienceFiltering)
{
  LryaLab lab = CreateLryaLabAB50();

  // First try to assign experiment with audience mismatch
  std::string variationKey;
  std::string experimentKey = "report_test_first_time_user";
  std::string userId = "user_id_0";
  std::vector<std::string> audienceTags = {};
  AssignmentStatus status;
  status = lab.ActivateExperiment(experimentKey,
                                  userId,
                                  audienceTags,
                                  variationKey);
  EXPECT_EQ(AssignmentStatus::AudienceMismatch, status);
  EXPECT_EQ("", variationKey);

  // Now restore an assignment, even though we are NOT in the audience
  status = lab.RestoreActiveExperiment(experimentKey, userId, "treatment");

  // A subsequent call to activate the experiment should succeed, even if the audience
  // does not match
  status = lab.ActivateExperiment(experimentKey,
                                  userId,
                                  audienceTags,
                                  variationKey);
  EXPECT_EQ(AssignmentStatus::Assigned, status);
  EXPECT_EQ("treatment", variationKey);

}


} // namespace LryaLab
} // namespace DriveEngine
} // namespace Lrya
