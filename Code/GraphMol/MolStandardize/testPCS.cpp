//
//  Copyright (C) 2018 Susan H. Leung
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <string>
#include <GraphMol/RDKixBase.h>
#include <fstream>
#include <iostream>
#include <RDGeneral/BadFileException.h>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include "Metal.h"
#include "Validate.h"
#include "MolStandardize.h"
#include <GraphMol/RDKixBase.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/ROMol.h>
#include <RDGeneral/Invariant.h>
typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

using namespace RDKix;

typedef enum {
  StandardizeSmShort,
  StandardizeSmLong,
  FragmentShort,
  FragmentLong,
  TautomerShort,
  TautomerLong,
  ValidateShort,
  ValidateLong,
  MetalShort,
  MetalLong,
  NormalizeShort,
  NormalizeLong,
  ReionizeShort,
  ReionizeLong
} RDKixStandardizeMode;

RDKixStandardizeMode setMode(const std::string &argv1,
                             const std::string &argv2) {
  RDKixStandardizeMode standardize_mode;
  if (argv1 == "Metal" && argv2 == "short") {
    standardize_mode = RDKixStandardizeMode::MetalShort;
  }
  if (argv1 == "Metal" && argv2 == "long") {
    standardize_mode = RDKixStandardizeMode::MetalLong;
  }
  if (argv1 == "Fragment" && argv2 == "short") {
    standardize_mode = RDKixStandardizeMode::FragmentShort;
  }
  if (argv1 == "Fragment" && argv2 == "long") {
    standardize_mode = RDKixStandardizeMode::FragmentLong;
  }
  if (argv1 == "StandardizeSm" && argv2 == "short") {
    standardize_mode = RDKixStandardizeMode::StandardizeSmShort;
  }
  if (argv1 == "StandardizeSm" && argv2 == "long") {
    standardize_mode = RDKixStandardizeMode::StandardizeSmLong;
  }
  if (argv1 == "Validate" && argv2 == "short") {
    standardize_mode = RDKixStandardizeMode::ValidateShort;
  }
  if (argv1 == "Validate" && argv2 == "long") {
    standardize_mode = RDKixStandardizeMode::ValidateLong;
  }
  if (argv1 == "Normalize" && argv2 == "short") {
    standardize_mode = RDKixStandardizeMode::NormalizeShort;
  }
  if (argv1 == "Normalize" && argv2 == "long") {
    standardize_mode = RDKixStandardizeMode::NormalizeLong;
  }
  if (argv1 == "Reionize" && argv2 == "short") {
    standardize_mode = RDKixStandardizeMode::ReionizeShort;
  }
  if (argv1 == "Reionize" && argv2 == "long") {
    standardize_mode = RDKixStandardizeMode::ReionizeLong;
  }
  // check if argv is within defined enum types
  if (standardize_mode < RDKixStandardizeMode::StandardizeSmShort ||
      standardize_mode > RDKixStandardizeMode::ReionizeLong) {
    throw ValueErrorException("Invalid RDKix standardize mode");
  }
  std::cout << "Mode: " << standardize_mode << std::endl;

  return standardize_mode;
}

std::pair<std::string, std::string> readLine(const std::string &line) {
  std::pair<std::string, std::string> smiles =
      std::pair<std::string, std::string>("", "");
  // empty line
  if (line.length() == 0) {
    return smiles;
  }
  if (line.substr(0, 2) == "//") {
    // comment line
    return smiles;
  }

  boost::char_separator<char> tabSep("\t");
  tokenizer tokens(line, tabSep);
  std::vector<std::string> result(tokens.begin(), tokens.end());

  // line must have at least two tab separated values
  if (result.size() < 2) {
    std::cout << "Invalid line." << std::endl;
    return smiles;
  }

  std::string smi = result[0];
  boost::erase_all(smi, " ");
  result.erase(result.begin());  // delete first element

  std::string molvsSmi;
  // dealing with multiple outputs from molvs
  unsigned int counter = 0;
  for (const auto &r : result) {
    if (counter == 0) {
      molvsSmi = r;
    } else {
      molvsSmi = molvsSmi + " " + r;
    }
    ++counter;
  }

  //	tokenizer::iterator token = tokens.begin();
  //
  //	// smiles from PCS
  //	std::string smi = *token;
  //	boost::erase_all(smi, " ");
  //	++token;
  //
  //	// smiles after MolVS
  //	std::string molvsSmi = *token;
  ////	boost::erase_all(molvsSmi, " ");
  //	++token;

  return std::pair<std::string, std::string>(smi, molvsSmi);
}

