//
// file RDKixSVSettings.H
// David Cosgrove
// AstraZeneca
// 4th July 2014
//
// Command-line parsing for RDKixSV

#ifndef RDKIXSVSETTINGS_H
#define RDKIXSVSETTINGS_H

#include <string>
#include <vector>

#include <boost/program_options/options_description.hpp>

// ****************************************************************************

class RDKixSVSettings {

public :

  RDKixSVSettings( int argc , char **argv );

  std::string usage_text() const { return usage_text_; }
  std::vector<std::string> mol_files() const { return mol_files_; }
  std::string smarts_file() const { return smarts_file_; }

private :

  std::string usage_text_;
  std::vector<std::string> mol_files_;
  std::string smarts_file_;

  void build_program_options( boost::program_options::options_description &desc );

};

#endif // RDKIXSVSETTINGS_H
