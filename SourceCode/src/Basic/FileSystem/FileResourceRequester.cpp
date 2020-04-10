#include "LogManager.h"
#include "FileSystemManager.h"
#include "AssetResourceManager.h"
#include "FileResourceRequester.h"

______________SD_START_BASIC_NAMESPACE_______________

SD_SINGLETON_DECLARATION_IMPL(FileResourceRequester);

FileResourceRequester::FileResourceRequester()
{
}

FileResourceRequester::~FileResourceRequester()
{
}

bool FileResourceRequester::AskFile(const FilePathString &i_location, FileData &io_file)
{
    bool result = false;
    if (FileSystemManager::IsNull() == false) {
        result = FileSystemManager::GetRef().OpenFile(i_location, io_file);
        if (result == false) {
            SDLOGD("File[%s] isn't exist!!! Ask this file to AssetResourceManager.", i_location.c_str());
        }
    }
    else {
        SDLOGW("FileSystemManager isn't launched!!! Ask this file to AssetResourceManager.", i_location.c_str());
    }

    if (result == false) {
        if (AssetResourceManager::IsNull() == false) {
            result = AssetResourceManager::GetRef().LoadAsset(i_location, io_file);
            if (result == false) {
                SDLOGW("File[%s] isn't exist in file system and asset!!! Please check.", i_location.c_str());
            }
        }
        else {
            SDLOGW("AssetResourceManager isn't launched!!! Please check.", i_location.c_str());
        }
    }

    return result;
}

_______________SD_END_BASIC_NAMESPACE________________