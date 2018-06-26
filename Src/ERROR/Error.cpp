#include "Error.h"
#include <vector>

namespace SQLError
{
	static std::fstream log_file;

	void BaseError::PrintError() const
	{
		// 输出异常
		std::cout << ErrorInfo;
	}

	void BaseError::WriteToLog()
	{
		// 写入日志
		log_file.open("log", std::ios::out | std::ios::app);
		log_file << ErrorInfo << std::endl;
		log_file.close();
	}

}

void SQLError::DispatchError(const BaseError &error)
{
	error.PrintError();
}

SQLError::LSEEK_ERROR::LSEEK_ERROR()
{
	ErrorInfo = "(LSEEK_FAILED) The file handle is invalid or the value for origin is invalid "
		"or the position the position specified by offset is before the beginning of the file.";
}


SQLError::READ_ERROR::READ_ERROR()
{
	ErrorInfo = "(READ_FAILED) Illegal page number (less than zero).";
}


SQLError::WRITE_ERROR::WRITE_ERROR()
{
	ErrorInfo = "(WRITE_FAILED) The file handle is invalid or the file is not opened for writing "
		"or there is not enough space left on the device for the operation.";
}


SQLError::FILENAME_CONVERT_ERROR::FILENAME_CONVERT_ERROR()
{
	ErrorInfo = "(FILENAME_CONVERT_FAILED) File name convert failed";
}


SQLError::KEY_INSERT_ERROR::KEY_INSERT_ERROR()
{
	ErrorInfo = "(KEY_INSERT_FAILED) Key Word Insert Failed! The record that to inset has been excisted!";
}


SQLError::BPLUSTREE_DEGREE_TOOBIG_ERROR::BPLUSTREE_DEGREE_TOOBIG_ERROR()
{
	ErrorInfo = "(BPLUSTREE_DEGREE_TOOBIG) A page of file can not contain a tree node!";
}


SQLError::KeyAttr_NameLength_ERROR::KeyAttr_NameLength_ERROR()
{
	ErrorInfo = "(BPLUSTREE_DEGREE_TOOBIG) KeyAttr name length flowover,it may be happen in where you set the record's key!";
}
