//---------------------------------------------------------------------------

#include <vcl.h>
#include <map>
#include <set>
#include <vector>
#include <typeinfo>
#include <objbase.h>
#include <cstdio>
#pragma hdrstop
#include "DModUnit.h"
#include "MainUnit.h"
#include "DataUnit.h"
#include "TreeUnit.h"
#include "PenUnit.h"
#include "ExtraUnit.h"
#include "SystemUnit.h"
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "BaseGrid"
#pragma link "SigViewerLib_OCX"
#pragma link "AdvGrid"
#pragma link "BaseGrid"
#pragma link "SigViewerLib_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
map<String, int> P_ID;
multimap<String, TData2> Find_Map;
DynamicArray<String> Map_PGUID;
DynamicArray< TNode<TData> > node;
DynamicArray<TEdge> edge;
int num_of_pic = 0;

int used_red_pen, used_black_pen, used_clamp_pen;
int using_red, using_black, using_clamp;
bool ClickDown;
int Partner;
pair<long, long> Coordinate;
const long dx[4] = {0, 35, 3, 6};
const long dy[4] = {0, 23, 22, 20};
set<pair<long, long> > Map_Node;    // x, y
set<Pen> Pen_Node;    				// 表笔编号, 表笔类型, x, y

int Now_Node;
String Now_Nature;

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
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GetDataButtonClick(TObject *Sender) {
    /*this->TreeView->Items->Add(NULL, "123"); // 添加节点
    TTreeNode* abc = this->TreeView->Items->Add(NULL, "123");
    this->TreeView->Items->AddChild(abc, "2323");
    this->TreeView->Items->Add(NULL, "123");*/
    //TTreeNode* root = NULL;
    //FindSon(root, "");
}
//---------------------------------------------------------------------------
void TForm1::BuildTree() {
    for (int i = node.Low; i <= node.High; ++ i) {
         //ShowMessage(node[i].Data.UPGUID);
         if (node[i].Data.UPGUID != "") {
            int fa = P_ID.find(node[i].Data.UPGUID) -> second;
            if (fa > 0)
                AddEdge(fa - 1, i);
        }
    }
    P_ID.clear();
    for (int i = node.High; i >= node.Low; -- i) {
        TData tmpdata = node[i].Data;
        if (tmpdata.UPGUID == "") {
            TTreeNode* parent = this -> TreeView -> Items -> Add(NULL, tmpdata.JdText);
            //ShowMessage(tmpdata.JdText);
            //parent -> Data = i;
            FindSon(i, parent);
        }
    }
    //node.Length = 0;
    //edge.Length = 0;
}
//---------------------------------------------------------------------------
void TForm1::ReadData(TADOQuery *AdoQuery) {
    TData tmpdata;
    tmpdata.ID = AdoQuery -> FieldByName("ID") -> AsInteger;
    tmpdata.ISDELETE = AdoQuery -> FieldByName("ISDELETE") -> AsInteger;
    tmpdata.JdContentType = AdoQuery -> FieldByName("JdContentType") -> AsInteger;
    tmpdata.Index_ = AdoQuery -> FieldByName("Index_") -> AsInteger;
    tmpdata.PGUID = AdoQuery -> FieldByName("PGUID") -> AsString;
    tmpdata.UPGUID = AdoQuery -> FieldByName("UPGUID") -> AsString;
    tmpdata.S_UDTIME = AdoQuery -> FieldByName("S_UDTIME") -> AsString;
    tmpdata.SYNC_TIME = AdoQuery -> FieldByName("SYNC_TIME") -> AsString;
    tmpdata.JdText = AdoQuery -> FieldByName("JdText") -> AsString;
    tmpdata.JdType = AdoQuery -> FieldByName("JdType") -> AsString;
    tmpdata.DevName = AdoQuery -> FieldByName("DevName") -> AsString;
    tmpdata.IconSxName = AdoQuery -> FieldByName("IconSxName") -> AsString;
    tmpdata.MapGuid = AdoQuery -> FieldByName("MapGuid") -> AsString;
    tmpdata.DevID = AdoQuery -> FieldByName("DevID") -> AsString;
    node.Length ++;
    node[node.High].Data = tmpdata;
    node[node.High].Head = -1;
    node[node.High].fa = node.High;
}
//---------------------------------------------------------------------------
void TForm1::AddEdge(int x, int y) {
    edge.Length++;
    edge[edge.High].to = y;
    edge[edge.High].next = node[x].Head;
    node[x].Head = edge.High;
    node[y].fa = x;
}
//---------------------------------------------------------------------------
/*void TForm1::FindSon(TTreeNode* node, String uid) {
    String sql = "";
    sql = "select * from YJZH_MapFenLei where ISDELETE = 0 and UPGUID = '" + uid + "'";
    TADOQuery *TempADOQuery = new TADOQuery(NULL);
    TempADOQuery -> Connection = DMod -> ADOConnection;
    DMod -> OpenSql(sql, TempADOQuery);
    while (!TempADOQuery -> Eof) {
        String jdtext = TempADOQuery -> FieldByName("JdText") -> AsString;
        String pguid = TempADOQuery -> FieldByName("PGUID") -> AsString;
        TTreeNode* parent;
        if (node == NULL)
            parent = this -> TreeView -> Items -> Add(node, jdtext);
        else
            parent = this -> TreeView -> Items -> AddChild(node, jdtext);
        FindSon(parent, pguid);
        TempADOQuery -> Next();
    }
    delete TempADOQuery;
}*/
//---------------------------------------------------------------------------
void TForm1::FindSon(int u, TTreeNode* tnode) {
    tnode -> Data = (void *)u;
    for (int i = node[u].Head; i != -1; i = edge[i].next) {
        TTreeNode* pa;
        int v = edge[i].to;
        pa = this -> TreeView -> Items -> AddChild(tnode, node[v].Data.JdText);
        FindSon(v, pa);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AdvStringGrid1EditCellDone(TObject *Sender,
      int ACol, int ARow)
{
    // 退出触发
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AdvStringGrid1CellValidate(TObject *Sender,
      int ACol, int ARow, AnsiString &Value, bool &Valid)
{
    TADOQuery *tempQuery = new TADOQuery(NULL);
    tempQuery -> Connection = DMod -> ADOConnection3;
    if (ARow == AdvStringGrid1 -> RowCount - 1) {
        if (Value == "")
            return;
        CoInitialize(NULL);
        String pguid = newGUID();
        String sql = "insert into Nature (PGUID, 属性, UPGUID) values('" + pguid + "', '"+ Value + "', '" + node[Now_Node].Data.PGUID + "')";
        DMod -> ExecSql(sql, tempQuery);
        AdvStringGrid1->AddRow();
        AdvStringGrid1 -> Cells[0][ARow] = ARow;
        AdvStringGrid1 -> Cells[1][ARow + 1] = node[Now_Node].Data.JdText;
    }
    else {
        if (Value == "") {
            String sql = "delete * from Nature where 属性 = '" + Now_Nature + "' and UPGUID = '" + node[Now_Node].Data.PGUID + "'";
            //ShowMessage(sql);
            DMod -> ExecSql(sql, tempQuery);
            Value = AdvStringGrid1->Cells[ACol][ARow + 1];
            AdvStringGrid1->RemoveRows(ARow, 1);
            for (int i = 1; i < AdvStringGrid1->RowCount - 1; ++ i)
                AdvStringGrid1->Cells[0][i] = i;
        }
        else {
            String sql = "update Nature set 属性 = '" + Value + "' where 属性 = '" + Now_Nature + "' and UPGUID = '" + node[Now_Node].Data.PGUID + "'";
            DMod -> ExecSql(sql, tempQuery);
        }
    }
    //ShowMessage(Value);
    //ShowMessage(AdvStringGrid1->Cells[ACol][ARow]);
    delete tempQuery;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AdvStringGrid1CanEditCell(TObject *Sender,
      int ARow, int ACol, bool &CanEdit)
{
    // 编辑前可触发
    if (ACol == 1) {
        CanEdit = false;
        return;
    }
    if (AdvStringGrid1->Cells[1][ARow] != node[Now_Node].Data.JdText) {
        CanEdit = false;
        return;
    }
    Now_Nature = AdvStringGrid1->Cells[2][ARow];
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AdvStringGrid1RowMove(TObject *Sender, int ARow,
      bool &Allow)
{
    //
    if (AdvStringGrid1->Cells[1][ARow] != node[Now_Node].Data.JdText)
        Allow = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AdvStringGrid1RowMoved(TObject *Sender,
      int FromIndex, int ToIndex)
{
    TADOQuery *tempQuery = new TADOQuery(NULL);
    tempQuery -> Connection = DMod -> ADOConnection3;
    for (int i = 1; i < AdvStringGrid1->RowCount - 1; ++ i) {
        String sql = "update Nature set Index_ = " + IntToStr(i) + " where 属性 = '" + AdvStringGrid1->Cells[2][i] + "' and UPGUID = '" + node[Now_Node].Data.PGUID + "'";
        //ShowMessage(sql);
        DMod-> ExecSql(sql, tempQuery);
        AdvStringGrid1->Cells[0][i] = i;
    }
    delete tempQuery;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AdvStringGrid1RowMoving(TObject *Sender, int ARow,
      bool &Allow)
{
    //
    if (AdvStringGrid1->Cells[1][ARow] == node[Now_Node].Data.JdText && AdvStringGrid1->Cells[2][ARow] != "")
        Allow = 1;
    else
        Allow = 0;

}
//---------------------------------------------------------------------------
void TForm1::FindFather(TTreeNode *tnode, int &level) {
    int u = (int)tnode->Data;
    TTreeNode* pa = tnode->Parent;
    if (node[u].fa != u)
        FindFather(pa, level);
    String pguid = node[u].Data.PGUID;
    TADOQuery *tempQuery = new TADOQuery(NULL);
    tempQuery -> Connection = DMod -> ADOConnection3;
    String sql = "select * from Nature where UPGUID = '" + pguid + "' order by Index_";
    DMod -> OpenSql(sql, tempQuery);
    if (tempQuery -> Eof) {
        delete tempQuery;
        return;
    }
    int cnt = 0;
    while (!tempQuery -> Eof) {
        AdvStringGrid1 -> Cells[0][level + cnt] = level + cnt;
        AdvStringGrid1 -> Cells[1][level + cnt] = node[u].Data.JdText;
        AdvStringGrid1 -> Cells[2][level + cnt]  = tempQuery -> FieldByName("属性") -> AsString;
        tempQuery -> Next();
        ++ cnt;
        AdvStringGrid1->AddRow();
    }
    level += cnt;
    delete tempQuery;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewChange(TObject *Sender, TTreeNode *Node)
{
    Now_Node = (int)Node -> Data;
    Map_PGUID.Length = 0;
    AdvStringGrid -> Clear();
    AdvStringGrid -> RowCount = 2;
    AdvStringGrid -> Cells[0][0] = "类型名称";
    AdvStringGrid -> Cells[1][0] = "图像名称";
    String name = Node -> Text;
    int tindex = (int)Node -> Data;
    //ShowMessage(node[tindex].Data.JdText);
    String uid = node[tindex].Data.PGUID;
    multimap<String, TData2>::iterator it;
    int cnt = 0;
    for (it = Find_Map.equal_range(uid).first; it != Find_Map.equal_range(uid).second; ++ it)
        ++ cnt;
    if (cnt == 0)
        cnt = 1;
    AdvStringGrid -> RowCount = cnt + 1;
    AdvStringGrid -> FixedRows = 1;
    //ShowMessage((*it).second.TypeName);
    for (cnt = 1, it = Find_Map.equal_range(uid).first; it != Find_Map.equal_range(uid).second; ++ it, ++ cnt) {
        AdvStringGrid -> Cells[0][cnt] = it -> second.TypeName;
        AdvStringGrid -> Cells[1][cnt] = it -> second.MapName;
        Map_PGUID.Length ++;
        Map_PGUID[cnt - 1] = it -> second.MapID;
    }
    //AdvStringGrid -> AutoSize = true;
    AdvStringGrid -> AutoSizeColumns(True, 4);
    AdvStringGrid -> Options << goColSizing;
    AdvStringGrid -> Options << goRowSizing;
    AdvStringGrid -> Options << goRowSelect;
    AdvStringGrid -> WordWrap = true;
    //ShowMessage(AdvStringGrid -> ColWidths[0]);
    //ShowMessage(AdvStringGrid -> ColWidths[1]);
    GroupBox2 -> Width = AdvStringGrid -> ColWidths[0] + AdvStringGrid -> ColWidths[1] + 8;
    //ShowMessage(GroupBox2 -> Width);
    AdvStringGridClickCell(AdvStringGrid, 1, 0);
    //初始化属性列表
    /*
    TADOQuery *tempQuery = new TADOQuery(NULL);
    tempQuery -> Connection = DMod -> ADOConnection3;
    String sql = "delete * from Nature";
    DMod -> ExecSql(sql, tempQuery);
    */
    AdvStringGrid1 -> Clear();
    AdvStringGrid1 -> Options << goEditing;
    AdvStringGrid1 -> Options << goColSizing;
    AdvStringGrid1 -> Options >> goRowSizing;
    AdvStringGrid1 -> RowCount = 2;
    AdvStringGrid1 -> ColCount = 3;
    AdvStringGrid1 -> FixedRows = 1;
    AdvStringGrid1 -> FixedCols = 1;
    AdvStringGrid1->ColWidths[0] = 32;
    AdvStringGrid1 -> Cells[0][0] = "序号";
    AdvStringGrid1 -> Cells[1][0] = "结点名称";
    AdvStringGrid1 -> Cells[2][0] = "所带属性";

    FindFather(Node, 1);
    AdvStringGrid1 -> Cells[1][AdvStringGrid1 -> RowCount - 1] = node[tindex].Data.JdText;
    AdvStringGrid1->Options << goRowMoving;
    //ShowMessage(AdvStringGrid1 -> RowCount);
    //AdvStringGrid1->ColCount = 3;
    //AdvStringGrid1->MergeCells(1, 0, AdvStringGrid1->ColCount - 1, 1);
    //AdvStringGrid1 -> AutoSizeColumns(True, 4);

    AdvStringGrid2->Clear();
    AdvStringGrid2 -> Options << goEditing;
    AdvStringGrid2 -> Options << goColSizing;
    AdvStringGrid2 -> Options >> goRowSizing;
    AdvStringGrid2 -> RowCount = 2;
    AdvStringGrid2 -> ColCount = 3;
    AdvStringGrid2 -> FixedRows = 1;
    AdvStringGrid2 -> FixedCols = 1;
    AdvStringGrid2 -> ColWidths[0] = 32;
    AdvStringGrid2 -> Cells[0][0] = "序号";
    AdvStringGrid2 -> Cells[1][0] = "数据类型";
    AdvStringGrid2 -> Cells[2][0] = "值";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    // SigViewer 初始化
    SigViewer1->Init( L"123", L"" );
    SigViewer1->ShowToolbar = false;
    Panel -> Visible = true;
    // AdvStringGird 初始化
    AdvStringGrid -> Clear();
    AdvStringGrid -> RowCount = 2;
    AdvStringGrid -> Cells[0][0] = "类型名称";
    AdvStringGrid -> Cells[1][0] = "图像名称";
    AdvStringGrid -> AutoSizeColumns(true, 4);
    AdvStringGrid -> Options << goColSizing;
    AdvStringGrid -> Options << goRowSizing;
    AdvStringGrid -> Options << goRowSelect;
    AdvStringGrid -> WordWrap = true;
    GroupBox2 -> Width = AdvStringGrid -> ColWidths[0] * 2 + 8;
    // 读取数据库并存入DynamicArray
    TADOQuery *AdoQ = new TADOQuery(NULL);
    node.Length = 0;
    edge.Length = 0;
    AdoQ -> Connection = DMod -> ADOConnection1;
    String sql = "select * from YJZH_MapFenLei where ISDELETE = 0 order by Index_ desc";
    DMod -> OpenSql(sql, AdoQ);
    int cnt = 0;
    while (!AdoQ -> Eof) {
        ++ cnt;
        ReadData(AdoQ);
        TData tmpdata;
        tmpdata = node[node.High].Data;
        //ShowMessage(tmpdata.PGUID);
        P_ID[tmpdata.PGUID] = cnt;
        AdoQ -> Next();
    }
    AdoQ = new TADOQuery(NULL);
    AdoQ -> Connection = DMod -> ADOConnection1;
    sql = "select * from YJZH_SheJiYuan where ISDELETE = 0 ";
    DMod -> OpenSql(sql, AdoQ);
    while (!AdoQ -> Eof) {
        ReadData2(AdoQ);
        AdoQ -> Next();
    }
    this -> TreeView -> Items -> Clear();
    BuildTree();
    AdvStringGridClickCell(AdvStringGrid, 1, 0);
    delete AdoQ;
}
//---------------------------------------------------------------------------
void TForm1::ReadData2(TADOQuery *AdoQuery) {
    TData2 tmpdata;
    tmpdata.ID = AdoQuery -> FieldByName("ID") -> AsInteger;
    tmpdata.ISDELETE = AdoQuery -> FieldByName("ISDELETE") -> AsInteger;
    tmpdata.OrgID = AdoQuery -> FieldByName("OrgID") -> AsInteger;
    tmpdata.ShowIndex = AdoQuery -> FieldByName("ShowIndex") -> AsInteger;
    tmpdata.PGUID = AdoQuery -> FieldByName("PGUID") -> AsString;
    tmpdata.UPGUID = AdoQuery -> FieldByName("UPGUID") -> AsString;
    tmpdata.S_UDTIME = AdoQuery -> FieldByName("S_UDTIME") -> AsString;
    tmpdata.SYNC_TIME = AdoQuery -> FieldByName("SYNC_TIME") -> AsString;
    tmpdata.TypeName = AdoQuery -> FieldByName("TypeName") -> AsString;
    tmpdata.MapID = AdoQuery -> FieldByName("MapID") -> AsString;
    tmpdata.MapName = AdoQuery -> FieldByName("MapName") -> AsString;
    //multimap<String, TData2>::iterator it;
    Find_Map.insert(pair<String, TData2>(tmpdata.UPGUID, tmpdata));
}
//---------------------------------------------------------------------------
void TForm1::emf_Analysis() {
    /*
        储存上一张图的数据
    */
    // 初始化
    Map_Node.clear();
    Pen_Node.clear();
    ClickDown = 0;
    using_red = using_black = using_clamp = 0;
    used_red_pen = used_black_pen = used_clamp_pen = 0;
    /*
        读取这张图的数据
    */
    String file = "1.emf";
    TMemoryStream *stream = new TMemoryStream();
    stream->LoadFromFile( file );
    stream->Position = 0;
    int pos = 0;
    stream->Seek(-4,soFromEnd);
    stream->Read( &pos, 4 );
    int HeadBeginPos = pos - 1;
    stream->Seek( HeadBeginPos, soFromBeginning );

    char header[2];
    stream->Read( header, 2 );

    int LinkPointPos = HeadBeginPos - 4;
    stream->Seek( LinkPointPos, soFromBeginning );
    stream->Read( &LinkPointPos, 4 );
    stream->Seek( LinkPointPos - 1, soFromBeginning );

    ExplainLinkPoint( stream );
}
//---------------------------------------------------------------------------
void TForm1::ExplainLinkPoint( TStream* pStream )
{
    char flag[2];
    pStream->Read( flag, 2 );
    if( flag[0] == 'C' && flag[1] == 'N' )
    {   //版本
        int m_ver = 0;
        pStream->Read( &m_ver, 4 );
        if( m_ver == 0xFFFF0000 + 1 || m_ver == 0xFFFF0000 + 2 )
        {
            //图纸ID
            int FDrawingID = 0;
            pStream->Read( &FDrawingID, 4 );
            //LogOut("图纸ID:" + AnsiString(FDrawingID) );
            //  RECT
            TRect rect;
            pStream->Read( &rect.left, 4 );
            pStream->Read( &rect.top, 4 );
            pStream->Read( &rect.right, 4 );
            pStream->Read( &rect.bottom, 4 );
            //LogOut( "RECT:" + AnsiString( rect.Left ) + " " +  AnsiString( rect.Top )+ " " + AnsiString( rect.Right )+ " " + AnsiString( rect.Bottom ) );
            //数量
            int count = 0;
            pStream->Read( &count, 4 );
            //LogOut("数量:" + AnsiString(count) );
            //
            for( int i = 0; i < count; i++ )
            {
                //LogOut("------------------------------------------------");
                //id
                int id = 0;
                pStream->Read( &id, 4 );
                //LogOut("  所属图符id:" + AnsiString(id) );
                //group
                int group = 0;
                pStream->Read( &group, 4 );
                //LogOut("  分组:" + AnsiString(group) );
                //guid
                short guid_length = 0;
                pStream->Read( &guid_length, 2 );
                //LogOut("  GUID长度:" + AnsiString(guid_length) );
                //guid
                AnsiString m_guid = "";
                m_guid.SetLength( guid_length );
                pStream->Read( m_guid.c_str(), guid_length );
                //LogOut("  GUID:" + AnsiString(guid_length) );
                //conncount
                int concount = 0;
                pStream->Read( &concount, 4 );
                //LogOut("  数据长度(个数*4):" + AnsiString(concount) );
                for (int m = 0; m < concount; m+=4)
                {
                    int m_index = 0;
                    pStream->Read( &m_index, 4 );
                    int type = 0;
                    pStream->Read( &type, 4 );

                    int x = 0;
                    pStream->Read( &x, 4 );

                    int y = 0;
                    pStream->Read( &y, 4 );
                    if (m_index < 0)
                        continue;
                    Map_Node.insert(make_pair(x, y));
                    /*LogOut("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
                    LogOut("  m_index:" + AnsiString(m_index) );
                    LogOut("  type:" + AnsiString(type) );
                    LogOut("  x:" + AnsiString(x) );
                    LogOut("  y:" + AnsiString(y) );
                    */
                   // m_index >= 0 表示连接点
                }
            }     
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AdvStringGridClickCell(TObject *Sender, int ARow,
      int ACol)
{
    Panel -> Visible = true;
    if (Map_PGUID.Length < ARow)
        return;
    TADOQuery *AdoQ = new TADOQuery(NULL);
    AdoQ -> Connection = DMod -> ADOConnection2;
    String sql = "select EmfMapData from JXMAP where PGUID = '" + Map_PGUID[ARow - 1] + "'";
    DMod -> OpenSql(sql, AdoQ);
    //ShowMessage( Map_PGUID[ARow - 1]);
    if (AdoQ -> Eof) {
        delete AdoQ;
        return;
    }
    if((TBlobField*)AdoQ -> FieldByName("EmfMapData") -> IsNull) {
        delete AdoQ;
        return;
    }
    TADOBlobStream* stream = new TADOBlobStream( (TBlobField*)AdoQ -> FieldByName("EmfMapData"), bmRead );
    AnsiString s = ExtractFilePath( Application->ExeName ) + "1.emf";
    stream->SaveToFile(s);
    delete stream;
    Panel -> Visible = false;
    s = ExtractFilePath( Application->ExeName ) + "1.emf";
    SigViewer1 -> OpenDrawingFile(++ num_of_pic , WideString(s), L"" );
    emf_Analysis();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SigViewer1Paint(TObject *Sender, long DC)
{
    HDC hdc = (HDC)DC;
    double zoom =(double)this->SigViewer1->RealZoom/10000;     //缩放系数
    if( zoom == 0)
    {
        zoom = 1;
    }
    String p_path = ExtractFilePath( Application->ExeName )+"picture\\";

    //画红表笔
    TPicture  *  r_pen0;  //红表笔
    long r_x0,r_y0;
    r_pen0 = new TPicture;
    r_pen0->LoadFromFile(p_path+"红表笔.bmp");
    int bmp_width = 0;
    int bmp_height = 0;
    Graphics::TBitmap *bm = r_pen0->Bitmap; //
    bmp_width = bm->Width;
    bmp_height = bm->Height;
    long x0 =  this->SigViewer1->Width - 200;
    SigViewer1->WindowToView(x0, 20, &r_x0, &r_y0);  //屏幕坐标转换成控件坐标
    int orgin[4]; //图片坐标与宽高
    orgin[0] = r_x0;
    orgin[1] = r_y0;
    orgin[2] = bmp_width/zoom;  //保持绘制的图片固定尺寸，不随图纸变大缩小
    orgin[3] = bmp_height/zoom;
    DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
    delete r_pen0;

    //画黑表笔
    TPicture  *  b_pen0;  //黑表笔
    b_pen0 = new  TPicture;
    b_pen0->LoadFromFile(p_path+"黑表笔.bmp");
    long b_x0,b_y0;
    bm = b_pen0->Bitmap;
    bmp_width = bm->Width;
    bmp_height = bm->Height;

    x0 =  this->SigViewer1->Width - 150;
    SigViewer1->WindowToView(x0, 20, &b_x0, &b_y0);
    //图片坐标与宽高
    orgin[0] = b_x0;
    orgin[1] = b_y0;
    orgin[2] = bmp_width/zoom;
    orgin[3] = bmp_height/zoom;
    DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
    delete b_pen0;

    //画电流钳
    TPicture  *c_pen0; //电流钳
    long c_x0,c_y0;
    c_pen0 = new TPicture;
    c_pen0->LoadFromFile(p_path+"电流钳.bmp");
    bm = c_pen0->Bitmap;
    bmp_width = bm->Width;
    bmp_height = bm->Height;
    x0 = this->SigViewer1->Width - 100;
    SigViewer1->WindowToView(x0, 20, &c_x0, &c_y0);
    //图片坐标与宽高
    orgin[0] = c_x0;
    orgin[1] = c_y0;
    orgin[2] = bmp_width/zoom;
    orgin[3] = bmp_height/zoom;
    DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
    delete c_pen0;

    //画矩形框
    this->SigViewer1->DcSetPenColor(DC,clRed) ;
    this->SigViewer1->DcSetPenWidth(DC,1) ;
    this->SigViewer1->DcMoveTo(DC,r_x0-5/zoom ,r_y0-5/zoom);
    this->SigViewer1->DcLineTo(DC,c_x0+bmp_width/zoom+5/zoom ,r_y0-5/zoom);
    this->SigViewer1->DcLineTo(DC,c_x0+bmp_width/zoom+5/zoom ,r_y0+ bmp_height/zoom+5/zoom);
    this->SigViewer1->DcLineTo(DC,r_x0-5/zoom ,r_y0+ bmp_height/zoom+5/zoom);
    this->SigViewer1->DcLineTo(DC,r_x0-5/zoom ,r_y0-5/zoom);

    //
    //this->SigViewer1->DcEllipse(DC,)

    //画移动表笔
    long tx = Coordinate.first, ty = Coordinate.second;
    if (ClickDown == 1) {
        TPicture  *movepen;
        if (using_red) {
            movepen = new TPicture;
            movepen->LoadFromFile(p_path+"红表笔.bmp");
            bm = movepen->Bitmap;
            bmp_width = bm->Width;
            bmp_height = bm->Height;
            bm->Canvas->Font->Color = clRed;
            bm->Canvas->TextOutA(dx[1] - 10, dy[1] - 22, IntToStr(using_red));
            bm->Canvas->Pen->Color = clGreen;
            bm->Canvas->Rectangle(1,1,3,3);
            bm->Canvas->Rectangle(1,12,3,14);
            bm->Canvas->Rectangle(1,23,3,25);
            bm->Canvas->Rectangle(18,1,20,3);
            bm->Canvas->Rectangle(36,1,38,3);
            bm->Canvas->Rectangle(18,23,20,25);
            bm->Canvas->Rectangle(36,12,38,14);
            bm->Canvas->Rectangle(36,23,38,25);
            SigViewer1->WindowToView(tx - dx[1], ty - dy[1], &r_x0, &r_y0);
            orgin[0] = r_x0;
            orgin[1] = r_y0;
            orgin[2] = bmp_width/zoom;
            orgin[3] = bmp_height/zoom;
            DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
        }
        else if(using_black) {
            movepen = new  TPicture;
            movepen->LoadFromFile(p_path+"黑表笔.bmp");
            bm = movepen->Bitmap;
            bmp_width = bm->Width;
            bmp_height = bm->Height;
            bm->Canvas->Font->Color = clBlack;
            bm->Canvas->TextOutA(dx[2], dy[2] - 22, IntToStr(using_black));
            bm->Canvas->Pen->Color = clGreen;
            bm->Canvas->Rectangle(1,1,3,3);
            bm->Canvas->Rectangle(1,12,3,14);
            bm->Canvas->Rectangle(1,23,3,25);
            bm->Canvas->Rectangle(18,1,20,3);
            bm->Canvas->Rectangle(36,1,38,3);
            bm->Canvas->Rectangle(18,23,20,25);
            bm->Canvas->Rectangle(36,12,38,14);
            bm->Canvas->Rectangle(36,23,38,25);
            SigViewer1->WindowToView(tx - dx[2], ty - dy[2], &b_x0, &b_y0);
            orgin[0] = b_x0;
            orgin[1] = b_y0;
            orgin[2] = bmp_width/zoom;
            orgin[3] = bmp_height/zoom;
            DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
        }
        else if (using_clamp){
            movepen = new TPicture;
            movepen->LoadFromFile(p_path+"电流钳.bmp");
            bm = movepen->Bitmap;
            bmp_width = bm->Width;
            bmp_height = bm->Height;
            bm->Canvas->Font->Color = clBlue;
            bm->Canvas->TextOutA(dx[3], dy[3] - 22, IntToStr(using_clamp));
            bm->Canvas->Pen->Color = clGreen;
            bm->Canvas->Rectangle(1,1,3,3);
            bm->Canvas->Rectangle(1,13,3,15);
            bm->Canvas->Rectangle(1,25,3,27);
            bm->Canvas->Rectangle(16,1,18,3);
            bm->Canvas->Rectangle(35,1,37,3);
            bm->Canvas->Rectangle(16,25,18,27);
            bm->Canvas->Rectangle(35,13,37,15);
            bm->Canvas->Rectangle(35,25,37,27);
            SigViewer1->WindowToView(tx - dx[3], ty - dy[3], &c_x0, &c_y0);
            orgin[0] = c_x0;
            orgin[1] = c_y0;
            orgin[2] = bmp_width/zoom;
            orgin[3] = bmp_height/zoom;
            DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
        }
        delete movepen;
    }

    //画选中表笔
    set<Pen>::iterator it;

    for (it = Pen_Node.begin(); it != Pen_Node.end(); ++ it) {
        TPicture  *nodepen;
        tx = it -> x, ty = it -> y;
        if (it -> type == 1) {
            nodepen = new TPicture;
            nodepen->LoadFromFile(p_path+"红表笔.bmp");
            bm = nodepen->Bitmap;
            bmp_width = bm->Width;
            bmp_height = bm->Height;
            bm->Canvas->Font->Color = clRed;
            bm->Canvas->TextOutA(dx[1] - 10, dy[1] - 22, IntToStr(it -> id));
            //SigViewer1->WindowToView(tx - dx[1], ty - dy[1], &r_x0, &r_y0);
            orgin[0] = tx - dx[1] / zoom;
            orgin[1] = ty - dy[1] / zoom;
            orgin[2] = bmp_width/zoom;  //保持绘制的图片固定尺寸，不随图纸变大缩小
            orgin[3] = bmp_height/zoom;
            DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
            this->SigViewer1->DcSetPenColor(DC,clRed);
            this->SigViewer1->DcEllipse(DC, tx - 3 / zoom, ty - 3 / zoom, tx + 3 / zoom, ty + 3 / zoom);
        }
        else if(it -> type == 2) {
            nodepen = new  TPicture;
            nodepen->LoadFromFile(p_path+"黑表笔.bmp");
            bm = nodepen->Bitmap;
            bmp_width = bm->Width;
            bmp_height = bm->Height;
            bm->Canvas->Font->Color = clBlack;
            bm->Canvas->TextOutA(dx[2], dy[2] - 22, IntToStr(it -> id));
            //SigViewer1->WindowToView(tx - dx[2], ty - dy[2], &b_x0, &b_y0);
            orgin[0] = tx - dx[2] / zoom;
            orgin[1] = ty - dy[2] / zoom;
            orgin[2] = bmp_width/zoom;  //保持绘制的图片固定尺寸，不随图纸变大缩小
            orgin[3] = bmp_height/zoom;
            DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
            this->SigViewer1->DcSetPenColor(DC,clBlack);
            this->SigViewer1->DcEllipse(DC, tx - 3 / zoom, ty - 3 / zoom, tx + 3 / zoom, ty + 3 / zoom);
        }
        else {
            nodepen = new TPicture;
            nodepen->LoadFromFile(p_path+"电流钳.bmp");
            bm = nodepen->Bitmap;
            bmp_width = bm->Width;
            bmp_height = bm->Height;
            bm->Canvas->Font->Color = clBlue;
            bm->Canvas->TextOutA(dx[3], dy[3] - 22, IntToStr(it -> id));
            //SigViewer1->WindowToView(tx - dx[3], ty - dy[3], &c_x0, &c_y0);
            orgin[0] = tx - dx[3] / zoom;
            orgin[1] = ty - dy[3] / zoom;
            orgin[2] = bmp_width/zoom;  //保持绘制的图片固定尺寸，不随图纸变大缩小
            orgin[3] = bmp_height/zoom;
            DrawPicture(hdc, orgin, bm->Canvas->Handle, 0, 0, bmp_width, bmp_height, RGB(255, 255, 255));
            this->SigViewer1->DcSetPenColor(DC,clYellow);
            this->SigViewer1->DcEllipse(DC, tx - 3 / zoom, ty - 3 / zoom, tx + 3 / zoom, ty + 3 / zoom);
        }
        delete nodepen;
    }
}
//---------------------------------------------------------------------------
void TForm1::DrawPicture(HDC hdcDest, int Orign[4], HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc,int nHeightSrc, UINT crTransparent)
{
    int nXOriginDest,nYOriginDest, nWidthDest, nHeightDest;
    nXOriginDest =  Orign[0];
    nYOriginDest = Orign[1];
    nWidthDest = Orign[2];
    nHeightDest = Orign[3];

    HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// 创建兼容位图
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// 创建单色掩码位图
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

	// 将源DC中的位图拷贝到临时DC中
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,
		hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	// 设置透明色
	SetBkColor(hImageDC, crTransparent);

	// 生成透明区域为白色，其它区域为黑色的掩码位图
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	// 生成透明区域为黑色，其它区域保持不变的位图
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// 透明部分保持屏幕不变，其它部分变成黑色
	SetBkColor(hdcDest,RGB(0xff,0xff,0xff));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// "或"运算,生成最终效果
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);

	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}
//---------------------------------------------------------------------------
int Check_Pen(long x, long y) {
    if (Form1->SigViewer1->Width - 200 <= x && x <= Form1->SigViewer1->Width - 162
        && 20 <= y && y <= 20 + 25)
        return 1;
    if (Form1->SigViewer1->Width - 150 <= x && x <= Form1->SigViewer1->Width - 112
        && 20 <= y && y <= 20 + 25)
        return 2;
    if (Form1->SigViewer1->Width - 100 <= x && x <= Form1->SigViewer1->Width - 63
        && 20 <= y && y <= 20 + 27)
        return 3;
    return 0;
}
//---------------------------------------------------------------------------
bool Check_Point(long x, long y) {
    set<pair<long, long> >::iterator it;
    it = Map_Node.find(make_pair(x, y));
    if (it == Map_Node.end())
        return 0;
    else
        return 1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SigViewer1MouseDown(TObject *Sender,
      Sigviewerlib_tlb::TxMouseButton Button, long X, long Y, long *Cancel)
{
// 鼠标按下
    set<pair<long, long> >::iterator iter;
    long tx0, ty0;
    SigViewer1->WindowToView(X, Y, &tx0, &ty0);
    for (iter = Map_Node.begin(); iter != Map_Node.end(); ++ iter) {
        int tx = iter -> first, ty = iter -> second;
        if ((tx - tx0) * (tx - tx0) + (ty - ty0) * (ty - ty0) <= 36) {
            tx0 = tx, ty0 = ty;
            break;
        }
    }
    if (Button == 0) {
        if (ClickDown == 0) {
            int pos = Check_Pen(X, Y);
            vector<set<Pen>::iterator> iter_List;
            set<Pen>::iterator it;
            switch(pos) {
                case 0:
                    for (it = Pen_Node.begin(); it != Pen_Node.end(); ++ it) {
                        if (it -> x == tx0 && it -> y == ty0)
                            iter_List.push_back(it);
                    }
                    if (iter_List.size() >= 2) {
                        //ShowMessage("请选择表笔移动");
                        // balabala...
                        Choose_Move_Pen(iter_List);
                    }
                    else if (iter_List.size() == 1){
                        it = iter_List[0];
                        ClickDown = 1;
                        if (it -> type == 1)
                            using_red = it -> id;
                        if (it -> type == 2)
                            using_black = it -> id;
                        if (it -> type == 3)
                            using_clamp = it -> id;
                        Partner = it -> partner;
                        Pen_Node.erase(it);
                    }
                    break;
                case 1:
                    if (used_red_pen > used_black_pen) {
                        ShowMessage("尚有未配对的红表笔");
                        break;
                    }
                    else {
                        ClickDown = 1;
                        ++ used_red_pen;
                        using_red = used_red_pen;
                        Partner = 0;
                    }
                    break;
                case 2:
                    if (used_black_pen > used_red_pen) {
                        ShowMessage("尚有未配对的黑表笔");
                        break;
                    }
                    else {
                        ClickDown = 1;
                        ++ used_black_pen;
                        using_black = used_black_pen;
                        Partner = 0;
                    }
                    break;
                case 3:
                    ClickDown = 1;
                    ++ used_clamp_pen;
                    using_clamp = used_clamp_pen;
                    Partner = 0;
                    break;
            }
        }
        else if (Check_Point(tx0, ty0)){
            ClickDown = 0;
            Pen tmppen;
            set<Pen>::iterator it;
            tmppen.x = tx0, tmppen.y = ty0;
            if (using_red) {
                tmppen.id = using_red;
                using_red = 0;
                tmppen.type = 1;
                tmppen.partner = 0;
                for (it = Pen_Node.begin(); it != Pen_Node.end(); ++ it)
                    if (it -> id == tmppen.id && it -> type == 2) {
                        tmppen.partner = tmppen.id;
                        tx0 = it -> x, ty0 = it -> y;
                        break;
                    }
                if (it != Pen_Node.end()) {
                    Pen_Node.erase(it);
                    Pen_Node.insert(tmppen);
                    tmppen.x = tx0;
                    tmppen.y = ty0;
                    tmppen.type = 2;
                    Pen_Node.insert(tmppen);
                }
                else {
                    Pen_Node.insert(tmppen);
                }
            }
            else if (using_black) {
                tmppen.id = using_black;
                using_black = 0;
                tmppen.type = 2;
                tmppen.partner = 0;
                for (it = Pen_Node.begin(); it != Pen_Node.end(); ++ it)
                    if (it -> id == tmppen.id && it -> type == 1) {
                        tmppen.partner = tmppen.id;
                        tx0 = it -> x, ty0 = it -> y;
                        break;
                    }
                if (it != Pen_Node.end()) {
                    Pen_Node.erase(it);
                    Pen_Node.insert(tmppen);
                    tmppen.x = tx0;
                    tmppen.y = ty0;
                    tmppen.type = 1;
                    Pen_Node.insert(tmppen);
                }
                else {
                    Pen_Node.insert(tmppen);
                }
            }
            else if (using_clamp) {
                tmppen.id = using_clamp;
                using_clamp = 0;
                tmppen.type = 3;
                tmppen.partner = 0;
                Pen_Node.insert(tmppen);
            }
        }
    }
    else {
        if (ClickDown == 0) {
            vector<set<Pen>::iterator> iter_List;
            set<Pen>::iterator it;
            for (it = Pen_Node.begin(); it != Pen_Node.end(); ++ it) {
                if (it -> x == tx0 && it -> y == ty0)
                    iter_List.push_back(it);
            }
            if (iter_List.size() >= 2) {
                //ShowMessage("请选择表笔删除");
                // balabala...
                Choose_Delete_Pen(iter_List);
            }
            else if (iter_List.size() == 1){
                if (iter_List[0] -> partner == 0) {
                    if (iter_List[0] -> type == 1)
                        -- used_red_pen;      
                    if (iter_List[0] -> type == 2)
                        -- used_black_pen;
                    Pen_Node.erase(iter_List[0]);
                }
                else {
                    int PTR = iter_List[0] -> partner;
                    for (it = Pen_Node.begin(); it != Pen_Node.end();) {
                        if (it -> partner == PTR && it -> type != 3) {
                            Pen_Node.erase(it++);
                        }
                        else
                            ++ it;
                    }
                }
            }
        }
        else {
            if (Partner) {
                ClickDown = 0;
                using_red = using_black = using_clamp = 0;
                set<Pen>::iterator it;
                for (it = Pen_Node.begin(); it != Pen_Node.end(); ++ it)
                    if (it -> partner == Partner) {
                        Partner = 0;
                        Pen_Node.erase(it);
                        break;
                    }
            }
            else {
                ClickDown = 0;
                if (using_red) {
                    if (using_red == used_red_pen)
                        using_red = 0, -- used_red_pen;
                    else
                        using_red = 0;
                }
                if (using_black) {
                    if (using_black == used_black_pen)
                        using_black = 0, -- used_black_pen;
                    else
                        using_black = 0;
                }
                if (using_clamp) {
                    if (using_clamp == used_clamp_pen)
                        using_clamp = 0, -- used_clamp_pen;
                    else
                        using_clamp = 0;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SigViewer1MouseMove(TObject *Sender, long X, long Y, long *Cancel)
{
// 鼠标移动
    //*Cancel = 1;
    Coordinate.first = X, Coordinate.second = Y;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SigViewer1MouseUp(TObject *Sender,
      Sigviewerlib_tlb::TxMouseButton Button, long X, long Y, long *Cancel)
{
// 鼠标抬起
}
//---------------------------------------------------------------------------
void TForm1::Choose_Move_Pen(const vector<set<Pen>::iterator> &L) {
    ExtraForm->Caption = "请选择所要移动的表笔";
    ExtraForm->Move_Pen(L);
    ExtraForm->ShowModal();
    set<Pen>::iterator it;
    //ShowMessage(ExtraForm->Move_num);
    it = L[ExtraForm->Move_num];
    ClickDown = 1;
    if (it -> type == 1)
        using_red = it -> id;
    if (it -> type == 2)
        using_black = it -> id;
    if (it -> type == 3)
        using_clamp = it -> id;
    Partner = it -> partner;
    Pen_Node.erase(it);
}
//---------------------------------------------------------------------------
void TForm1::Choose_Delete_Pen(const vector<set<Pen>::iterator> &L) {
    ExtraForm->Caption = "请选择所要删除的表笔";
    ExtraForm->Delete_Pen(L);
    ExtraForm->ShowModal();
    set<Pen>::iterator it;
    //ShowMessage(ExtraForm->Delete_num.size());
    vector<int> v = ExtraForm->Delete_num;
    for (int i = 0; i < v.size(); ++ i) {
        int pos = v[i];
        if (L[pos] -> partner == 0) {
            if (L[pos] -> type == 1)
                -- used_red_pen;
            if (L[pos] -> type == 2)
                -- used_black_pen;
            Pen_Node.erase(L[pos]);
        }
        else {
            int PTR = L[pos] -> partner;
            for (it = Pen_Node.begin(); it != Pen_Node.end();) {
                if (it -> partner == PTR && it -> type != 3) {
                    Pen_Node.erase(it++);
                }
                else
                    ++ it;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimerTimer(TObject *Sender)
{
    ::InvalidateRect((HWND)this->SigViewer1->DrawingWindow, NULL, TRUE);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AdvStringGrid2MouseDown(TObject *Sender,
        TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //
    int row, col;
    AdvStringGrid2->MouseToCell(X, Y, col, row);
    AdvStringGrid2->Col = col;
    AdvStringGrid2->Row = row;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
    //
    Form2->ShowModal();
    //ShowMessage(AdvStringGrid2->ColWidths[0]);
}

void __fastcall TForm1::N2Click(TObject *Sender)
{
    //DBCOLUMNFLAGS_ISDEFAULTSTREAM;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AdvStringGrid2CanEditCell(TObject *Sender, int ARow,
      int ACol, bool &CanEdit)
{
    //
    if (ACol == 2) {
        CanEdit = 0;
        return;
    }
    if (AdvStringGrid2->Cells[ARow][0] == ARow) {
        CanEdit = 0;
        return;
    }
    CanEdit = 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AdvStringGrid2EditCellDone(TObject *Sender,
      int ACol, int ARow)
{
    //
    if (ACol == 1)
        AdvStringGrid2->Cells[0][ARow] = ARow;
    AdvStringGrid2->AddRow();
    Form2->ShowModal();
}
//---------------------------------------------------------------------------



