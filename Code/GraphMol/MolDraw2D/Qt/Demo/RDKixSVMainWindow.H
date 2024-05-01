//
// file RDKixSV.H
// David Cosgrove
// AstraZeneca
//
// 20th June 2014
//
// This is the main class for the program RDKixSV, defining the MainWindow.

#ifndef RDKIXSV_H
#define RDKIXSV_H

#include <QMainWindow>

#include <vector>

#include <GraphMol/ROMol.h>

namespace RDKix {
class MolSupplier;
}

// ****************************************************************************

namespace RDKixSV {

class RDKixSVPanel;

// ****************************************************************************

class RDKixSVMainWindow : public QMainWindow {

  Q_OBJECT

public :

  RDKixSVMainWindow( int argc , char **argv );
  ~RDKixSVMainWindow() = default;

  typedef enum { SMILES , SDF , UNKNOWN } FILE_TYPE;

private :

  QAction *file_exit_ , *file_read_mols_ , *file_read_smarts_;
  QAction *file_write_left_ , *file_write_right_ , *file_write_smarts_;
  QAction *smarts_match_ , *smarts_edit_ , *smarts_new_;

  std::vector<RDKix::ROMOL_SPTR> mols_;
  RDKixSV::RDKixSVPanel *left_panel_;
  RDKixSV::RDKixSVPanel *right_panel_;

  // smarts_ pair contains label and SMARTS string
  std::vector<std::pair<std::string,std::string> > smarts_;

  QString last_dir_;

  void build_actions();
  void build_file_actions();
  void build_smarts_actions();
  void build_menubar();
  void build_widget();
  void parse_args( int argc , char **argv );

  FILE_TYPE get_filetype( const std::string &filename , bool &is_compressed );

  void read_mols( const std::string &filename );
  void read_smarts( const std::string &filename );
  void write_mols( RDKixSV::RDKixSVPanel &panel ,
                   const std::string &filename );

  void match_smarts( const std::vector<std::pair<std::string,std::string> > &smts );
  std::vector<std::pair<std::string,std::string> > select_smarts( bool multi_select = true );
  void update_smarts( const std::string &new_name , const std::string &new_val );

private slots :

  void slot_exit();
  void slot_read_mols();
  void slot_read_smarts();
  void slot_match_smarts();
  void slot_edit_smarts();
  void slot_new_smarts();
  void slot_write_left_molecules();
  void slot_write_right_molecules();
  void slot_write_smarts();

};

} // EO namespace RDKixSVMainWindow

#endif // RDKIXSV_H
