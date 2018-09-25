//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SystemUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "csDataTypeDef_ocxProj1_OCX"
#pragma link "AdvGrid"
#pragma link "BaseGrid"
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormCreate(TObject *Sender)
{
    this->csDataTypeDef_ocx1->DataBaseType =  1  ;
    this->csDataTypeDef_ocx1->DBFilePath = ExtractFilePath(Application->ExeName)+"data\\ZSK_H0000Z000K06.mdb";
    this->csDataTypeDef_ocx1->DBtbqz = "H0000Z000K06";
    this->csDataTypeDef_ocx1->InitShow(WideString(Form2->Edit->Text),WideString(""),true) ;
}
//---------------------------------------------------------------------------

