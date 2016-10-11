//---------------------------------------------------------------------------


#pragma hdrstop
#include "DiskEnum.h"
#include "Unit1.h"
//#define  USE_SP_DRVINFO_DATA_V1 1
//DEFINE_GUID( GUID_HARD_DISK, 0x4d36e96aL, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18);
//DEFINE_GUID( GUID_DEVCLASS_VOLUME, 0x71a27cddL, 0x812a, 0x11d0, 0xbe, 0xc7, 0x08, 0x00, 0x2b, 0xe2, 0x09, 0x2f );

//#define ARRAY_SIZE(arr)     (sizeof(arr)/sizeof(arr[0]))
//#pragma comment (lib, "setupapi.lib")



DiskEnum *disknum = new DiskEnum();
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm1::RegisterClick(TObject *Sender)
{
	ListBox1->Items->Clear();
	disknum->Scan();
	std::vector<DiskInfo> *pInfo = disknum->GetDiskInfo();
        std::vector<DiskInfo>::iterator iter = pInfo->begin();
        while (iter != pInfo->end())
        {
        	DiskInfo item = *iter;
                ListBox1->Items->Add((item.Mount+L":\\").c_str());
                ListBox1->Items->Add((item.FriendlyName).c_str());
                ListBox1->Items->Add((item.InstanceID).c_str());
        	ListBox1->Items->Add((item.DiskID).c_str());
                ListBox1->Items->Add((item.Volume).c_str());
                ListBox1->Items->Add((item.ParentDriver).c_str());

                ListBox1->Items->Add(L"--------------------------------------");
                iter++;
        }

}
//---------------------------------------------------------------------------

