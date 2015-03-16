#include <messmer/fspp/fstest/FsTest.h>
#include <messmer/tempfile/src/TempDir.h>

#include "../src/CopyDevice.h"

using std::unique_ptr;
using std::make_unique;

using fspp::Device;

using namespace copyfs;

class CopyFsTestFixture: public FileSystemTestFixture {
public:
  unique_ptr<Device> createDevice() override {
    return make_unique<CopyDevice>(rootDir.path());
  }

  tempfile::TempDir rootDir;
};

FSPP_ADD_FILESYTEM_TESTS(CopyFS, CopyFsTestFixture);
