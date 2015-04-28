// ---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SDIMain.h"
#include "ControlI2C.h"
#include "stdio.h"
// ---------------------------------------------------------------------
#pragma resource "*.dfm"
TSDIAppForm *SDIAppForm;

// ---------------------------------------------------------------------
__fastcall TSDIAppForm::TSDIAppForm(TComponent *AOwner) : TForm(AOwner) {
}
// ---------------------------------------------------------------------------

void __fastcall TSDIAppForm::WriteDataButtonClick(TObject *Sender) {
	int ret;
	//Scan device number(must call)
	ret = VII_ScanDevice(1);
	if(ret <= 0){
		Application->MessageBox(L"No device connected", L"Warning", MB_ICONWARNING);
        return;
	}
	//Open device(must call)
	ret = VII_OpenDevice(VII_USBI2C,0,0);
	if(ret != ERR_SUCCESS){
		Application->MessageBox(L"Open device error!",L"Warning",MB_ICONWARNING);
		return;
	}
	//Initialize device
	VII_INIT_CONFIG I2C_InitConfig;
	I2C_InitConfig.AddrType = VII_ADDR_7BIT;
	I2C_InitConfig.ControlMode = VII_HCTL_MODE;
	I2C_InitConfig.MasterMode = VII_MASTER;
	I2C_InitConfig.SubAddrWidth = VII_SUB_ADDR_1BYTE;
	I2C_InitConfig.ClockSpeed = 400000;
	ret = VII_InitI2C(VII_USBI2C,0,0,&I2C_InitConfig);
	if(ret != ERR_SUCCESS){
		Application->MessageBox(L"Initialize device error!",L"Warning",MB_ICONWARNING);
		return;
	}
	//Write I2C test
	uint8_t write_data_buffer[1024];
	AnsiString write_data_str =  this->WriteDataEdit->Text;
	TStringList *write_data_str_list = new TStringList;;
	AnsiString str;
	str = StringReplace(write_data_str," ", "\r\n", TReplaceFlags() << rfReplaceAll);
	write_data_str_list->Text = str;
	for (int i = 0; i < write_data_str_list->Count; i++){
			write_data_buffer[i] = StrToInt("0x"+write_data_str_list->Strings[i]);
	}

	ret = VII_WriteBytes(VII_USBI2C,0,0,0xA0,0x08,write_data_buffer,8);
	if(ret != ERR_SUCCESS){
				Application->MessageBox(L"Write data error!",L"Warning",MB_ICONWARNING);
	}else{
				Application->MessageBox(L"Write data success!",L"Message",MB_OK);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TSDIAppForm::ReadDataButtonClick(TObject *Sender)
{
	int ret;
	uint8_t read_data_buff[8]={0};
	VII_INIT_CONFIG I2C_InitConfig;
	//Check the device is connected
	ret = VII_ScanDevice(1);
	if(ret <= 0){
		Application->MessageBox(L"No device connected",L"Warning",MB_ICONWARNING);
		return;
	}
	//Open device,must call
	ret = VII_OpenDevice(VII_USBI2C,0,0);
	if(ret != ERR_SUCCESS){
		Application->MessageBox(L"Open device error!",L"Warning",MB_ICONWARNING);
		return;
	}
	//Initialize device
	I2C_InitConfig.AddrType = VII_ADDR_7BIT;
	I2C_InitConfig.ControlMode = VII_HCTL_MODE;
	I2C_InitConfig.MasterMode = VII_MASTER;
	I2C_InitConfig.SubAddrWidth = VII_SUB_ADDR_1BYTE;
	I2C_InitConfig.ClockSpeed = 400000;
	ret = VII_InitI2C(VII_USBI2C,0,0,&I2C_InitConfig);
	if(ret != ERR_SUCCESS){
		Application->MessageBox(L"Initialize device error!",L"Warning",MB_ICONWARNING);
		return;
	}
	//Read I2C test
	ret = VII_ReadBytes(VII_USBI2C,0,0,0xA0,0x08,read_data_buff,8);
	if(ret != ERR_SUCCESS){
		Application->MessageBox(L"Read data error!",L"Warning",MB_ICONWARNING);
	}else{
		char str[100]={0};
		sprintf(str,"%02X %02X %02X %02X %02X %02X %02X %02X",
		read_data_buff[0],read_data_buff[1],read_data_buff[2],read_data_buff[3],
		read_data_buff[4],read_data_buff[5],read_data_buff[6],read_data_buff[7]);
		wchar_t wstr[400]={0};
		MultiByteToWideChar( CP_ACP, 0, str,strlen(str)+1, wstr, sizeof(wchar_t)*400/sizeof(wstr[0]));
		this->ReadDataEdit->SetTextBuf(wstr);
	}

}
//---------------------------------------------------------------------------

