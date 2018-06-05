/****************************************************************** 

** 文件名: Buffer.h

** Copyright (c) 

** 创建人: 谭东亮

** 日  期: 2018-6-3

** 描  述: MiniSQL buffer模块所有类和结构
           通过该模块实现物理文件的写入和删除
** 版  本: 1.00

******************************************************************/

#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <iostream>
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>

#define FILE_PAGESIZE		4096	// 内存页(==文件页)大小
#define MEM_PAGEAMOUNT		6	// 内存页数量
#define MAX_FILENAME_LEN    256		// 文件名（包含路径）最大长度

class Clock;
Clock* GetGlobalClock();

/*********************************************************
*             文件地址,定位文件中的位置
**********************************************************/
class FileAddr
{
	friend class FILECOND;
public:
	void SetFileAddr(unsigned long _filePageID, unsigned int  _offSet);
	unsigned long filePageID;     // 文件页编号
	unsigned int  offSet;         // 页内偏移量
	bool operator==(const FileAddr &rhs)
	{
		return (this->filePageID == rhs.filePageID && this->offSet == rhs.offSet);
	}
};


/*********************************************************
*             页头信息，用以标识文件页
**********************************************************/
class PAGEHEAD
{
public:
	void Initialize();          // 初始化为文件第一页
	unsigned long pageId;		// 页编号
	bool isFixed;				// 页是否常驻内存
};


/*********************************************************
*               文件头信息
**********************************************************/
class FILECOND
{
public:
	void Initialize();
	FileAddr DelFirst;                // 第一条被删除记录地址
	FileAddr DelLast;                 // 最后一条被删除记录地址  
	FileAddr NewInsert;               // 文件末尾可插入新数据的地址
	unsigned long total_page;         // 目前文件中共有页数
};

/*********************************************************
*
*   名称：内存页类
*   功能：提供保存文件页的空间，以及该页相关的信息
*   约束：内存页的大小固定
*
**********************************************************/
class MemPage
{
	friend class MemFile;
	friend class Clock;
public:
	MemPage();
	~MemPage();
	void Back2File() const;       // 把内存中的页写回到文件中
	bool SetModified();           // 设置为脏页

public:
	unsigned long fileId;         // 文件指针，while fileId==0 时为被抛弃的页
	unsigned long filePageID;     // 文件页号

	mutable bool bIsLastUsed;     // 最近一次访问内存是否被使用，用于Clock算法
	mutable bool isModified;      // 是否脏页

	void *Ptr2PageBegin;          // 实际保存物理文件数据的地址
	PAGEHEAD *pageHead;           // 页头指针
	FILECOND* GetFileCond();      // 文件头指针（while filePageID == 0）
};

/*********************************************************
*
*   名称：内存页管理类（Clock页面置换算法）
*   功能：物理页面在内存中的缓存，加速对物理文件的读写
*   约束：调用者保证需要被载入的物理文件都存在，且加载的页面不越界
*
**********************************************************/
class Clock
{
	friend class MemFile;
public:
	Clock();
	~Clock();
	// 返回磁盘文件内存地址
	MemPage* GetMemAddr(unsigned long fileId, unsigned long filePageID);

	// 创建新页，适用于创建新文件或者添加新页的情况下
	MemPage* CreatNewPage(unsigned long fileId, unsigned long filePageID);

private:
	// 返回一个可替换的内存页索引
	// 原页面内容该写回先写回
	unsigned int GetReplaceablePage();

	// return the file page memory address if it is in memory
	// otherwise return nullptr;
	MemPage* GetExistedPage(unsigned long fileId, unsigned long filePageID);
	MemPage* LoadFromFile(unsigned long fileId, unsigned long filePageID);

	// Clock置换算法
	unsigned long ClockSwap();

	 
private:
	MemPage* MemPages[MEM_PAGEAMOUNT+1];  // 内存页对象数组
};

/*********************************************************
*   名称：内存文件类
*   功能：通过物理文件在内存中的映射文件的操作，从而操作物理文件
*   约束：假设所有被操作的文件都存在且已经打开
**********************************************************/
class MemFile
{
	friend class BUFFER;
public:
	FileAddr AddRecord(void*source, size_t sz_wipe);
	FileAddr DeleteRecord(void*source, size_t sz_wipe, FileAddr *fd_to_wipe);
	// 写入数据
	void* MemRead(FileAddr *mem_to_read);                           // 读取内存文件,返回读取位置指针
	FileAddr MemWrite(const void* source, size_t length);           // 在可写入地址写入数据
	FileAddr MemWrite(const void* source, size_t length, FileAddr* dest);
	void MemWipe(void*source, size_t sz_wipe, FileAddr *fd_to_wipe);

	MemPage * AddExtraPage();                                       // 当前文件添加一页空间
	MemPage* GetFileFirstPage();                                    // 得到文件首页

private:
	// 构造
	MemFile(const char *file_name, unsigned long file_id);
private:
	char fileName[MAX_FILENAME_LEN];
	unsigned long fileId;                                          // 文件指针
	unsigned long total_page;                                      // 目前文件中共有页数
};


class BUFFER
{
public:
	BUFFER() = default;
	~BUFFER();
	MemFile* GetMemFile(const char *fileName);
	void CreateFile(const char *fileName);
	void CloseFile();
public:
	std::vector<MemFile*> memFile;  // 保存已经打开的文件列表
};



#endif //define _BUFFER_H_
