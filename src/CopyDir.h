#pragma once
#ifndef COPYFS_LIB_COPYDIR_H_
#define COPYFS_LIB_COPYDIR_H_

#include "CopyNode.h"
#include <messmer/fspp/fs_interface/Dir.h>
#include <messmer/fspp/fs_interface/File.h>

namespace copyfs {

class CopyDir: public fspp::Dir, CopyNode {
public:
  CopyDir(CopyDevice *device, const bf::path &path);
  virtual ~CopyDir();

  //TODO return type variance to CopyFile/CopyDir?
  cpputils::unique_ref<fspp::OpenFile> createAndOpenFile(const std::string &name, mode_t mode, uid_t uid, gid_t gid) override;
  void createDir(const std::string &name, mode_t mode, uid_t uid, gid_t gid) override;
  void createSymlink(const std::string &name, const boost::filesystem::path &target, uid_t uid, gid_t gid) override;
  void remove() override;

  cpputils::unique_ref<std::vector<Entry>> children() const override;

private:
  DISALLOW_COPY_AND_ASSIGN(CopyDir);
};

}

#endif
