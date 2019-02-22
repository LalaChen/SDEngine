#include <windows.h>

#include "WindowsFileSystemManager.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

void FileMgrUnitTest()
{
	std::cout << "********************* File Sys Mgr Test *********************" << std::endl;
	FilePathString test_dir = "TestDir\\";
	FilePathString test_bak_dir = "TestDirBackUp\\";
	FilePathString work_dir = FileSystemManager::GetRef().GetWorkDir() + test_dir; //ProjDir
	FilePathString work_bak_dir = FileSystemManager::GetRef().GetWorkDir() + test_bak_dir;

	auto file_callback = [test_bak_dir, test_dir](const std::string &path, const std::string &file, int level)
	{
		FilePathString equal_path = path;
		size_t rep_start = equal_path.find(test_bak_dir);
		if (rep_start < equal_path.size()) {
			equal_path.replace(rep_start, test_bak_dir.size(), test_dir);
			bool dir_rst = FileSystemManager::GetRef().IsExistedFileOrPath(equal_path);
			if (dir_rst == false) {
				FileSystemManager::GetRef().CreateDir(equal_path);
			}
			FileSystemManager::GetRef().CopyFileTo(path + file, equal_path + file);
			for (int i = 0; i < level; i++)
			{
				std::cout << "\t";
			}
			std::cout << "path = " << path << " file[" << file << "]" << std::endl;
		}
		else {

		}
	};

	auto dir_callback = [test_bak_dir, test_dir](const std::string &path, const std::string &file, int level)
	{
		FilePathString equal_path = path;
		size_t rep_start = equal_path.find(test_bak_dir);
		if (rep_start < equal_path.size()) {
			equal_path.replace(rep_start, test_bak_dir.size(), test_dir);
			bool dir_rst = FileSystemManager::GetRef().IsExistedFileOrPath(equal_path);
			if (dir_rst == false) {
				FileSystemManager::GetRef().CreateDir(equal_path);
			}
			for (int i = 0; i < level; i++)
			{
				std::cout << "\t";
			}
			std::cout << "Handle Dir [" << path << "] End" << std::endl;
		}
	};
	//1. Copy File.
	std::cout << "Test 1 => Copy File to Dir : " << std::endl;
	FileSystemManager::GetRef().VisitDir(work_bak_dir, file_callback, dir_callback, true);

	//2. copy file
	{
		std::cout << "Test2 => copy file : " << std::endl;
		FilePathString dst_dir = work_dir + "CopyFile/";
		FilePathString dst_fn = "CopyFile.txt";
		FileSystemManager::GetRef().CopyFileTo(work_dir + dst_fn, dst_dir + dst_fn);
		bool dir_rst = FileSystemManager::GetRef().IsExistedFileOrPath(dst_dir);
		bool file_rst = FileSystemManager::GetRef().IsExistedFile(dst_dir + dst_fn);
		if (dir_rst == true && file_rst == true){
			std::cout << "Copy File[" << dst_fn << "] to Path[" << dst_dir + dst_fn << "] is success !!!" << std::endl;
		}
		else{
			if (dir_rst == false) {
				std::cout << "Copy File[" << dst_fn << "] to Path[" << dst_dir + dst_fn << "] is failure!!!" 
					      << "Dst Dir [" << dst_dir << "] isn't exist!!!" << std::endl;
			}
			else {
				std::cout << "Copy File[" << dst_fn << "] to Path[" << dst_dir + dst_fn << "] is failure!!!"
					<< "Dst File [" << dst_dir + dst_fn << "] isn't exist!!!" << std::endl;
			}
		}
	}
	//3. create directory
	{
		std::cout << "Test3 => create directory : " << std::endl;
		FilePathString dst_dir = work_dir + "NewDir/";
		FileSystemManager::GetRef().CreateDir(dst_dir);
		bool dir_rst = FileSystemManager::GetRef().IsExistedFileOrPath(dst_dir);
		if (dir_rst == true) {
			std::cout << "Create Dir[" << dst_dir << "] is success!!!" << std::endl;
		}
		else {
			std::cout << "Create Dir[" << dst_dir << "] is failure!!!" << std::endl;
		}
	}
	//4. exist directory or file
	{
		std::cout << "Test4 => exist directory or file : " << std::endl;
		bool result = false;
		result = FileSystemManager::GetRef().IsExistedFileOrPath(work_dir + "NewDir");
		std::cout << "dir NewDir result(1) : " << result << std::endl;
		result = FileSystemManager::GetRef().IsExistedFileOrPath(work_dir + "CopyFile1.txt");
		std::cout << "file CopyFile1 result(1) : " << result << std::endl;
		result = FileSystemManager::GetRef().IsExistedFileOrPath(work_dir + "NewDir2");
		std::cout << "dir NewDir2 result(1) : " << result << std::endl;
		result = FileSystemManager::GetRef().IsExistedFileOrPath(work_dir + "NewDir2\\NewDir2.txt");
		std::cout << "file NewDir2.txt in NewDir2 result(1) : " << result << std::endl;
	}
	//5. find first file in dir
	{
		std::cout << "Test5 => find first file in dir : " << std::endl;
		FilePathString rst_file = FileSystemManager::GetRef().GetFirstFileNameInTargetDir(work_dir, ".txt", false);
		std::cout << "first file in dir [" << work_dir << "] is [" << rst_file << std::endl;
	}
	//6. delete directory
	{
		std::cout << "Test6 => delete directory : " << std::endl;
		FilePathString dst_dir = work_dir + "EmptyDir/";
		if (FileSystemManager::GetRef().IsExistedFileOrPath(dst_dir) == true) {
			bool dir_rst = FileSystemManager::GetRef().DeleteDir(dst_dir);
			if (dir_rst == true) {
				std::cout << "Delete Dir[" << dst_dir << "] is success!!!" << std::endl;
			}
			else {
				std::cout << "Delete Dir[" << dst_dir << "] is failure!!!" << std::endl;
			}
		}
		else {
			std::cout << "Dir[" << dst_dir <<"] isn't exist!!!" << std::endl;
		}
	}
	{
	}
	
	std::cout <<std::endl << std::endl << "********************* CN_ZH Test *********************" << std::endl;

	FilePathString wb_tw_path_dir = work_dir + WStringToString(L"中文路徑\\中文路徑檔案.txt", SD_CODE_PAGE);

	FilePathString wb_cn_path_dir = work_dir + WStringToString(L"简体中文路径\\简体中文路径檔案.txt", SD_CODE_PAGE);

	{
		std::cout << "Test7 =>  Load zhtwfd test" << std::endl;
		//1. load zh-tw file.
		FileData zhtwfd;
		//2. print zh-tw file data.
		if (FileSystemManager::GetRef().OpenFile(wb_tw_path_dir, zhtwfd) == true)
			std::cout << "zhtw File Context = " << zhtwfd.GetDataString() << std::endl;
		else std::cout << "zhtwfd Load Failure!!!" << std::endl;
	}

	{
		std::cout << "Test8 =>  Load zhcnfd test" << std::endl;
		//3. load zh-cn file.
		FileData zhcnfd;
		//4. print zh-tw file data.
		if (FileSystemManager::GetRef().OpenFile(wb_cn_path_dir, zhcnfd) == true)
			std::cout << "zhcn File Context = " << zhcnfd.GetDataString() << std::endl;
		else std::cout << "zhcn Load Failure!!!" << std::endl;
	}

	{
		std::cout << "Test9 => Create Dir and Move file" << std::endl;
		FilePathString wb_tw_dest_path_dir = work_dir + WStringToString(L"中文路徑copy\\", SD_CODE_PAGE);
		FilePathString wb_tw_dest_fn = WStringToString(L"中文檔案.txt", SD_CODE_PAGE);
		FilePathString wb_cn_dest_path_dir = work_dir + WStringToString(L"简体中文路径copy\\", SD_CODE_PAGE);
		FilePathString wb_cn_dest_fn = WStringToString(L"简体中文路径檔案.txt", SD_CODE_PAGE);

		FileSystemManager::GetRef().CreateDir(wb_tw_dest_path_dir);
		if (FileSystemManager::GetRef().IsExistedFileOrPath(wb_tw_dest_path_dir) == true) {
			FileSystemManager::GetRef().CopyFileTo(wb_tw_path_dir, wb_tw_dest_path_dir + wb_tw_dest_fn);
			if (FileSystemManager::GetRef().IsExistedFile(wb_tw_dest_path_dir + wb_tw_dest_fn) == true) {
				std::cout << "zhtw File copy successful!!!" << std::endl;
			}
			else {
				std::cout << "zhtw File copy failure!!!" << std::endl;
			}
		}
		else {
			std::cout << "zhtw dir create failure!!!" << std::endl;
		}

		FileSystemManager::GetRef().CreateDir(wb_cn_dest_path_dir);
		if (FileSystemManager::GetRef().IsExistedFileOrPath(wb_cn_dest_path_dir) == true) {
			FileSystemManager::GetRef().CopyFileTo(wb_cn_path_dir, wb_cn_dest_path_dir + wb_cn_dest_fn);
			if (FileSystemManager::GetRef().IsExistedFile(wb_cn_dest_path_dir + wb_cn_dest_fn) == true) {
				std::cout << "zhcn File copy successful!!!" << std::endl;
			}
			else {
				std::cout << "zhcn File copy failure!!!" << std::endl;
			}
		}
		else {
			std::cout << "zhcn dir create failure!!!" << std::endl;
		}
	}


	//Final. detete directory recusive
	///*
	{
		std::cout << "Test Final : Delete Test Dir !!!" << std::endl;
		FilePathString dst_dir = work_dir;
		FileSystemManager::GetRef().DeleteAllFilesInDir(dst_dir, true, true);
	}
	//*/
}

void ReplaceCharTest()
{
	std::cout << "********************* Replace Char Test *********************" << std::endl;
	std::string fname = "1:2\\ 3 test+file-name<>^!@#$%?//";
	std::cout << fname << std::endl;
	std::cout << ReplaceIllegalCharOfFile(fname) << std::endl;
	std::cout << ReplaceIllegalCharOfFile(fname, '^') << std::endl;
	std::cout << " NUL symbol = " << (int)('\0');
}

int main(int argc, char **argv)
{
	new WindowsFileSystemManager();
	FileSystemManager::GetRef().Initialize();
	FileMgrUnitTest();
	ReplaceCharTest();
}