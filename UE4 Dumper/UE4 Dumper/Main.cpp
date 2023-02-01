#include <Windows.h>
#include <sstream>

#include "Dumper.h"

int main() {
	//This is to initialize the offsets before anything is done. Bare with this or implement your own method
	Offsets::NewUEVersion();
	//Offsets::OldUEVersion();

	Offsets::ProcessID = GetProcessID(Offsets::TargetProcess);
	Offsets::ModuleBase = GetBaseAddress(Offsets::ProcessID, Offsets::TargetProcess);
	Offsets::Handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, Offsets::ProcessID);//Use Kernel if u have so much problem ;)

	cout << "ProcessID Found at " << Offsets::ProcessID << endl;
	cout << "Base Address Found at 0x" << hex << Offsets::ModuleBase << endl;

	string OutPutDirectory("D:/Sources/Dumped/");
	OutPutDirectory += Offsets::GameName;
	const char* FolderPath = OutPutDirectory.c_str();

	CreateDirectory(FolderPath, NULL);

	//Using F Keys, tried Imgui its lagy as hell
	cout << "\n1. Dump String - F1" << endl;
	cout << "2. ObjectsDump - F2" << endl;
	cout << "3. SDK Dump(Offsets Dump using GUObject) - F3" << endl;
	cout << "4. SDKW Dump(Offsets Dump using GWorld) - F4" << endl;
	cout << "5. Dump Actors - F5" << endl;
	cout << "6. Exit - End" << endl;

	while (true) {
		if (GetAsyncKeyState(VK_F1) & 1) {
			cout << "\nDumping Strings" << endl;
			DumpStrings(OutPutDirectory);
		}
		if (GetAsyncKeyState(VK_F2) & 1) {
			cout << "\nDumping Objects" << endl;
			DumpObjects(OutPutDirectory);
		}
		if (GetAsyncKeyState(VK_F3) & 1) {
			cout << "\nDumping SDK (using GUObject)" << endl;
			DumpSDK(OutPutDirectory);
		}
		if (GetAsyncKeyState(VK_F4) & 1) {
			cout << "\nDumping SDKW (using GWorld)" << endl;
			DumpSDKW(OutPutDirectory);
		}
		if (GetAsyncKeyState(VK_F5) & 1) {
			cout << "\nDumping Actors" << endl;
			DumpActors(OutPutDirectory);
		}
		if (GetAsyncKeyState(VK_END) & 1) {
			exit(0);
		}
	}
}