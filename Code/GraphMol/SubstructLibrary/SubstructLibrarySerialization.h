//  Copyright (c) 2019, Novartis Institutes for BioMedical Research Inc.
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
// n.b. must be included at the END of SubstructLibrary.h
#ifndef RDK_SUBSTRUCT_LIBRARY_SERIALIZATION
#define RDK_SUBSTRUCT_LIBRARY_SERIALIZATION

#ifdef RDK_USE_BOOST_SERIALIZATION
#include <RDGeneral/BoostStartInclude.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/archive_exception.hpp>
#include <RDGeneral/BoostEndInclude.h>

BOOST_SERIALIZATION_ASSUME_ABSTRACT(RDKix::MolHolderBase)
BOOST_SERIALIZATION_ASSUME_ABSTRACT(RDKix::FPHolderBase)

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive &, RDKix::MolHolderBase &, const unsigned int) {}

template <class Archive>
void save(Archive &ar, const RDKix::MolHolder &molholder,
          const unsigned int version) {
  ar &boost::serialization::base_object<RDKix::MolHolderBase>(molholder);

  if (version < 2) {
    std::int64_t pkl_count = molholder.getMols().size();
    ar &pkl_count;

    for (auto &mol : molholder.getMols()) {
      std::string pkl;
      RDKix::MolPickler::pickleMol(*mol.get(), pkl);
      ar << pkl;
    }
  } else {
    ar &molholder.getMols();
  }
}

template <class Archive>
void load(Archive &ar, RDKix::MolHolder &molholder,
          const unsigned int version) {
  ar &boost::serialization::base_object<RDKix::MolHolderBase>(molholder);

  std::vector<boost::shared_ptr<RDKix::ROMol>> &mols = molholder.getMols();
  mols.clear();

  if (version < 2) {
    std::int64_t pkl_count = -1;
    ar &pkl_count;

    for (std::int64_t i = 0; i < pkl_count; ++i) {
      std::string pkl;
      ar >> pkl;
      mols.push_back(boost::make_shared<RDKix::ROMol>(pkl));
    }
  } else {
    ar &mols;
  }
}

template <class Archive, class MolHolder>
void serialize_strings(Archive &ar, MolHolder &molholder,
                       const unsigned int version) {
  RDUNUSED_PARAM(version);
  ar &boost::serialization::base_object<RDKix::MolHolderBase>(molholder);
  ar &molholder.getMols();
}

template <class Archive>
void serialize(Archive &ar, RDKix::CachedMolHolder &molholder,
               const unsigned int version) {
  serialize_strings(ar, molholder, version);
}

template <class Archive>
void serialize(Archive &ar, RDKix::CachedSmilesMolHolder &molholder,
               const unsigned int version) {
  serialize_strings(ar, molholder, version);
}

template <class Archive>
void serialize(Archive &ar, RDKix::CachedTrustedSmilesMolHolder &molholder,
               const unsigned int version) {
  serialize_strings(ar, molholder, version);
}

template <class Archive>
void save(Archive &ar, const RDKix::FPHolderBase &fpholder,
          const unsigned int version) {
  RDUNUSED_PARAM(version);
  std::vector<std::string> pickles;
  for (auto &fp : fpholder.getFingerprints()) {
    pickles.push_back(fp->toString());
  }
  ar &pickles;
}

template <class Archive>
void load(Archive &ar, RDKix::FPHolderBase &fpholder,
          const unsigned int version) {
  RDUNUSED_PARAM(version);
  std::vector<std::string> pickles;
  std::vector<ExplicitBitVect *> &fps = fpholder.getFingerprints();

  ar &pickles;
  for (size_t i = 0; i < fps.size(); ++i) {
    delete fps[i];
  }
  fps.clear();

  for (auto &pkl : pickles) {
    fps.push_back(new ExplicitBitVect(pkl));
  }
}

