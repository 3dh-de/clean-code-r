#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "HfCardReader.h"


/**
 * Default constructor
 */
HfCardReader::HfCardReader() {
    hDll = NULL;
    ReleaseFunctions ();
}

/**
 * Release the loaded functions and dll
 */
HfCardReader::~HfCardReader() {
    ReleaseFunctions ();
}


/**
 * loads a function from the DLL.
 *
 * If loading fails, false is stored in ret. If lodaing is successfull,
 * ret is not changed.
 */
#define GET_FUNCTION(ret,name) if((name=(DLL_##name)GetProcAddress(hDll,#name))==NULL)ret=false

/**
 * Load all the functions in the DLL
 */
bool HfCardReader::GetFunctions() {
    if (IsLoaded())
        ReleaseFunctions();

    if ((hDll = LoadLib()) == NULL)
        return false;

    bool ret = true;
	
	GET_FUNCTION(ret, OpenComm);
	GET_FUNCTION(ret, CloseComm);
	GET_FUNCTION(ret, SetDeviceAddress);
	GET_FUNCTION(ret, GetSerialNum);
	GET_FUNCTION(ret, SetSerialNum);
	GET_FUNCTION(ret, GetVersionNum);
	GET_FUNCTION(ret, LoadProgram);
	GET_FUNCTION(ret, GetVersionAPI);
	GET_FUNCTION(ret, SetFirmwareBaudrate);
	GET_FUNCTION(ret, GetUserInfo);
	GET_FUNCTION(ret, SetUserInfo);
	GET_FUNCTION(ret, SetExtPort);
	GET_FUNCTION(ret, GetPort);
	GET_FUNCTION(ret, ActiveLED);
	GET_FUNCTION(ret, SetLED);
	GET_FUNCTION(ret, ActiveBuzzer);
	GET_FUNCTION(ret, RF_Field);
	GET_FUNCTION(ret, MF_Request);
	GET_FUNCTION(ret, MF_Anticoll);
	GET_FUNCTION(ret, MF_Anticoll2);
	GET_FUNCTION(ret, MF_Anticoll3);
	GET_FUNCTION(ret, MF_Select);
	GET_FUNCTION(ret, MF_Select2);
	GET_FUNCTION(ret, MF_Select3);
	GET_FUNCTION(ret, MF_Halt);
	GET_FUNCTION(ret, MF_ReaderHalt);
#if 0
	GET_FUNCTION(ret, SetBaudrateSAM);
	GET_FUNCTION(ret, AnswerToResetSAM);
	GET_FUNCTION(ret, SAM_Generic);
	GET_FUNCTION(ret, SLE_Generic);
	GET_FUNCTION(ret, RequestType_B);
	GET_FUNCTION(ret, AntiType_B);
	GET_FUNCTION(ret, SelectType_B);
	GET_FUNCTION(ret, Halt_B);
	GET_FUNCTION(ret, Generic_B);
	GET_FUNCTION(ret, Request_AB);
	GET_FUNCTION(ret, SetTypeB);

	GET_FUNCTION(ret, MF_Auth);
	GET_FUNCTION(ret, MF_Read);
	GET_FUNCTION(ret, MF_Write);
	GET_FUNCTION(ret, MF_Increment);
	GET_FUNCTION(ret, MF_Decrement);
	GET_FUNCTION(ret, MF_Transfer);
	GET_FUNCTION(ret, MF_Restore);
	GET_FUNCTION(ret, MF_InitValue);
	GET_FUNCTION(ret, MF_ReadValue);
	GET_FUNCTION(ret, MF_LoadKey);
	GET_FUNCTION(ret, MF_StoreKeyToEE);
	GET_FUNCTION(ret, MF_LoadKeyFromEE);
	GET_FUNCTION(ret, MF_HLRead);
	GET_FUNCTION(ret, MF_HLWrite);
	GET_FUNCTION(ret, MF_HLInitVal);
	GET_FUNCTION(ret, MF_HLDec);
	GET_FUNCTION(ret, MF_HLInc);
	GET_FUNCTION(ret, MF_HLRequest);
#endif
	GET_FUNCTION(ret, ReadChar);
	GET_FUNCTION(ret, GenericCMD);
	GET_FUNCTION(ret, SetWiegandStatus);
	GET_FUNCTION(ret, SHC1102_Auth);
	GET_FUNCTION(ret, SHC1102_Read);
	GET_FUNCTION(ret, SHC1102_Write);
	GET_FUNCTION(ret, SHC1102_Halt);
    return ret;
}

