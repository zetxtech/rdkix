//
//  Copyright 2001-2021 Greg Landrum and other RDKix contributors
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <RDGeneral/export.h>
#ifndef RD_TYPES_H
#define RD_TYPES_H

#ifdef WIN32
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#define _DEFINED_USE_MATH_DEFINES
#endif
#endif
#include <cmath>
#ifdef _DEFINED_USE_MATH_DEFINES
#undef _DEFINED_USE_MATH_DEFINES
#undef _USE_MATH_DEFINES
#endif

#include "Invariant.h"
#include "Dict.h"

#include <vector>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <numeric>
#include <list>
#include <limits>

#include <cstring>
#include <any>
#include <RDGeneral/BoostStartInclude.h>
#include <boost/lexical_cast.hpp>
#include <RDGeneral/BoostEndInclude.h>

namespace RDKix {

namespace detail {
// used in various places for computed properties
RDKIX_RDGENERAL_EXPORT extern const std::string computedPropName;
}  // namespace detail

namespace common_properties {
///////////////////////////////////////////////////////////////
// Molecule Props
RDKIX_RDGENERAL_EXPORT extern const std::string _Name;            // string
RDKIX_RDGENERAL_EXPORT extern const std::string MolFileInfo;      // string
RDKIX_RDGENERAL_EXPORT extern const std::string MolFileComments;  // string
RDKIX_RDGENERAL_EXPORT extern const std::string
    _2DConf;  // int (combine into dimension?)
RDKIX_RDGENERAL_EXPORT extern const std::string _3DConf;  // int
RDKIX_RDGENERAL_EXPORT extern const std::string
    _doIsoSmiles;  // int (should probably be removed)
RDKIX_RDGENERAL_EXPORT extern const std::string extraRings;  // vec<vec<int> >
RDKIX_RDGENERAL_EXPORT extern const std::string
    _smilesAtomOutputOrder;  // vec<int> computed
RDKIX_RDGENERAL_EXPORT extern const std::string
    _smilesBondOutputOrder;  // vec<int> computed
RDKIX_RDGENERAL_EXPORT extern const std::string _StereochemDone;  // int
RDKIX_RDGENERAL_EXPORT extern const std::string _NeedsQueryScan;  // int (bool)
RDKIX_RDGENERAL_EXPORT extern const std::string _fragSMARTS;      // std::string
RDKIX_RDGENERAL_EXPORT extern const std::string
    maxAttachIdx;  // int TemplEnumTools.cpp
RDKIX_RDGENERAL_EXPORT extern const std::string origNoImplicit;  // int (bool)
RDKIX_RDGENERAL_EXPORT extern const std::string
    ringMembership;  //? unused (molopstest.cpp)

// Computed Values
// ConnectivityDescriptors
RDKIX_RDGENERAL_EXPORT extern const std::string
    _connectivityHKDeltas;  // std::vector<double> computed
RDKIX_RDGENERAL_EXPORT extern const std::string
    _connectivityNVals;  // std::vector<double> computed

RDKIX_RDGENERAL_EXPORT extern const std::string
    _crippenLogP;  // double computed
RDKIX_RDGENERAL_EXPORT extern const std::string
    _crippenLogPContribs;  // std::vector<double> computed

RDKIX_RDGENERAL_EXPORT extern const std::string _crippenMR;  // double computed
RDKIX_RDGENERAL_EXPORT extern const std::string
    _crippenMRContribs;  // std::vector<double> computed

RDKIX_RDGENERAL_EXPORT extern const std::string _labuteASA;  // double computed
RDKIX_RDGENERAL_EXPORT extern const std::string
    _labuteAtomContribs;  // vec<double> computed
RDKIX_RDGENERAL_EXPORT extern const std::string
    _labuteAtomHContrib;  // double computed

RDKIX_RDGENERAL_EXPORT extern const std::string _tpsa;  // double computed
RDKIX_RDGENERAL_EXPORT extern const std::string
    _tpsaAtomContribs;  // vec<double> computed

RDKIX_RDGENERAL_EXPORT extern const std::string
    numArom;  // int computed (only uses in tests?)
RDKIX_RDGENERAL_EXPORT extern const std::string
    _MMFFSanitized;  // int (bool) computed

RDKIX_RDGENERAL_EXPORT extern const std::string
    _CrippenLogP;  // Unused (in the basement)
RDKIX_RDGENERAL_EXPORT extern const std::string
    _CrippenMR;  // Unused (in the basement)
RDKIX_RDGENERAL_EXPORT extern const std::string
    _GasteigerCharge;  // used to hold partial charges
RDKIX_RDGENERAL_EXPORT extern const std::string
    _GasteigerHCharge;  // used to hold partial charges from implicit Hs

///////////////////////////////////////////////////////////////
// Atom Props

// Chirality stuff
RDKIX_RDGENERAL_EXPORT extern const std::string
    _BondsPotentialStereo;  // int (or bool) COMPUTED
RDKIX_RDGENERAL_EXPORT extern const std::string
    _CIPCode;  // std::string COMPUTED
RDKIX_RDGENERAL_EXPORT extern const std::string _CIPRank;  // int COMPUTED
RDKIX_RDGENERAL_EXPORT extern const std::string _ChiralityPossible;  // int
RDKIX_RDGENERAL_EXPORT extern const std::string
    _UnknownStereo;  // int (bool) AddHs/Chirality
RDKIX_RDGENERAL_EXPORT extern const std::string
    _ringStereoAtoms;  // int vect Canon/Chiral/MolHash/MolOps//Renumber//RWmol
RDKIX_RDGENERAL_EXPORT extern const std::string
    _ringStereochemCand;  // chirality bool COMPUTED
RDKIX_RDGENERAL_EXPORT extern const std::string
    _ringStereoWarning;  // obsolete ?
RDKIX_RDGENERAL_EXPORT extern const std::string _chiralPermutation;  // int

// Smiles parsing
RDKIX_RDGENERAL_EXPORT extern const std::string _SmilesStart;  // int
RDKIX_RDGENERAL_EXPORT extern const std::string
    _TraversalBondIndexOrder;  // ? unused
RDKIX_RDGENERAL_EXPORT extern const std::string
    _TraversalRingClosureBond;  // unsigned int
RDKIX_RDGENERAL_EXPORT extern const std::string _TraversalStartPoint;  // bool
RDKIX_RDGENERAL_EXPORT extern const std::string
    _queryRootAtom;  // int SLNParse/SubstructMatch
RDKIX_RDGENERAL_EXPORT extern const std::string _hasMassQuery;  // atom bool
RDKIX_RDGENERAL_EXPORT extern const std::string _protected;  // atom int (bool)
RDKIX_RDGENERAL_EXPORT extern const std::string
    _supplementalSmilesLabel;  // atom string (SmilesWrite)
RDKIX_RDGENERAL_EXPORT extern const std::string
    _unspecifiedOrder;  // atom int (bool) smarts/smiles
RDKIX_RDGENERAL_EXPORT extern const std::string
    _RingClosures;  // INT_VECT smarts/smiles/canon
RDKIX_RDGENERAL_EXPORT extern const std::string
    atomLabel;  // atom string from CXSMILES
RDKIX_RDGENERAL_EXPORT extern const std::string OxidationNumber;  // int

// MDL Style Properties (MolFileParser)
RDKIX_RDGENERAL_EXPORT extern const std::string molAtomMapNumber;   // int
RDKIX_RDGENERAL_EXPORT extern const std::string molFileAlias;       // string
RDKIX_RDGENERAL_EXPORT extern const std::string molFileValue;       // string
RDKIX_RDGENERAL_EXPORT extern const std::string molInversionFlag;   // int
RDKIX_RDGENERAL_EXPORT extern const std::string molParity;          // int
RDKIX_RDGENERAL_EXPORT extern const std::string molStereoCare;      // int
RDKIX_RDGENERAL_EXPORT extern const std::string molRxnComponent;    // int
RDKIX_RDGENERAL_EXPORT extern const std::string molRxnRole;         // int
RDKIX_RDGENERAL_EXPORT extern const std::string molTotValence;      // int
RDKIX_RDGENERAL_EXPORT extern const std::string molSubstCount;      // int
RDKIX_RDGENERAL_EXPORT extern const std::string molAttachPoint;     // int
RDKIX_RDGENERAL_EXPORT extern const std::string molAttachOrder;     // int
RDKIX_RDGENERAL_EXPORT extern const std::string molAtomClass;       // string
RDKIX_RDGENERAL_EXPORT extern const std::string molAtomSeqId;       // int
RDKIX_RDGENERAL_EXPORT extern const std::string molRxnExactChange;  // int
RDKIX_RDGENERAL_EXPORT extern const std::string molReactStatus;     // int
RDKIX_RDGENERAL_EXPORT extern const std::string molFileLinkNodes;   // string

RDKIX_RDGENERAL_EXPORT extern const std::string _MolFileRLabel;  // unsigned int
RDKIX_RDGENERAL_EXPORT extern const std::string _MolFileChiralFlag;  // int
RDKIX_RDGENERAL_EXPORT extern const std::string _MolFileAtomQuery;   // int
RDKIX_RDGENERAL_EXPORT extern const std::string _MolFileBondQuery;   // int
RDKIX_RDGENERAL_EXPORT extern const std::string _MolFileBondEndPts;  // string
RDKIX_RDGENERAL_EXPORT extern const std::string _MolFileBondAttach;  // string
RDKIX_RDGENERAL_EXPORT extern const std::string
    _MolFileBondType;  // unsigned int
RDKIX_RDGENERAL_EXPORT extern const std::string
    _MolFileBondStereo;  // unsigned int
RDKIX_RDGENERAL_EXPORT extern const std::string
    _MolFileBondCfg;  // unsigned int

RDKIX_RDGENERAL_EXPORT extern const std::string
    MRV_SMA;  // smarts string from Marvin
RDKIX_RDGENERAL_EXPORT extern const std::string dummyLabel;  // atom string

RDKIX_RDGENERAL_EXPORT extern const std::string
    _QueryAtomGenericLabel;  // string

// Reaction Information (Reactions.cpp)
RDKIX_RDGENERAL_EXPORT extern const std::string _QueryFormalCharge;  //  int
RDKIX_RDGENERAL_EXPORT extern const std::string _QueryHCount;        // int
RDKIX_RDGENERAL_EXPORT extern const std::string _QueryIsotope;       // int
RDKIX_RDGENERAL_EXPORT extern const std::string
    _QueryMass;  // int = round(float * 1000)
RDKIX_RDGENERAL_EXPORT extern const std::string
    _ReactionDegreeChanged;                                // int (bool)
RDKIX_RDGENERAL_EXPORT extern const std::string NullBond;  // int (bool)
RDKIX_RDGENERAL_EXPORT extern const std::string _rgroupAtomMaps;
RDKIX_RDGENERAL_EXPORT extern const std::string _rgroupBonds;
RDKIX_RDGENERAL_EXPORT extern const std::string reactantAtomIdx;
RDKIX_RDGENERAL_EXPORT extern const std::string reactionMapNum;

// SLN
RDKIX_RDGENERAL_EXPORT extern const std::string
    _AtomID;  // unsigned int SLNParser
RDKIX_RDGENERAL_EXPORT extern const std::string
    _starred;  // atom int COMPUTED (SLN)
RDKIX_RDGENERAL_EXPORT extern const std::string
    _SLN_s;  // string SLNAttribs (chiral info)
RDKIX_RDGENERAL_EXPORT extern const std::string _Unfinished_SLN_;  // int (bool)

// Smarts Smiles
RDKIX_RDGENERAL_EXPORT extern const std::string _brokenChirality;  // atom bool
RDKIX_RDGENERAL_EXPORT extern const std::string isImplicit;  // atom int (bool)
RDKIX_RDGENERAL_EXPORT extern const std::string
    smilesSymbol;  // atom string (only used in test?)

// Tripos
RDKIX_RDGENERAL_EXPORT extern const std::string
    _TriposAtomType;  // string Mol2FileParser
// missing defs for _TriposAtomName//_TriposPartialCharge...

// molecule drawing
RDKIX_RDGENERAL_EXPORT extern const std::string _displayLabel;   // string
RDKIX_RDGENERAL_EXPORT extern const std::string _displayLabelW;  // string

///////////////////////////////////////////////////////////////
// misc props
RDKIX_RDGENERAL_EXPORT extern const std::string
    TWOD;  // need THREED -> confusing using in TDTMol supplier
           //  converge with _2DConf?
RDKIX_RDGENERAL_EXPORT extern const std::string BalabanJ;   // mol double
RDKIX_RDGENERAL_EXPORT extern const std::string BalanbanJ;  // typo!! fix...

RDKIX_RDGENERAL_EXPORT extern const std::string Discrims;  // FragCatalog Entry
// Subgraphs::DiscrimTuple (uint32,uint32,uint32)
RDKIX_RDGENERAL_EXPORT extern const std::string
    DistanceMatrix_Paths;  // boost::shared_array<double>
//  - note, confusing creation of names in
//  - getDistanceMat
RDKIX_RDGENERAL_EXPORT extern const std::string internalRgroupSmiles;
RDKIX_RDGENERAL_EXPORT extern const std::string molNote;
RDKIX_RDGENERAL_EXPORT extern const std::string atomNote;
RDKIX_RDGENERAL_EXPORT extern const std::string bondNote;
RDKIX_RDGENERAL_EXPORT extern const std::string _isotopicHs;

}  // namespace common_properties
#ifndef WIN32
typedef long long int LONGINT;
#else
typedef __int64 LONGINT;
#endif
#ifdef max
#undef max  // FUCK I hate this nonsense
#endif
#ifdef min
#undef min  // FUCK I hate this nonsense
#endif

RDKIX_RDGENERAL_EXPORT extern const double MAX_DOUBLE;
RDKIX_RDGENERAL_EXPORT extern const double EPS_DOUBLE;
RDKIX_RDGENERAL_EXPORT extern const double SMALL_DOUBLE;
RDKIX_RDGENERAL_EXPORT extern const double MAX_INT;
RDKIX_RDGENERAL_EXPORT extern const double MAX_LONGINT;

typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;

typedef std::vector<int> INT_VECT;
typedef INT_VECT::iterator INT_VECT_I;
typedef INT_VECT::const_iterator INT_VECT_CI;
typedef INT_VECT::reverse_iterator INT_VECT_RI;
typedef INT_VECT::const_reverse_iterator INT_VECT_CRI;

typedef std::list<int> INT_LIST;
typedef INT_LIST::iterator INT_LIST_I;
typedef INT_LIST::const_iterator INT_LIST_CI;

typedef std::list<INT_VECT> LIST_INT_VECT;
typedef LIST_INT_VECT::iterator LIST_INT_VECT_I;
typedef LIST_INT_VECT::const_iterator LIST_INT_VECT_CI;

typedef std::vector<INT_VECT> VECT_INT_VECT;
typedef VECT_INT_VECT::iterator VECT_INT_VECT_I;
typedef VECT_INT_VECT::const_iterator VECT_INT_VECT_CI;

typedef std::vector<UINT>::const_iterator UINT_VECT_CI;
typedef std::vector<UINT> UINT_VECT;

typedef std::vector<std::string>::const_iterator STR_VECT_CI;
typedef std::vector<std::string>::iterator STR_VECT_I;
typedef std::vector<std::string> STR_VECT;

typedef std::vector<double> DOUBLE_VECT;
typedef DOUBLE_VECT::iterator DOUBLE_VECT_I;
typedef DOUBLE_VECT::const_iterator DOUBLE_VECT_CI;
typedef std::vector<DOUBLE_VECT> VECT_DOUBLE_VECT;
typedef VECT_DOUBLE_VECT::iterator VECT_DOUBLE_VECT_I;
typedef VECT_DOUBLE_VECT::const_iterator VECT_DOUBLE_VECT_CI;

typedef std::map<std::string, UINT> STR_UINT_MAP;
typedef std::map<std::string, UINT>::const_iterator STR_UINT_MAP_CI;

typedef std::map<int, INT_VECT> INT_INT_VECT_MAP;
typedef INT_INT_VECT_MAP::const_iterator INT_INT_VECT_MAP_CI;

typedef std::map<int, int> INT_MAP_INT;
typedef INT_MAP_INT::iterator INT_MAP_INT_I;
typedef INT_MAP_INT::const_iterator INT_MAP_INT_CI;

typedef std::deque<int> INT_DEQUE;
typedef INT_DEQUE::iterator INT_DEQUE_I;
typedef INT_DEQUE::const_iterator INT_DEQUE_CI;

typedef std::map<int, INT_DEQUE> INT_INT_DEQ_MAP;
typedef INT_INT_DEQ_MAP::const_iterator INT_INT_DEQ_MAP_CI;

typedef std::set<int> INT_SET;
typedef INT_SET::iterator INT_SET_I;
typedef INT_SET::const_iterator INT_SET_CI;

//! functor to compare two doubles with a tolerance
struct RDKIX_RDGENERAL_EXPORT ltDouble {
 public:
  ltDouble() {}
  bool operator()(double d1, double d2) const {
    if (fabs(d1 - d2) < _tol) {
      return false;
    } else {
      return (d1 < d2);
    }
  }

