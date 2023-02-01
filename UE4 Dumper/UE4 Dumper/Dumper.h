#pragma once

#include "PropertyFlags.h"

/******************** GNames Dumping ********************/
void DumpBlocks(ofstream& Dump, UINT32& count, uintptr_t FNamePool, UINT32 blockId, UINT32 blockSize) {
    uintptr_t It = Read<uintptr_t>(FNamePool + Offsets::FNamePoolToBlocks + (blockId * Offsets::PointerSize));
    uintptr_t End = It + blockSize - Offsets::FNameEntryToString;
    UINT32 Block = blockId;
    UINT16 Offset = 0;
    while (It < End) {
        uintptr_t FNameEntry = It;
        INT16 FNameEntryHeader = Read<INT16>(FNameEntry);
        int StrLength = FNameEntryHeader >> Offsets::FNameEntryToLenBit;
        if (StrLength) {
            bool wide = FNameEntryHeader & 1;

            ///Unicode Dumping Not Supported
            if (StrLength > 0) {
                //String Length Limit
                if (StrLength < 250) {
                    UINT32 key = (Block << 16 | Offset);
                    uintptr_t StrPtr = FNameEntry + Offsets::FNameEntryToString;

                    string Names = ReadStringNew(StrPtr, StrLength);

                    if (isDecrypt) {
                        //Dump << (wide ? "Wide" : "") << dec << "{" << StrLength << "} " << "[" << key << "]: " << XorCypher(Names) << endl;
                        Dump << "[" << count << "]: " << XorCypher(Names) << endl;
                    } else {
                        //Dump << (wide ? "Wide" : "") << dec << "{" << StrLength << "} " << "[" << key << "]: " << Names << endl;
                        Dump << "[" << count << "]: " << Names << endl;
                    }
                    count++;
                }
            }
            else {
                StrLength = -StrLength;//Negative lengths are for Unicode Characters
            }

            //Next
            Offset += StrLength / Offsets::FNameStride;
            UINT16 bytes = Offsets::FNameEntryToString + StrLength * (wide ? sizeof(wchar_t) : sizeof(char));
            It += (bytes + Offsets::FNameStride - 1u) & ~(Offsets::FNameStride - 1u);
        }
        else {// Null-terminator entry found
            break;
        }
    }
}

void DumpStrings(string out) {
    UINT32 count = 0;
    ofstream Dump(out + "/NameDump.txt", ofstream::out);
    if (Dump.is_open()) {
        if (isNew) {
            uintptr_t FNamePool = (Offsets::ModuleBase + Offsets::GNames) + Offsets::GNamesToFNamePool;

            UINT32 BlockSize = Offsets::FNameStride * 65536;
            UINT32 CurrentBlock = Read<UINT32>(FNamePool + Offsets::FNamePoolToCurrentBlock);
            UINT32 CurrentByteCursor = Read<UINT32>(FNamePool + Offsets::FNamePoolToCurrentByteCursor);

            //All Blocks Except Current
            for (UINT32 BlockIdx = 0; BlockIdx < CurrentBlock; ++BlockIdx) {
                DumpBlocks(Dump, count, FNamePool, BlockIdx, BlockSize);
            }

            //Last Block
            DumpBlocks(Dump, count, FNamePool, CurrentBlock, CurrentByteCursor);
        }
        else {
            for (UINT32 i = 0; i < DumpLimit; i++) {
                string s = GetFNameFromID(i);
                if (!s.empty()) {
                    Dump << "[" << i << "]: " << s << endl;
                    count++;
                }
            }
        }
        Dump.close();
    }
    cout << "Strings Dumped" << endl;
}

void DumpObjects(string out) {
    ofstream Dump(out + "/ObjectsDump.txt", ofstream::out);
    UINT32 Count = 0;
    if (Dump.is_open()) {
        INT32 ObjectsCount = GetObjectCount();
        if (ObjectsCount < 10 || ObjectsCount > 999999) {
            ObjectsCount = DumpLimit;
        }
        for (INT32 i = 0; i < ObjectsCount; i++) {
            uintptr_t GUObjectArray = GetUObjectFromID(i);
            if (UObject::isValid(GUObjectArray)) {
                Dump << setbase(16) << "[0x" << i << "]:" << endl;
                Dump << "Name: " << UObject::getName(GUObjectArray).c_str() << endl;
                Dump << "Class: " << UObject::getClassName(GUObjectArray).c_str() << endl;
                Dump << "ObjectPtr: 0x" << setbase(16) << GUObjectArray << endl;
                Dump << "ClassPtr: 0x" << setbase(16) << UObject::getClass(GUObjectArray) << endl;
                Dump << endl;
                Count++;
            }
        }
        Dump.close();
    }
    cout << "Objects Dumped" << endl;
}

void DumpSDK(string out) {
    ofstream Dump(out + "/SDK.txt", ofstream::out);
    if (Dump.is_open()) {

        INT32 ObjectsCount = GetObjectCount();
        if (ObjectsCount < 10 || ObjectsCount > 999999) {
            ObjectsCount = DumpLimit;
        }
        for (INT32 i = 0; i < ObjectsCount; i++) {
            uintptr_t GUObjectArray = GetUObjectFromID(i);
            if (UObject::isValid(GUObjectArray)) {
                DumpStructures(Dump, UObject::getClass(GUObjectArray));
            }
        }
        Dump.close();
    }
    cout << "SDK Dumped" << endl;
}

void DumpSDKW(string out) {
    ofstream Dump(out + "/SDKW.txt", ofstream::out);
    if (Dump.is_open()) {

        uintptr_t GWorld = Read<uintptr_t>(Offsets::ModuleBase + Offsets::GWorld);

        if (UObject::isValid(GWorld)) {
            //Iterate World
            DumpStructures(Dump, UObject::getClass(GWorld));
            //Iterate Entities
            uintptr_t PersistentLevel = Read<uintptr_t>(GWorld + Offsets::UWorldToPersistentLevel);
            uintptr_t AActors = Read<uintptr_t>(PersistentLevel + Offsets::ULevelToAActors);
            int AActorsCount = Read<int>(PersistentLevel + Offsets::ULevelToAActorsCount);

            for (int i = 0; i < AActorsCount; i++) {
                uintptr_t Base = Read<uintptr_t>(AActors + (i * Offsets::PointerSize));
                if (UObject::isValid(Base)) {
                    DumpStructures(Dump, UObject::getClass(Base));
                }
            }
        }
        Dump.close();
    }
    cout << "SDKW Dumped" << endl;
}

void DumpActors(string out) {
    ofstream Dump(out + "/ActorsDump.txt", ofstream::out);
    UINT32 count = 0;
    if (Dump.is_open()) {
        uintptr_t GWorld = Read<uintptr_t>(Offsets::ModuleBase + Offsets::GWorld);
        if (UObject::isValid(GWorld)) {
            uintptr_t PersistentLevel = Read<uintptr_t>(GWorld + Offsets::UWorldToPersistentLevel);
            uintptr_t AActors = Read<uintptr_t>(PersistentLevel + Offsets::ULevelToAActors);
            int AActorsCount = Read<int>(PersistentLevel + Offsets::ULevelToAActorsCount);

            for (int i = 0; i < AActorsCount; i++) {
                uintptr_t Base = Read<uintptr_t>(AActors + (i * Offsets::PointerSize));
                if (UObject::isValid(Base)) {
                    Dump << "[" << count << "]: Name - " << UObject::getName(Base) << ", Address - 0x" << hex << Base << endl;
                    count++;
                }
            }
        }
        cout << "Actors Dumped" << endl;
        Dump.close();
    }

}