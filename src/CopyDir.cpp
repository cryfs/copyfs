#include "CopyDir.h"
#include "CopyFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "messmer/fspp/fuse/FuseErrnoException.h"
#include "CopyDevice.h"
#include "CopyOpenFile.h"
#include <messmer/cpp-utils/assert/assert.h>

//TODO Get rid of this in favor of exception hierarchy
using fspp::fuse::CHECK_RETVAL;

namespace bf = boost::filesystem;

using cpputils::unique_ref;
using cpputils::make_unique_ref;
using std::string;
using std::vector;

namespace copyfs {

CopyDir::CopyDir(CopyDevice *device, const bf::path &path)
  :CopyNode(device, path) {
  ASSERT(bf::is_directory(base_path()), "Path isn't a valid directory");
}

CopyDir::~CopyDir() {
}

unique_ref<fspp::OpenFile> CopyDir::createAndOpenFile(const string &name, mode_t mode, uid_t uid, gid_t gid) {
  //TODO uid/gid?
  auto file_path = base_path() / name;
  if (bf::exists(file_path)) {
	throw fspp::fuse::FuseErrnoException(EEXIST);
  }
  //Create file
  int fd = ::creat(file_path.c_str(), mode);
  CHECK_RETVAL(fd);
  //TODO Don't close and reopen, that's inperformant
  ::close(fd);
  return make_unique_ref<CopyOpenFile>(device(), path() / name, O_RDWR | O_TRUNC);
}

void CopyDir::createDir(const string &name, mode_t mode, uid_t uid, gid_t gid) {
  //TODO uid/gid?
  auto dir_path = base_path() / name;
  //Create dir
  int retval = ::mkdir(dir_path.c_str(), mode);
  CHECK_RETVAL(retval);
}

void CopyDir::remove() {
  int retval = ::rmdir(base_path().c_str());
  CHECK_RETVAL(retval);
}

unique_ref<vector<CopyDir::Entry>> CopyDir::children() const {
  DIR *dir = ::opendir(base_path().c_str());
  if (dir == nullptr) {
    throw fspp::fuse::FuseErrnoException(errno);
  }

  // Set errno=0 so we can detect whether it changed later
  errno = 0;

  auto result = make_unique_ref<vector<Entry>>();

  struct dirent *entry = ::readdir(dir);
  while(entry != nullptr) {
#ifdef _DIRENT_HAVE_D_TYPE
    if(entry->d_type == DT_DIR) {
      result->push_back(Entry(EntryType::DIR, entry->d_name));
    } else if(entry->d_type == DT_REG) {
      result->push_back(Entry(EntryType::FILE, entry->d_name));
    } else if (entry->d_type == DT_LNK) {
      result->push_back(Entry(EntryType::SYMLINK, entry->d_name));
    }  // else: Ignore files we can't handle (e.g. block device, pipe, ...)
#else
    if(bf::is_symlink(entry->d_name)) { //We have to check for symlink first, because bf::is_directory/bf::is_regular_file return true if the symlink is pointing to a respective entry
     result->push_back(Entry(EntryType::SYMLINK, entry->d_name));
    } else if(bf::is_regular_file(entry->d_name)) {
      result->push_back(Entry(EntryType::FILE, entry->d_name));
    } else if(bf::is_directory(entry->d_name)) {
      result->push_back(Entry(EntryType::DIR, entry->d_name));
    } // else: Ignore files we can't handle (e.g. block device, pipe, ...)
#endif
    entry = ::readdir(dir);
  }
  //On error, ::readdir returns nullptr and sets errno.
  if (errno != 0) {
    int readdir_errno = errno;
    ::closedir(dir);
    throw fspp::fuse::FuseErrnoException(readdir_errno);
  }
  int retval = ::closedir(dir);
  CHECK_RETVAL(retval);
  return result;
}

void CopyDir::createSymlink(const std::string &name, const boost::filesystem::path &target, uid_t uid, gid_t gid) {
  //TODO uid/gid?
  auto from_path = base_path() / name;
  int retval = ::symlink(target.c_str(), from_path.c_str());
  CHECK_RETVAL(retval);
}

}

