// HfCardReader.h: interface for the dynamic loading of the uEye_tools.dll
#ifndef HFCARDREADER_H
#define HFCARDREADER_H

#include <windows.h>

#define DECLARE_FUNCTION(name) DLL_##name name

/**
 * Import class for dll functions
 */
class HfCardReader
{
private:
	/******************* System Functions ******************************/
	///1. Set communciation baudrate.
	typedef int (__stdcall* DLL_OpenComm)  (char *com,int Baudrate);
	///2. To close communication port.
	typedef int (__stdcall* DLL_CloseComm)  (void);
	///4. Get device(firmware) address and serial number from target device.
	typedef int (__stdcall* DLL_SetDeviceAddress)   (int DeviceAddress,unsigned char &newAddress,unsigned char mode,unsigned char *buffer);
	///5. Set the device(firmware) serial number.
	typedef int (__stdcall* DLL_GetSerialNum)   (int DeviceAddress,int &CurrentAddress, char *SerialNum);
	typedef int (__stdcall* DLL_SetSerialNum)   (int DeviceAddress,char *SerialNum);
	typedef int (__stdcall* DLL_GetVersionNum)   (int DeviceAddress,char *VersionNum);
	typedef int (__stdcall* DLL_LoadProgram)   (int DeviceAddress);
	typedef int (__stdcall* DLL_GetVersionAPI)  (char *Ver);
	typedef int (__stdcall* DLL_SetFirmwareBaudrate)   (int DeviceAddress,unsigned char Baudrate);
	typedef int (__stdcall* DLL_GetUserInfo)   (int DeviceAddress, char *UserInfo);
	typedef int (__stdcall* DLL_SetUserInfo)   (int DeviceAddress,char *UserInfo);
	/******************* I/O Functions ******************************/
	typedef int (__stdcall* DLL_SetExtPort)   (int DeviceAddress,unsigned char Ports);
	typedef int (__stdcall* DLL_GetPort)   (int DeviceAddress,unsigned char *status);
	typedef int (__stdcall* DLL_ActiveLED)   (int DeviceAddress,unsigned char NumLED,unsigned char time,unsigned char cycle);
	typedef int (__stdcall* DLL_SetLED)   (int DeviceAddress,unsigned char LED);
	typedef int (__stdcall* DLL_ActiveBuzzer)   (int DeviceAddress,unsigned char mode,unsigned char *buffer);
	typedef int (__stdcall* DLL_RF_Field)   (int DeviceAddress,unsigned char mode);
	/******************* ISO14443 Type A Functions ******************************/
	typedef int (__stdcall* DLL_MF_Request)   (int DeviceAddress, unsigned char inf_mode, unsigned char *buffer);
	typedef int (__stdcall* DLL_MF_Anticoll)   (int DeviceAddress,unsigned char *snr,unsigned char &status);
	typedef int (__stdcall* DLL_MF_Anticoll2)   (int DeviceAddress,unsigned char *snr,unsigned char &status);
	typedef int (__stdcall* DLL_MF_Anticoll3)   (int DeviceAddress,unsigned char *snr,unsigned char &status);
	typedef int (__stdcall* DLL_MF_Select)   (int DeviceAddress,unsigned char *snr);
	typedef int (__stdcall* DLL_MF_Select2)   (int DeviceAddress,unsigned char *snr);
	typedef int (__stdcall* DLL_MF_Select3)   (int DeviceAddress,unsigned char *snr);
	typedef int (__stdcall* DLL_MF_Halt)   (int DeviceAddress,unsigned char mode);
	typedef int (__stdcall* DLL_MF_ReaderHalt)   (int DeviceAddress,unsigned char mode);
	/******************* SAM Functions ******************************/
	typedef int (__stdcall* DLL_SetBaudrateSAM)   (int DeviceAddress,int Baudrate);
	typedef int (__stdcall* DLL_AnswerToResetSAM)   (int DeviceAddress,unsigned char *buffer,int &nBytes);
	typedef int (__stdcall* DLL_SAM_Generic)   (int DeviceAddress,unsigned char LRC_Flag,unsigned char &length,unsigned char *buffer);
	typedef int (__stdcall* DLL_SLE_Generic)   (int DeviceAddress,unsigned char CRC_Flag,unsigned char &length,unsigned char *buffer);
	/******************* ISO14443 Type B Functions ******************************/
	typedef int (__stdcall* DLL_RequestType_B)   (int DeviceAddress,unsigned char Afi,unsigned char mode, unsigned char *buffer,int &length);
	typedef int (__stdcall* DLL_AntiType_B)   (int DeviceAddress,unsigned char &status, unsigned char *buffer);
	typedef int (__stdcall* DLL_SelectType_B)   (int DeviceAddress,unsigned char *SerialNum,unsigned char CID,unsigned char Param1, unsigned char Param2, unsigned char Param3);
	typedef int (__stdcall* DLL_Halt_B)   (int DeviceAddress,unsigned char *_Srn);
	typedef int (__stdcall* DLL_Generic_B)   (int DeviceAddress,unsigned char &length,unsigned char *buffer);
	typedef int (__stdcall* DLL_Request_AB)   (int DeviceAddress,int &length, unsigned char *buffer);
	typedef int (__stdcall* DLL_SetTypeB)   (int DeviceAddress);
	/******************* MIFARE Functions ******************************/
	typedef int (__stdcall* DLL_MF_Auth)   (int DeviceAddress,unsigned char mode, unsigned char *snr, unsigned char block);
	typedef int (__stdcall* DLL_MF_Read)   (int DeviceAddress,unsigned char add_blk, unsigned char num_blk, unsigned char *buffer);
	typedef int (__stdcall* DLL_MF_Write)   (int DeviceAddress,unsigned char add_blk, unsigned char num_blk, unsigned char *buffer);
	typedef int (__stdcall* DLL_MF_Increment)   (int DeviceAddress,unsigned char add_blk, int value);
	typedef int (__stdcall* DLL_MF_Decrement)   (int DeviceAddress,unsigned char add_blk, int value);
	typedef int (__stdcall* DLL_MF_Transfer)   (int DeviceAddress,unsigned char add_blk);
	typedef int (__stdcall* DLL_MF_Restore)   (int DeviceAddress,unsigned char add_blk);
	typedef int (__stdcall* DLL_MF_InitValue)   (int DeviceAddress,unsigned char add_blk, int value);
	typedef int (__stdcall* DLL_MF_ReadValue)   (int DeviceAddress,unsigned char add_blk, int *value);
	typedef int (__stdcall* DLL_MF_LoadKey)   (int DeviceAddress,unsigned char *key);
	typedef int (__stdcall* DLL_MF_StoreKeyToEE)   (int DeviceAddress,unsigned char Key_type,unsigned char Sector,unsigned char *Key);
	typedef int (__stdcall* DLL_MF_LoadKeyFromEE)   (int DeviceAddress,unsigned char Key_type,unsigned char Sector);
	/******************* MIFARE High Level Functions ******************************/
	typedef int (__stdcall* DLL_MF_HLRead)   (int DeviceAddress,unsigned char mode, unsigned char blk_add,unsigned char num_blk, unsigned char *snr, unsigned char *buffer);
	typedef int (__stdcall* DLL_MF_HLWrite)   (int DeviceAddress,unsigned char mode, unsigned char blk_add,unsigned char num_blk, unsigned char *snr, unsigned char *buffer);
	typedef int (__stdcall* DLL_MF_HLInitVal)   (int DeviceAddress,unsigned char mode, unsigned char SectNum, unsigned char *snr, int value);
	typedef int (__stdcall* DLL_MF_HLDec)   (int DeviceAddress,unsigned char mode, unsigned char SectNum, unsigned char *snr, int *value);
	typedef int (__stdcall* DLL_MF_HLInc)   (int DeviceAddress,unsigned char mode, unsigned char SectNum, unsigned char *snr, int *value);
	typedef int (__stdcall* DLL_MF_HLRequest)   (int DeviceAddress, unsigned char mode, int &len, unsigned char *snr);
	/******************* Internal unofficial Functions ******************************/
	typedef int (__stdcall* DLL_ReadChar)  (unsigned char *byte);
	typedef int (__stdcall* DLL_GenericCMD)   (int DeviceAddress,unsigned char mode, unsigned char *buffer, int &NumByte);
	typedef int (__stdcall* DLL_SetWiegandStatus)   (int DeviceAddress,unsigned char status);
	typedef int (__stdcall* DLL_SHC1102_Auth)   (int DeviceAddress,unsigned char *key);
	typedef int (__stdcall* DLL_SHC1102_Read)   (int DeviceAddress,unsigned char block, unsigned char *dat);
	typedef int (__stdcall* DLL_SHC1102_Write)   (int DeviceAddress,unsigned char block, unsigned char *dat);
	typedef int (__stdcall* DLL_SHC1102_Halt)   (int DeviceAddress);

public:
    HfCardReader ();
    virtual ~HfCardReader ();

