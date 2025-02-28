//
//  Copyright (C) 2020-2022 David Cosgrove and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
//
// Original author: David Cosgrove (CozChemIx).
//
// This is an abstract base class for drawing text into a MolDraw2D
// object.

#ifndef RDKIX_DRAWTEXT_H
#define RDKIX_DRAWTEXT_H

#include <string>
#include <vector>

#include <RDGeneral/export.h>
#include <Geometry/point.h>
#include <GraphMol/MolDraw2D/MolDraw2DHelpers.h>
#include <GraphMol/MolDraw2D/StringRect.h>

using RDGeom::Point2D;

namespace RDKix {
class MolDraw2D;
namespace MolDraw2D_detail {

// for aligning the drawing of text to the passed in coords.
enum class TextDrawType : unsigned char {
  TextDrawNormal = 0,
  TextDrawSuperscript,
  TextDrawSubscript
};
std::ostream &operator<<(std::ostream &oss, const OrientType &o);
std::ostream &operator<<(std::ostream &oss, const TextAlignType &tat);
std::ostream &operator<<(std::ostream &oss, const TextDrawType &tdt);

// ****************************************************************************
// This is an implementation class, not intended to be used by the great
// unwashed. If you want to draw a string used MolDraw2D::drawString().
class RDKIX_MOLDRAW2D_EXPORT DrawText {
 public:
  virtual ~DrawText() = 0;

  static constexpr double DEFAULT_FONT_SCALE =
      0.6;  // seems to be a good number

  DrawText(double max_fnt_sz, double min_fnt_sz);
  DrawText(const DrawText &) = delete;
  DrawText(DrawText &&) = delete;
  DrawText &operator=(const DrawText &) = delete;
  DrawText &operator=(DrawText &&) = delete;

  DrawColour const &colour() const;
  void setColour(const DrawColour &col);

  // size in "pixels" i.e scale() * base_font_size_.
  double fontSize() const;
  void setFontSize(double new_size);
  double baseFontSize() const;
  void setBaseFontSize(double new_size);
  double maxFontSize() const;
  void setMaxFontSize(double new_max);
  double minFontSize() const;
  void setMinFontSize(double new_max);
  double fontScale() const;
  // returns false if min or max font size is hit, true otherwise.
  // ignoreLimits ignores minFontSize and maxFontSize.
  bool setFontScale(double new_scale, bool ignoreLimits = false);

  // these are only relevant for the FreeType DrawText classes.
  virtual std::string getFontFile() const { return ""; }
  virtual void setFontFile(const std::string &font_file) {
    RDUNUSED_PARAM(font_file);
  }

  //! using the current scale, work out the size of the label
  /*!
     Bear in mind when implementing this, that, for example, NH2 will appear as
     NH<sub>2</sub> to convey that the 2 is a subscript, and this needs to
     be accounted for in the width and height.
   */
  virtual void getStringSize(const std::string &label, double &label_width,
                             double &label_height) const;
  // returns the extremes of the label, in draw (pixel) coords.  dontSplit
  // true suppresses the call to atomLabelToPieces.
  void getStringExtremes(const std::string &label, OrientType orient,
                         double &x_min, double &y_min, double &x_max,
                         double &y_max, bool dontSplit = false) const;
  // get the rectangles that go round each character of the string.  If
  // dontSplit is false, it assumes it's an atom label and splits it
  // in a sensible way for that.  If the OrientType is C, it applies
  // textAlign, otherwise the text is aligned set appropriately for the
  // OrientType.
  void getStringRects(const std::string &text, OrientType orient,
                      std::vector<std::shared_ptr<StringRect>> &rects,
                      std::vector<TextDrawType> &draw_modes,
                      std::vector<char> &draw_chars, bool dontSplit = false,
                      TextAlignType textAlign = TextAlignType::MIDDLE) const;

  //! drawString centres the string on cds.
  virtual void drawString(const std::string &str, const Point2D &cds,
                          TextAlignType align);
  // Aligns them according to OrientType.  This version assumes it's an
  // atomic symbol and calls atomLabelToPieces.  This will behave
  // badly with general text.  Surround the string with <lit></lit>
  // if that's an issue, or use the version above.
  void drawString(const std::string &label, const Point2D &cds,
                  OrientType orient);

