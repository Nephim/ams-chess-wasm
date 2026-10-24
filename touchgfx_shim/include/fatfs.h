#ifndef SHIM_FATFS_H
#define SHIM_FATFS_H

#include <string>

void WriteToSDCARD(const char* data);
std::string ReadFromSDCARD();

#endif // SHIM_FATFS_H
