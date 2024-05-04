//
//  Copyright (C) 2020-2022 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
// Original author: Greg Landrum
//

#ifndef RDKIX_DRAWTEXTFTJS_H
#define RDKIX_DRAWTEXTFTJS_H

#include <iosfwd>
#include <emscripten.h>
#include <emscripten/val.h>
#include <GraphMol/MolDraw2D/DrawTextFT.h>

namespace RDKix {
class MolDraw2DJS;
namespace MolDraw2D_detail {
// ****************************************************************************
class DrawTextFTJS : public DrawTextFT {
 public:
  ~DrawTextFTJS() override = default;

  DrawTextFTJS(double max_fnt_sz, double min_fnt_sz,
               const std::string &font_file, emscripten::val &context);
  DrawTextFTJS(const DrawTextFTJS &) = delete;
  DrawTextFTJS(DrawTextFTJS &&) = delete;
  DrawTextFTJS &operator=(const DrawTextFTJS &) = delete;
  DrawTextFTJS &operator=(DrawTextFTJS &&) = delete;

  int MoveToFunctionImpl(const FT_Vector *to) override;
  int LineToFunctionImpl(const FT_Vector *to) override;
  int ConicToFunctionImpl(const FT_Vector *control,
                          const FT_Vector *to) override;
  int CubicToFunctionImpl(const FT_Vector *controlOne,
                          const FT_Vector *controlTwo,
                          const FT_Vector *to) override;

  // adds x_trans_ and y_trans_ to coords returns x advance distance
  double extractOutline() override;

  emscripten::val &context_;
};

}  // namespace MolDraw2D_detail
}  // namespace RDKix

#endif  // RDKIX_DRAWTEXTFTSVG_H
