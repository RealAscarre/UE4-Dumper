#pragma once

#include <list>
#include <vector>
#include <iomanip>
#include <fstream>

#include "Structures.h"

vector<UINT32> structIDMap;

/******************** Functions ********************/
bool isStartWith(string str, const char* check) {
    return (str.rfind(check, 0) == 0);
}

bool isEqual(string s1, const char* check) {
    string s2(check);
    return (s1 == s2);
}

bool isScanned(UINT32 id) {
    for (int i = 0; i < structIDMap.size(); i++) {
        if (structIDMap[i] == id) { return true; }
    }
    return false;
}

bool isContain(string str, string check) {
    size_t found = str.find(check);
    return (found != string::npos);
}

/******************** Define Property Types ********************/
string ResolveProperty(list<uintptr_t>& recurrce, uintptr_t prop) {
    if (prop) {
        string cname = UObject::getClassName(prop);

        if (isEqual(cname, "ObjectProperty") || isEqual(cname, "WeakObjectProperty")
            || isEqual(cname, "LazyObjectProperty") || isEqual(cname, "AssetObjectProperty")
            || isEqual(cname, "SoftObjectProperty")) {
            uintptr_t propertyClass = UObjectProperty::getPropertyClass(prop);
            recurrce.push_back(propertyClass);
            return UObject::getName(propertyClass) + "*";
        }
        else if (isEqual(cname, "ClassProperty") || isEqual(cname, "AssetClassProperty") ||
            isEqual(cname, "SoftClassProperty")) {
            uintptr_t metaClass = UClassProperty::getMetaClass(prop);
            recurrce.push_back(metaClass);
            return "class " + UObject::getName(metaClass);
        }
        else if (isEqual(cname, "InterfaceProperty")) {
            uintptr_t interfaceClass = UInterfaceProperty::getInterfaceClass(prop);
            recurrce.push_back(interfaceClass);
            return "interface class" + UObject::getName(interfaceClass);
        }
        else if (isEqual(cname, "StructProperty")) {
            uintptr_t Struct = UStructProperty::getStruct(prop);
            recurrce.push_back(Struct);
            return UObject::getName(Struct);
        }
        else if (isEqual(cname, "ArrayProperty")) {
            return ResolveProperty(recurrce, UArrayProperty::getInner(prop)) + "[]";
        }
        else if (isEqual(cname, "SetProperty")) {
            return "<" + ResolveProperty(recurrce, USetProperty::getElementProp(prop)) + ">";
        }
        else if (isEqual(cname, "MapProperty")) {
            return "<" + ResolveProperty(recurrce, UMapProperty::getKeyProp(prop)) + "," +
                ResolveProperty(recurrce, UMapProperty::getValueProp(prop)) + ">";
        }
        else if (isEqual(cname, "BoolProperty")) {
            return "bool";
        }
        else if (isEqual(cname, "ByteProperty")) {
            return "byte";
        }
        else if (isEqual(cname, "IntProperty")) {
            return "int";
        }
        else if (isEqual(cname, "Int8Property")) {
            return "int8";
        }
        else if (isEqual(cname, "Int16Property")) {
            return "int16";
        }
        else if (isEqual(cname, "Int64Property")) {
            return "int64";
        }
        else if (isEqual(cname, "UInt16Property")) {
            return "uint16";
        }
        else if (isEqual(cname, "UINT32Property")) {
            return "UINT32";
        }
        else if (isEqual(cname, "UINT64Property")) {
            return "UINT64";
        }
        else if (isEqual(cname, "DoubleProperty")) {
            return "double";
        }
        else if (isEqual(cname, "FloatProperty")) {
            return "float";
        }
        else if (isEqual(cname, "EnumProperty")) {
            return "enum";
        }
        else if (isEqual(cname, "StrProperty")) {
            return "FString";
        }
        else if (isEqual(cname, "TextProperty")) {
            return "FText";
        }
        else if (isEqual(cname, "NameProperty")) {
            return "FName";
        }
        else if (isEqual(cname, "DelegateProperty") ||
            isEqual(cname, "MulticastDelegateProperty")) {
            return "delegate";
        }
        else {
            return UObject::getName(prop) + "(" + cname + ")";
        }
    }
    return "NULL";
}

