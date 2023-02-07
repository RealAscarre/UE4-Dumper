#pragma once

//MenuUI Booleans
bool isNew = false, isDecrypt = false;

UINT32 MAX_SIZE = 100;
UINT32 ClassCount = 0;
UINT32 DumpLimit = 30000;

namespace Offsets {

    const char* GameName;
    const char* TargetProcess;

    DWORD ProcessID;
    uintptr_t ModuleBase;
    HANDLE Handle;

    uintptr_t PointerSize;

    uintptr_t GWorld;
    uintptr_t GNames;
    uintptr_t GUObjectArray;

    //TNameEntyry
    uintptr_t FNameEntryToNameString;

    //Class: FNamePool
    uintptr_t FNameStride;
    uintptr_t GNamesToFNamePool;
    uintptr_t FNamePoolToCurrentBlock;
    uintptr_t FNamePoolToCurrentByteCursor;
    uintptr_t FNamePoolToBlocks;

    //Class: FNameEntry
    uintptr_t FNameEntryToLenBit;
    uintptr_t FNameEntryToString;

    //Class: FUObjectArray
    uintptr_t FUObjectArrayToTUObjectArray;
    uintptr_t FUObjectItemPadd;
    uintptr_t FUObjectItemSize;

    //Class: TUObjectArray
    uintptr_t TUObjectArrayToNumElements;

    //Class: UObject
    uintptr_t UObjectToInternalIndex;
    uintptr_t UObjectToClassPrivate;
    uintptr_t UObjectToFNameIndex;
    uintptr_t UObjectToOuterPrivate;

    //Class: FField
    uintptr_t FFieldToClass;
    uintptr_t FFieldToNext;
    uintptr_t FFieldToName;

    //Class: UField
    uintptr_t UFieldToNext;

    //Class: UStruct
    uintptr_t UStructToSuperStruct;
    uintptr_t UStructToChildren;
    uintptr_t UStructToChildProperties;

    //Class: UFunction
    uintptr_t UFunctionToFunctionFlags;
    uintptr_t UFunctionToFunc;

    //Class: UProperty
    uintptr_t UPropertyToElementSize;
    uintptr_t UPropertyToPropertyFlags;
    uintptr_t UPropertyToOffsetInternal;

    //Class: UBoolProperty
    uintptr_t UBoolPropertyToFieldSize;
    uintptr_t UBoolPropertyToByteOffset;
    uintptr_t UBoolPropertyToByteMask;
    uintptr_t UBoolPropertyToFieldMask;

    //Class: UObjectProperty
    uintptr_t UObjectPropertyToPropertyClass;

    //Class: UClassProperty
    uintptr_t UClassPropertyToMetaClass;

    //Class: UInterfaceProperty
    uintptr_t UInterfacePropertyToInterfaceClass;

    //Class: UArrayProperty
    uintptr_t UArrayPropertyToInnerProperty;

    //Class: UMapProperty
    uintptr_t UMapPropertyToKeyProp;
    uintptr_t UMapPropertyToValueProp;

    //Class: USetProperty
    uintptr_t USetPropertyToElementProp;

    //Class: UStructProperty
    uintptr_t UStructPropertyToStruct;

    //Class: UWorld
    uintptr_t UWorldToPersistentLevel;

    //Class: ULevel
    uintptr_t ULevelToAActors;
    uintptr_t ULevelToAActorsCount;

