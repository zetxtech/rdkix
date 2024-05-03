//
#include <RDGeneral/export.h>
#ifdef RDK_USE_BOOST_IOSTREAMS

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace RDKix {
// gzstream from a file
class RDKIX_RDSTREAMS_EXPORT gzstream
    : public boost::iostreams::filtering_istream {
  std::ifstream is;

 public:
  gzstream(const std::string &fname);
};
}  // namespace RDKix
#endif