string ResolvePropertyNew(list<uintptr_t>& recurrce, uintptr_t prop) {
    if (prop) {
        string cname = FField::getClassName(prop);

        if (isEqual(cname, "ObjectProperty") || isEqual(cname, "WeakObjectProperty")
            || isEqual(cname, "LazyObjectProperty") || isEqual(cname, "AssetObjectProperty")
            || isEqual(cname, "SoftObjectProperty")) {
            uintptr_t propertyClass = UObjectProperty::getPropertyClass(prop);
            recurrce.push_back(propertyClass);
            return UObject::getName(propertyClass) + "*";
        }
        else if (isEqual(cname, "ClassProperty") || isEqual(cname, "AssetClassProperty") ||
            isEqual(cname, "SoftClassProperty")) {
            uintptr_t metaClass = UClassProperty::getMetaClass(prop);
            recurrce.push_back(metaClass);
            return "class " + UObject::getName(metaClass);
        }
        else if (isEqual(cname, "InterfaceProperty")) {
            uintptr_t interfaceClass = UInterfaceProperty::getInterfaceClass(prop);
            recurrce.push_back(interfaceClass);
            return "interface class" + UObject::getName(interfaceClass);
        }
        else if (isEqual(cname, "StructProperty")) {
            uintptr_t Struct = UStructProperty::getStruct(prop);
            recurrce.push_back(Struct);
            return UObject::getName(Struct);
        }
        else if (isEqual(cname, "ArrayProperty")) {
            return ResolvePropertyNew(recurrce, UArrayProperty::getInner(prop)) + "[]";
        }
        else if (isEqual(cname, "SetProperty")) {
            return "<" + ResolvePropertyNew(recurrce, USetProperty::getElementProp(prop)) + ">";
        }
        else if (isEqual(cname, "MapProperty")) {
            return "<" + ResolvePropertyNew(recurrce, UMapProperty::getKeyProp(prop)) + "," +
                ResolvePropertyNew(recurrce, UMapProperty::getValueProp(prop)) + ">";
        }
        else if (isEqual(cname, "BoolProperty")) {
            return "bool";
        }
        else if (isEqual(cname, "ByteProperty")) {
            return "byte";
        }
        else if (isEqual(cname, "IntProperty")) {
            return "int";
        }
        else if (isEqual(cname, "Int8Property")) {
            return "int8";
        }
        else if (isEqual(cname, "Int16Property")) {
            return "int16";
        }
        else if (isEqual(cname, "Int64Property")) {
            return "int64";
        }
        else if (isEqual(cname, "UInt16Property")) {
            return "uint16";
        }
        else if (isEqual(cname, "UINT32Property")) {
            return "UINT32";
        }
        else if (isEqual(cname, "UINT64Property")) {
            return "UINT64";
        }
        else if (isEqual(cname, "DoubleProperty")) {
            return "double";
        }
        else if (isEqual(cname, "FloatProperty")) {
            return "float";
        }
        else if (isEqual(cname, "EnumProperty")) {
            return "enum";
        }
        else if (isEqual(cname, "StrProperty")) {
            return "FString";
        }
        else if (isEqual(cname, "TextProperty")) {
            return "FText";
        }
        else if (isEqual(cname, "NameProperty")) {
            return "FName";
        }
        else if (isEqual(cname, "DelegateProperty") ||
            isEqual(cname, "MulticastDelegateProperty")) {
            return "delegate";
        }
        else {
            return FField::getName(prop) + "(" + cname + ")";
        }
    }
    return "NULL";
}