template <class Archive>
void serialize(Archive &ar, RDKix::PatternHolder &pattern_holder,
               const unsigned int version) {
  RDUNUSED_PARAM(version);
  ar &boost::serialization::base_object<RDKix::FPHolderBase>(pattern_holder);
  if (Archive::is_saving::value &&
      pattern_holder.getNumBits() != RDKix::PatternHolder::defaultNumBits()) {
    ar &pattern_holder.getNumBits();
  } else if (Archive::is_loading::value) {
    try {
      ar &pattern_holder.getNumBits();
    } catch (boost::archive::archive_exception &) {
      pattern_holder.getNumBits() = RDKix::PatternHolder::defaultNumBits();
    }
  }
}

template <class Archive>
void serialize(Archive &ar, RDKix::TautomerPatternHolder &pattern_holder,
               const unsigned int version) {
  RDUNUSED_PARAM(version);
  ar &boost::serialization::base_object<RDKix::FPHolderBase>(pattern_holder);
  ar &pattern_holder.getNumBits();
}

template <class Archive>
void serialize(Archive &, RDKix::KeyHolderBase &, const unsigned int) {}

template <class Archive>
void serialize(Archive &ar, RDKix::KeyFromPropHolder &key_holder,
               const unsigned int) {
  ar &boost::serialization::base_object<RDKix::KeyHolderBase>(key_holder);
  ar &key_holder.getPropName();
  ar &key_holder.getKeys();
}

template <class Archive>
void registerSubstructLibraryTypes(Archive &ar) {
  ar.register_type(static_cast<RDKix::MolHolder *>(nullptr));
  ar.register_type(static_cast<RDKix::CachedMolHolder *>(nullptr));
  ar.register_type(static_cast<RDKix::CachedSmilesMolHolder *>(nullptr));
  ar.register_type(static_cast<RDKix::CachedTrustedSmilesMolHolder *>(nullptr));
  ar.register_type(static_cast<RDKix::PatternHolder *>(nullptr));
  ar.register_type(static_cast<RDKix::TautomerPatternHolder *>(nullptr));
  ar.register_type(static_cast<RDKix::KeyFromPropHolder *>(nullptr));
}

template <class Archive>
void save(Archive &ar, const RDKix::SubstructLibrary &slib,
          const unsigned int version) {
  RDUNUSED_PARAM(version);
  registerSubstructLibraryTypes(ar);
  ar &slib.getSearchOrder();
  ar &slib.getKeyHolder();
  ar &slib.getMolHolder();
  ar &slib.getFpHolder();
}

template <class Archive>
void load(Archive &ar, RDKix::SubstructLibrary &slib,
          const unsigned int version) {
  RDUNUSED_PARAM(version);
  registerSubstructLibraryTypes(ar);
  if (version > 1) {
    ar &slib.getSearchOrder();
    ar &slib.getKeyHolder();
  }
  ar &slib.getMolHolder();
  ar &slib.getFpHolder();
  slib.resetHolders();
}

}  // end namespace serialization
}  // end namespace boost

BOOST_CLASS_VERSION(RDKix::MolHolder, 2);
BOOST_CLASS_VERSION(RDKix::CachedMolHolder, 1);
BOOST_CLASS_VERSION(RDKix::CachedSmilesMolHolder, 1);
BOOST_CLASS_VERSION(RDKix::CachedTrustedSmilesMolHolder, 1);
BOOST_CLASS_VERSION(RDKix::PatternHolder, 1);
BOOST_CLASS_VERSION(RDKix::TautomerPatternHolder, 1);
BOOST_CLASS_VERSION(RDKix::SubstructLibrary, 2);

BOOST_SERIALIZATION_SPLIT_FREE(RDKix::MolHolder);
BOOST_SERIALIZATION_SPLIT_FREE(RDKix::FPHolderBase);
BOOST_SERIALIZATION_SPLIT_FREE(RDKix::SubstructLibrary);

#endif
#endif
