#pragma once

#include "Memory.h"

/******************** GNames Decryption ********************/
string XorCypher(string Input) {
    int key = Input.size();
    string Output = Input;

    for (int i = 0; i < Input.size(); i++)
        Output[i] = Input[i] ^ key;

    return Output;
}

string GetFNameFromID(UINT32 index) {
    if (isNew) {
        UINT32 Block = index >> 16;
        UINT16 Offset = index & 65535;

        uintptr_t FNamePool = ((Offsets::ModuleBase + Offsets::GNames) + Offsets::GNamesToFNamePool);

        uintptr_t NamePoolChunk = Read<uintptr_t>(FNamePool + Offsets::FNamePoolToBlocks + (Block * Offsets::PointerSize));
        uintptr_t FNameEntry = NamePoolChunk + (Offsets::FNameStride * Offset);

        INT16 FNameEntryHeader = Read<INT16>(FNameEntry);
        uintptr_t StrPtr = FNameEntry + Offsets::FNameEntryToString;
        int StrLength = FNameEntryHeader >> Offsets::FNameEntryToLenBit;

        ///Unicode Dumping Not Supported Yet
        if (StrLength > 0 && StrLength < 250) {
            if (isDecrypt) {
                return XorCypher(ReadStringNew(StrPtr, StrLength));
            } else {
                return ReadStringNew(StrPtr, StrLength);
            }
        }
        else {
            return "None";
        }
    } else {
        uintptr_t TNameEntryArray = Read<uintptr_t>(Offsets::ModuleBase + Offsets::GNames);
        uintptr_t FNameEntryArr = Read<uintptr_t>(TNameEntryArray + ((index / 0x4000) * Offsets::PointerSize));
        uintptr_t FNameEntry = Read<uintptr_t>(FNameEntryArr + ((index % 0x4000) * Offsets::PointerSize));
        
        if (isDecrypt) {
            return XorCypher(ReadString(FNameEntry + Offsets::FNameEntryToNameString, MAX_SIZE));
        } else {
            return ReadString(FNameEntry + Offsets::FNameEntryToNameString, MAX_SIZE);
        }
    }
}

/******************** GUobject Decryption ********************/
INT32 GetObjectCount() {
    return Read<INT32>((Offsets::ModuleBase + Offsets::GUObjectArray) + Offsets::FUObjectArrayToTUObjectArray + Offsets::TUObjectArrayToNumElements);
}

uintptr_t GetUObjectFromID(UINT32 index) {
    uintptr_t FUObjectArray = (Offsets::ModuleBase + Offsets::GUObjectArray);
    if (isNew) {
        uintptr_t TUObjectArray = Read<uintptr_t>(FUObjectArray + Offsets::FUObjectArrayToTUObjectArray);
        uintptr_t Chunk = Read<uintptr_t>(TUObjectArray + ((index / 0x10000) * Offsets::PointerSize));
        return Read<uintptr_t>(Chunk + Offsets::FUObjectItemPadd + ((index % 0x10000) * Offsets::FUObjectItemSize));
    } else {
        uintptr_t TUObjectArray = Read<uintptr_t>(FUObjectArray + Offsets::FUObjectArrayToTUObjectArray);
        return Read<uintptr_t>(TUObjectArray + (index * Offsets::FUObjectItemSize));
    }
}

/******************** Unreal Engine GUObject Structure ********************/
struct UObject {
    static INT32 getIndex(uintptr_t object) {
        return Read<INT32>(object + Offsets::UObjectToInternalIndex);
    }

    static uintptr_t getClass(uintptr_t object) {//UClass*
        return Read<uintptr_t>(object + Offsets::UObjectToClassPrivate);
    }

    static UINT32 getNameID(uintptr_t object) {
        return Read<UINT32>(object + Offsets::UObjectToFNameIndex);
    }

    static uintptr_t getOuter(uintptr_t object) {//UObject*
        return Read<uintptr_t>(object + Offsets::UObjectToOuterPrivate);
    }

    static string getName(uintptr_t object) {
        return GetFNameFromID(getNameID(object));
    }

    static string getClassName(uintptr_t object) {
        return getName(getClass(object));
    }

    static bool isValid(uintptr_t object) {
        return (object > 0 && getNameID(object) > 0 && getClass(object) > 0);
    }
};

/******************** Unreal Engine UField Structure ********************/
struct UField {
    static uintptr_t getNext(uintptr_t field) {//UField*
        return Read<uintptr_t>(field + Offsets::UFieldToNext);
    }
};

/******************** Unreal Engine FField Structure ********************/
struct FField {
    static string getName(uintptr_t fField) {
        return GetFNameFromID(Read<UINT32>(fField + Offsets::FFieldToName));
    }

