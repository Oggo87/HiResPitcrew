#include <iomanip>
#include <malloc.h>
#include <sstream>
#include <string>
#include <windows.h>
using namespace std;

string dwordToString(DWORD address) {

	ostringstream outputString;

	outputString.str(string());
	outputString << "0x" << hex << setw(sizeof(DWORD) * 2) << setfill('0') << address;

	return outputString.str();

}

string ptrToString(LPVOID address) {

	return dwordToString(PtrToUlong(address));
}

void PatchAddress(LPVOID address, BYTE* patch, SIZE_T size) {
	DWORD oldProtect;

	string addressString = ptrToString(address);

	if (VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {

		memcpy(address, patch, size);
		VirtualProtect(address, size, oldProtect, &oldProtect);

		OutputDebugStringA(("Memory patched successfully at address " + addressString + "\n").c_str());
	}
	else {
		OutputDebugStringA(("Error while patching address " + addressString + "\n").c_str());
	}
}

DWORD WINAPI MainThread(LPVOID param) {

	ostringstream outputString;

	////////////////////////////////////////////
	//Code for pitcrew buffers starts here
	//Original vertex buffer at 0x00aa0828
	//Original normals buffer at 0x00a9f560
	//Original D3D size of 0x4000 at 0x004F2CA6,
	////////////////////////////////////////////

	char* pitcrewVertexBuffer = (char*)VirtualAlloc(NULL, 65536 * 4 * 3, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pitcrewVertexBuffer) {
		OutputDebugStringA("Error allocating memory\n");
		return 1;
	}

	outputString.str(string());
	outputString << hex << showbase << PtrToUlong(pitcrewVertexBuffer)
		<< ", " << PtrToUlong(pitcrewVertexBuffer + 4)
		<< ", " << PtrToUlong(pitcrewVertexBuffer + 8);

	OutputDebugStringA(("Address of new Vertex Buffer: " + outputString.str() + "\n").c_str());

	//Patch exe for new vertex buffer
	PatchAddress((LPVOID)0x004F929A, (BYTE*)&pitcrewVertexBuffer, sizeof(pitcrewVertexBuffer));
	PatchAddress((LPVOID)0x004F92F5, (BYTE*)&pitcrewVertexBuffer, sizeof(pitcrewVertexBuffer));
	pitcrewVertexBuffer += 4;
	PatchAddress((LPVOID)0x004F92FE, (BYTE*)&pitcrewVertexBuffer, sizeof(pitcrewVertexBuffer));
	pitcrewVertexBuffer += 4;
	PatchAddress((LPVOID)0x004F9307, (BYTE*)&pitcrewVertexBuffer, sizeof(pitcrewVertexBuffer));

	char* pitcrewNormalsBuffer = (char*)VirtualAlloc(NULL, 65536 * 4 * 3, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pitcrewNormalsBuffer) {
		OutputDebugStringA("Error allocating memory\n");
		return 1;
	}

	outputString.str(string());
	outputString << hex << showbase << PtrToUlong(pitcrewNormalsBuffer)
		<< ", " << PtrToUlong(pitcrewNormalsBuffer + 4)
		<< ", " << PtrToUlong(pitcrewNormalsBuffer + 8);

	OutputDebugStringA(("Address of new Normals Buffer: " + outputString.str() + "\n").c_str());

	//Patch exe for new normals buffer
	PatchAddress((LPVOID)0x004F92A1, (BYTE*)&pitcrewNormalsBuffer, sizeof(pitcrewNormalsBuffer));
	PatchAddress((LPVOID)0x004F8187, (BYTE*)&pitcrewNormalsBuffer, sizeof(pitcrewNormalsBuffer));
	PatchAddress((LPVOID)0x004F92D8, (BYTE*)&pitcrewNormalsBuffer, sizeof(pitcrewNormalsBuffer));
	pitcrewNormalsBuffer += 4;
	PatchAddress((LPVOID)0x004F92E4, (BYTE*)&pitcrewNormalsBuffer, sizeof(pitcrewNormalsBuffer));
	PatchAddress((LPVOID)0x004F81AB, (BYTE*)&pitcrewNormalsBuffer, sizeof(pitcrewNormalsBuffer));
	pitcrewNormalsBuffer += 4;
	PatchAddress((LPVOID)0x004F92ED, (BYTE*)&pitcrewNormalsBuffer, sizeof(pitcrewNormalsBuffer));

	//Patch exe for larger D3D allocated memory space
	int size = 0x10000000;
	PatchAddress((LPVOID)0x004F2CA6, (BYTE*)&size, sizeof(int));

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}