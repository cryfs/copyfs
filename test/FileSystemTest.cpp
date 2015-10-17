#include <messmer/fspp/fstest/FsTest.h>
#include <messmer/cpp-utils/tempfile/TempDir.h>

#include "../src/CopyDevice.h"

using cpputils::unique_ref;
using cpputils::make_unique_ref;

using fspp::Device;

using namespace copyfs;

class CopyFsTestFixture: public FileSystemTestFixture {
public:
  CopyFsTestFixture(): rootDir() {}

  unique_ref<Device> createDevice() override {
    return make_unique_ref<CopyDevice>(rootDir.path());
  }

  cpputils::TempDir rootDir;
};

FSPP_ADD_FILESYTEM_TESTS(CopyFS, CopyFsTestFixture);
