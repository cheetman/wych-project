#include "utils.h"

#include <QString>
#include <Windows.h>
#include <tchar.h>

typedef struct _CLIENT_ID
{
    PVOID UniqueProcess;
    PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef long (WINAPI *PRTL_CREATE_USER_THREAD)
(
    __in HANDLE Process,
    __in_opt PSECURITY_DESCRIPTOR ThreadSecurityDescriptor,
    __in char Flags,
    __in_opt ULONG ZeroBits,
    __in_opt SIZE_T MaximumStackSize,
    __in_opt SIZE_T CommittedStackSize,
    __in PTHREAD_START_ROUTINE StartAddress,
    __in_opt PVOID Parameter,
    __out_opt PHANDLE Thread,
    __out_opt PCLIENT_ID ClientId
);

Utils::Utils()
{}

QString Utils::SizeToString(const quint64 val)
{
    if (val == NULL) {
        return "";
    }

    if (val > 1024 * 1024 * 1024) {
        return QString::number(val / (1024.0f * 1024.0f * 1024.0f), 'f', 1) + "GB";
    }
    else if (val > 1024 * 1024) {
        return QString::number(val / (1024.0f * 1024.0f), 'f', 1) + "MB";
    }
    else if (val > 1024) {
        return QString::number(val / (1024.0f), 'f', 1) + "KB";
    }
    else {
        return QString::number(val) + "B";
    }
}

QString Utils::FromSpecialEncoding(const QString& InputStr)
{
#ifdef Q_OS_WIN
    return QString::fromLocal8Bit(InputStr.toLatin1());

#else // ifdef Q_OS_WIN
    QTextCodec *codec = QTextCodec::codecForName("gbk");

    if (codec)
    {
        return codec->toUnicode(InputStr.toLatin1());
    }
    else
    {
        return QString("");
    }
#endif // ifdef Q_OS_WIN
}

QString Utils::ToSpecialEncoding(const QString& InputStr)
{
#ifdef Q_OS_WIN
    return QString::fromLatin1(InputStr.toLocal8Bit());

#else // ifdef Q_OS_WIN
    QTextCodec *codec = QTextCodec::codecForName("gbk");

    if (codec)
    {
        return QString::fromLatin1(codec->fromUnicode(InputStr));
    }
    else
    {
        return QString("");
    }
#endif // ifdef Q_OS_WIN
}

size_t Utils::ReadFile(IN LPCTSTR file_in, OUT LPVOID *pFileBuffer) {
    FILE *fp;

    fp = _tfopen(file_in, TEXT("rb"));

    if (fp == NULL) {
        MessageBox(NULL, TEXT("fp == null"), NULL, NULL);
        return 0;
    }
    LPVOID ptempFileBuffer;
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    ptempFileBuffer = malloc(file_size);
    fseek(fp, 0, SEEK_SET); // 将指针指回文件头
    fread(ptempFileBuffer, file_size, 1, fp);

    if (ptempFileBuffer == NULL) {
        MessageBox(NULL, TEXT("ptempfilebuffer == null"), NULL, NULL);
        return 0;
    }
    *pFileBuffer = ptempFileBuffer; // 赋值，完成工作
    fclose(fp);                     // 收尾
    return file_size;
}

bool Utils::RVA_TO_FOA(
    PIMAGE_NT_HEADERS32 pNTHeader,
    PIMAGE_SECTION_HEADER pSectionHeader, IN DWORD RVA,
    OUT PDWORD FOA)
{
    if (RVA <
        pNTHeader->OptionalHeader
        .SizeOfHeaders) // 如果是在头部，在节之前，说明并不需要拉伸，RVA
    // = FOA
    {
        *FOA = RVA;
        return true;
    }

    for (int i = 0; i < pNTHeader->FileHeader.NumberOfSections;
         i++) // 循环每一个节表
        if ((RVA >= pSectionHeader[i].VirtualAddress) &&
            (RVA < pSectionHeader[i].VirtualAddress +
             pSectionHeader[i].Misc.VirtualSize))
        {
            *FOA = pSectionHeader[i].PointerToRawData + RVA -
                   pSectionHeader[i].VirtualAddress;
            return true;
        }

    return false; // 如果一直没有找到，返回false
}

bool Utils::RVA_TO_FOA_64(
    PIMAGE_NT_HEADERS64 pNTHeader,
    PIMAGE_SECTION_HEADER pSectionHeader, IN DWORD RVA,
    OUT PDWORD FOA)
{
    if (RVA <
        pNTHeader->OptionalHeader
        .SizeOfHeaders) // 如果是在头部，在节之前，说明并不需要拉伸，RVA
    // = FOA
    {
        *FOA = RVA;
        return true;
    }

    for (int i = 0; i < pNTHeader->FileHeader.NumberOfSections;
         i++) // 循环每一个节表
        if ((RVA >= pSectionHeader[i].VirtualAddress) &&
            (RVA < pSectionHeader[i].VirtualAddress +
             pSectionHeader[i].Misc.VirtualSize))
        {
            *FOA = pSectionHeader[i].PointerToRawData + RVA -
                   pSectionHeader[i].VirtualAddress;
            return true;
        }

    return false; // 如果一直没有找到，返回false
}

bool Utils::FOA_TO_RVA(
    PIMAGE_NT_HEADERS32 pNTHeader,
    PIMAGE_SECTION_HEADER pSectionHeader, IN DWORD FOA,
    OUT PDWORD RVA)
{
    if ((FOA < pNTHeader->OptionalHeader.SizeOfHeaders) ||
        (pNTHeader->OptionalHeader.SectionAlignment ==
         pNTHeader->OptionalHeader.FileAlignment))
    {
        *RVA = FOA;
        return true;
    }

    for (int i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++)
    {
        if ((FOA >= pSectionHeader[i].PointerToRawData) &&
            (FOA < pSectionHeader[i].PointerToRawData +
             pSectionHeader[i].Misc.VirtualSize))
        {
            *RVA = pSectionHeader[i].VirtualAddress + FOA -
                   pSectionHeader[i].PointerToRawData;
            return true;
        }
    }
    return false;
}

bool Utils::FOA_TO_RVA_64(
    PIMAGE_NT_HEADERS64 pNTHeader,
    PIMAGE_SECTION_HEADER pSectionHeader, IN DWORD FOA,
    OUT PDWORD RVA)
{
    if ((FOA < pNTHeader->OptionalHeader.SizeOfHeaders) ||
        (pNTHeader->OptionalHeader.SectionAlignment ==
         pNTHeader->OptionalHeader.FileAlignment))
    {
        *RVA = FOA;
        return true;
    }

    for (int i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++)
    {
        if ((FOA >= pSectionHeader[i].PointerToRawData) &&
            (FOA < pSectionHeader[i].PointerToRawData +
             pSectionHeader[i].Misc.VirtualSize))
        {
            *RVA = pSectionHeader[i].VirtualAddress + FOA -
                   pSectionHeader[i].PointerToRawData;
            return true;
        }
    }
    return false;
}

DWORD Utils::Rva2Offset(DWORD dwRva, UINT_PTR uiBaseAddress)
{
    WORD wIndex = 0;
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    PIMAGE_NT_HEADERS     pNtHeaders = NULL;

    pNtHeaders = (PIMAGE_NT_HEADERS)(uiBaseAddress + ((PIMAGE_DOS_HEADER)uiBaseAddress)->e_lfanew);

    pSectionHeader = (PIMAGE_SECTION_HEADER)((UINT_PTR)(&pNtHeaders->OptionalHeader) + pNtHeaders->FileHeader.SizeOfOptionalHeader);

    if (dwRva < pSectionHeader[0].PointerToRawData) return dwRva;

    for (wIndex = 0; wIndex < pNtHeaders->FileHeader.NumberOfSections; wIndex++)
    {
        if ((dwRva >= pSectionHeader[wIndex].VirtualAddress) && (dwRva < (pSectionHeader[wIndex].VirtualAddress + pSectionHeader[wIndex].SizeOfRawData))) return dwRva - pSectionHeader[wIndex].VirtualAddress + pSectionHeader[wIndex].PointerToRawData;
    }

    return 0;
}

// ===============================================================================================//
DWORD Utils::GetReflectiveLoaderOffset(VOID *lpReflectiveDllBuffer)
{
    UINT_PTR uiBaseAddress = 0;
    UINT_PTR uiExportDir = 0;
    UINT_PTR uiNameArray = 0;
    UINT_PTR uiAddressArray = 0;
    UINT_PTR uiNameOrdinals = 0;
    DWORD    dwCounter = 0;

#ifdef WIN_X64
    DWORD dwCompiledArch = 2;
#else // ifdef WIN_X64

    // This will catch Win32 and WinRT.
    DWORD dwCompiledArch = 1;
#endif // ifdef WIN_X64

    uiBaseAddress = (UINT_PTR)lpReflectiveDllBuffer;

    // get the File Offset of the modules NT Header
    uiExportDir = uiBaseAddress + ((PIMAGE_DOS_HEADER)uiBaseAddress)->e_lfanew;

    // currenlty we can only process a PE file which is the same type as the one this fuction has
    // been compiled as, due to various offset in the PE structures being defined at compile time.
    if (((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.Magic == 0x010B) // PE32
    {
        if (dwCompiledArch != 1) return 0;
    }
    else if (((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.Magic == 0x020B) // PE64
    {
        if (dwCompiledArch != 2) return 0;
    }
    else
    {
        return 0;
    }

    // uiNameArray = the address of the modules export directory entry
    uiNameArray = (UINT_PTR)&((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

    // get the File Offset of the export directory
    uiExportDir = uiBaseAddress + Rva2Offset(((PIMAGE_DATA_DIRECTORY)uiNameArray)->VirtualAddress, uiBaseAddress);

    // get the File Offset for the array of name pointers
    uiNameArray = uiBaseAddress + Rva2Offset(((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfNames, uiBaseAddress);

    // get the File Offset for the array of addresses
    uiAddressArray = uiBaseAddress + Rva2Offset(((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfFunctions, uiBaseAddress);

    // get the File Offset for the array of name ordinals
    uiNameOrdinals = uiBaseAddress + Rva2Offset(((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfNameOrdinals, uiBaseAddress);

    // get a counter for the number of exported functions...
    dwCounter = ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->NumberOfNames;

    // loop through all the exported functions to find the ReflectiveLoader
    while (dwCounter--)
    {
        char *cpExportedFunctionName = (char *)(uiBaseAddress + Rva2Offset(DEREF_32(uiNameArray), uiBaseAddress));

        if (strstr(cpExportedFunctionName, "ReflectiveLoader") != NULL)
        {
            // get the File Offset for the array of addresses
            uiAddressArray = uiBaseAddress + Rva2Offset(((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfFunctions, uiBaseAddress);

            // use the functions name ordinal as an index into the array of name pointers
            uiAddressArray += (DEREF_16(uiNameOrdinals) * sizeof(DWORD));

            // return the File Offset to the ReflectiveLoader() functions code...
            return Rva2Offset(DEREF_32(uiAddressArray), uiBaseAddress);
        }

        // get the next exported function name
        uiNameArray += sizeof(DWORD);

        // get the next exported function name ordinal
        uiNameOrdinals += sizeof(WORD);
    }

    return 0;
}

// ===============================================================================================//
// Loads a PE image from memory into the address space of a host process via the image's exported ReflectiveLoader function
// Note: You must compile whatever you are injecting with REFLECTIVEDLLINJECTION_VIA_LOADREMOTELIBRARYR
//       defined in order to use the correct RDI prototypes.
// Note: The hProcess handle must have these access rights: PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
//       PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ
// Note: If you are passing in an lpParameter value, if it is a pointer, remember it is for a different address space.
// Note: This function currently cant inject accross architectures, but only to architectures which are the
//       same as the arch this function is compiled as, e.g. x86->x86 and x64->x64 but not x64->x86 or x86->x64.
HANDLE WINAPI Utils::LoadRemoteLibraryR(HANDLE hProcess, LPVOID lpBuffer, DWORD dwLength, LPVOID lpParameter)
{
    BOOL   bSuccess = FALSE;
    LPVOID lpRemoteLibraryBuffer = NULL;
    LPTHREAD_START_ROUTINE lpReflectiveLoader = NULL;
    HANDLE hThread = NULL;
    DWORD  dwReflectiveLoaderOffset = 0;
    DWORD  dwThreadId = 0;
    PRTL_CREATE_USER_THREAD RtlCreateUserThread = NULL;

    __try
    {
        do
        {
            if (!hProcess || !lpBuffer || !dwLength) break;

            // check if the library has a ReflectiveLoader...
            dwReflectiveLoaderOffset = GetReflectiveLoaderOffset(lpBuffer);

            if (!dwReflectiveLoaderOffset)
            {
                // OutputDebugString("GetReflectiveLoaderOffset FAILED!");
                wprintf(TEXT("GetReflectiveLoaderOffset FAILED!"));
                break;
            }

            // alloc memory (RWX) in the host process for the image...
            lpRemoteLibraryBuffer = VirtualAllocEx(hProcess, NULL, dwLength, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

            if (!lpRemoteLibraryBuffer)
            {
                // OutputDebugString("VirtualAllocEx FAILED!");
                wprintf(TEXT("VirtualAllocEx FAILED!"));
                break;
            }

            // write the image into the host process...
            if (!WriteProcessMemory(hProcess, lpRemoteLibraryBuffer, lpBuffer, dwLength, NULL))
            {
                // OutputDebugString("WriteProcessMemory FAILED!");
                wprintf(TEXT("WriteProcessMemory FAILED!"));
                break;
            }

            // add the offset to ReflectiveLoader() to the remote library address...
            lpReflectiveLoader = (LPTHREAD_START_ROUTINE)((ULONG_PTR)lpRemoteLibraryBuffer + dwReflectiveLoaderOffset);

            // create a remote thread in the host process to call the ReflectiveLoader!
            // OutputDebugString("INJECTING DLL!");
            RtlCreateUserThread = (PRTL_CREATE_USER_THREAD)(GetProcAddress(GetModuleHandle(TEXT("ntdll")), "RtlCreateUserThread"));
            RtlCreateUserThread(hProcess, NULL, 0, 0, 0, 0, lpReflectiveLoader, lpParameter, &hThread, NULL);

            if (hThread == NULL)
            {
                // OutputDebugString("Injection FAILED!");
                wprintf(TEXT("Injection FAILED!"));
                break;
            }

            WaitForSingleObject(hThread, INFINITE);

            VirtualFreeEx(hProcess, lpRemoteLibraryBuffer, dwLength, MEM_RELEASE);
        } while (0);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        hThread = NULL;
    }
    return hThread;
}
