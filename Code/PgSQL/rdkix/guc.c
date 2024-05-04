//
//  Copyright (c) 2010-2021 Novartis Institutes for BioMedical Research Inc.
//    and other RDKix contributors
//  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Novartis Institutes for BioMedical Research Inc.
//       nor the names of its contributors may be used to endorse or promote
//       products derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include <postgres.h>
#include <fmgr.h>
#include <utils/guc.h>

#include "guc.h"

static double rdkix_tanimoto_smlar_limit = 0.5;
static double rdkix_dice_smlar_limit = 0.5;
static double rdkix_agent_FP_bit_ratio = 0.2;
static double rdkix_threshold_unmapped_reactant_atoms = 0.2;
static bool rdkix_do_chiral_sss = false;
static bool rdkix_do_enhanced_stereo_sss = false;
static bool rdkix_ignore_reaction_agents = false;
static bool rdkix_move_unmmapped_reactants_to_agents = true;
static bool rdkix_init_reaction = true;
static bool rdkix_guc_inited = false;

#define SSS_FP_SIZE 2048
#define LAYERED_FP_SIZE 1024
#define RDKIX_FP_SIZE 1024
#define MORGAN_FP_SIZE 512
#define FEATMORGAN_FP_SIZE 512
#define HASHED_TORSION_FP_SIZE 1024
#define HASHED_PAIR_FP_SIZE 2048
#define AVALON_FP_SIZE 512
#define REACTION_SSS_FP_SIZE 4096
#define REACTION_DIFFERENCE_FP_SIZE 2048
#define REACTION_SSS_FP_TYPE 5
#define REACTION_DIFFERENCE_FP_TYPE 1
#define REACTION_DFP_WEIGHT_AGENTS 1
#define REACTION_DFP_WEIGHT_NONAGENTS 10

static int rdkix_sss_fp_size = SSS_FP_SIZE;
static int rdkix_morgan_fp_size = MORGAN_FP_SIZE;
static int rdkix_featmorgan_fp_size = FEATMORGAN_FP_SIZE;
static int rdkix_layered_fp_size = LAYERED_FP_SIZE;
static int rdkix_rdkix_fp_size = RDKIX_FP_SIZE;
static int rdkix_hashed_torsion_fp_size = HASHED_TORSION_FP_SIZE;
static int rdkix_hashed_atompair_fp_size = HASHED_PAIR_FP_SIZE;
static int rdkix_avalon_fp_size = AVALON_FP_SIZE;
static int rdkix_reaction_sss_fp_size = REACTION_SSS_FP_SIZE;
static int rdkix_reaction_difference_fp_size = REACTION_DIFFERENCE_FP_SIZE;
static int rdkix_reaction_sss_fp_type = REACTION_SSS_FP_TYPE;
static int rdkix_reaction_difference_fp_type = REACTION_DIFFERENCE_FP_TYPE;
static int rdkix_difference_FP_weight_agents = REACTION_DFP_WEIGHT_AGENTS;
static int rdkix_difference_FP_weight_nonagents = REACTION_DFP_WEIGHT_NONAGENTS;

#if PG_VERSION_NUM < 90100
#error The earliest supported postgresql version is 9.1
#endif

