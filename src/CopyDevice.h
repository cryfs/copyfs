#pragma once
#ifndef MESSMER_COPYFS_SRC_COPYDEVICE_H_
#define MESSMER_COPYFS_SRC_COPYDEVICE_H_

#include <boost/filesystem.hpp>
#include <messmer/fspp/fs_interface/Device.h>

#include "messmer/cpp-utils/macros.h"

namespace copyfs {

namespace bf = boost::filesystem;

class CopyDevice final: public fspp::Device {
public:
  explicit CopyDevice(const bf::path &rootdir);
  ~CopyDevice();

  void statfs(const boost::filesystem::path &path, struct ::statvfs *fsstat) override;

  const bf::path &RootDir() const;
private:
  boost::optional<cpputils::unique_ref<fspp::Node>> Load(const bf::path &path) override;

  const bf::path _root_path;

  DISALLOW_COPY_AND_ASSIGN(CopyDevice);
};

inline const bf::path &CopyDevice::RootDir() const {
  return _root_path;
}

}

#endif