    static string getClassName(uintptr_t fField) {
        return GetFNameFromID(Read<UINT32>(Read<uintptr_t>(fField + Offsets::FFieldToClass)));
    }

    static uintptr_t getNext(uintptr_t fField) {//UField*
        return Read<uintptr_t>(fField + Offsets::FFieldToNext);
    }
};

/******************** Unreal Engine UStruct Structure ********************/
struct UStruct {
    static uintptr_t getSuperClass(uintptr_t structz) {//UStruct*
        return Read<uintptr_t>(structz + Offsets::UStructToSuperStruct);
    }

    static uintptr_t getChildren(uintptr_t structz) {//UField*
        return Read<uintptr_t>(structz + Offsets::UStructToChildren);
    }

    static uintptr_t getChildProperties(uintptr_t structz) {//UField*
        return Read<uintptr_t>(structz + Offsets::UStructToChildProperties);
    }

    static string getClassName(uintptr_t clazz) {
        return UObject::getName(clazz);
    }

    static string getClassPath(uintptr_t object) {
        uintptr_t clazz = UObject::getClass(object);
        string classname = UObject::getName(clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }

    static string getStructClassPath(uintptr_t clazz) {
        string classname = UObject::getName(clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }
};

/******************** Unreal Engine UFunction Structure ********************/
struct UFunction {
    static INT32 getFunctionFlags(uintptr_t func) {
        return Read<INT32>(func + Offsets::UFunctionToFunctionFlags);
    }

    static uintptr_t getFunc(uintptr_t func) {
        return Read<uintptr_t>(func + Offsets::UFunctionToFunc);
    }
};

/******************** Unreal Engine UProperty Structure ********************/
struct UProperty {
    static INT32 getElementSize(uintptr_t prop) {
        return Read<INT32>(prop + Offsets::UPropertyToElementSize);
    }

    static UINT64 getPropertyFlags(uintptr_t prop) {
        return Read<UINT64>(prop + Offsets::UPropertyToPropertyFlags);
    }

    static INT32 getOffset(uintptr_t prop) {
        return Read<INT32>(prop + Offsets::UPropertyToOffsetInternal);
    }
};

/******************** Unreal Engine UBoolProperty Structure ********************/
struct UBoolProperty {
    static UINT8 getFieldSize(uintptr_t prop) {
        return Read<UINT8>(prop + Offsets::UBoolPropertyToFieldSize);
    }

    static UINT8 getByteOffset(uintptr_t prop) {
        return Read<UINT8>(prop + Offsets::UBoolPropertyToByteOffset);
    }

    static UINT8 getByteMask(uintptr_t prop) {
        return Read<UINT8>(prop + Offsets::UBoolPropertyToByteMask);
    }

    static UINT8 getFieldMask(uintptr_t prop) {
        return Read<UINT8>(prop + Offsets::UBoolPropertyToFieldMask);
    }
};

/******************** Unreal Engine UObjectProperty Structure ********************/
struct UObjectProperty {
    static uintptr_t getPropertyClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::UObjectPropertyToPropertyClass);
    }
};

/******************** Unreal Engine UClassProperty Structure ********************/
struct UClassProperty {
    static uintptr_t getMetaClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::UClassPropertyToMetaClass);
    }
};

/******************** Unreal Engine UInterfaceProperty Structure ********************/
struct UInterfaceProperty {
    static uintptr_t getInterfaceClass(uintptr_t prop) {//class UClass*
        return Read<uintptr_t>(prop + Offsets::UInterfacePropertyToInterfaceClass);
    }
};

/******************** Unreal Engine UArrayProperty Structure ********************/
struct UArrayProperty {
    static uintptr_t getInner(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::UArrayPropertyToInnerProperty);
    }
};

/******************** Unreal Engine UMapProperty Structure ********************/
struct UMapProperty {
    static uintptr_t getKeyProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::UMapPropertyToKeyProp);
    }

    static uintptr_t getValueProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::UMapPropertyToValueProp);
    }
};

/******************** Unreal Engine USetProperty Structure ********************/
struct USetProperty {
    static uintptr_t getElementProp(uintptr_t prop) {//UProperty*
        return Read<uintptr_t>(prop + Offsets::USetPropertyToElementProp);
    }
};

/******************** Unreal Engine UStructProperty Structure ********************/
struct UStructProperty {
    static uintptr_t getStruct(uintptr_t prop) {//UStruct*
        return Read<uintptr_t>(prop + Offsets::UStructPropertyToStruct);
    }
};