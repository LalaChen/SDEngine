#include "LogManager.h"
#include "FileSystemManager.h"
#include "AssetResourceManager.h"
#include "FileResourceRequester.h"

______________SD_START_BASIC_NAMESPACE_______________

SD_SINGLETON_DECLARATION_IMPL(FileResourceRequester);

const std::string FileResourceRequester::sFilePathTypeStrings[FilePathType_MAX_DEFINE_VALUE] = {
    std::string("file:///"),
    std::string("asset:///")
};

FileResourceRequester::FileResourceRequester()
{
}

FileResourceRequester::~FileResourceRequester()
{
}

bool FileResourceRequester::AskFile(const FilePathString &i_location, FileData &io_file)
{
    bool result = false;
    FilePathString final_path;
    FilePathTypeEnum fp_type = ConvertFilePath(i_location, final_path);

    if (FileSystemManager::IsNull() == false) {
        if (fp_type == FilePathType_MAX_DEFINE_VALUE || fp_type == FilePathType_LOCAL_FILE) {
            result = FileSystemManager::GetRef().OpenFile(final_path, io_file);
            if (result == false) {
                if (fp_type != FilePathType_MAX_DEFINE_VALUE) {
                    SDLOG("File[%s] isn't exist!!!", i_location.c_str());
                    return result;
                }
                else {
                    SDLOGE("File[%s] isn't exist!!! Ask this file to AssetResourceManager.", i_location.c_str());
                }
            }
        }
        else {
            SDLOG("filepath[%s] isn't local file.", i_location.c_str());
        }
    }
    else {
        SDLOGW("FileSystemManager isn't launched!!! Ask this file[%s] to AssetResourceManager.", i_location.c_str());
    }

    if (result == false) {
        if (AssetResourceManager::IsNull() == false) {
            if (fp_type == FilePathType_MAX_DEFINE_VALUE || fp_type == FilePathType_ASSET) {
                result = AssetResourceManager::GetRef().LoadAsset(i_location, io_file);
                if (result == false) {
                    SDLOGW("File[%s] isn't exist in file system and asset!!! Please check.", i_location.c_str());
                }
            }
            else {
                SDLOG("filepath[%s] isn't asset.", i_location.c_str());
            }
        }
        else {
            SDLOGW("AssetResourceManager isn't launched!!! Ask File[%s] failure. Please check.", i_location.c_str());
        }
    }

    return result;
}

FilePathTypeEnum FileResourceRequester::ConvertFilePath(const FilePathString &i_location, FilePathString &io_final_path)
{
    for (uint32_t type = 0; type < static_cast<uint32_t>(FilePathType_MAX_DEFINE_VALUE); ++type) {
        size_t tag_pos = i_location.find(sFilePathTypeStrings[type]);
        if (tag_pos != std::string::npos) {
            size_t path_pos = tag_pos + sFilePathTypeStrings[type].length();
            io_final_path = i_location.substr(path_pos, i_location.size() - path_pos);
            return static_cast<FilePathTypeEnum>(type);
        }
    }
    io_final_path = i_location;
    return FilePathType_MAX_DEFINE_VALUE;
}

FilePathString FileResourceRequester::CatenateFilePath(const FilePathString &i_location, const FilePathString &i_target_dir)
{
    size_t tag_pos = i_location.find("///");
    if (tag_pos != std::string::npos) {
        tag_pos += 3;
    }

    FilePathString final_dir = i_target_dir;
    FilePathString fp;
    if (final_dir[final_dir.size() - 1] != '/') {
        final_dir += "/";
    }

    if (tag_pos != std::string::npos) {
        fp = i_location;
        fp.insert(tag_pos, final_dir);
    }
    else {
        fp = final_dir + i_location;
    }

    std::size_t current_dir_pos = fp.find("./");
    if (current_dir_pos != std::string::npos) {
        fp.replace(current_dir_pos, 2, "", 0, 0);
    }

    return fp;
}

_______________SD_END_BASIC_NAMESPACE________________