    void NewUEVersion() {
        isNew = true;//True = UE4 Version >= 4.23

        GameName = "Game_Name_Here";
        TargetProcess = "Executable_File_Name.exe";

        PointerSize = 0x8;//0x8 = 64 Bit and 0x4 = 32 Bit

        GWorld = 0x0;
        GNames = 0x0;
        GUObjectArray = 0x0;

        //Class: FNamePool
        FNameStride = 0x2;
        GNamesToFNamePool = 0x0;
        FNamePoolToCurrentBlock = 0x8;
        FNamePoolToCurrentByteCursor = 0xC;
        FNamePoolToBlocks = 0x10;

        //Class: FNameEntry
        FNameEntryToLenBit = 0x6;
        FNameEntryToString = 0x2;

        //Class: FUObjectArray
        FUObjectArrayToTUObjectArray = 0x0;//Sometimes 0x10
        FUObjectItemPadd = 0x0;
        FUObjectItemSize = 0x18;

        //Class: TUObjectArray
        TUObjectArrayToNumElements = 0x14;

        //Class: UObject
        UObjectToInternalIndex = 0xC;
        UObjectToClassPrivate = 0x10;
        UObjectToFNameIndex = 0x18;
        UObjectToOuterPrivate = 0x20;

        //Class: FField
        FFieldToClass = 0x8;
        FFieldToNext = 0x20;
        FFieldToName = FFieldToNext + PointerSize;

        //Class: UField
        UFieldToNext = 0x28;

        //Class: UStruct
        UStructToSuperStruct = 0x40;
        UStructToChildren = UStructToSuperStruct + PointerSize;
        UStructToChildProperties = 0x50;

        //Class: UFunction
        UFunctionToFunctionFlags = 0xB0;
        UFunctionToFunc = UFunctionToFunctionFlags + UFieldToNext;

        //Class: UProperty
        UPropertyToElementSize = 0x3C;
        UPropertyToPropertyFlags = 0x40;
        UPropertyToOffsetInternal = 0x4C;

        //Class: UBoolProperty
        UBoolPropertyToFieldSize = 0x78;
        UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
        UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
        UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

        //Class: UObjectProperty
        UObjectPropertyToPropertyClass = 0x78;

        //Class: UClassProperty
        UClassPropertyToMetaClass = 0x80;

        //Class: UInterfaceProperty
        UInterfacePropertyToInterfaceClass = 0x80;

        //Class: UArrayProperty
        UArrayPropertyToInnerProperty = 0x78;

        //Class: UMapProperty
        UMapPropertyToKeyProp = 0x78;
        UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

        //Class: USetProperty
        USetPropertyToElementProp = 0x78;

        //Class: UStructProperty
        UStructPropertyToStruct = 0x78;

        //Class: UWorld
        UWorldToPersistentLevel = 0x30;

        //Class: ULevel
        ULevelToAActors = 0x98;
        ULevelToAActorsCount = ULevelToAActors + PointerSize;
    }

    void OldUEVersion() {
        isNew = false;//False = UE4 Version < 4.23 (Till 4.22)

        GameName = "Game_Name_Here";
        TargetProcess = "Executable_File_Name.exe";

        PointerSize = 0x8;//0x8 = 64 Bit and 0x4 = 32 Bit

        //Global
        GWorld = 0x0;
        GNames = 0x0;
        GUObjectArray = 0x0;

        //FNameEntry
        FNameEntryToNameString = 0x10;

        //Class: FUObjectArray
        FUObjectArrayToTUObjectArray = 0x10;

        //Class: TUObjectArray
        TUObjectArrayToNumElements = 0x0;//Sometimes 0xC
        FUObjectItemPadd = 0x0;
        FUObjectItemSize = 0x18;

        //Class: UObject
        UObjectToInternalIndex = 0xC;
        UObjectToClassPrivate = 0x10;
        UObjectToFNameIndex = 0x18;
        UObjectToOuterPrivate = 0x20;

        //Class: UField
        UFieldToNext = 0x28;

        //Class: UStruct
        UStructToSuperStruct = 0x30;
        UStructToChildren = UStructToSuperStruct + PointerSize;

        //Class: UFunction
        UFunctionToFunctionFlags = 0x88;
        UFunctionToFunc = UFunctionToFunctionFlags + UFieldToNext;

        //Class: UProperty
        UPropertyToElementSize = 0x34;
        UPropertyToPropertyFlags = 0x38;
        UPropertyToOffsetInternal = 0x50;

        //Class: UBoolProperty
        UBoolPropertyToFieldSize = 0x70;
        UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
        UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
        UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

        //Class: UObjectProperty
        UObjectPropertyToPropertyClass = 0x78;

        //Class: UClassProperty
        UClassPropertyToMetaClass = 0x78;

        //Class: UInterfaceProperty
        UInterfacePropertyToInterfaceClass = 0x78;

        //Class: UArrayProperty
        UArrayPropertyToInnerProperty = 0x78;

        //Class: UMapProperty
        UMapPropertyToKeyProp = 0x78;
        UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

        //Class: USetProperty
        USetPropertyToElementProp = 0x78;

        //Class: UStructProperty
        UStructPropertyToStruct = 0x78;

        //Class: UWorld
        UWorldToPersistentLevel = 0x30;

        //Class: ULevel
        ULevelToAActors = 0x98;
        ULevelToAActorsCount = ULevelToAActors + PointerSize;
    }
}
