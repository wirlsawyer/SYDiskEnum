//---------------------------------------------------------------------------

#ifndef DiskEnumH
#define DiskEnumH
//---------------------------------------------------------------------------
#include <windows.h>
#include   <cfgmgr32.h> // here
#include   <SetupAPI.h> // here
#include <vcl.h>
#include <string>
#include <vector>
class DiskPhysicalInfo
{
public:
	std::wstring InstanceID;
        std::wstring FriendlyName;
        std::wstring DiskID;
        std::wstring ParentDriver;
};

class DiskInfo : public DiskPhysicalInfo
{
public:
	std::wstring Mount;
        std::wstring Volume;
};

class DiskEnum
{
public:
	DiskEnum()
        {
        	memset(m_Mount, 0, sizeof(m_Mount));
                wsprintf(m_Mount, L"%s", L" CDEFGHIJKLMNOPQRSTUVWXYZ");
        }

        ~DiskEnum()
        {

        }
        bool Scan()
        {
               if (this->GetPhysicalInstanceID() == false) return false;

               int Count = GetRegDWORDWithKey(L"Count");
               if (Count == -1) return false;

               m_vecDevices.clear();

               wchar_t wszKey[16];
               for (int i = 1; i < Count; i++)
               {
                   memset(wszKey, 0, sizeof(wszKey));
                   wsprintf(wszKey, L"%d", i);

                   DiskInfo item;
                   item.Volume = GetRegStringWithKey( L"SYSTEM\\ControlSet001\\services\\fvevol\\Enum", wszKey);
                   item.Mount = m_Mount[i];
                   if (item.Volume.length() == 0) return false;
                   DiskPhysicalInfo phyInfo = GetPhyInfoWithVolume(item.Volume.substr(15, 38));
                   item.InstanceID = phyInfo.InstanceID;
                   item.FriendlyName = phyInfo.FriendlyName;
                   item.DiskID = phyInfo.DiskID;
                   item.ParentDriver = phyInfo.ParentDriver;
                   m_vecDevices.push_back(item);
               }
               return true;
        }
        std::vector<DiskInfo> *GetDiskInfo()
        {
        	return &m_vecDevices;
        }

