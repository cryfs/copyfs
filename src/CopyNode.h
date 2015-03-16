#pragma once
#ifndef COPYFS_LIB_COPYNODE_H_
#define COPYFS_LIB_COPYNODE_H_

#include "CopyDevice.h"
#include <messmer/fspp/fs_interface/Node.h>
#include "messmer/cpp-utils/macros.h"


namespace copyfs {

class CopyNode: public virtual fspp::Node {
public:
  CopyNode(CopyDevice *device, const boost::filesystem::path &path);
  virtual ~CopyNode();

  void stat(struct ::stat *result) const override;
  void access(int mask) const override;
  void rename(const boost::filesystem::path &to) override;
  void utimens(const timespec times[2]) override;

protected:
  boost::filesystem::path base_path() const;
  const boost::filesystem::path &path() const;
  CopyDevice *device();
  const CopyDevice *device() const;

private:
  CopyDevice *const _device;
  boost::filesystem::path _path;

  DISALLOW_COPY_AND_ASSIGN(CopyNode);
};

inline boost::filesystem::path CopyNode::base_path() const {
  return _device->RootDir() / _path;
}

inline const boost::filesystem::path &CopyNode::path() const {
  return _path;
}

inline CopyDevice *CopyNode::device() {
  return const_cast<CopyDevice*>(const_cast<const CopyNode*>(this)->device());
}

inline const CopyDevice *CopyNode::device() const {
  return _device;
}

}

#endif
