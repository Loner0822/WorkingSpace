object DMod: TDMod
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Left = 520
  Top = 262
  Height = 150
  Width = 216
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Mode=Share Deny N' +
      'one;Persist Security Info=False;Jet OLEDB:System database="";Jet' +
      ' OLEDB:Registry Path="";Jet OLEDB:Database Password="";Jet OLEDB' +
      ':Engine Type=5;Jet OLEDB:Database Locking Mode=1;Jet OLEDB:Globa' +
      'l Partial Bulk Ops=2;Jet OLEDB:Global Bulk Transactions=1;Jet OL' +
      'EDB:New Database Password="";Jet OLEDB:Create System Database=Fa' +
      'lse;Jet OLEDB:Encrypt Database=False;Jet OLEDB:Don'#39't Copy Locale' +
      ' on Compact=False;Jet OLEDB:Compact Without Replica Repair=False' +
      ';Jet OLEDB:SFP=False'
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 32
  end
  object ADOConnection2: TADOConnection
    ConnectionString = 'Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False'
    Mode = cmShareDenyNone
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 32
    Top = 48
  end
  object ADOConnection3: TADOConnection
    ConnectionString = 'Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False'
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 120
  end
  object ADOConnection4: TADOConnection
    ConnectionString = 'Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False'
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 120
    Top = 48
  end
end
