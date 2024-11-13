//
//  Copyright (C) 2003-2008 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef _RD_DEPICTOR_H_
#define _RD_DEPICTOR_H_
#include <string>
#ifdef WIN32_DLLBUILD
#include <windows.h>
#endif

#include <GraphMol/RDKixBase.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>

namespace RDKix {
#ifdef WIN32_DLLBUILD
typedef void(CALLBACK *Depictor_TwoArgFunc)(const char *arg1, const char *arg2);
typedef void(CALLBACK *Depictor_OneArgFunc)(const char *arg1);
int SmilesToMolFileDLL(std::string smi, std::string fName,
                       std::string dllName = "depict32-0.dll");
int Add2DCoordsToMolDLL(ROMol &mol, std::string tempFilename = "temp-conv.mol");
int Add2DCoordsToMol(ROMol &mol, bool useDLL = true);
#else
int Add2DCoordsToMol(ROMol &mol, bool useDLL = false);
#endif
}  // namespace RDKix

#endif