 private:
  double _tol{1.0e-8};
};

//! std::map from double to integer.
typedef std::map<double, int, ltDouble> DOUBLE_INT_MAP;

//! functor for returning the larger of two values
template <typename T>
struct RDKIX_RDGENERAL_EXPORT larger_of {
  T operator()(T arg1, T arg2) { return arg1 > arg2 ? arg1 : arg2; }
};

//! functor for comparing two strings
struct RDKIX_RDGENERAL_EXPORT charptr_functor {
  bool operator()(const char *s1, const char *s2) const {
    // std::cout << s1 << " " << s2 << " " << strcmp(s1, s2) << "\n";

    return strcmp(s1, s2) < 0;
  }
};

//! \brief calculate the union of two INT_VECTs and put the results in a
//! third vector
RDKIX_RDGENERAL_EXPORT void Union(const INT_VECT &r1, const INT_VECT &r2,
                                  INT_VECT &res);

//! \brief calculate the intersection of two INT_VECTs and put the results in a
//! third vector
RDKIX_RDGENERAL_EXPORT void Intersect(const INT_VECT &r1, const INT_VECT &r2,
                                      INT_VECT &res);

//! calculating the union of the INT_VECT's in a VECT_INT_VECT
/*!
    \param rings   the INT_VECT's to consider
    \param res     used to return results
    \param exclude any values in this optional INT_VECT will be excluded
           from the union.
*/
RDKIX_RDGENERAL_EXPORT void Union(const VECT_INT_VECT &rings, INT_VECT &res,
                                  const INT_VECT *exclude = nullptr);

//! given a current combination of numbers change it to the next possible
// combination
/*!
  \param comb the <b>sorted</b> vector to consider
  \param tot the maximum number possible in the vector

  \return -1 on failure, the index of the last number changed on success.
  Example:
    for all combinations 3 of numbers between 0 and tot=5
    given (0,1,2) the function wil return (0,1,3) etc.


*/
RDKIX_RDGENERAL_EXPORT int nextCombination(INT_VECT &comb, int tot);

};  // namespace RDKix

#endif