  // put the label on end2, and then move end2 so that it is at
  // the intersection of a string rectangle and the line from end1 to
  // end2, if there is an intersection.  Mostly for trimming bonds
  // back from atom labels.  end1 and end2 in draw coords.
  void adjustLineForString(const std::string &label, OrientType orient,
                           const Point2D &end1, Point2D &end2) const;

  // draw the char, with the bottom left hand corner at cds
  virtual void drawChar(char c, const Point2D &cds) = 0;

  // puts a colourful rectangle around each character in the string.
  // For debugging, mostly.
  void drawStringRects(const std::string &label, OrientType orient,
                       TextAlignType talign, const Point2D &cds,
                       MolDraw2D &mol_draw) const;

  // cds in draw coords
  // does the label at cds intersect the given StringRect.
  bool doesRectIntersect(const std::string &label, OrientType orient,
                         const Point2D &cds, const StringRect &rect) const;
  // does the vector of StringRects, each translated by cds, intersect the
  // given StringRect.
  bool doesRectIntersect(const std::vector<std::shared_ptr<StringRect>> &rects,
                         const Point2D &cds, const StringRect &rect) const;
  bool doesLineIntersect(const std::string &label, OrientType orient,
                         const Point2D &cds, const Point2D &end1,
                         const Point2D &end2, double padding) const;
  bool doesLineIntersect(const std::vector<std::shared_ptr<StringRect>> &rects,
                         const Point2D &cds, const Point2D &end1,
                         const Point2D &end2, double padding) const;
  bool doesStringIntersect(
      const std::vector<std::shared_ptr<StringRect>> &rects,
      const Point2D &cds1, const std::string &label2, OrientType orient2,
      const Point2D &cds2) const;
  bool doesStringIntersect(const std::string &label1, OrientType orient1,
                           const Point2D &cds1, const std::string &label2,
                           OrientType orient2, const Point2D &cds2) const;

  virtual void alignString(
      TextAlignType align, const std::vector<TextDrawType> &draw_modes,
      std::vector<std::shared_ptr<StringRect>> &rects) const;
  // adjust the string rectangles up and down for super- and subscripts
  void adjustStringRectsForSuperSubScript(
      const std::vector<TextDrawType> &draw_modes,
      std::vector<std::shared_ptr<StringRect>> &rects) const;
  // return a scale factor appropriate for the character and draw type
  // (normal or super- or subscript)
  double selectScaleFactor(char c, TextDrawType draw_type) const;

  // amount to scale subscripts and superscripts by
  constexpr static double SUBS_SCALE = 0.66;
  constexpr static double SUPER_SCALE = 0.66;

  DrawColour colour_;
  double font_scale_;
  double max_font_size_;
  double min_font_size_;
  double base_font_size_ = DEFAULT_FONT_SCALE;

  // return a vector of StringRects, one for each char in text, with
  // super- and subscripts taken into account.  Sizes in pixel coords,
  // i.e. scaled by fontScale().
  virtual void getStringRects(const std::string &text,
                              std::vector<std::shared_ptr<StringRect>> &rects,
                              std::vector<TextDrawType> &draw_modes,
                              std::vector<char> &draw_chars) const = 0;
  void drawChars(const Point2D &a_cds,
                 const std::vector<std::shared_ptr<StringRect>> &rects,
                 const std::vector<TextDrawType> &draw_modes,
                 const std::vector<char> &draw_chars);
};

//! establishes whether to put string draw mode into super- or sub-script
//! mode based on contents of instring from i onwards. Increments i
//! appropriately
//! \returns true or false depending on whether it did something or not
RDKIX_MOLDRAW2D_EXPORT bool setStringDrawMode(const std::string &instring,
                                              TextDrawType &draw_mode,
                                              size_t &i);

// take the label for the given atom and return the individual pieces
// that need to be drawn for it.  So NH<sub>2</sub> will return
// "N", "H<sub>2</sub>".
std::vector<std::string> atomLabelToPieces(const std::string &label,
                                           OrientType orient);

}  // namespace MolDraw2D_detail
}  // namespace RDKix

#endif  // RDKIX_DRAWTEXT_H
