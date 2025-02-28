//
//  Copyright (C) 2002-2017 Greg Landrum, Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <RDGeneral/export.h>
#ifndef _RD_MOLWRITERS_H_
#define _RD_MOLWRITERS_H_

#include <RDGeneral/types.h>

#include <iostream>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

#ifdef RDK_BUILD_MAEPARSER_SUPPORT
namespace schrodinger {
namespace mae {
class Writer;
}  // namespace mae
}  // namespace schrodinger
#endif  // RDK_BUILD_MAEPARSER_SUPPORT

#include <GraphMol/ROMol.h>

namespace RDKix {

static int defaultConfId = -1;
static const std::string defaultMaeHeavyAtomColor = "A0A0A0";

class RDKIX_FILEPARSERS_EXPORT MolWriter : private boost::noncopyable {
 public:
  virtual ~MolWriter() {}
  virtual void write(const ROMol &mol, int confId = defaultConfId) = 0;
  virtual void flush() = 0;
  virtual void close() = 0;
  virtual void setProps(const STR_VECT &propNames) = 0;
  virtual unsigned int numMols() const = 0;
};

//! The SmilesWriter is for writing molecules and properties to
//! delimited text files.
class RDKIX_FILEPARSERS_EXPORT SmilesWriter : public MolWriter {
  /******************************************************************************
   * A Smiles Table writer - this is how it is used
   *  - create a SmilesWriter with a output file name (or a ostream), a
   *delimiter,
   *     and a list of properties that need to be written out
   *  - then a call is made to the write function for each molecule that needs
   *to
   *     be written out
   ******************************************************************************/
 public:
  /*!
    \param fileName       : filename to write to ("-" to write to stdout)
    \param delimiter      : delimiter to use in the text file
    \param nameHeader     : used to label the name column in the output. If this
                            is provided as the empty string, no names will be
    written.
    \param includeHeader  : toggles inclusion of a header line in the output
    \param isomericSmiles : toggles generation of isomeric SMILES
    \param kekuleSmiles   : toggles the generation of kekule SMILES

   */
  SmilesWriter(const std::string &fileName, const std::string &delimiter = " ",
               const std::string &nameHeader = "Name",
               bool includeHeader = true, bool isomericSmiles = true,
               bool kekuleSmiles = false);
  //! \overload
  SmilesWriter(std::ostream *outStream, std::string delimiter = " ",
               std::string nameHeader = "Name", bool includeHeader = true,
               bool takeOwnership = false, bool isomericSmiles = true,
               bool kekuleSmiles = false);

  ~SmilesWriter() override;

  //! \brief set a vector of property names that are need to be
  //! written out for each molecule
  void setProps(const STR_VECT &propNames) override;

  //! \brief write a new molecule to the file
  void write(const ROMol &mol, int confId = defaultConfId) override;

  //! \brief flush the ostream
  void flush() override {
    PRECONDITION(dp_ostream, "no output stream");
    try {
      dp_ostream->flush();
    } catch (...) {
      try {
        if (dp_ostream->good()) {
          dp_ostream->setstate(std::ios::badbit);
        }
      } catch (const std::runtime_error &) {
      }
    }
  }

  //! \brief close our stream (the writer cannot be used again)
  void close() override {
    if (dp_ostream) {
      flush();
    }
    if (df_owner) {
      delete dp_ostream;
      df_owner = false;
    }
    dp_ostream = nullptr;
  }

  //! \brief get the number of molecules written so far
  unsigned int numMols() const override { return d_molid; }

 private:
  // local initialization
  void init(const std::string &delimiter, const std::string &nameHeader,
            bool includeHeader, bool isomericSmiles, bool kekuleSmiles);

  // dumps a header line to the output stream
  void dumpHeader() const;

  std::ostream *dp_ostream;
  bool df_owner;
  bool df_includeHeader;     // whether or not to include a title line
  unsigned int d_molid;      // the number of the molecules we wrote so far
  std::string d_delim;       // delimiter string between various records
  std::string d_nameHeader;  // header for the name column in the output file
  STR_VECT d_props;        // list of property name that need to be written out
  bool df_isomericSmiles;  // whether or not to do isomeric smiles
  bool df_kekuleSmiles;    // whether or not to do kekule smiles
};

//! The SDWriter is for writing molecules and properties to
//! SD files
class RDKIX_FILEPARSERS_EXPORT SDWriter : public MolWriter {
  /**************************************************************************************
   * A SD file ( or stream) writer - this is how it is used
   *  - create a SDWriter with a output file name (or a ostream),
   *     and a list of properties that need to be written out
   *  - then a call is made to the write function for each molecule that needs
   *to be written out
   **********************************************************************************************/
 public:
  /*!
    \param fileName       : filename to write to ("-" to write to stdout)
   */
  SDWriter(const std::string &fileName);
  SDWriter(std::ostream *outStream, bool takeOwnership = false);

