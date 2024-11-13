#! /bin/bash -x
python parse_doxy_html.py atom.doxytext RDKix::Atom > Atom_doc.i
python parse_doxy_html.py bond.doxytext RDKix::Bond > Bond_doc.i
python parse_doxy_html.py bonditerators.doxytext RDKix::BondIterator_ >  BondIterators_doc.i
python parse_doxy_html.py chemreactions.doxytext RDKix::ChemicalReaction > ChemReactions_doc.i
python parse_doxy_html.py conformer.doxytext RDKix::Conformer > Conformer_doc.i
python parse_doxy_html.py explicitbitvect.doxytext RDKix::ExplicitBitVect > ExplicitBitVect_doc.i
python parse_doxy_html.py molops.doxytext RDKix::MolOps > RDKFuncs_doc.i
python parse_doxy_html.py periodictable.doxytext RDKix::PeriodicTable > PeriodicTable_doc.i
python parse_doxy_html.py queryatom.doxytext RDKix::QueryAtom > QueryAtom_doc.i
python parse_doxy_html.py point.doxytext RDGeom::Point3D > Point3D_doc.i
python parse_doxy_html.py querybond.doxytext RDKix::QueryBond > QueryBond_doc.i
python parse_doxy_html.py ringinfo.doxytext RDKix::RingInfo > RingInfo_doc.i
python parse_doxy_html.py romol.doxytext RDKix::ROMol > ROMol_doc.i
python parse_doxy_html.py rwmol.doxytext RDKix::RWMol > RWMol_doc.i
python parse_doxy_html.py sdmolsupplier.doxytext RDKix::SDMolSupplier > SDMolSupplier_doc.i
python parse_doxy_html.py smilesmolsupplier.doxytext RDKix::SmilesMolSupplier > SmilesMolSupplier_doc.i
python parse_doxy_html.py smileswriter.doxytext RDKix::SmilesWriter > SmilesWriter_doc.i
python parse_doxy_html.py tdtmolsupplier.doxytext RDKix::TDTMolSupplier > TDTMolSupplier_doc.i
python parse_doxy_html.py tdtwriter.doxytext RDKix::TDTWriter > TDTWriter_doc.i
python parse_doxy_html.py transform2d.doxytext RDGeom::Transform2D > Transform2D_doc.i
python parse_doxy_html.py transform3d.doxytext RDGeom::Transform3D > Transform3D_doc.i