/******************** Write Structures with Class, Offsets & Size ********************/
list<uintptr_t> WriteChildStructures(ofstream& sdk, uintptr_t childprop) {
    list<uintptr_t> recurrce;
    uintptr_t child = childprop;
    while (child) {
        uintptr_t prop = child;
        string oname = UObject::getName(prop);
        string cname = UObject::getClassName(prop);

        if (isEqual(cname, "ObjectProperty") || isEqual(cname, "WeakObjectProperty") ||
            isEqual(cname, "LazyObjectProperty") || isEqual(cname, "AssetObjectProperty") ||
            isEqual(cname, "SoftObjectProperty")) {
            uintptr_t propertyClass = UObjectProperty::getPropertyClass(prop);

            sdk << "\t" << UObject::getName(propertyClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(propertyClass);
        }
        else if (isEqual(cname, "ClassProperty") || isEqual(cname, "AssetClassProperty") ||
            isEqual(cname, "SoftClassProperty")) {
            uintptr_t metaClass = UClassProperty::getMetaClass(prop);

            sdk << "\tclass " << UObject::getName(metaClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(metaClass);
        }
        else if (isEqual(cname, "InterfaceProperty")) {
            uintptr_t interfaceClass = UInterfaceProperty::getInterfaceClass(prop);

            sdk << "\tinterface class " << UObject::getName(interfaceClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "StructProperty")) {
            uintptr_t Struct = UStructProperty::getStruct(prop);

            sdk << "\t" << UObject::getName(Struct) << " " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(Struct);
        }
        else if (isEqual(cname, "ArrayProperty")) {
            sdk << "\t" << ResolveProperty(recurrce, UArrayProperty::getInner(prop)) << "[] " << oname
                << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "SetProperty")) {
            sdk << "\t<" << ResolveProperty(recurrce, USetProperty::getElementProp(prop)) << "> "
                << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "MapProperty")) {
            sdk << "\t<" << ResolveProperty(recurrce, UMapProperty::getKeyProp(prop)) << ","
                << ResolveProperty(recurrce, UMapProperty::getValueProp(prop)) << "> " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "BoolProperty")) {
            sdk << "\tbool " << oname << ";"
                << setbase(10) << "//(ByteOffset: " << (int)UBoolProperty::getByteOffset(prop)
                << ", ByteMask: " << (int)UBoolProperty::getByteMask(prop)
                << ", FieldMask: " << (int)UBoolProperty::getFieldMask(prop) << ")"
                << "[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "ByteProperty")) {
            sdk << "\tbyte " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "IntProperty")) {
            sdk << "\tint " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "Int8Property")) {
            sdk << "\tint8 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "Int16Property")) {
            sdk << "\tint16 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "Int64Property")) {
            sdk << "\tint64 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "UInt16Property")) {
            sdk << "\tuint16 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "UINT32Property")) {
            sdk << "\tUINT32 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "UINT64Property")) {
            sdk << "\tUINT64 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "DoubleProperty")) {
            sdk << "\tdouble " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "FloatProperty")) {
            sdk << "\tfloat " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "EnumProperty")) {
            sdk << "\tenum " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "StrProperty")) {
            sdk << "\tFString " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "TextProperty")) {
            sdk << "\tFText " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "NameProperty")) {
            sdk << "\tFName " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "DelegateProperty") ||
            isEqual(cname, "MulticastDelegateProperty") ||
            isEqual(cname, "MulticastInlineDelegateProperty") ||
            isEqual(cname, "MulticastSparseDelegateProperty")) {
            sdk << "\tdelegate " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "XigPtrProperty")) {
            sdk << "\tXigPtrProperty " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isStartWith(cname, "Function") || isEqual(cname, "DelegateFunction")) {
            string returnVal = "void";
            string params = "";

            uintptr_t funcParam = UStruct::getChildren(prop);
            while (funcParam) {
                UINT64 PropertyFlags = UProperty::getPropertyFlags(funcParam);

                if ((PropertyFlags & 0x0000000000000400) == 0x0000000000000400) {
                    returnVal = ResolveProperty(recurrce, funcParam);
                }
                else {
                    if ((PropertyFlags & 0x0000000000000100) == 0x0000000000000100) {
                        params += "out ";
                    }
                    /*if((PropertyFlags & 0x0000000008000000) == 0x0000000008000000){
                        params += "ref ";
                    }*/
                    if ((PropertyFlags & 0x0000000000000002) == 0x0000000000000002) {
                        params += "const ";
                    }
                    params += ResolveProperty(recurrce, funcParam);
                    params += " ";
                    params += UObject::getName(funcParam);
                    params += ", ";
                }

                funcParam = UField::getNext(funcParam);
            }

            if (!params.empty()) {
                params.pop_back();
                params.pop_back();
            }

            sdk << "\t";

            INT32 FunctionFlags = UFunction::getFunctionFlags(prop);

            if ((FunctionFlags & 0x00002000) == 0x00002000) {
                sdk << "static" << " ";
            }
            /*if((FunctionFlags & 0x00000001) == 0x00000001){
                sdk << "final" << " ";
            }
            if((FunctionFlags & 0x00020000) == 0x00020000){
                sdk << "public" << " ";
            }
            if((FunctionFlags & 0x00040000) == 0x00040000){
                sdk << "private" << " ";
            }
            if((FunctionFlags & 0x00080000) == 0x00080000){
                sdk << "protected" << " ";
            }*/

            sdk << returnVal << " " << oname << "(" << params << ");"
                << "// 0x" << setbase(16) << (UFunction::getFunc(prop) - Offsets::ModuleBase) << endl;
        }
        else {
            sdk << "\t" << cname << " " << oname << ";"
                << "//[Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }

        child = UField::getNext(child);
    }
    return recurrce;
}