std::vector<std::pair<std::string, std::string>> readCSV(
    const RDKixStandardizeMode &func) {
  std::string rdbase = std::getenv("RDBASE");
  std::string filename;
  switch (func) {
    case RDKixStandardizeMode::MetalShort:
      filename =
          rdbase + "/rdkix/Chem/MolStandardize/test_data/1kPCS_metals.csv.gz";
      break;
    case RDKixStandardizeMode::MetalLong:
      filename =
          rdbase + "/rdkix/Chem/MolStandardize/test_data/100kPCS_metals.csv.gz";
      break;
    case RDKixStandardizeMode::StandardizeSmShort:
      filename =
          rdbase +
          "/rdkix/Chem/MolStandardize/test_data/1kPCS_standardize_sm.csv.gz";
      break;
    case RDKixStandardizeMode::StandardizeSmLong:
      filename =
          rdbase +
          "/rdkix/Chem/MolStandardize/test_data/100kPCS_standardize_sm.csv.gz";
      break;
    case RDKixStandardizeMode::ValidateShort:
      filename =
          rdbase + "/rdkix/Chem/MolStandardize/test_data/1kPCS_validate.csv.gz";
      // filename = "/data/dipper/leung/gsoc/downloads/1kPCS_validate.csv.gz";
      break;
    case RDKixStandardizeMode::ValidateLong:
      filename = rdbase +
                 "/rdkix/Chem/MolStandardize/test_data/100kPCS_validate.csv.gz";
      // filename = "/data/dipper/leung/gsoc/downloads/100kPCS_validate.csv.gz";
      break;
    case RDKixStandardizeMode::FragmentShort:
      filename =
          rdbase + "/rdkix/Chem/MolStandardize/test_data/1kPCS_fragment.csv.gz";
      break;
    case RDKixStandardizeMode::FragmentLong:
      filename = rdbase +
                 "/rdkix/Chem/MolStandardize/test_data/100kPCS_fragment.csv.gz";
      break;
    case RDKixStandardizeMode::ReionizeShort:
      filename =
          rdbase + "/rdkix/Chem/MolStandardize/test_data/1kPCS_reionize.csv.gz";
      break;
    case RDKixStandardizeMode::ReionizeLong:
      filename = rdbase +
                 "/rdkix/Chem/MolStandardize/test_data/100kPCS_reionize.csv.gz";
      break;
    case RDKixStandardizeMode::NormalizeShort:
      filename = rdbase +
                 "/rdkix/Chem/MolStandardize/test_data/1kPCS_normalize.csv.gz";
      break;
    case RDKixStandardizeMode::NormalizeLong:
      filename =
          rdbase +
          "/rdkix/Chem/MolStandardize/test_data/100kPCS_normalize.csv.gz";
      break;
  }

  std::cout << "Reading: " << filename << std::endl;

  std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);
  if (!file) {
    std::cerr << "Unable to open file.\n";
    exit(1);  // call system to stop
  }
  boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
  inbuf.push(boost::iostreams::gzip_decompressor());
  inbuf.push(file);
  // Convert streambuf to istream
  std::istream instream(&inbuf);
  // Iterate lines
  std::string line;
  std::vector<std::pair<std::string, std::string>> res;
  while (std::getline(instream, line)) {
    std::pair<std::string, std::string> smiles = readLine(line);
    // smiles.first and smiles.second will be empty for comment lines
    if (!(smiles.first == "" & smiles.second == "")) {
      res.push_back(smiles);
    }
  }
  // Cleanup
  file.close();
  return res;
}