  ~SDWriter() override;

  //! \brief set a vector of property names that are need to be
  //! written out for each molecule
  void setProps(const STR_VECT &propNames) override;

  //! \brief return the text that would be written to the file
  static std::string getText(const ROMol &mol, int confId = defaultConfId,
                             bool kekulize = true, bool force_V3000 = false,
                             int molid = -1, STR_VECT *propNames = nullptr);

  //! \brief write a new molecule to the file
  void write(const ROMol &mol, int confId = defaultConfId) override;

  //! \brief flush the ostream
  void flush() override {
    PRECONDITION(dp_ostream, "no output stream");
    try {
      dp_ostream->flush();
    } catch (...) {
      try {
        if (dp_ostream->good()) {
          dp_ostream->setstate(std::ios::badbit);
        }
      } catch (const std::runtime_error &) {
      }
    }
  }

  //! \brief close our stream (the writer cannot be used again)
  void close() override {
    if (dp_ostream) {
      flush();
    }
    if (df_owner) {
      delete dp_ostream;
      df_owner = false;
    }
    dp_ostream = nullptr;
  }

  //! \brief get the number of molecules written so far
  unsigned int numMols() const override { return d_molid; }

  void setForceV3000(bool val) { df_forceV3000 = val; }
  bool getForceV3000() const { return df_forceV3000; }

  void setKekulize(bool val) { df_kekulize = val; }
  bool getKekulize() const { return df_kekulize; }

 private:
  void writeProperty(const ROMol &mol, const std::string &name);

  std::ostream *dp_ostream;
  bool df_owner;
  unsigned int d_molid;  // the number of the molecules we wrote so far
  STR_VECT d_props;      // list of property name that need to be written out
  bool df_forceV3000;    // force writing the mol blocks as V3000
  bool df_kekulize;      // toggle kekulization of molecules on writing
};

//! The TDTWriter is for writing molecules and properties to
//! TDT files
class RDKIX_FILEPARSERS_EXPORT TDTWriter : public MolWriter {
  /**************************************************************************************
   * A TDT file ( or stream) writer - this is how it is used
   *  - create a TDTWriter with a output file name (or a ostream),
   *     and a list of properties that need to be written out
   *  - then a call is made to the write function for each molecule that needs
   *to be written out
   **********************************************************************************************/
 public:
  /*!
    \param fileName       : filename to write to ("-" to write to stdout)
   */
  TDTWriter(const std::string &fileName);
  TDTWriter(std::ostream *outStream, bool takeOwnership = false);

  ~TDTWriter() override;

  //! \brief set a vector of property names that are need to be
  //! written out for each molecule
  void setProps(const STR_VECT &propNames) override;

  //! \brief write a new molecule to the file
  void write(const ROMol &mol, int confId = defaultConfId) override;

  //! \brief flush the ostream
  void flush() override {
    PRECONDITION(dp_ostream, "no output stream");
    try {
      dp_ostream->flush();
    } catch (...) {
      try {
        if (dp_ostream->good()) {
          dp_ostream->setstate(std::ios::badbit);
        }
      } catch (const std::runtime_error &) {
      }
    }
  }

  //! \brief close our stream (the writer cannot be used again)
  void close() override {
    if (dp_ostream) {
      // if we've written any mols, finish with a "|" line
      if (d_molid > 0) {
        *dp_ostream << "|\n";
      }
      flush();
    }
    if (df_owner) {
      delete dp_ostream;
      df_owner = false;
    }
    dp_ostream = nullptr;
  }

  //! \brief get the number of molecules written so far
  unsigned int numMols() const override { return d_molid; }

  void setWrite2D(bool state = true) { df_write2D = state; }
  bool getWrite2D() const { return df_write2D; }

  void setWriteNames(bool state = true) { df_writeNames = state; }
  bool getWriteNames() const { return df_writeNames; }

  void setNumDigits(unsigned int numDigits) { d_numDigits = numDigits; }
  unsigned int getNumDigits() const { return d_numDigits; }

 private:
  void writeProperty(const ROMol &mol, const std::string &name);