#undef GET_FUNCTION


/**
 * Set the function pointer back to 0
 */
#define RELEASE_FUNCTION(name) name=NULL;


/**
 * Release all loaded functions and free the DLL
 */
void HfCardReader::ReleaseFunctions() {
	RELEASE_FUNCTION(OpenComm);
	RELEASE_FUNCTION(CloseComm);
	RELEASE_FUNCTION(SetDeviceAddress);
	RELEASE_FUNCTION(GetSerialNum);
	RELEASE_FUNCTION(SetSerialNum);
	RELEASE_FUNCTION(GetVersionNum);
	RELEASE_FUNCTION(LoadProgram);
	RELEASE_FUNCTION(GetVersionAPI);
	RELEASE_FUNCTION(SetFirmwareBaudrate);
	RELEASE_FUNCTION(GetUserInfo);
	RELEASE_FUNCTION(SetUserInfo);
	RELEASE_FUNCTION(SetExtPort);
	RELEASE_FUNCTION(GetPort);
	RELEASE_FUNCTION(ActiveLED);
	RELEASE_FUNCTION(SetLED);
	RELEASE_FUNCTION(ActiveBuzzer);
	RELEASE_FUNCTION(RF_Field);
	RELEASE_FUNCTION(MF_Request);
	RELEASE_FUNCTION(MF_Anticoll);
	RELEASE_FUNCTION(MF_Anticoll2);
	RELEASE_FUNCTION(MF_Anticoll3);
	RELEASE_FUNCTION(MF_Select);
	RELEASE_FUNCTION(MF_Select2);
	RELEASE_FUNCTION(MF_Select3);
	RELEASE_FUNCTION(MF_Halt);
	RELEASE_FUNCTION(MF_ReaderHalt);
#if 0
	RELEASE_FUNCTION(SetBaudrateSAM);
	RELEASE_FUNCTION(AnswerToResetSAM);
	RELEASE_FUNCTION(SAM_Generic);
	RELEASE_FUNCTION(SLE_Generic);
	RELEASE_FUNCTION(RequestType_B);
	RELEASE_FUNCTION(AntiType_B);
	RELEASE_FUNCTION(SelectType_B);
	RELEASE_FUNCTION(Halt_B);
	RELEASE_FUNCTION(Generic_B);
	RELEASE_FUNCTION(Request_AB);
	RELEASE_FUNCTION(SetTypeB);

	RELEASE_FUNCTION(MF_Auth);
	RELEASE_FUNCTION(MF_Read);
	RELEASE_FUNCTION(MF_Write);
	RELEASE_FUNCTION(MF_Increment);
	RELEASE_FUNCTION(MF_Decrement);
	RELEASE_FUNCTION(MF_Transfer);
	RELEASE_FUNCTION(MF_Restore);
	RELEASE_FUNCTION(MF_InitValue);
	RELEASE_FUNCTION(MF_ReadValue);
	RELEASE_FUNCTION(MF_LoadKey);
	RELEASE_FUNCTION(MF_StoreKeyToEE);
	RELEASE_FUNCTION(MF_LoadKeyFromEE);
	RELEASE_FUNCTION(MF_HLRead);
	RELEASE_FUNCTION(MF_HLWrite);
	RELEASE_FUNCTION(MF_HLInitVal);
	RELEASE_FUNCTION(MF_HLDec);
	RELEASE_FUNCTION(MF_HLInc);
	RELEASE_FUNCTION(MF_HLRequest);
#endif
	RELEASE_FUNCTION(ReadChar);
	RELEASE_FUNCTION(GenericCMD);
	RELEASE_FUNCTION(SetWiegandStatus);
	RELEASE_FUNCTION(SHC1102_Auth);
	RELEASE_FUNCTION(SHC1102_Read);
	RELEASE_FUNCTION(SHC1102_Write);
	RELEASE_FUNCTION(SHC1102_Halt);
    if (hDll != NULL)
        FreeLibrary (hDll);

	hDll = NULL;
}

#undef RELEASE_FUNCTION


/**
 * Init the dll
 *
 * Try to load the functions, it it fails, abort and release all loaded functions
 */
bool HfCardReader::Init() {
    if (!GetFunctions()) {
        ReleaseFunctions();
        return false;
    }
    return true;
}

/**
 * Exit
 *
 * Release all loaded functions and free the DLL
 */
bool HfCardReader::Exit() {
    ReleaseFunctions();
    return true;
}

