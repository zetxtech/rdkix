//
//  Copyright (C) 2013-2021 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
/*! \file MonomerInfo.h

  \brief Defines Monomer information classes

*/
#include <RDGeneral/export.h>
#ifndef RD_MONOMERINFO_H
#define RD_MONOMERINFO_H

#include <string>
#include <utility>
#include <boost/shared_ptr.hpp>

namespace RDKix {

//! The abstract base class for atom-level monomer info
class RDKIX_GRAPHMOL_EXPORT AtomMonomerInfo {
 public:
  typedef enum { UNKNOWN = 0, PDBRESIDUE, OTHER } AtomMonomerType;

  virtual ~AtomMonomerInfo() {}

  AtomMonomerInfo() : d_name("") {}
  AtomMonomerInfo(AtomMonomerType typ, std::string nm = "")
      : d_monomerType(typ), d_name(std::move(nm)) {}
  AtomMonomerInfo(const AtomMonomerInfo &other)
      : d_monomerType(other.d_monomerType), d_name(other.d_name) {}

  const std::string &getName() const { return d_name; }
  void setName(const std::string &nm) { d_name = nm; }
  AtomMonomerType getMonomerType() const { return d_monomerType; }
  void setMonomerType(AtomMonomerType typ) { d_monomerType = typ; }

  virtual AtomMonomerInfo *copy() const { return new AtomMonomerInfo(*this); }

 private:
  AtomMonomerType d_monomerType{UNKNOWN};
  std::string d_name;
};

//! Captures atom-level information about peptide residues
class RDKIX_GRAPHMOL_EXPORT AtomPDBResidueInfo : public AtomMonomerInfo {
 public:
  AtomPDBResidueInfo() : AtomMonomerInfo(PDBRESIDUE) {}
  AtomPDBResidueInfo(const AtomPDBResidueInfo &other)
      : AtomMonomerInfo(other),
        d_serialNumber(other.d_serialNumber),
        d_altLoc(other.d_altLoc),
        d_residueName(other.d_residueName),
        d_residueNumber(other.d_residueNumber),
        d_chainId(other.d_chainId),
        d_insertionCode(other.d_insertionCode),
        d_occupancy(other.d_occupancy),
        d_tempFactor(other.d_tempFactor),
        df_heteroAtom(other.df_heteroAtom),
        d_secondaryStructure(other.d_secondaryStructure),
        d_segmentNumber(other.d_segmentNumber) {}

  AtomPDBResidueInfo(const std::string &atomName, int serialNumber = 0,
                     std::string altLoc = "", std::string residueName = "",
                     int residueNumber = 0, std::string chainId = "",
                     std::string insertionCode = "", double occupancy = 1.0,
                     double tempFactor = 0.0, bool isHeteroAtom = false,
                     unsigned int secondaryStructure = 0,
                     unsigned int segmentNumber = 0)
      : AtomMonomerInfo(PDBRESIDUE, atomName),
        d_serialNumber(serialNumber),
        d_altLoc(std::move(altLoc)),
        d_residueName(std::move(residueName)),
        d_residueNumber(residueNumber),
        d_chainId(std::move(chainId)),
        d_insertionCode(std::move(insertionCode)),
        d_occupancy(occupancy),
        d_tempFactor(tempFactor),
        df_heteroAtom(isHeteroAtom),
        d_secondaryStructure(secondaryStructure),
        d_segmentNumber(segmentNumber) {}

  int getSerialNumber() const { return d_serialNumber; }
  void setSerialNumber(int val) { d_serialNumber = val; }
  const std::string &getAltLoc() const { return d_altLoc; }
  void setAltLoc(const std::string &val) { d_altLoc = val; }
  const std::string &getResidueName() const { return d_residueName; }
  void setResidueName(const std::string &val) { d_residueName = val; }
  int getResidueNumber() const { return d_residueNumber; }
  void setResidueNumber(int val) { d_residueNumber = val; }
  const std::string &getChainId() const { return d_chainId; }
  void setChainId(const std::string &val) { d_chainId = val; }
  const std::string &getInsertionCode() const { return d_insertionCode; }
  void setInsertionCode(const std::string &val) { d_insertionCode = val; }
  double getOccupancy() const { return d_occupancy; }
  void setOccupancy(double val) { d_occupancy = val; }
  double getTempFactor() const { return d_tempFactor; }
  void setTempFactor(double val) { d_tempFactor = val; }
  bool getIsHeteroAtom() const { return df_heteroAtom; }
  void setIsHeteroAtom(bool val) { df_heteroAtom = val; }
  unsigned int getSecondaryStructure() const { return d_secondaryStructure; }
  void setSecondaryStructure(unsigned int val) { d_secondaryStructure = val; }
  unsigned int getSegmentNumber() const { return d_segmentNumber; }
  void setSegmentNumber(unsigned int val) { d_segmentNumber = val; }

  AtomMonomerInfo *copy() const override {
    return static_cast<AtomMonomerInfo *>(new AtomPDBResidueInfo(*this));
  }

 private:
  // the fields here are from the PDB definition
  // (http://www.wwpdb.org/documentation/format33/sect9.html#ATOM) [9 Aug, 2013]
  // element and charge are not present since the atom itself stores that
  // information
  unsigned int d_serialNumber = 0;
  std::string d_altLoc = "";
  std::string d_residueName = "";
  int d_residueNumber = 0;
  std::string d_chainId = "";
  std::string d_insertionCode = "";
  double d_occupancy = 1.0;
  double d_tempFactor = 0.0;
  // additional, non-PDB fields:
  bool df_heteroAtom = false;  // is this from a HETATM record?
  unsigned int d_secondaryStructure = 0;
  unsigned int d_segmentNumber = 0;
};
};  // namespace RDKix
//! allows AtomPDBResidueInfo objects to be dumped to streams
RDKIX_GRAPHMOL_EXPORT std::ostream &operator<<(
    std::ostream &target, const RDKix::AtomPDBResidueInfo &apri);

#endif