  std::ostream *dp_ostream;
  bool df_owner;
  unsigned int d_molid;  // the number of molecules we wrote so far
  STR_VECT d_props;      // list of property name that need to be written out
  bool df_write2D;       // write 2D coordinates instead of 3D
  bool df_writeNames;    // write a name record for each molecule
  unsigned int
      d_numDigits;  // number of digits to use in our output of coordinates;
};

//! The PDBWriter is for writing molecules to Brookhaven Protein
//! DataBank format files.
class RDKIX_FILEPARSERS_EXPORT PDBWriter : public MolWriter {
 public:
  PDBWriter(const std::string &fileName, unsigned int flavor = 0);
  PDBWriter(std::ostream *outStream, bool takeOwnership = false,
            unsigned int flavor = 0);
  ~PDBWriter() override;

  //! \brief write a new molecule to the file
  void write(const ROMol &mol, int confId = defaultConfId) override;

  void setProps(const STR_VECT &) override {}

  //! \brief flush the ostream
  void flush() override {
    PRECONDITION(dp_ostream, "no output stream");
    try {
      dp_ostream->flush();
    } catch (...) {
      try {
        if (dp_ostream->good()) {
          dp_ostream->setstate(std::ios::badbit);
        }
      } catch (const std::runtime_error &) {
      }
    }
  }

  //! \brief close our stream (the writer cannot be used again)
  void close() override {
    if (dp_ostream) {
      flush();
    }
    if (df_owner) {
      delete dp_ostream;
      df_owner = false;
    }
    dp_ostream = nullptr;
  }

  //! \brief get the number of molecules written so far
  unsigned int numMols() const override { return d_count; }

 private:
  std::ostream *dp_ostream;
  unsigned int d_flavor;
  unsigned int d_count;
  bool df_owner;
};

#ifdef RDK_BUILD_MAEPARSER_SUPPORT

class RDKIX_FILEPARSERS_EXPORT MaeWriter : public MolWriter {
  /**************************************************************************************
   * A highly experimental Maestro file (or stream) writer. Many features are
   * not supported yet, e.g. chirality and bond stereo, stereo groups, substance
   * groups, isotopes or dummy atoms. Note that except for stereochemistry
   * labels these aren't supported by the MaeMolSupplier either.
   *
   * Usage:
   *  - create a MaeWriter with an output file name (or a ostream),
   *     and a list of mol/atom/bond properties that need to be written out.
   *     If no property names are specified, all properties will be exported.
   *     Properties that are specified, but are not present will be ignored.
   *  - then, a call is made to the write function for each molecule
   *     that needs to be written out.
   *
   * Notes:
   *  - kekulization is mandatory, as the Maestro format does not
   *     have the concept of an aromatic bond.
   *  - Ownership of the output stream is mandatory, since it needs
   *     to be managed though a shared_ptr, as this is what maeparser
   *     writer takes.
   ***************************************************************************************/
 public:
  /*!
    \param fileName       : filename to write to (stdout is *not* supported)
   */
  MaeWriter(const std::string &fileName);

  /*!
  \note Note that this takes ownership of the output stream.
  */
  MaeWriter(std::ostream *outStream);

  MaeWriter(std::shared_ptr<std::ostream> outStream);

  ~MaeWriter() override;

  //! \brief set a vector of property names that are need to be
  //! written out for each molecule
  void setProps(const STR_VECT &propNames) override;

  //! \brief return the text that would be written to the file
  static std::string getText(
      const ROMol &mol,
      const std::string &heavyAtomColor = defaultMaeHeavyAtomColor,
      int confId = defaultConfId, const STR_VECT &propNames = STR_VECT());

  //! \brief write a new molecule to the file
  void write(const ROMol &mol, int confId = defaultConfId) override;

  //! \brief write a new molecule to the file, specifying the HTML color string
  //! which should be used for heavy atoms when the file is opened in Maestro.
  void write(const ROMol &mol, const std::string &heavyAtomColor,
             int confId = defaultConfId);

  //! \brief flush the ostream
  void flush() override;
  //! \brief close our stream (the writer cannot be used again)
  void close() override;

  //! \brief get the number of molecules written so far
  unsigned int numMols() const override { return d_molid; }

 protected:
  MaeWriter() = default;  // used in the Python wrapper

  std::shared_ptr<std::ostream> dp_ostream;

 private:
  void open();

  std::unique_ptr<schrodinger::mae::Writer> dp_writer;
  unsigned d_molid = 0;  // the number of the molecules we wrote so far
  STR_VECT d_props;      // list of property name that need to be written out
};

#endif  // RDK_BUILD_MAEPARSER_SUPPORT

}  // namespace RDKix

#endif
