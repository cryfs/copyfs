add_library(copyfs CopyDevice.cpp CopyDir.cpp CopyFile.cpp CopyNode.cpp CopyOpenFile.cpp)

target_link_libraries(copyfs)