    bool Exit ();
	bool Init ();

	/******************* System Functions ******************************/
	DECLARE_FUNCTION(OpenComm);
	DECLARE_FUNCTION(CloseComm);
	DECLARE_FUNCTION(SetDeviceAddress);
	DECLARE_FUNCTION(GetSerialNum);
	DECLARE_FUNCTION(SetSerialNum);
	DECLARE_FUNCTION(GetVersionNum);
	DECLARE_FUNCTION(LoadProgram);
	DECLARE_FUNCTION(GetVersionAPI);
	DECLARE_FUNCTION(SetFirmwareBaudrate);
	DECLARE_FUNCTION(GetUserInfo);
	DECLARE_FUNCTION(SetUserInfo);
	/******************* I/O Functions ******************************/
	DECLARE_FUNCTION(SetExtPort);
	DECLARE_FUNCTION(GetPort);
	DECLARE_FUNCTION(ActiveLED);
	DECLARE_FUNCTION(SetLED);
	DECLARE_FUNCTION(ActiveBuzzer);
	DECLARE_FUNCTION(RF_Field);
	/******************* ISO14443 Type A Functions ******************************/
	DECLARE_FUNCTION(MF_Request);
	DECLARE_FUNCTION(MF_Anticoll);
	DECLARE_FUNCTION(MF_Anticoll2);
	DECLARE_FUNCTION(MF_Anticoll3);
	DECLARE_FUNCTION(MF_Select);
	DECLARE_FUNCTION(MF_Select2);
	DECLARE_FUNCTION(MF_Select3);
	DECLARE_FUNCTION(MF_Halt);
	DECLARE_FUNCTION(MF_ReaderHalt);
#if 0
	/******************* SAM Functions ******************************/
	DECLARE_FUNCTION(SetBaudrateSAM);
	DECLARE_FUNCTION(AnswerToResetSAM);
	DECLARE_FUNCTION(SAM_Generic);
	DECLARE_FUNCTION(SLE_Generic);
	/******************* ISO14443 Type B Functions ******************************/
	DECLARE_FUNCTION(RequestType_B);
	DECLARE_FUNCTION(AntiType_B);
	DECLARE_FUNCTION(SelectType_B);
	DECLARE_FUNCTION(Halt_B);
	DECLARE_FUNCTION(Generic_B);
	DECLARE_FUNCTION(Request_AB);
	DECLARE_FUNCTION(SetTypeB);
	/******************* MIFARE Functions ******************************/
	DECLARE_FUNCTION(MF_Auth);
	DECLARE_FUNCTION(MF_Read);
	DECLARE_FUNCTION(MF_Write);
	DECLARE_FUNCTION(MF_Increment);
	DECLARE_FUNCTION(MF_Decrement);
	DECLARE_FUNCTION(MF_Transfer);
	DECLARE_FUNCTION(MF_Restore);
	DECLARE_FUNCTION(MF_InitValue);
	DECLARE_FUNCTION(MF_ReadValue);
	DECLARE_FUNCTION(MF_LoadKey);
	DECLARE_FUNCTION(MF_StoreKeyToEE);
	DECLARE_FUNCTION(MF_LoadKeyFromEE);
	/******************* MIFARE High Level Functions ******************************/
	DECLARE_FUNCTION(MF_HLRead);
	DECLARE_FUNCTION(MF_HLWrite);
	DECLARE_FUNCTION(MF_HLInitVal);
	DECLARE_FUNCTION(MF_HLDec);
	DECLARE_FUNCTION(MF_HLInc);
	DECLARE_FUNCTION(MF_HLRequest);
#endif	
	/******************* Internal unofficial Functions ******************************/
	DECLARE_FUNCTION(ReadChar);
	DECLARE_FUNCTION(GenericCMD);
	DECLARE_FUNCTION(SetWiegandStatus);
	DECLARE_FUNCTION(SHC1102_Auth);
	DECLARE_FUNCTION(SHC1102_Read);
	DECLARE_FUNCTION(SHC1102_Write);
	DECLARE_FUNCTION(SHC1102_Halt);
    bool IsLoaded () const { return hDll != NULL; }

private:
    bool      GetFunctions();
    void      ReleaseFunctions();
	HINSTANCE LoadLib()   { return ::LoadLibrary( L"CVAPIV01.dll" ); }
    HINSTANCE hDll;
};

#undef DECLARE_FUNCTION


#endif // HFCARDREADER_H