list<uintptr_t> WriteChildStructuresNew(ofstream& sdk, uintptr_t childprop) {
    list<uintptr_t> recurrce;
    uintptr_t child = childprop;
    while (child) {
        uintptr_t prop = child;
        string oname = FField::getName(prop);
        string cname = FField::getClassName(prop);

        if (isEqual(cname, "ObjectProperty") || isEqual(cname, "WeakObjectProperty") ||
            isEqual(cname, "LazyObjectProperty") || isEqual(cname, "AssetObjectProperty") ||
            isEqual(cname, "SoftObjectProperty")) {
            uintptr_t propertyClass = UObjectProperty::getPropertyClass(prop);

            sdk << "\t" << UObject::getName(propertyClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(propertyClass);
        }
        else if (isEqual(cname, "ClassProperty") || isEqual(cname, "AssetClassProperty") ||
            isEqual(cname, "SoftClassProperty")) {
            uintptr_t metaClass = UClassProperty::getMetaClass(prop);

            sdk << "\tclass " << UObject::getName(metaClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(metaClass);
        }
        else if (isEqual(cname, "InterfaceProperty")) {
            uintptr_t interfaceClass = UInterfaceProperty::getInterfaceClass(prop);

            sdk << "\tinterface class " << UObject::getName(interfaceClass) << "* " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "StructProperty")) {
            uintptr_t Struct = UStructProperty::getStruct(prop);

            sdk << "\t" << UObject::getName(Struct) << " " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;

            recurrce.push_back(Struct);
        }
        else if (isEqual(cname, "ArrayProperty")) {
            sdk << "\t" << ResolvePropertyNew(recurrce, UArrayProperty::getInner(prop)) << "[] "
                << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "SetProperty")) {
            sdk << "\t<" << ResolvePropertyNew(recurrce, USetProperty::getElementProp(prop)) << "> "
                << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "MapProperty")) {
            sdk << "\t<" << ResolvePropertyNew(recurrce, UMapProperty::getKeyProp(prop)) << ","
                << ResolvePropertyNew(recurrce, UMapProperty::getValueProp(prop)) << "> " << oname
                << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "BoolProperty")) {
            sdk << "\tbool " << oname << ";"
                << setbase(10) << "//(ByteOffset: " << (int)UBoolProperty::getByteOffset(prop)
                << ", ByteMask: " << (int)UBoolProperty::getByteMask(prop)
                << ", FieldMask: " << (int)UBoolProperty::getFieldMask(prop) << ")"
                << "[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "ByteProperty")) {
            sdk << "\tbyte " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "IntProperty")) {
            sdk << "\tint " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "Int8Property")) {
            sdk << "\tint8 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "Int16Property")) {
            sdk << "\tint16 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "Int64Property")) {
            sdk << "\tint64 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "UInt16Property")) {
            sdk << "\tuint16 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "UINT32Property")) {
            sdk << "\tUINT32 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "UINT64Property")) {
            sdk << "\tUINT64 " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "DoubleProperty")) {
            sdk << "\tdouble " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "FloatProperty")) {
            sdk << "\tfloat " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "EnumProperty")) {
            sdk << "\tenum " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "StrProperty")) {
            sdk << "\tFString " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "TextProperty")) {
            sdk << "\tFText " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "NameProperty")) {
            sdk << "\tFName " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "DelegateProperty") ||
            isEqual(cname, "MulticastDelegateProperty") ||
            isEqual(cname, "MulticastInlineDelegateProperty") ||
            isEqual(cname, "MulticastSparseDelegateProperty")) {
            sdk << "\tdelegate " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else if (isEqual(cname, "XigPtrProperty")) {
            sdk << "\tXigPtrProperty " << oname << ";"
                << "//[Offset: 0x" << setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }
        else {
            sdk << "\t" << cname << " " << oname << ";"
                << "//[Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }

        child = FField::getNext(child);
    }
    return recurrce;
}

