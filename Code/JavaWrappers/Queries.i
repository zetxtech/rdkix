//
//  Copyright (C) 2024 Gareth Jones, Glysade LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

%include <boost_shared_ptr.i>
%shared_ptr(RDKix::QueryAtom)
%shared_ptr(RDKix::QueryBond)

// Adapted from the Python Wrappers

%{
#include <GraphMol/RDKixBase.h>
#include <GraphMol/RDKixQueries.h>
#include <RDGeneral/types.h>
%}

%include <GraphMol/RDKixBase.h>;
%include <GraphMol/RDKixQueries.h>;
%include <RDGeneral/types.h>;

/*
  NOTE: it looks like there is a typo in the below code
  ATOM_GREATER_QUERY is intentionally being used for the LessQueryAtom
  and ATOM_LESS_QUERY for GreaterQueryAtom in the python API.
  The C++ API is internally consistent and logical, but having
  AtomNumLessQueryAtom(6) return atoms where 6 is less than their atomic
  number feels backwards in Python.

 */
 %define QAFUNC1(funcname, func, type)
 %inline %{

   boost::shared_ptr<RDKix::QueryAtom> funcname ## EqualsQueryAtom(type val, bool negate=false) {
    boost::shared_ptr<RDKix::QueryAtom> res(new RDKix::QueryAtom());
    res->setQuery(RDKix:: ## func(val));
    if (negate) {
      res->getQuery()->setNegation(true);
    }
    return res;
  }

  boost::shared_ptr<RDKix::QueryAtom> funcname ## LessQueryAtom(type val, bool negate=false) {
    boost::shared_ptr<RDKix::QueryAtom> res(new RDKix::QueryAtom());
    res->setQuery(
        RDKix:: ## func <RDKix::ATOM_GREATER_QUERY>(val, std::string( # funcname "Less")));
    if (negate) {
      res->getQuery()->setNegation(true);
    }
    return res;
  }

  boost::shared_ptr<RDKix::QueryAtom> funcname ## GreaterQueryAtom(type val, bool negate=false) {
    boost::shared_ptr<RDKix::QueryAtom> res(new RDKix::QueryAtom());
    res->setQuery(
        RDKix:: ## func <RDKix::ATOM_LESS_QUERY>(val, std::string(# funcname "Greater")));
    if (negate) {
      res->getQuery()->setNegation(true);
    }
    return res;
  }
%}
%enddef

