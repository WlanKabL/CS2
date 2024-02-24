#pragma once
#include <Windows.h>


template <typename ReadType>
BOOL ReadMemoryInternal(DWORD64 Address, ReadType& Value, size_t Size, bool Weapon = false)
{
    if (Address == 0 || Address < 0xAA70AA8 && Address > 0x7FFD5DC070000)
        return false;

    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(reinterpret_cast<LPCVOID>(Address), &mbi, sizeof(mbi)) == 0)
        return false;


    if (mbi.State != MEM_COMMIT || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
        return false;


    if (Size > (static_cast<size_t>(mbi.RegionSize) - (static_cast<size_t>(reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(Address))) - static_cast<size_t>(reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(mbi.BaseAddress))))))
        return false;

    memcpy(&Value, reinterpret_cast<void*>(Address), Size);

    return true;
}

bool HexStringToBytes(const std::string& hex, std::vector<uint8_t>& bytes) 
{
    std::stringstream ss(hex);
    std::string item;
    while (std::getline(ss, item, ' ')) 
    {
        if (item == "??")
            bytes.push_back(0x00);

        else 
        {
            unsigned int byte;
            std::stringstream itemStream;
            itemStream << std::hex << item;
            itemStream >> byte;
            if (itemStream.fail()) {
                return false;
            }
            bytes.push_back(static_cast<uint8_t>(byte));
        }
    }
    return true;
}

uintptr_t PatternScan(uintptr_t StartRange, uintptr_t EndRange, const std::string& Pattern, bool ExtractAddress, bool ExtractOffset, int Bytes) 

{
    std::vector<uint8_t> bytes;
    if (!HexStringToBytes(Pattern, bytes))
        return 0; 


    uintptr_t Start = StartRange;
    uintptr_t End = EndRange - bytes.size();

    for (uintptr_t Current = Start; Current <= End; ++Current)
    {
        bool Found = true;
        for (size_t i = 0; i < bytes.size(); ++i) {
            if (bytes[i] != 0x00 && *reinterpret_cast<uint8_t*>(Current + i) != bytes[i])
            {
                Found = false;
                break;
            }
        }

        if (Found)
        {
            if (ExtractAddress)
            {
                int32_t RelativeOffset = *reinterpret_cast<int32_t*>(Current + Bytes);
                uintptr_t AbsoluteAddress = Current + 7 + RelativeOffset;
                return AbsoluteAddress;
            }

            if (ExtractOffset)
            {
                int32_t RelativeOffset = *reinterpret_cast<int32_t*>(Current + Bytes);
                return RelativeOffset;
            }

            return Current;
          
        }
    }

    return 0;
}

void** VirtualTableCopy(void** original)
{
    int vtableSize = 8192;
    void** newVTable = new void* [vtableSize];

    for (int i = 0; i < vtableSize; ++i)
        newVTable[i] = original[i];

    return newVTable;
}
