#include "FileData.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{
//---------------- File Data -------------------
FileData::FileData()
{
}

FileData::FileData(const FileData &i_src)
	:m_file_content(i_src.m_file_content)
{
}

FileData::~FileData()
{
}

void FileData::SetFileData(const std::vector<unsigned char> &i_src)
{
	m_file_content = i_src;
}

void FileData::SetFileData(const std::string &i_src)
{
	m_file_content.resize(i_src.size());
	for (int i = 0; i < i_src.size(); i++)
	{
		m_file_content[i] = static_cast<unsigned char>(i_src[i]);
	}
}

bool FileData::IsEmpty() const
{
	if (m_file_content.size() == 0) return true;
	else return false;
}

size_t FileData::GetSize() const
{
	return m_file_content.size();
}

const BytePtr FileData::GetDataCharAddr() const
{
	return (const BytePtr)(&m_file_content[0]);
}

const UBytePtr FileData::GetDataUCharAddr() const
{
	return (const UBytePtr)(&m_file_content[0]);
}

std::string FileData::GetDataString() const
{
	std::string content;
	for (int i = 0; i < m_file_content.size(); i++)
		content += m_file_content[i];
	content += '\0';
	return content;
}

std::wstring FileData::GetDataWString() const
{
	return StringToWString(GetDataString());
}
//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}