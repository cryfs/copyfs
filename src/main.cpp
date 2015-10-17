#include "messmer/cpp-utils/tempfile/TempDir.h"
#include "messmer/fspp/fuse/Fuse.h"
#include "messmer/fspp/impl/FilesystemImpl.h"
#include "CopyDevice.h"
#include <iostream>

using cpputils::TempDir;
using copyfs::CopyDevice;
using fspp::FilesystemImpl;
using fspp::fuse::Fuse;

int main (int argc, char *argv[])
{
  TempDir dir;
  CopyDevice device(dir.path().c_str());
  FilesystemImpl fsimpl(&device);
  Fuse fuse(&fsimpl);
  std::cout << "CopyFS initialized\nBase directory: " << dir.path().c_str() << std::endl;
  fuse.run(argc, argv);
  return 0;
}
