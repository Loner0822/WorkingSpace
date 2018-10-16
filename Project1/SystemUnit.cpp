//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "SystemUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "csDataTypeDef_ocxProj1_OCX"
#pragma link "AdvGrid"
#pragma link "BaseGrid"
#pragma link "AdvGrid"
#pragma link "BaseGrid"
#pragma link "csDataTypeDef_ocxProj1_OCX"
#pragma resource "*.dfm"
TForm2 *Form2;
String Now_Parm;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
const char* newGUID()
{
    static char buf[64] = {0};
    GUID guid;
    if (S_OK == ::CoCreateGuid(&guid))
    {
        _snprintf(buf, sizeof(buf),
                  "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                  guid.Data1,
                  guid.Data2,
                  guid.Data3,
                  guid.Data4[0], guid.Data4[1],
                  guid.Data4[2], guid.Data4[3],
                  guid.Data4[4], guid.Data4[5],
                  guid.Data4[6], guid.Data4[7]
                    );
    }
    return (const char*)buf;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormCreate(TObject *Sender)
{
    this->csDataTypeDef_ocx1->DataBaseType =  1  ;
    this->csDataTypeDef_ocx1->DBFilePath = ExtractFilePath(Application->ExeName)+"Data\\ZSK_H0000Z000K06.mdb";
    this->csDataTypeDef_ocx1->DBtbqz = "H0000Z000K06";

    AdvStringGrid1->Clear();
    AdvStringGrid1->Options << goEditing;
    //AdvStringGrid1->Options << goRowSelect;
    AdvStringGrid1->Options << goColSizing;
    AdvStringGrid1->Options >> goRowSizing;
    AdvStringGrid1->RowCount = 2;
    AdvStringGrid1->ColCount = 4;
    AdvStringGrid1->FixedRows = 1;
    AdvStringGrid1->FixedCols = 1;
    AdvStringGrid1->ColWidths[0] = 32;
    AdvStringGrid1->ColWidths[2] = 0;
    AdvStringGrid1->ColWidths[3] = 0;
    AdvStringGrid1->Cells[0][0] = "序号";
    AdvStringGrid1->Cells[1][0] = "数据类型";

    TADOQuery *tempQuery = new TADOQuery(NULL);
    tempQuery->Connection = DMod->ADOConnection3;
    String sql = "select PGUID, PARM, DEPARTMENT from ZSK_PARM_H0000Z000K06 where ISDELETE = 0";
    DMod->OpenSql(sql + sql_Dep + " order by Index_ asc, ID asc", tempQuery);
    int cnt = 0;
    while (!tempQuery->Eof) {
        AdvStringGrid1->AddRow();
        ++ cnt;
        AdvStringGrid1->Cells[0][cnt] = cnt;
        AdvStringGrid1->Cells[1][cnt] = tempQuery->FieldByName("PARM")->AsString;
        AdvStringGrid1->Cells[2][cnt] = tempQuery->FieldByName("PGUID")->AsString;
        AdvStringGrid1->Cells[3][cnt] = tempQuery->FieldByName("DEPARTMENT")->AsString;
        tempQuery->Next();
    }
    if (AdvStringGrid1->Cells[1][cnt + 1] != "")
        AdvStringGrid1->AddRow();
    delete tempQuery;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::AdvStringGrid1CellValidate(TObject *Sender,
      int ACol, int ARow, AnsiString &Value, bool &Valid)
{
    TADOQuery *tempQuery = new TADOQuery(NULL);
    tempQuery->Connection = DMod->ADOConnection3;
    if (ARow == AdvStringGrid1->RowCount - 1) {
        if (Value == "" ) {
            if (Now_Parm == "")
                return;
            else {
                String sql = "update ZSK_PARM_H0000Z000K06 set ISDELETE = 1 where PARM = '" + Now_Parm + "'";
                DMod->ExecSql(sql + sql_Dep, tempQuery);
                Value = AdvStringGrid1->Cells[ACol][ARow + 1];
                AdvStringGrid1->RemoveRows(ARow, 1);
                for (int i = 1; i < AdvStringGrid1->RowCount - 1; ++ i)
                    AdvStringGrid1->Cells[0][i] = i;
                return;
            }
        }
        CoInitialize(NULL);
        String pguid = newGUID();
        String sql = "select * from ZSK_PARM_H0000Z000K06 where PARM = '" + Value + "' and ISDELETE = 0";
        DMod->OpenSql(sql + sql_Dep, tempQuery);
        if (!tempQuery->Eof) {
            delete tempQuery;
            Valid = false;
            ShowMessage(Value + "已经被定义");
            Value = "";
            return;
        }
        sql = "insert into ZSK_PARM_H0000Z000K06 (PGUID, S_UDTIME, PARM, DEPARTMENT) values('" + pguid + "', '" + Now().FormatString("yyyy-MM-dd hh:mm:ss") + "', '" + Value + "', '" + Department + "')";
        DMod->ExecSql(sql, tempQuery);
        AdvStringGrid1->AddRow();
        AdvStringGrid1->Cells[0][ARow] = ARow;
        AdvStringGrid1->Cells[2][ARow] = pguid;
    }
    else {
        if (Value == "") {
            //String sql = "delete * from ZSK_PARM_H0000Z000K06 where PARM = '" + Now_Parm + "'";
			String sql = "update ZSK_PARM_H0000Z000K06 set ISDELETE = 1 where PARM = '" + Now_Parm + "'";
            DMod->ExecSql(sql + sql_Dep, tempQuery);
            Value = AdvStringGrid1->Cells[ACol][ARow + 1];
            AdvStringGrid1->RemoveRows(ARow, 1);
            for (int i = 1; i < AdvStringGrid1->RowCount - 1; ++ i)
                AdvStringGrid1->Cells[0][i] = i;
        }
        else {
            String sql = "select * from ZSK_PARM_H0000Z000K06 where PARM = '" + Value + "' and ISDELETE = 0";
            DMod->OpenSql(sql + sql_Dep, tempQuery);
            if (!tempQuery->Eof) {
                delete tempQuery;
                Valid = false;
                ShowMessage(Value + "已经被定义");
                Value = "";
                return;
            }
            sql = "update ZSK_PARM_H0000Z000K06 set PARM = '" + Value + "' where PARM = '" + Now_Parm + "' and ISDELETE = 0";
            DMod->ExecSql(sql + sql_Dep, tempQuery);
        }
    }
    delete tempQuery;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::AdvStringGrid1CanEditCell(TObject *Sender,
      int ARow, int ACol, bool &CanEdit)
{
    if (AdvStringGrid1->Cells[3][ARow] != Department && AdvStringGrid1->Cells[1][ARow] != "") {
		CanEdit = false;
		return;
	}
    Now_Parm = AdvStringGrid1->Cells[ACol][ARow];
}
//---------------------------------------------------------------------------

void __fastcall TForm2::AdvStringGrid1RowMoved(TObject *Sender,
      int FromIndex, int ToIndex)
{
    //
    TADOQuery *tempQuery = new TADOQuery(NULL);
    tempQuery->Connection = DMod->ADOConnection3;
    for (int i = 1; i < AdvStringGrid1->RowCount - 1; ++ i) {
        String sql = "update ZSK_PARM_H0000Z000K06 set Index_ = " + IntToStr(i) + " where PARM = '" + AdvStringGrid1->Cells[1][i] + "' and ISDELETE = 0";
        DMod->ExecSql(sql, tempQuery);
        AdvStringGrid1->Cells[0][i] = i;
    }
    delete tempQuery;
    //AdvStringGrid1->MoveRow()
}
//---------------------------------------------------------------------------

void __fastcall TForm2::AdvStringGrid1RowMove(TObject *Sender, int ARow,
      bool &Allow)
{
    if (ARow >= 1 && ARow < AdvStringGrid1->RowCount - 1)
        Allow = 1;
    else
        Allow = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::AdvStringGrid1RowMoving(TObject *Sender, int ARow,
      bool &Allow)
{
    if (AdvStringGrid1->Cells[1][ARow] != "")
        Allow = 1;
    else
        Allow = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::AdvStringGrid1SelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    this->csDataTypeDef_ocx1->InitShow(WideString(AdvStringGrid1->Cells[2][ARow]),WideString(""),true);
}
//---------------------------------------------------------------------------


void __fastcall TForm2::AdvStringGrid1DblClickCell(TObject *Sender,
      int ARow, int ACol)
{
    if (AdvStringGrid1->Cells[3][ARow] == "")
        return;
    int u_id = StrToInt(AdvStringGrid1->Cells[3][ARow]);
    if (AdvStringGrid1->Cells[3][ARow] != Department && AdvStringGrid1->Cells[1][ARow] != "") {
		ShowMessage("无法修改上级" + Department_Name[u_id] + "的数据");
		return;
	}
}
//---------------------------------------------------------------------------

