//
// Copyright (c) 2003-2008 greg Landrum and Rational Discovery LLC
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef __RD_SPARSEBITVECTS_H__
#define __RD_SPARSEBITVECTS_H__

#include "BitVect.h"

#include <set>
using std::set;
#include <iterator>
#include <algorithm>

typedef set<int> IntSet;
typedef IntSet::iterator IntSetIter;
typedef IntSet::const_iterator IntSetConstIter;

//! a class for bit vectors that are sparsely occupied.
/*!
    SparseBitVect objects store only their on bits, in an
    std::set.

    They are, as you might expect, quite memory efficient for sparsely populated
    vectors but become rather a nightmare if they need to be negated.

 */
class RDKIX_DATASTRUCTS_EXPORT SparseBitVect : public BitVect {
 public:
  SparseBitVect() {}
  //! initialize with a particular size;
  explicit SparseBitVect(unsigned int size) : dp_bits(nullptr), d_size(0) {
    _initForSize(size);
  }

  //! copy constructor
  SparseBitVect(const SparseBitVect &other) : BitVect(other) {
    d_size = 0;
    dp_bits = nullptr;
    _initForSize(other.getNumBits());
    IntSet *bv = other.dp_bits;
    std::copy(bv->begin(), bv->end(), std::inserter(*dp_bits, dp_bits->end()));
  }
  //! construct from a string pickle
  SparseBitVect(const std::string &);
  //! construct from a text pickle
  SparseBitVect(const char *data, const unsigned int dataLen);

  SparseBitVect &operator=(const SparseBitVect &);
  ~SparseBitVect() override { delete dp_bits; }

  bool operator[](const unsigned int which) const override;
  SparseBitVect operator|(const SparseBitVect &) const;
  SparseBitVect operator&(const SparseBitVect &) const;
  SparseBitVect operator^(const SparseBitVect &) const;
  SparseBitVect operator~() const;

  //! returns a (const) pointer to our raw storage
  const IntSet *getBitSet() const { return dp_bits; }

  unsigned int getNumBits() const override { return d_size; }
  bool setBit(const unsigned int which) override;
  bool setBit(const IntSetIter which);
  bool unsetBit(const unsigned int which) override;
  bool getBit(const unsigned int which) const override;
  bool getBit(const IntVectIter which) const;
  bool getBit(const IntSetIter which) const;

  unsigned int getNumOnBits() const override {
    return static_cast<unsigned int>(dp_bits->size());
  }
  unsigned int getNumOffBits() const override {
    return d_size - static_cast<unsigned int>(dp_bits->size());
  }

  std::string toString() const override;

  void getOnBits(IntVect &v) const override;
  void clearBits() override { dp_bits->clear(); }
  IntSet *dp_bits{
      nullptr};  //!< our raw data, exposed for the sake of efficiency

  bool operator==(const SparseBitVect &o) const {
    return *dp_bits == *o.dp_bits;
  }
  bool operator!=(const SparseBitVect &o) const {
    return *dp_bits != *o.dp_bits;
  }

 private:
  unsigned int d_size{0};
  void _initForSize(const unsigned int size) override;
};

#endif
