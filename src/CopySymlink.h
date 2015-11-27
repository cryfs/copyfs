#pragma once
#ifndef MESSMER_COPYFS_SRC_COPYSYMLINK_H_
#define MESSMER_COPYFS_SRC_COPYSYMLINK_H_

#include "CopyNode.h"
#include <messmer/fspp/fs_interface/Symlink.h>

namespace copyfs {

class CopySymlink final: public fspp::Symlink, CopyNode {
public:
  CopySymlink(CopyDevice *device, const bf::path &path);
  ~CopySymlink();

  boost::filesystem::path target() const override;
  void remove() override;

private:
  DISALLOW_COPY_AND_ASSIGN(CopySymlink);
};

}

#endif