list<uintptr_t> WriteChildStructuresNew_Func(ofstream& sdk, uintptr_t childprop) {
    list<uintptr_t> recurrce;
    uintptr_t child = childprop;
    while (child) {
        uintptr_t prop = child;
        string oname = UObject::getName(prop);
        string cname = UObject::getClassName(prop);

        if (isStartWith(cname, "Function") || isEqual(cname, "DelegateFunction")) {
            string returnVal = "void";
            string params = "";

            uintptr_t funcParam = UStruct::getChildProperties(prop);
            while (funcParam) {
                UINT64 PropertyFlags = UProperty::getPropertyFlags(funcParam);

                if ((PropertyFlags & 0x0000000000000400) == 0x0000000000000400) {
                    returnVal = ResolvePropertyNew(recurrce, funcParam);
                }
                else {
                    if ((PropertyFlags & 0x0000000000000100) == 0x0000000000000100) {
                        params += "out ";
                    }
                    /*if((PropertyFlags & 0x0000000008000000) == 0x0000000008000000){
                        params += "ref ";
                    }*/
                    if ((PropertyFlags & 0x0000000000000002) == 0x0000000000000002) {
                        params += "const ";
                    }
                    params += ResolvePropertyNew(recurrce, funcParam);
                    params += " ";
                    params += FField::getName(funcParam);
                    params += ", ";
                }

                funcParam = FField::getNext(funcParam);
            }

            if (!params.empty()) {
                params.pop_back();
                params.pop_back();
            }

            sdk << "\t";

            INT32 FunctionFlags = UFunction::getFunctionFlags(prop);

            if ((FunctionFlags & 0x00002000) == 0x00002000) {
                sdk << "static" << " ";
            }
            /*if((FunctionFlags & 0x00000001) == 0x00000001){
                sdk << "final" << " ";
            }
            if((FunctionFlags & 0x00020000) == 0x00020000){
                sdk << "public" << " ";
            }
            if((FunctionFlags & 0x00040000) == 0x00040000){
                sdk << "private" << " ";
            }
            if((FunctionFlags & 0x00080000) == 0x00080000){
                sdk << "protected" << " ";
            }*/

            sdk << returnVal << " " << oname << "(" << params << ");"
                << "// 0x" << setbase(16) << (UFunction::getFunc(prop) - Offsets::ModuleBase) << endl;
        }
        else {
            sdk << "\t" << cname << " " << oname << ";"
                << "//[Size: 0x" << setbase(16) << UProperty::getElementSize(prop) << "]" << endl;
        }

        child = UField::getNext(child);
    }
    return recurrce;
}

/******************** Write Structures ********************/
void DumpStructures(ofstream& sdk, uintptr_t clazz) {
    list<uintptr_t> recurrce;

    uintptr_t currStruct = clazz;
    while (UObject::isValid(currStruct)) {
        string name = UObject::getName(currStruct);
        if (isEqual(name, "None") || isContain(name, "/Game/") || isContain(name, "_png") ||
            name.empty()) {
            break;
        }

        UINT32 NameID = UObject::getNameID(currStruct);
        if (!isScanned(NameID)) {
            //Dumping
            structIDMap.push_back(NameID);
            sdk << "Class: " << UStruct::getStructClassPath(currStruct) << endl;
            if (isNew) {
                recurrce.merge(WriteChildStructuresNew(sdk, UStruct::getChildProperties(currStruct)));
                recurrce.merge(WriteChildStructuresNew_Func(sdk, UStruct::getChildren(currStruct)));
            }
            else {
                recurrce.merge(WriteChildStructures(sdk, UStruct::getChildren(currStruct)));
            }
            sdk << "\n--------------------------------" << endl;
            ClassCount++;
        }

        currStruct = UStruct::getSuperClass(currStruct);
    }

    for (auto it = recurrce.begin(); it != recurrce.end(); ++it)
        DumpStructures(sdk, *it);
}