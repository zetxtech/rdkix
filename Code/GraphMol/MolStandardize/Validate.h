//
//  Copyright (C) 2018-2021 Susan H. Leung and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
/*! \file Validate.h

        \brief Defines the ValidationErrorInfo class and four different
   validation methods: RDKixValidation, MolVSValidation, AllowedAtomsValidation,
   DisallowedAtomsValidation.

*/
#include <RDGeneral/export.h>
#ifndef RD_VALIDATE_H
#define RD_VALIDATE_H

#include <GraphMol/RDKixBase.h>
#include <GraphMol/ROMol.h>
#include <GraphMol/Atom.h>
#include <iostream>
#include <exception>
#include <string>
#include <utility>
#include <vector>

namespace RDKix {
class RWMol;
class ROMol;

namespace MolStandardize {

//! The ValidationErrorInfo class is used to store the information returned by a
/// ValidationMethod validate.
class RDKIX_MOLSTANDARDIZE_EXPORT ValidationErrorInfo : public std::exception {
 public:
  ValidationErrorInfo(std::string msg) : d_msg(std::move(msg)) {
    BOOST_LOG(rdInfoLog) << d_msg << std::endl;
  }
  const char *what() const noexcept override { return d_msg.c_str(); }
  ~ValidationErrorInfo() noexcept override = default;

 private:
  std::string d_msg;
};  // class ValidationErrorInfo

//! The ValidationMethod class is the abstract base class upon which all the
/// four different ValidationMethods inherit from.
class RDKIX_MOLSTANDARDIZE_EXPORT ValidationMethod {
 public:
  ValidationMethod() = default;
  virtual ~ValidationMethod() = default;

  virtual std::vector<ValidationErrorInfo> validate(
      const ROMol &mol, bool reportAllFailures) const = 0;
};

//! The RDKixValidation class throws an error when there are no atoms in the
/// molecule or when there is incorrect atom valency.
/*!

  <b>Notes:</b>
    - RDKix automatically throws up atom valency issues but this class was made
  for completeness of the project.
*/
class RDKIX_MOLSTANDARDIZE_EXPORT RDKixValidation : public ValidationMethod {
 public:
  std::vector<ValidationErrorInfo> validate(
      const ROMol &mol, bool reportAllFailures) const override;
};

//////////////////////////////
/// MolVS Validations
//
//! The MolVSValidations class includes most of the same validations as
/// molvs.validations, namely NoAtomValidation, FragmentValidation,
/// NeutralValidation, IsotopeValidation. MolVS also has IsNoneValidation and
/// DichloroethaneValidation but these were not included here (yet).
class RDKIX_MOLSTANDARDIZE_EXPORT MolVSValidations {
 public:
  virtual void run(const ROMol &mol, bool reportAllFailures,
                   std::vector<ValidationErrorInfo> &errors) const = 0;
  virtual boost::shared_ptr<MolVSValidations> copy() const = 0;
};

//! The NoAtomValidation class throws an error if no atoms are present in the
/// molecule.
class RDKIX_MOLSTANDARDIZE_EXPORT NoAtomValidation final
    : public MolVSValidations {
 public:
  void run(const ROMol &mol, bool reportAllFailures,
           std::vector<ValidationErrorInfo> &errors) const override;
  //! makes a copy of NoAtomValidation object and returns a MolVSValidations
  //! pointer to it
  boost::shared_ptr<MolVSValidations> copy() const override {
    return boost::make_shared<NoAtomValidation>(*this);
  }
};

//! The FragmentValidation class logs if certain fragments are present.
class RDKIX_MOLSTANDARDIZE_EXPORT FragmentValidation final
    : public MolVSValidations {
 public:
  void run(const ROMol &mol, bool reportAllFailures,
           std::vector<ValidationErrorInfo> &errors) const override;
  //! makes a copy of FragmentValidation object and returns a MolVSValidations
  //! pointer to it
  boost::shared_ptr<MolVSValidations> copy() const override {
    return boost::make_shared<FragmentValidation>(*this);
  }
};

//! The NeutralValidation class logs if not an overall neutral system.
class RDKIX_MOLSTANDARDIZE_EXPORT NeutralValidation final
    : public MolVSValidations {
 public:
  void run(const ROMol &mol, bool reportAllFailures,
           std::vector<ValidationErrorInfo> &errors) const override;
  //! makes a copy of NeutralValidation object and returns a MolVSValidations
  //! pointer to it
  boost::shared_ptr<MolVSValidations> copy() const override {
    return boost::make_shared<NeutralValidation>(*this);
  }
};

//! The IsotopeValidation class logs if molecule contains isotopes.
class RDKIX_MOLSTANDARDIZE_EXPORT IsotopeValidation final
    : public MolVSValidations {
 public:
  void run(const ROMol &mol, bool reportAllFailures,
           std::vector<ValidationErrorInfo> &errors) const override;
  //! makes a copy of IsotopeValidation object and returns a MolVSValidations
  //! pointer to it
  boost::shared_ptr<MolVSValidations> copy() const override {
    return boost::make_shared<IsotopeValidation>(*this);
  }
};

////////////////////////////////

//! The MolVSValidation class can be used to perform all MolVSValidions.
class RDKIX_MOLSTANDARDIZE_EXPORT MolVSValidation : public ValidationMethod {
 public:
  // constructor
  MolVSValidation();
  //! overloaded constructor to take in a user-defined list of MolVSValidations
  MolVSValidation(
      const std::vector<boost::shared_ptr<MolVSValidations>> validations);
  MolVSValidation(const MolVSValidation &other);
  ~MolVSValidation() override;

  std::vector<ValidationErrorInfo> validate(
      const ROMol &mol, bool reportAllFailures) const override;

 private:
  std::vector<boost::shared_ptr<MolVSValidations>> d_validations;
};

//! The AllowedAtomsValidation class lets the user input a list of atoms,
//! anything not on
/// the list throws an error.
class RDKIX_MOLSTANDARDIZE_EXPORT AllowedAtomsValidation
    : public ValidationMethod {
 public:
  AllowedAtomsValidation(std::vector<std::shared_ptr<Atom>> atoms)
      : d_allowedList(std::move(atoms)) {}
  std::vector<ValidationErrorInfo> validate(
      const ROMol &mol, bool reportAllFailures) const override;

 private:
  std::vector<std::shared_ptr<Atom>> d_allowedList;
};

//! The DisallowedAtomsValidation class lets the user input a list of atoms and
//! as long
/// as there are no atoms from the list it is deemed acceptable.
class RDKIX_MOLSTANDARDIZE_EXPORT DisallowedAtomsValidation
    : public ValidationMethod {
 public:
  DisallowedAtomsValidation(std::vector<std::shared_ptr<Atom>> atoms)
      : d_disallowedList(std::move(atoms)) {}
  std::vector<ValidationErrorInfo> validate(
      const ROMol &mol, bool reportAllFailures) const override;

 private:
  std::vector<std::shared_ptr<Atom>> d_disallowedList;
};

//! A convenience function for quickly validating a single SMILES string.
RDKIX_MOLSTANDARDIZE_EXPORT std::vector<ValidationErrorInfo> validateSmiles(
    const std::string &smiles);

}  // namespace MolStandardize
}  // namespace RDKix

#endif