        bool IsExistWithVolume(wchar_t *pwszVolume)
        {
        	std::vector<DiskInfo> *pInfo = &m_vecDevices;
	        std::vector<DiskInfo>::iterator iter = pInfo->begin();
	        while (iter != pInfo->end())
	        {
	        	DiskInfo item = *iter;
  	                if (_wcsicmp(item.Volume.c_str(), pwszVolume) == 0)
                        {
                        	return true;
                        }
	                iter++;
	        }
                return false;
        }
private:
        DWORD GetRegDWORDWithKey(std::wstring strKey)
        {
        	//Get
		bool bExist = false;
		HKEY hKey;
		DWORD dwType = REG_DWORD;
		DWORD dwLen = 0;
		DWORD dwValue = 0;
		if (RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\ControlSet001\\services\\fvevol\\Enum", &hKey) == ERROR_SUCCESS)
		{
			//cal len
			if (RegQueryValueEx( hKey, strKey.c_str(), NULL, &dwType, NULL, &dwLen) == ERROR_SUCCESS)
			{
				//strValue.resize(dwLen);
				if (RegQueryValueEx(hKey, strKey.c_str(), NULL, &dwType, (LPBYTE)&dwValue, &dwLen) == ERROR_SUCCESS)
				{
					bExist = true;
				}
			}
			RegCloseKey(hKey);
		}
                if (bExist) {
                    return dwValue;
                }
                return -1;
        }

        std::wstring GetRegStringWithKey(std::wstring strPath, std::wstring strKey)
        {
        	//Get
		bool bExist = false;
		HKEY hKey;
		DWORD dwType = REG_SZ;
		DWORD dwLen = 0;
		std::wstring strValue = L"";
		if (RegOpenKey(HKEY_LOCAL_MACHINE, strPath.c_str(), &hKey) == ERROR_SUCCESS)
		{
			//cal len
			if (RegQueryValueEx( hKey, strKey.c_str(), NULL, &dwType, NULL, &dwLen) == ERROR_SUCCESS)
			{
				strValue.resize(dwLen);
				if (RegQueryValueEx(hKey, strKey.c_str(), NULL, &dwType, (LPBYTE)strValue.data(), &dwLen) == ERROR_SUCCESS)
				{
					bExist = true;
				}
			}
			RegCloseKey(hKey);
		}
                if (bExist) {
                    return strValue;
                }
                return L"";
        }

        bool GetPhysicalInstanceID()
        {
                m_vecPhy.clear();

        	LPWSTR classGuid = L"DiskDrive";
	        GUID *DevGuid;
		DWORD GuidSize = 0;
		GUID DevTemp;
		DevGuid = &DevTemp;
		GuidSize = sizeof(GUID);


		SetupDiClassGuidsFromName(classGuid,DevGuid,GuidSize,&GuidSize);

	 	BYTE DevHWID[512];
		memset(DevHWID, 0, 512);

		HDEVINFO hInfo = SetupDiGetClassDevsW(DevGuid, NULL, NULL, DIGCF_PRESENT);

		SP_DEVINFO_DATA data;
		data.cbSize = sizeof(SP_DEVINFO_DATA);

		int ItemCount = 0;
		BOOL ret=false, bEndFlag = false;
		while(!bEndFlag)
		{
			ret = SetupDiEnumDeviceInfo(hInfo, ItemCount, &data);
			if(!ret)
			{
				DWORD err = GetLastError();
				if(err != ERROR_NO_MORE_ITEMS)
					::MessageBoxW(NULL, L"Enumerate the USB device Fail !!", L"Message",MB_OK);

				bEndFlag = true;
				break;
			}

                        //------------------------------------------------
	                DWORD nSize=0 ;
        	        TCHAR wszInstanceID [4000];
                        ZeroMemory(wszInstanceID, sizeof(wszInstanceID));
                	if (SetupDiGetDeviceInstanceId (hInfo, &data, wszInstanceID, sizeof(wszInstanceID), &nSize))
                        {
	                	wszInstanceID[nSize] ='\0';
                        }
                        //------------------------------------------------
                        DEVINST dnParent = 0;
	                TCHAR wszParentDriver[512] = {0};
        	        CM_Get_Parent(&dnParent, data.DevInst, 0);
                	ZeroMemory(wszParentDriver, sizeof(wszParentDriver));
	                CM_Get_Device_ID(dnParent, wszParentDriver, sizeof(wszParentDriver), 0);

                	//------------------------------------------------
	        	DWORD DataT;
	                TCHAR wszFriendlyName[300];
	                ZeroMemory(wszFriendlyName, sizeof(wszFriendlyName));
	                if(SetupDiGetDeviceRegistryProperty(hInfo, &data, SPDRP_FRIENDLYNAME, &DataT, (PBYTE)wszFriendlyName, sizeof(wszFriendlyName), NULL))
			{
                        	DiskPhysicalInfo item;
                                item.InstanceID = wszInstanceID;
                                item.FriendlyName = wszFriendlyName;
                                item.ParentDriver = wszParentDriver;
                                std::wstring path = L"SYSTEM\\ControlSet001\\Enum\\";
                                path.append(wszInstanceID);
                                path.append(L"\\Device Parameters\\Partmgr");
                                item.DiskID = GetRegStringWithKey(path, L"DiskId");
                                m_vecPhy.push_back(item);
			}
                	//------------------------------------------------
               		ItemCount++;
		}


              	return true;
        }

        DiskPhysicalInfo GetPhyInfoWithVolume(std::wstring strVolume)
        {
                std::vector<DiskPhysicalInfo> *pInfo = &m_vecPhy;
	        std::vector<DiskPhysicalInfo>::iterator iter = pInfo->begin();
	        while (iter != pInfo->end())
	        {
	        	DiskPhysicalInfo item = *iter;
  	                if ( _wcsicmp(strVolume.c_str(), item.DiskID.c_str() )==0) {
                            return item;
                        }
	                iter++;
	        }
                DiskPhysicalInfo result;
                return result;
        }
private:
	wchar_t m_Mount[26];
        std::vector<DiskPhysicalInfo> m_vecPhy;
	std::vector<DiskInfo> m_vecDevices;
};
#endif

