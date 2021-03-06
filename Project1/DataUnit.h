//---------------------------------------------------------------------------

#include <vcl.h>
#ifndef DataUnitH
#define DataUnitH
//---------------------------------------------------------------------------

// YJZH_MapFenLei ����
class TData {
public:
    TData();
    ~TData();
    __property int ID = { read = GetID, write = SetID };
    __property int ISDELETE = { read = GetIsDelete, write = SetIsDelete };
    __property int JdContentType = { read = GetJdContentType, write = SetJdContentType };
    __property int Index_ = { read = GetIndex, write = SetIndex };
    __property String PGUID = {read = GetPGUID, write = SetPGUID };
    __property String UPGUID = {read = GetUPGUID, write = SetUPGUID };
    __property String S_UDTIME = {read = GetS_UDTIME, write = SetS_UDTIME };
    __property String SYNC_TIME = {read = GetSYNC_TIME, write = SetSYNC_TIME };
    __property String JdText = {read = GetJdText, write = SetJdText };
    __property String JdType = {read = GetJdType, write = SetJdType };
    __property String DevName = {read = GetDevName, write = SetDevName };
    __property String IconSxName = {read = GetIconSxName, write = SetIconSxName };
    __property String MapGuid = {read = GetMapGuid, write = SetMapGuid };
    __property String DevID = {read = GetDevID, write = SetDevID };
    TData& operator = (const TData& rhs) {
        SetID(rhs.ID);
        SetIsDelete(rhs.ISDELETE);
        SetJdContentType(rhs.JdContentType);
        SetIndex(rhs.Index_);
        SetPGUID(rhs.PGUID);
        SetUPGUID(rhs.UPGUID);
        SetS_UDTIME(rhs.S_UDTIME);
        SetSYNC_TIME(rhs.SYNC_TIME);
        SetJdText(rhs.JdText);
        SetJdType(rhs.JdType);
        SetDevName(rhs.DevName);
        SetIconSxName(rhs.IconSxName);
        SetMapGuid(rhs.MapGuid);
        SetDevID(rhs.DevID);
        return *this;
    }
    

private:
    int Id;
    void __fastcall SetID(int value);
    int __fastcall GetID();

    int IsDelete;
    void __fastcall SetIsDelete(int value);
    int __fastcall GetIsDelete();

    int jdcontenttype;
    void __fastcall SetJdContentType(int value);
    int __fastcall GetJdContentType();

    int index;
    void __fastcall SetIndex(int value);
    int __fastcall GetIndex();

    String pguid;
    void __fastcall SetPGUID(String str);
    String __fastcall GetPGUID();

    String upguid;
    void __fastcall SetUPGUID(String str);
    String __fastcall GetUPGUID();

    String sudtime;
    void __fastcall SetS_UDTIME(String str);
    String __fastcall GetS_UDTIME();

    String synctime;
    void __fastcall SetSYNC_TIME(String str);
    String __fastcall GetSYNC_TIME();

    String jdtext;
    void __fastcall SetJdText(String str);
    String __fastcall GetJdText();

    String jdtype;
    void __fastcall SetJdType(String str);
    String __fastcall GetJdType();

    String devname;
    void __fastcall SetDevName(String str);
    String __fastcall GetDevName();

    String iconsxname;
    void __fastcall SetIconSxName(String str);
    String __fastcall GetIconSxName();

    String mapguid;
    void __fastcall SetMapGuid(String str);
    String __fastcall GetMapGuid();

    String devid;
    void __fastcall SetDevID(String str);
    String __fastcall GetDevID();
};

// YJZH_SheJiYuan ����
class TData2 {
public:
    TData2();
    ~TData2();
    __property int ID = { read = GetID, write = SetID };
    __property int ISDELETE = { read = GetIsDelete, write = SetIsDelete };
    __property int OrgID = { read = GetOrgID, write = SetOrgID };
    __property int ShowIndex = { read = GetShowIndex, write = SetShowIndex };
    __property String PGUID = {read = GetPGUID, write = SetPGUID };
    __property String UPGUID = {read = GetUPGUID, write = SetUPGUID };
    __property String S_UDTIME = {read = GetS_UDTIME, write = SetS_UDTIME };
    __property String SYNC_TIME = {read = GetSYNC_TIME, write = SetSYNC_TIME };
    __property String TypeName = {read = GetTypeName, write = SetTypeName };
    __property String MapID = {read = GetMapID, write = SetMapID };
    __property String MapName = {read = GetMapName, write = SetMapName };
    TData2& operator = (const TData2& rhs) {
        SetID(rhs.ID);
        SetIsDelete(rhs.ISDELETE);
        SetOrgID(rhs.OrgID);
        SetShowIndex(rhs.ShowIndex);
        SetPGUID(rhs.PGUID);
        SetUPGUID(rhs.UPGUID);
        SetS_UDTIME(rhs.S_UDTIME);
        SetSYNC_TIME(rhs.SYNC_TIME);
        SetTypeName(rhs.TypeName);
        SetMapID(rhs.MapID);
        SetMapName(rhs.MapName);
        return *this;
    }
    

private:
    int Id;
    void __fastcall SetID(int value);
    int __fastcall GetID();

    int IsDelete;
    void __fastcall SetIsDelete(int value);
    int __fastcall GetIsDelete();

    int orgid;
    void __fastcall SetOrgID(int value);
    int __fastcall GetOrgID();

    int showindex;
    void __fastcall SetShowIndex(int value);
    int __fastcall GetShowIndex();

    String pguid;
    void __fastcall SetPGUID(String str);
    String __fastcall GetPGUID();

    String upguid;
    void __fastcall SetUPGUID(String str);
    String __fastcall GetUPGUID();

    String sudtime;
    void __fastcall SetS_UDTIME(String str);
    String __fastcall GetS_UDTIME();

    String synctime;
    void __fastcall SetSYNC_TIME(String str);
    String __fastcall GetSYNC_TIME();

    String type_name;
    void __fastcall SetTypeName(String str);
    String __fastcall GetTypeName();

    String mapid;
    void __fastcall SetMapID(String str);
    String __fastcall GetMapID();

    String mapname;
    void __fastcall SetMapName(String str);
    String __fastcall GetMapName();
};

#endif
