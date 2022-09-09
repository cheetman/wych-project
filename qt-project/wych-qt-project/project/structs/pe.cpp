#include "pe.h"
#include <Windows.h>
#include <tchar.h>
#include <QString>
#include <QMessageBox>
#include <itemview10pe.h>


PE::PE(const  wchar_t *path, void *parent)
{
    FILE *fp = _tfopen(path, TEXT("rb"));

    if (fp == NULL) {
        MessageBox(NULL, TEXT("fp == null"), NULL, NULL);
        return;
    }
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    LPVOID ptempFileBuffer = malloc(file_size);

    if (ptempFileBuffer == NULL) {
        MessageBox(NULL, TEXT("ptempfilebuffer == null"), NULL, NULL);
        fclose(fp);
        return;
    }
    fseek(fp, 0, SEEK_SET);         // 将指针指回文件头
    fread(ptempFileBuffer, file_size, 1, fp);
    m_lpFileData = ptempFileBuffer; // 赋值，完成工作
    m_fileSize = file_size;
    fclose(fp);
    this->parent = parent;
}

PE::~PE() {
    if (m_lpFileData) {
        free(m_lpFileData);
    }
}

bool PE::build() {
    if (m_lpFileData == NULL) {
        return false;
    }

    // 1.获取DosHeader
    if (*((PWORD)m_lpFileData) != IMAGE_DOS_SIGNATURE) {
        warning("the first word is not MZ!");
        return false;
    }
    m_lpDosHeader = (PIMAGE_DOS_HEADER)m_lpFileData;

    if (isX86()) {
        // 2.获取NTHeader
        m_lpNtHeader32 = (PIMAGE_NT_HEADERS32)((size_t)m_lpFileData + m_lpDosHeader->e_lfanew);

        if (m_lpNtHeader32->Signature != IMAGE_NT_SIGNATURE) {
            warning("nt header error!signature not match!");
            return false;
        }


        // 3.获取节表地址
        m_lpSecHeader = (PIMAGE_SECTION_HEADER)(
            (size_t)m_lpNtHeader32                              // NT头地址(fov)
            + sizeof(IMAGE_NT_SIGNATURE)                        // + PE文件标识(4字节)
            + sizeof(IMAGE_FILE_HEADER)                         // + 文件头(20字节)
            + m_lpNtHeader32->FileHeader.SizeOfOptionalHeader); // + 可选头(SizeOfOptionalHeader字节)

        // 4.数据目录地址，各种表地址
        m_lpExportTable =  &(m_lpNtHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
        m_lpImportTable =  &(m_lpNtHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
        m_lpResourceTable =  &(m_lpNtHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]);
        m_lpRelocationTable =  &(m_lpNtHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
    } else {
        // 获取NTHeader
        m_lpNtHeader64 = (PIMAGE_NT_HEADERS64)((size_t)m_lpFileData + m_lpDosHeader->e_lfanew);

        if (m_lpNtHeader64->Signature != IMAGE_NT_SIGNATURE) {
            warning("nt header error!signature not match!");
            return false;
        }

        // 3.获取节表地址
        m_lpSecHeader = (PIMAGE_SECTION_HEADER)(
            (size_t)m_lpNtHeader64                              // NT头地址(fov)
            + sizeof(IMAGE_NT_SIGNATURE)                        // + PE文件标识(4字节)
            + sizeof(IMAGE_FILE_HEADER)                         // + 文件头(20字节)
            + m_lpNtHeader64->FileHeader.SizeOfOptionalHeader); // + 可选头(SizeOfOptionalHeader字节)

        // 4.数据目录地址，各种表地址
        m_lpExportTable =  &(m_lpNtHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
        m_lpImportTable =  &(m_lpNtHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
        m_lpResourceTable =  &(m_lpNtHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]);
        m_lpRelocationTable =  &(m_lpNtHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
    }


    // 5.数据目录的foa算出来
    // 6.导出表
    if (m_lpExportTable->VirtualAddress) {
        rvaToFoa(m_lpExportTable->VirtualAddress,            &m_foaExportTable);
        m_lpExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((size_t)m_lpFileData + m_foaExportTable);

        rvaToFoa(m_lpExportDirectory->AddressOfFunctions,    &m_foaAddressOfFunctions);
        rvaToFoa(m_lpExportDirectory->AddressOfNames,        &m_foaAddressOfNames);
        rvaToFoa(m_lpExportDirectory->AddressOfNameOrdinals, &m_foaAddressOfNameOrdinals);
    }

    // 导入表
    if (m_lpImportTable->VirtualAddress) {
        rvaToFoa(m_lpImportTable->VirtualAddress, &m_foaImportTable);

        // 第一个表的地址,使用需要while
        m_lpImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((size_t)m_lpFileData + m_foaImportTable);
    }

    if (m_lpResourceTable->VirtualAddress) {
        rvaToFoa(m_lpResourceTable->VirtualAddress, &m_foaResourceTable);
    }

    // 6.重定位表
    if (m_lpRelocationTable->VirtualAddress) {
        rvaToFoa(m_lpRelocationTable->VirtualAddress, &m_foaRelocationTable);

        // 第一个表的地址,使用需要while
        m_lpRelocationBase = (PIMAGE_BASE_RELOCATION)((size_t)m_lpFileData + m_foaRelocationTable);
    }


    return true;
}

void PE::warning(const QString& msg) {
    if (parent) {
        ((ItemView10PE *)parent)->appendMessage(msg);
    }
}

bool PE::isX86() {
    if (*(PWORD)((size_t)m_lpFileData + m_lpDosHeader->e_lfanew + 0x14) == 0x00E0) {
        return true;
    } else {
        return false;
    }
}

bool PE::isX64() {
    if (*(PWORD)((size_t)m_lpFileData + m_lpDosHeader->e_lfanew + 0x14) == 0x00E0) {
        return false;
    } else {
        return true;
    }
}

bool PE::rvaToFoa(IN DWORD RVA, OUT PDWORD FOA) {
    if (isX86()) {
        if (RVA < m_lpNtHeader32->OptionalHeader.SizeOfHeaders) // 如果是在头部，在节之前，说明并不需要拉伸，RVA = FOA
        {
            *FOA = RVA;
            return true;
        }

        for (int i = 0; i < m_lpNtHeader32->FileHeader.NumberOfSections; i++) // 循环每一个节表
            if ((RVA >= m_lpSecHeader[i].VirtualAddress) &&
                (RVA < m_lpSecHeader[i].VirtualAddress + m_lpSecHeader[i].Misc.VirtualSize))
            {
                *FOA = m_lpSecHeader[i].PointerToRawData + RVA - m_lpSecHeader[i].VirtualAddress;
                return true;
            }

        warning("rvaToFoa error!");
        return false;
    } else {
        if (RVA < m_lpNtHeader64->OptionalHeader.SizeOfHeaders) // 如果是在头部，在节之前，说明并不需要拉伸，RVA = FOA
        {
            *FOA = RVA;
            return true;
        }

        for (int i = 0; i < m_lpNtHeader64->FileHeader.NumberOfSections; i++) // 循环每一个节表
            if ((RVA >= m_lpSecHeader[i].VirtualAddress) &&
                (RVA < m_lpSecHeader[i].VirtualAddress + m_lpSecHeader[i].Misc.VirtualSize))
            {
                *FOA = m_lpSecHeader[i].PointerToRawData + RVA - m_lpSecHeader[i].VirtualAddress;
                return true;
            }

        warning("rvaToFoa error!");
        return false;
    }
}