std::string rdkixMolStandardizeMetal(const std::string &smi) {
  MolStandardize::MetalDisconnector md;
  std::unique_ptr<RWMol> m(SmilesToMol(smi));
  md.disconnect(*m);
  //	std::cout << "Rdkix standardize: " << MolToSmiles(*m) << std::endl;
  return MolToSmiles(*m);
}

std::string rdkixMolStandardizeReionize(const std::string &smi) {
  MolStandardize::CleanupParameters params;
  std::unique_ptr<RWMol> m(SmilesToMol(smi));
  RWMOL_SPTR reionized(MolStandardize::reionize(m.get(), params));
  return MolToSmiles(*reionized);
}

std::string rdkixMolStandardizeNormalize(const std::string &smi) {
  MolStandardize::CleanupParameters params;
  std::unique_ptr<RWMol> m(SmilesToMol(smi));
  RWMOL_SPTR normalized(MolStandardize::normalize(m.get(), params));
  return MolToSmiles(*normalized);
}

std::string rdkixMolStandardizeFragment(const std::string &smi) {
  MolStandardize::CleanupParameters params;
  std::unique_ptr<RWMol> m(SmilesToMol(smi));
  RWMOL_SPTR fragmentParent(MolStandardize::fragmentParent(*m, params));
  return MolToSmiles(*fragmentParent);
}

std::string rdkixMolStandardizeStandardizeSm(const std::string &smi) {
  return MolStandardize::standardizeSmiles(smi);
}

std::string rdkixMolStandardizeValidate(const std::string &smi) {
  MolStandardize::MolVSValidation vm;
  std::unique_ptr<RWMol> m(SmilesToMol(smi));
  std::vector<MolStandardize::ValidationErrorInfo> errout =
      vm.validate(*m, true);
  std::string res;
  if (errout.size() != 0) {
    unsigned int counter = 0;
    for (const auto &err : errout) {
      if (counter == 0) {
        res = err.what();
      } else {
        res = res + " " + err.what();
      }
      ++counter;
    }
  } else {
    res = "[]";
  }
  return res;
}

void testfunc(const std::vector<std::pair<std::string, std::string>> &molvs_res,
              const RDKixStandardizeMode &func) {
  for (const auto &pair : molvs_res) {
    std::string smi = pair.first;
    std::string rdkix_smi;

    switch (func) {
      case RDKixStandardizeMode::MetalShort:
      case RDKixStandardizeMode::MetalLong:
        rdkix_smi = rdkixMolStandardizeMetal(smi);
        break;
      case RDKixStandardizeMode::ReionizeShort:
      case RDKixStandardizeMode::ReionizeLong:
        rdkix_smi = rdkixMolStandardizeReionize(smi);
        break;
      case RDKixStandardizeMode::NormalizeShort:
      case RDKixStandardizeMode::NormalizeLong:
        rdkix_smi = rdkixMolStandardizeNormalize(smi);
        break;
      case RDKixStandardizeMode::FragmentShort:
      case RDKixStandardizeMode::FragmentLong:
        rdkix_smi = rdkixMolStandardizeFragment(smi);
        break;
      case RDKixStandardizeMode::StandardizeSmShort:
      case RDKixStandardizeMode::StandardizeSmLong:
        rdkix_smi = rdkixMolStandardizeStandardizeSm(smi);
        break;
      case RDKixStandardizeMode::ValidateShort:
      case RDKixStandardizeMode::ValidateLong:
        rdkix_smi = rdkixMolStandardizeValidate(smi);
        break;
    }

    if (rdkix_smi != pair.second) {
      std::cout << "RDKIX DOES NOT MATCH MOLVS" << std::endl;
      std::cout << "smi, molvs standardize, rdkix standardize" << std::endl;
      std::cout << pair.first << std::endl
                << pair.second << std::endl
                << rdkix_smi << std::endl;
    }

    TEST_ASSERT(rdkix_smi == pair.second);
  }
}

int main(int argc, char *argv[]) {
  RDKixStandardizeMode standardize_mode = setMode(argv[1], argv[2]);
  std::vector<std::pair<std::string, std::string>> res =
      readCSV(standardize_mode);
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << "--func --short/long" << std::endl;
    return 1;
  }

  testfunc(res, standardize_mode);
  return 0;
}
