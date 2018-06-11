﻿#include <iostream>
#include "ERROR/Error.h"
#include "BUFFER/Buffer.h"

using namespace std;


// 判断POD数据
void IsPod()
{
	cout << std::is_pod<PAGEHEAD>::value << endl;
	cout << std::is_pod<FileAddr>::value << endl;
	cout << std::is_pod<FILECOND>::value << endl;
}

int main()
{
	//IsPod();

	BUFFER buffer;
	auto pMemFile = buffer["data"];
	if(!pMemFile)
		buffer.CreateFile("data");


	char s[] = "test00E";
	FileAddr fd_del = { 1,0X18 };
	//auto fd = buffer["data"]->DeleteRecord(&fd_del, sizeof(s));
	//auto fd = buffer["data"]->AddRecord(s, sizeof(s));
	//cout << fd.filePageID << " " << fd.offSet << endl;
	system("pause");
}