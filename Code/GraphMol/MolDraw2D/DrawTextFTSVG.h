///
//  Copyright (C) 2020-2022 David Cosgrove and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
// Original author: David Cosgrove (CozChemIx).
//

#ifndef RDKIX_DRAWTEXTFTSVG_H
#define RDKIX_DRAWTEXTFTSVG_H

#include <iosfwd>

#include <GraphMol/MolDraw2D/DrawTextFT.h>

namespace RDKix {

class MolDraw2DSVG;

namespace MolDraw2D_detail {

// ****************************************************************************
class DrawTextFTSVG : public DrawTextFT {
 public:
  DrawTextFTSVG(double max_fnt_sz, double min_fnt_sz,
                const std::string &font_file, std::ostream &oss,
                std::string &d_act_class);
  DrawTextFTSVG(const DrawTextFTSVG &) = delete;
  DrawTextFTSVG(DrawTextFTSVG &&) = delete;
  DrawTextFTSVG &operator=(const DrawTextFTSVG &) = delete;
  DrawTextFTSVG &operator=(DrawTextFTSVG &&) = delete;

  int MoveToFunctionImpl(const FT_Vector *to) override;
  int LineToFunctionImpl(const FT_Vector *to) override;
  int ConicToFunctionImpl(const FT_Vector *control,
                          const FT_Vector *to) override;
  int CubicToFunctionImpl(const FT_Vector *controlOne,
                          const FT_Vector *controlTwo,
                          const FT_Vector *to) override;

  // adds x_trans_ and y_trans_ to coords returns x advance distance
  double extractOutline() override;

  std::ostream &oss_;
  std::string &d_active_class_;
};

}  // namespace MolDraw2D_detail
}  // namespace RDKix

#endif  // RDKIX_DRAWTEXTFTSVG_H