static void initRDKixGUC() {
  if (rdkix_guc_inited) return;

  DefineCustomRealVariable(
      "rdkix.tanimoto_threshold", "Lower threshold of Tanimoto similarity",
      "Molecules with similarity lower than threshold are not similar by % operation",
      &rdkix_tanimoto_smlar_limit, 0.5, 0.0, 1.0, PGC_USERSET, 0, NULL, NULL,
      NULL);
  DefineCustomRealVariable(
      "rdkix.dice_threshold", "Lower threshold of Dice similarity",
      "Molecules with similarity lower than threshold are not similar by # operation",
      &rdkix_dice_smlar_limit, 0.5, 0.0, 1.0, PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomBoolVariable(
      "rdkix.do_chiral_sss",
      "Should stereochemistry be taken into account in substructure matching",
      "If false (the default), no stereochemistry information is used in substructure matches.",
      &rdkix_do_chiral_sss, false, PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomBoolVariable(
      "rdkix.do_enhanced_stereo_sss",
      "Should enhanced stereochemistry be taken into account in substructure matching",
      "If true, enhanced stereo information is used in substructure matching. Has no effect if rdkix.do_chiral_sss is false.",
      &rdkix_do_enhanced_stereo_sss, false, PGC_USERSET, 0, NULL, NULL, NULL);

  DefineCustomIntVariable(
      "rdkix.sss_fp_size",
      "Size (in bits) of the fingerprint used for substructure screening",
      "Size (in bits) of the fingerprint used for substructure screening",
      &rdkix_sss_fp_size, SSS_FP_SIZE, 64, 4096, PGC_USERSET, 0, NULL, NULL,
      NULL);
  DefineCustomIntVariable(
      "rdkix.morgan_fp_size", "Size (in bits) of morgan fingerprints",
      "Size (in bits) of morgan fingerprints", &rdkix_morgan_fp_size,
      MORGAN_FP_SIZE, 64, 9192, PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.featmorgan_fp_size", "Size (in bits) of featmorgan fingerprints",
      "Size (in bits) of featmorgan fingerprints", &rdkix_featmorgan_fp_size,
      FEATMORGAN_FP_SIZE, 64, 9192, PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.layered_fp_size", "Size (in bits) of layered fingerprints",
      "Size (in bits) of layered fingerprints", &rdkix_layered_fp_size,
      LAYERED_FP_SIZE, 64, 9192, PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.rdkix_fp_size", "Size (in bits) of RDKix fingerprints",
      "Size (in bits) of RDKix fingerprints", &rdkix_rdkix_fp_size,
      RDKIX_FP_SIZE, 64, 9192, PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.hashed_torsion_fp_size",
      "Size (in bits) of topological torsion bit vector fingerprints",
      "Size (in bits) of topological torsion bit vector fingerprints",
      &rdkix_hashed_torsion_fp_size, HASHED_TORSION_FP_SIZE, 64, 9192,
      PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.hashed_atompair_fp_size",
      "Size (in bits) of atom pair bit vector fingerprints",
      "Size (in bits) of atom pair torsion bit vector fingerprints",
      &rdkix_hashed_atompair_fp_size, HASHED_PAIR_FP_SIZE, 64, 9192,
      PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.reaction_sss_fp_size",
      "Size (in bits) of structural chemical reaction fingerprint",
      "Size (in bits) of structural chemical reaction fingerprint",
      &rdkix_reaction_sss_fp_size, REACTION_SSS_FP_SIZE, 64, 9192, PGC_USERSET,
      0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.reaction_difference_fp_size",
      "Size (in bits) of difference chemical reaction fingerprint",
      "Size (in bits) of difference chemical reaction fingerprint",
      &rdkix_reaction_difference_fp_size, REACTION_DIFFERENCE_FP_SIZE, 64, 9192,
      PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.reaction_sss_fp_type",
      "Type of structural chemical reaction fingerprint (1=Atompairs, 2=Topologcal torsion, 3=Morgan2, 4=RDKix, 5=Pattern)",
      "Type of structural chemical reaction fingerprint (1=Atompairs, 2=Topologcal torsion, 3=Morgan2, 4=RDKix, 5=Pattern)",
      &rdkix_reaction_sss_fp_type, REACTION_SSS_FP_TYPE, 1, 5, PGC_USERSET, 0,
      NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.reaction_difference_fp_type",
      "Type of difference chemical reaction fingerprint (1=Atompairs, 2=Topologcal torsion, 3=Morgan2)",
      "Type of difference chemical reaction fingerprint (1=Atompairs, 2=Topologcal torsion, 3=Morgan2)",
      &rdkix_reaction_difference_fp_type, REACTION_DIFFERENCE_FP_TYPE, 1, 3,
      PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomBoolVariable(
      "rdkix.ignore_reaction_agents",
      "Should agents of a chemical reaction be taken into account ",
      "If false (the default), agents (if provided) are taken into account for calculations/requests/similarity measures.",
      &rdkix_ignore_reaction_agents, false, PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomRealVariable(
      "rdkix.agent_FP_bit_ratio",
      "Allow to weight the impact of agents contained in a chemical reaction fingerprint",
      "A scaling factor below 1.0 down weighted the agents of a chemical reaction",
      &rdkix_agent_FP_bit_ratio, 0.2, 0.0, 3.0, PGC_USERSET, 0, NULL, NULL,
      NULL);
  DefineCustomBoolVariable(
      "rdkix.move_unmmapped_reactants_to_agents",
      "Should unmapped reactant  agents of a chemical reaction be taken into account ",
      "If true (the default), agents (if provided) are taken into account for calculations/requests/similarity measures.",
      &rdkix_move_unmmapped_reactants_to_agents, true, PGC_USERSET, 0, NULL,
      NULL, NULL);
  DefineCustomRealVariable(
      "rdkix.threshold_unmapped_reactant_atoms",
      "Set the ratio of allowed unmapped reactant atoms",
      "If the ratio of unmapped atoms in a reactant is larger than this the reactant is removed and added to the agents.",
      &rdkix_threshold_unmapped_reactant_atoms, 0.2, 0.0, 1.0, PGC_USERSET, 0,
      NULL, NULL, NULL);
  DefineCustomBoolVariable(
      "rdkix.init_reaction", "Checks if the reaction is ready for use",
      "By default it is true, however reactions can be used uninitialized too but be aware of potential upcoming errors.",
      &rdkix_init_reaction, true, PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.difference_FP_weight_agents",
      "In reaction difference fingerprints weight factor for agents comapred to reactants and products",
      "In reaction difference fingerprints weight factor for agents comapred to reactants and products",
      &rdkix_difference_FP_weight_agents, REACTION_DFP_WEIGHT_AGENTS, -10, 10,
      PGC_USERSET, 0, NULL, NULL, NULL);
  DefineCustomIntVariable(
      "rdkix.difference_FP_weight_nonagents",
      "In reaction difference fingerprints weight factor for reactants and products comapred to agents",
      "In reaction difference fingerprints weight factor for reactants and products comapred to agents",
      &rdkix_difference_FP_weight_nonagents, REACTION_DFP_WEIGHT_NONAGENTS, 1,
      20, PGC_USERSET, 0, NULL, NULL, NULL);

  DefineCustomIntVariable(
      "rdkix.avalon_fp_size", "Size (in bits) of avalon fingerprints",
      "Size (in bits) of avalon fingerprints", &rdkix_avalon_fp_size,
      AVALON_FP_SIZE, 64, 9192, PGC_USERSET, 0, NULL, NULL, NULL);
  rdkix_guc_inited = true;
}

double getTanimotoLimit(void) {
  if (!rdkix_guc_inited) initRDKixGUC();

  return rdkix_tanimoto_smlar_limit;
}

double getDiceLimit(void) {
  if (!rdkix_guc_inited) initRDKixGUC();

  return rdkix_dice_smlar_limit;
}

bool getDoChiralSSS(void) {
  if (!rdkix_guc_inited) initRDKixGUC();

  return rdkix_do_chiral_sss;
}

bool getDoEnhancedStereoSSS(void) {
  if (!rdkix_guc_inited) initRDKixGUC();

  return rdkix_do_enhanced_stereo_sss;
}

int getSubstructFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_sss_fp_size;
}

int getMorganFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_morgan_fp_size;
}

int getFeatMorganFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_featmorgan_fp_size;
}

int getLayeredFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_layered_fp_size;
}

int getRDKixFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_rdkix_fp_size;
}

int getHashedTorsionFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_hashed_torsion_fp_size;
}

int getHashedAtomPairFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_hashed_atompair_fp_size;
}

int getAvalonFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_avalon_fp_size;
}

int getReactionSubstructFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_reaction_sss_fp_size;
}

int getReactionDifferenceFpSize(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_reaction_difference_fp_size;
}

int getReactionSubstructFpType(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_reaction_sss_fp_type;
}

int getReactionDifferenceFpType(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_reaction_difference_fp_type;
}

bool getIgnoreReactionAgents(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_ignore_reaction_agents;
}

double getReactionStructuralFPAgentBitRatio(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_agent_FP_bit_ratio;
}

bool getMoveUnmappedReactantsToAgents(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_move_unmmapped_reactants_to_agents;
}

double getThresholdUnmappedReactantAtoms(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_threshold_unmapped_reactant_atoms;
}

bool getInitReaction(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_init_reaction;
}

int getReactionDifferenceFPWeightAgents(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_difference_FP_weight_agents;
}

int getReactionDifferenceFPWeightNonagents(void) {
  if (!rdkix_guc_inited) initRDKixGUC();
  return rdkix_difference_FP_weight_nonagents;
}

PGDLLEXPORT void _PG_init(void);
void _PG_init(void) { initRDKixGUC(); }
