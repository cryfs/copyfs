#include "messmer/tempfile/src/TempDir.h"
#include "messmer/fspp/fuse/Fuse.h"
#include "messmer/fspp/impl/FilesystemImpl.h"
#include "CopyDevice.h"

using tempfile::TempDir;
using copyfs::CopyDevice;
using fspp::FilesystemImpl;
using fspp::fuse::Fuse;

int main (int argc, char *argv[])
{
  TempDir dir;
  CopyDevice device(dir.path().c_str());
  FilesystemImpl fsimpl(&device);
  Fuse fuse(&fsimpl);
  printf("CopyFS initialized\nBase directory: %s\n", dir.path().c_str());
  fuse.run(argc, argv);
  return 0;
}