%define QAFUNC2(funcname, func, type)
%inline %{
  boost::shared_ptr<RDKix::QueryAtom> funcname(bool negate=false) {
    boost::shared_ptr<RDKix::QueryAtom> res(new RDKix::QueryAtom());
    res->setQuery(RDKix:: ## func());
    if (negate) {
      res->getQuery()->setNegation(true);
    }
    return res;
  }
%}
%enddef

QAFUNC1(AtomNum, makeAtomNumQuery, int);
QAFUNC1(ExplicitValence, makeAtomExplicitValenceQuery, int);
QAFUNC1(TotalValence, makeAtomTotalValenceQuery, int);
QAFUNC1(ExplicitDegree, makeAtomExplicitDegreeQuery, int);
QAFUNC1(TotalDegree, makeAtomTotalDegreeQuery, int);
QAFUNC1(NonHydrogenDegree, makeAtomNonHydrogenDegreeQuery, int);
QAFUNC1(HCount, makeAtomHCountQuery, int);
QAFUNC1(Mass, makeAtomMassQuery, int);
QAFUNC1(Isotope, makeAtomIsotopeQuery, int);
QAFUNC1(FormalCharge, makeAtomFormalChargeQuery, int);
QAFUNC1(Hybridization, makeAtomHybridizationQuery, int);
QAFUNC1(InNRings, makeAtomInNRingsQuery, int);
QAFUNC1(MinRingSize, makeAtomMinRingSizeQuery, int);
QAFUNC1(RingBondCount, makeAtomRingBondCountQuery, int);
QAFUNC1(NumRadicalElectrons, makeAtomNumRadicalElectronsQuery, int);
QAFUNC1(NumHeteroatomNeighbors, makeAtomNumHeteroatomNbrsQuery, int);
QAFUNC1(NumAliphaticHeteroatomNeighbors,
        makeAtomNumAliphaticHeteroatomNbrsQuery, int);

QAFUNC2(IsUnsaturatedQueryAtom, makeAtomUnsaturatedQuery, int);
QAFUNC2(IsAromaticQueryAtom, makeAtomAromaticQuery, int);
QAFUNC2(IsAliphaticQueryAtom, makeAtomAliphaticQuery, int);
QAFUNC2(IsInRingQueryAtom, makeAtomInRingQuery, int);
QAFUNC2(HasChiralTagQueryAtom, makeAtomHasChiralTagQuery, int);
QAFUNC2(MissingChiralTagQueryAtom, makeAtomMissingChiralTagQuery, int);
QAFUNC2(IsBridgeheadQueryAtom, makeAtomIsBridgeheadQuery, int);
QAFUNC2(AAtomQueryAtom, makeAAtomQuery, int);
QAFUNC2(AHAtomQueryAtom, makeAHAtomQuery, int);
QAFUNC2(QAtomQueryAtom, makeQAtomQuery, int);
QAFUNC2(QHAtomQueryAtom, makeQHAtomQuery, int);
QAFUNC2(XAtomQueryAtom, makeXAtomQuery, int);
QAFUNC2(XHAtomQueryAtom, makeXHAtomQuery, int);
QAFUNC2(MAtomQueryAtom, makeMAtomQuery, int);
QAFUNC2(MHAtomQueryAtom, makeMHAtomQuery, int);

%{

  template <class Ob, class Ret, class T>
  boost::shared_ptr<Ret> PropQuery(const std::string &propname, const T &v, bool negate) {
	boost::shared_ptr<Ret> res(new Ret());
	res->setQuery(RDKix::makePropQuery<Ob, T>(propname, v));
	if (negate) {
	  res->getQuery()->setNegation(true);
	}
	return res;
  }

  template <class Ob, class Ret, class T>
  boost::shared_ptr<Ret> PropQueryWithTol(const std::string &propname, const T &v, bool negate,
						const T &tol = T()) {
	boost::shared_ptr<Ret> res(new Ret());
	res->setQuery(RDKix::makePropQuery<Ob, T>(propname, v, tol));
	if (negate) {
	  res->getQuery()->setNegation(true);
	}
	return res;
  }

  template <class Ob, class Ret>
  boost::shared_ptr<Ret> PropQueryWithTol(const std::string &propname, const ExplicitBitVect &v,
						bool negate=false, float tol = 0.0) {
	boost::shared_ptr<Ret> res(new Ret());
	res->setQuery(RDKix::makePropQuery<Ob>(propname, v, tol));
	if (negate) {
	  res->getQuery()->setNegation(true);
	}
	return res;
  }

  boost::shared_ptr<RDKix::QueryAtom> HasPropQueryAtom(const std::string &propname, bool negate=false) {
	boost::shared_ptr<RDKix::QueryAtom> res(new RDKix::QueryAtom());
	res->setQuery(RDKix::makeHasPropQuery<RDKix::Atom>(propname));
	if (negate) {
	  res->getQuery()->setNegation(true);
	}
	return res;
  }

  boost::shared_ptr<RDKix::QueryAtom> HasIntPropWithValueQueryAtom(const std::string &propname, int val, bool negate=false) {
 	return PropQuery<RDKix::Atom, RDKix::QueryAtom, int>(propname, val, negate);
  }

  boost::shared_ptr<RDKix::QueryAtom> HasBoolPropWithValueQueryAtom(const std::string &propname, bool val, bool negate=false) {
	return PropQuery<RDKix::Atom, RDKix::QueryAtom, bool>(propname, val, negate);
  }

  boost::shared_ptr<RDKix::QueryAtom> HasStringPropWithValueQueryAtom(const std::string &propname, const std::string &val, bool negate=false) {
	return PropQuery<RDKix::Atom, RDKix::QueryAtom, std::string>(propname, val, negate);
  }

  boost::shared_ptr<RDKix::QueryAtom> HasDoublePropWithValueQueryAtom(const std::string &propname, double val, bool negate=false, double tol=0) {
	return PropQueryWithTol<RDKix::Atom, RDKix::QueryAtom, double>(propname, val, negate, tol);
  }

  boost::shared_ptr<RDKix::QueryAtom> HasBitVectPropWithValueQueryAtom(const std::string  &propname, const ExplicitBitVect &val, bool negate=false, float tol=0) {
	return PropQueryWithTol<RDKix::Atom, RDKix::QueryAtom>(propname, val, negate, tol);
  }

  boost::shared_ptr<RDKix::QueryBond> HasPropQueryBond(const std::string &propname, bool negate=false) {
	boost::shared_ptr<RDKix::QueryBond> res(new RDKix::QueryBond());
	res->setQuery(RDKix::makeHasPropQuery<RDKix::Bond>(propname));
	if (negate) {
	  res->getQuery()->setNegation(true);
	}
	return res;
  }

  boost::shared_ptr<RDKix::QueryBond> HasIntPropWithValueQueryBond(const std::string &propname, int val, bool negate=false) {
    return PropQuery<RDKix::Bond, RDKix::QueryBond, int>(propname, val, negate);
  }

  boost::shared_ptr<RDKix::QueryBond> HasBoolPropWithValueQueryBond(const std::string &propname, bool val, bool negate=false) {
    return PropQuery<RDKix::Bond, RDKix::QueryBond, bool>(propname, val, negate);
  }

  boost::shared_ptr<RDKix::QueryBond> HasStringPropWithValueQueryBond(const std::string &propname, const std::string &val, bool negate=false) {
    return PropQuery<RDKix::Bond, RDKix::QueryBond, std::string>(propname, val, negate);
  }

  boost::shared_ptr<RDKix::QueryBond> HasDoublePropWithValueQueryBond(const std::string &propname, double val, bool negate=false, double tol=0) {
    return PropQueryWithTol<RDKix::Bond, RDKix::QueryBond, double>(propname, val, negate, tol);
  }

%}

boost::shared_ptr<RDKix::QueryAtom> HasPropQueryAtom(const std::string &propname, bool negate=false);
boost::shared_ptr<RDKix::QueryBond> HasPropQueryBond(const std::string &propname, bool negate=false);
boost::shared_ptr<RDKix::QueryAtom> HasIntPropWithValueQueryAtom(const std::string &propname, int val, bool negate=false);
boost::shared_ptr<RDKix::QueryAtom> HasBoolPropWithValueQueryAtom(const std::string &propname, bool val, bool negate=false);
boost::shared_ptr<RDKix::QueryAtom> HasStringPropWithValueQueryAtom(const std::string &propname, const std::string &val, bool negate=false);
boost::shared_ptr<RDKix::QueryAtom> HasDoublePropWithValueQueryAtom(const std::string &propname, double val, bool negate=false, double tol=0);
boost::shared_ptr<RDKix::QueryAtom> HasBitVectPropWithValueQueryAtom(const std::string  &propname, const ExplicitBitVect &val, bool negate=false, float tol=0);
boost::shared_ptr<RDKix::QueryBond> HasPropQueryBond(const std::string &propname, bool negate=false);
boost::shared_ptr<RDKix::QueryBond> HasIntPropWithValueQueryBond(const std::string &propname, int val, bool negate=false);
boost::shared_ptr<RDKix::QueryBond> HasBoolPropWithValueQueryBond(const std::string &propname, bool val, bool negate=false);
boost::shared_ptr<RDKix::QueryBond> HasStringPropWithValueQueryBond(const std::string &propname, const std::string &val, bool negate=false);
boost::shared_ptr<RDKix::QueryBond> HasDoublePropWithValueQueryBond(const std::string &propname, double val, bool negate=false, double tol=0);

%extend RDKix::QueryAtom {
  void ExpandQuery(const RDKix::QueryAtom *other, Queries::CompositeQueryType how=Queries::COMPOSITE_AND, bool maintainOrder=true) {
	PRECONDITION(other, "bad atoms");
	if (other->hasQuery()) {
	  const RDKix::QueryAtom::QUERYATOM_QUERY *qry = other->getQuery();
	  ($self)->expandQuery(qry->copy(), how, maintainOrder);
	}
  }

  void setQuery(const RDKix::QueryAtom *other) {
	PRECONDITION(other, "bad atoms");
	if (other->hasQuery()) {
	  ($self)->setQuery(other->getQuery()->copy());
	}
  }
}

%extend RDKix::Atom {
  void ExpandQuery(const RDKix::QueryAtom *other, Queries::CompositeQueryType how=Queries::COMPOSITE_AND, bool maintainOrder=true) {
  PRECONDITION(other, "bad atoms");
  if (other->hasQuery()) {
	  const RDKix::QueryAtom::QUERYATOM_QUERY *qry = other->getQuery();
	  ($self)->expandQuery(qry->copy(), how, maintainOrder);
	}
  }

  void setQuery(const RDKix::QueryAtom *other) {
	PRECONDITION(other, "bad atoms");
	if (other->hasQuery()) {
	  ($self)->setQuery(other->getQuery()->copy());
	}
  }
}

%extend RDKix::QueryBond {
  void ExpandQuery(const RDKix::QueryBond *other, Queries::CompositeQueryType how=Queries::COMPOSITE_AND, bool maintainOrder=true) {
	PRECONDITION(other, "bad bonds");
	if (other->hasQuery()) {
	  const RDKix::QueryBond::QUERYBOND_QUERY *qry = other->getQuery();
	  ($self)->expandQuery(qry->copy(), how, maintainOrder);
	}
  }

  void SetQuery(const RDKix::QueryBond *other) {
	PRECONDITION(other, "bad bonds");
	if (other->hasQuery()) {
	  ($self)->setQuery(other->getQuery()->copy());
	}
  }
}

%extend RDKix::Bond {
  void ExpandQuery(const RDKix::QueryBond *other, Queries::CompositeQueryType how=Queries::COMPOSITE_AND, bool maintainOrder=true) {
	PRECONDITION(other, "bad bonds");
	if (other->hasQuery()) {
	  const RDKix::QueryBond::QUERYBOND_QUERY *qry = other->getQuery();
	  ($self)->expandQuery(qry->copy(), how, maintainOrder);
	}
  }

  void SetQuery(const RDKix::QueryBond *other) {
	PRECONDITION(other, "bad bonds");
	if (other->hasQuery()) {
	  ($self)->setQuery(other->getQuery()->copy());
	}
  }
}
