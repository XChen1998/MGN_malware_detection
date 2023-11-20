unit mainunit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, Menus, ComCtrls, StdCtrls,syncobjs,syncobjs2,symbolhandlerlite,globals;

const staticscanner_done=wm_user+1;
const rescan_done=wm_user+2;
const open_scanner=wm_user+3;

const wm_drawtreeviewdone=wm_user+4;
const wm_drawtreeviewAddToList=wm_user+5;


type TDrawTreeview=class(tthread)
  private
    st: string;
    offset: dword;
    offsetlist: array of dword;
    offsetsize: integer;
  public
    treeview: ttreeview;
    pointerlist: tmemorystream;
    progressbar: tprogressbar;
    procedure execute; override;
    procedure done;
    procedure AddToList;
end;

type TMatches = array of ttreenode;
type tpath = array of dword;

type TDissectData = class(tthread)
  private
    output: array [0..63] of string;
    addresses: array [0..63] of dword;
    ispointer: array [0..64] of boolean;

    outputpos: integer;
  public
    dissectaddress: dword;
    treenode: ttreenode;
    structsize,structsize0: dword;
    automatic: boolean;
    maxlevel: integer;
    filterstart,filterstop: integer;
    unalligned: boolean;
end;

type trescanpointers=class(tthread)
  public
    progressbar: tprogressbar;
    oldpointerlist: tmemorystream;
    newpointerlist: tmemorystream;
    address: dword;
    procedure execute; override;
end;

type
  toffsetlist = array of dword;
  TStaticscanner = class;
  TMethod2scanner = class (tthread)
  private
    results: tmemorystream;
    resultsfile: tfilestream;


    pathlist: array of dword;

//    procedure updatelist;
    function haspossiblepath(level: integer; address: dword;var recnr: integer):boolean;
    procedure addpossiblepath(address:dword; level: integer);
    procedure ScanAddress(saddress:dword; currentlevel: integer);
  public

    method3: boolean;
    startworking: tevent;
    isdone: boolean;
    stop: boolean;


    //--------------
    address: dword;
    addresstofind: dword;
    maxlevel: integer;
    structsize,structsize0: integer;
    filterstart,filterstop: integer;
    unalligned: boolean;
    paintlevel: integer;
    offsetlist: toffsetlist;
    fast: boolean;
    psychotic: boolean;

    filename: string;
    procedure flushresults;

    procedure execute; override;
    constructor create(suspended: boolean);
    destructor destroy; override;
  end;

  TStaticscanner = class(TThread)
  private
    updateline: integer; //not used for addentry

    memoryregion: array of tmemoryregion;
    lasttreenodeadded: ttreenode;
    addnode: ttreenode;
    addnodeextension: tmatches;

    method2scanners: array of tmethod2scanner;

//    procedure UpdateList;
    //procedure done;
//    procedure automaticfinish;
//    procedure addentry;
    procedure method2scan(address:dword);
  public
    automatic: boolean;
    automaticaddress: dword;
    filterstart:dword;
    filterstop:dword;
    start: dword;
    stop: dword;
    progressbar: TProgressbar;
    sz,sz0: integer;
    maxlevel: integer;
    unalligned: boolean;
    codescan: boolean;
    method2: boolean;
    method3: boolean;

    fast: boolean;
    psychotic: boolean;
    writableonly: boolean;
    unallignedbase: boolean;

    threadcount: integer;
    scannerpriority: TThreadPriority;

    filenames: array of string;
    phase: integer;
    currentpos: ^Dword;

    starttime: dword;

    procedure execute; override;
    destructor destroy; override;
  end;

type
  Tfrmpointerscanner = class(TForm)
    Label9: TLabel;
    ProgressBar1: TProgressBar;
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    New1: TMenuItem;
    N2: TMenuItem;
    Open1: TMenuItem;
    Save1: TMenuItem;
    Pointerscanner1: TMenuItem;
    Method1Fastbuttakesuptoomuchmemory1: TMenuItem;
    Method2Takesuplittlememoryandtakesyears1: TMenuItem;
    Method3Fastspeedandaveragememoryusage1: TMenuItem;
    N1: TMenuItem;
    Rescanmemory1: TMenuItem;
    SaveDialog1: TSaveDialog;
    OpenDialog1: TOpenDialog;
    Panel2: TPanel;
    Label1: TLabel;
    Label2: TLabel;
    Button1: TButton;
    Timer1: TTimer;
    Timer2: TTimer;
    TreeView2: TTreeView;
    Panel1: TPanel;
    Label10: TLabel;
    Label11: TLabel;
    Showresults1: TMenuItem;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    Label19: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    procedure Method3Fastspeedandaveragememoryusage1Click(Sender: TObject);
    procedure Timer2Timer(Sender: TObject);
    procedure Showresults1Click(Sender: TObject);
    procedure Save1Click(Sender: TObject);
    procedure Open1Click(Sender: TObject);
    procedure Rescanmemory1Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormShow(Sender: TObject);
    procedure TreeView2DblClick(Sender: TObject);
    procedure New1Click(Sender: TObject);
  private
    { Private declarations }
    start:tdatetime;
    
    rescan: trescanpointers;
    cewindowhandle: thandle;
    drawtreeviewthread: TDrawTreeview;

    procedure drawtreeviewdone(var message: tmessage); message wm_drawtreeviewdone;
    procedure drawtreeviewAddToList(var message: tmessage); message wm_drawtreeviewAddToList;

    procedure m_staticscanner_done(var message: tmessage); message staticscanner_done;
    procedure rescandone(var message: tmessage); message rescan_done;
    procedure openscanner(var message: tmessage); message open_scanner;
    procedure drawtreeview;
    procedure doneui;
    procedure loadpointers;
  public
    { Public declarations }
    pointerlist: tmemorystream;
    Staticscanner:TStaticScanner;
  end;

type TExecuter = class(tthread)
  public
    procedure execute; override;
end;

type tarraypath= array of tpath;

var
  frmPointerScanner: TfrmPointerScanner;
  staticlist: array of dword;
  dissectedstatics: dword=0;

  dissectedpointersLevelpos:array of integer;
  dissectedpointersLevel: array of array of dword;
  dissectedpointersLevelMREWS: array of TMultiReadExclusiveWriteSynchronizer; //every level has it's own lock


  treenodeswithchildrenpos: integer;
  treenodeswithchildren: array of ttreenode;
  treenodeswithchildrencs: tcriticalsection;

  matchednodescs: tcriticalsection;
  matchednodes: array of Tmatches;
  matchednodespos: integer;


  PossiblepathsLevelMREWS: array of TMultiReadExclusiveWriteSynchronizer;
  possiblepathsLevel: array of array of dword; //all addresses that finished in a address
  possiblepathsLevelpos: array of integer;

  method2semaphore: tsemaphore;
  


  totalpointers: integer;
  lastlevel: integer;

  pointersfound: dword=0;
  foundbyappending: dword=0;
  skipped: dword;

  lastaddress: dword;
  cpucount: integer;

  scanaddresscount: dword;
  incorrectresult: dword;
  continued: dword;

implementation

{$R *.dfm}

uses settings;

procedure TExecuter.execute;
begin
  try
    frmpointerscanner:=tfrmpointerscanner.Create(nil);
    frmpointerscanner.ShowModal;

    messagebox(0,'Exit pointerscan','exit',mb_ok);

    FreeLibraryAndExitThread(getmodulehandle('pscan.dll'),0);
  except
    on e: exception do
    begin
      messagebox(0,pchar('pointerscan crash.'),'error',mb_ok);
      messagebox(0,pchar('pointerscan crash. '+e.message),'error',mb_ok);
    end;

  end;
end;

function isdissected(level: integer; address: dword;var recnr: integer):boolean;
var i: integer;
    first,last: integer;
begin
  result:=false;
  recnr:=0;
  first:=0;

  if level>0 then
  begin
    result:=isdissected(level-1,address,recnr); //recnr is only used when it is NOT found
    if result then exit;
  end;

  //no need for a critical section, it doesnt become shorter, only thing that can happen is a address being added twice, but thats not the end of the world
  last:=dissectedpointerslevelpos[level];

  while first<last do
  begin
    i:=first+((last-first) div 2);
    if (i=first) or (i=last) then
    begin
      for i:=first to last-1 do
      begin
        if dissectedpointerslevel[level][i]=address then
        begin
          recnr:=i;
          result:=true;
          exit;
        end;
        if dissectedpointerslevel[level][i]>address then break;
      end;

      break;
    end;

    if dissectedpointerslevel[level][i]=address then
    begin
      recnr:=i;
      result:=true;
      exit;
    end;

    if address<dissectedpointerslevel[level][i] then
      last:=i
    else
      first:=i;
  end;
  recnr:=last;
end;


procedure adddisectedaddress(level:integer; dissectedaddress:dword);
var i,j: integer;
begin
  if not isdissected(level,dissectedaddress,i) then
  begin
    dissectedpointersLevelMREWS[level].beginwrite;
    try

      if dissectedpointerslevelpos[level]+1>=length(dissectedpointerslevel[level]) then //reallocate array
        setlength(dissectedpointerslevel[level],length(dissectedpointerslevel[level])*2); //double the memory (4mb, 8mb, 16mb,32mb,64mb,128mb,256mb,512mb,1024mb.....)

      //not found so add , i is a good indication of where
      dec(i);
      while (i>=0) and (dissectedpointerslevel[level][i]>dissectedaddress) do dec(i);
      if i=-1 then i:=0;

      while (i<dissectedpointerslevelpos[level]) and (dissectedpointerslevel[level][i]<dissectedaddress) do inc(i);

      //add it to the spot of i

      //first move evrything else to the right
      for j:=dissectedpointerslevelpos[level]-1 downto i do
        dissectedpointerslevel[level][j+1]:=dissectedpointerslevel[level][j];

      dissectedpointerslevel[level][i]:=dissectedaddress;

      dissectedpointerslevelpos[level]:=dissectedpointerslevelpos[level]+1;
    finally
      dissectedpointersLevelMREWS[level].EndWrite;
    end;
  end;

end;

procedure TDrawTreeview.done;
begin
  if frmpointerscanner<>nil then
  begin
    frmpointerscanner.treeview2.Items.EndUpdate;
    frmpointerscanner.progressbar1.position:=0;
    frmpointerscanner.file1.Enabled:=true;
    frmpointerscanner.Pointerscanner1.Enabled:=true;
  end;
end;

procedure TDrawtreeView.AddToList;
var y: ttreenode;
    i,j: integer;
begin
  y:=treeview.Items.Add(nil,st+inttohex(offset,8));

  for j:=0 to offsetsize-1 do
    treeview.Items.AddChild(y,inttohex(offsetlist[j],1));
end;

procedure TDrawTreeview.execute;
var total: integer;
    s: pchar;
    ssize: dword;
    stringlength: dword;
begin
//frmpointerscanner can nevr be closed while this routine is running. See the onclose event for why

  try
    pointerlist.Seek(0,sofrombeginning);

    treeview.items.clear;
    total:=0;
    setlength(offsetlist,10);

    getmem(s,100);
    ssize:=100;

    progressbar.Position:=0;
    progressbar.Max:=pointerlist.Size;

    try
      while (not terminated) and (frmpointerscanner.pointerlist.Position<>pointerlist.Size) do
      begin
        pointerlist.ReadBuffer(stringlength,sizeof(stringlength));
        if ssize<=stringlength then
        begin
          freemem(s);
          getmem(s,stringlength+1);
          ssize:=stringlength+1;
        end;

        pointerlist.ReadBuffer(s^,stringlength);
        s[stringlength]:=#0;
        st:=s;

        pointerlist.readbuffer(offset,sizeof(offset));

        pointerlist.ReadBuffer(offsetsize,sizeof(offsetsize));
        if length(offsetlist)<(offsetsize) then
          setlength(offsetlist,offsetsize*2);
        pointerlist.ReadBuffer(offsetlist[0],(offsetsize)*sizeof(offsetlist[0]));

        if st<>'' then st:=st+'+';

        if not terminated then
          sendmessage(frmpointerscanner.Handle,wm_drawtreeviewAddToList,0,0);

        inc(total);

        if (total mod 50)=0 then
          progressbar.Position:=pointerlist.Position;
      end;
    except
      //done, eof
    end;

    pointersfound:=total;
    
  finally
    freemem(s);
  end;

  if not terminated then
    sendmessage(frmpointerscanner.Handle,wm_drawtreeviewdone,0,0);
end;

//----------------------- scanner info --------------------------

//----------------------- method2scanner-------------------------

procedure TMethod2scanner.flushresults;
begin
  resultsfile.WriteBuffer(results.Memory^,results.Size);
  results.Seek(0,sofrombeginning);
  results.Clear;
end;


function TMethod2scanner.haspossiblepath(level: integer; address: dword;var recnr: integer):boolean;
var i: integer;
    first,last: integer;
begin
  result:=false;
  recnr:=0;
  first:=0;


  //check possible paths in higher levels
  if level<self.maxlevel then
  begin
    result:=haspossiblepath(level+1,address,recnr); //recnr is only used when it is NOT found
    if result then exit;
  end;

  //no need for a critical section, it doesnt become shorter, only thing that can happen is a address being added twice, but thats not the end of the world
  last:=possiblepathslevelpos[level];

  while first<last do
  begin
    i:=first+((last-first) div 2);
    if (i=first) or (i=last) then
    begin
      for i:=first to last-1 do
      begin
        if possiblepathslevel[level][i]=address then
        begin
          recnr:=i;
          result:=true;
          exit;
        end;
        if possiblepathslevel[level][i]>address then break;
      end;

      break;
    end;

    if possiblepathslevel[level][i]=address then
    begin
      recnr:=i;
      result:=true;
      exit;
    end;

    if address<possiblepathslevel[level][i] then
      last:=i
    else
      first:=i;
  end;
  recnr:=last;
end;



procedure TMethod2scanner.addpossiblepath(address: dword; level: integer);
var x,y: dword;
    i,j: integer;
    originalpos: integer;
begin
  //offsetlist now contains a pointer path
  if not haspossiblepath(level,address,i) then
  begin
    possiblepathsLevelMREWS[level].beginwrite;
    try

      if possiblepathslevelpos[level]+1>=length(possiblepathslevel[level]) then //reallocate array
        setlength(possiblepathslevel[level],length(possiblepathslevel[level])*2); //double the memory (4mb, 8mb, 16mb,32mb,64mb,128mb,256mb,512mb,1024mb.....)

      //not found so add , i is a good indication of where
      dec(i);
      while (i>=0) and (possiblepathslevel[level][i]>address) do dec(i);
      if i=-1 then i:=0;

      while (i<possiblepathslevelpos[level]) and (possiblepathslevel[level][i]<address) do inc(i);

      //add it to the spot of i

      //first move evrything else to the right
      for j:=possiblepathslevelpos[level]-1 downto i do
        possiblepathslevel[level][j+1]:=possiblepathslevel[level][j];

      possiblepathslevel[level][i]:=address;

      possiblepathslevelpos[level]:=possiblepathslevelpos[level]+1;
    finally
      possiblepathsLevelMREWS[level].EndWrite;
    end;
  end;
end;


procedure TMethod2Scanner.ScanAddress(saddress:dword; currentlevel: integer);
type
    PDWordArray = ^TDWordArray;
    TDWordArray = array[0..0] of DWORD;

var struct: PDWordArray;
    x: dword;
    i,j,k,l,m: integer;

    tempoffsetlist: array of toffsetlist;

    tempoffset: toffsetlist;

    sz: integer;
    mi: tmoduleinfo;

begin
  inc(scanaddresscount);

  if terminated then exit;

  if currentlevel=1 then
    sz:=structsize0
  else
    sz:=structsize;

  try
    if frmPointerScanner.Staticscanner=nil then
    begin
      terminate;
      exit; //it got freed
    end;

    if frmPointerScanner.Staticscanner.Terminated then
    begin
      terminate;
      exit;
    end;
  except
    terminate;
    exit;
  end;

  if (addresstofind>=saddress) and (addresstofind<(saddress+sz)) then
  begin
    //found it

    offsetlist[currentlevel]:=addresstofind-saddress;


    if results.Size>50*1024*1024 then flushresults;

    try
      //recheck to see if one of the pointers have changed in the meantime
      x:=offsetlist[0];
      pathlist[1]:=x;

      for i:=2 to currentlevel do
      begin
        x:=pdword(x)^+offsetlist[i];
        pathlist[i]:=x;
      end;

      if x<>addresstofind then inc(incorrectresult)
      else
      begin
        //correct so add it to the list of possible paths
        if not fast then
        begin
          for i:=1 to currentlevel do
            addpossiblepath(pathlist[i],i);
        end;


        if not symhandler.getmodulebyaddress(offsetlist[0],mi) then
        begin
          mi.modulename:='';
          mi.baseaddress:=0;
          mi.basesize:=$7fffffff;
        end;

        x:=length(mi.modulename);
        results.WriteBuffer(x,sizeof(x));
        results.WriteBuffer(mi.modulename[1],x);

        x:=offsetlist[0]-mi.baseaddress; //offset
        results.WriteBuffer(x,sizeof(x));

        i:=currentlevel-1;
        results.WriteBuffer(i,sizeof(i));
        results.WriteBuffer(offsetlist[2],(currentlevel-1)*sizeof(offsetlist[0]));
        inc(pointersfound);
      end;
    except
      inc(incorrectresult);
    end;

 
  end;


  if (not psychotic) and method3 and (currentlevel>1) and isdissected(currentlevel-1,saddress,i) then
  begin
    if fast then exit;

    //else see if it is in a possible path and if so continue, else exit
    if haspossiblepath(currentlevel-1,saddress,i) then
    begin
      //I see hope in this path
      inc(continued);
    end
    else
    begin
      inc(skipped);
      exit;
    end;
  end;



  try
    if frmPointerScanner.staticscanner=nil then exit;
  except
    exit;
  end;

  struct:=pointer(saddress);


  try
    for i:=0 to (sz-1) div 4 do
    begin
      if (unalligned or ((struct[i] mod 4)=0)) and (not IsBadReadPtr(pointer(struct[i]), 4)) and (currentlevel<maxlevel) and (struct[i]>=filterstart) and (struct[i]<=filterstop) then
      begin
        if currentlevel>1 then offsetlist[currentlevel]:=i*4;
        
        scanaddress(struct[i],currentlevel+1);
        if not psychotic then adddisectedaddress(currentlevel,struct[i]);
      end;
    end;
  except
    //end of structure reached
  end;
end;



procedure TMethod2Scanner.execute;
var wr: twaitresult;
    err: integer;
begin
  filename:=inttostr(getcurrentprocessid)+'-'+inttostr(getcurrentthreadid)+'.ptr';
  resultsfile:= tfilestream.Create(filename,fmcreate);

  
  while not terminated do
  begin
    wr:=startworking.WaitFor(infinite);
    if stop then exit;

    if wr=wrSignaled then
    begin
      try
        err:=0;
        if length(offsetlist)<maxlevel*2+1 then
        begin
          setlength(offsetlist,maxlevel*2+1);
          setlength(pathlist,maxlevel*2+1);
        end;

        offsetlist[1]:=address;
        offsetlist[0]:=address;

        scanaddress(address,1);
        err:=3;
        //adddisectedaddress(0,address);
      except
      end;

      isdone:=true;  //set isdone to true
      try
        method2semaphore.release;  //release the mainthead if it was waiting
      except
        messagebox(0,'err','err',0);
      end;
    end;
  end;
end;

constructor Tmethod2scanner.create(suspended:boolean);
begin
  results:= tmemorystream.Create;

  startworking:=tevent.create(nil,false,false,'');
  isdone:=true;

  inherited create(suspended);
end;

destructor TMethod2scanner.destroy;
begin
  results.free;
  resultsfile.free;
  startworking.free;
end;


//----------------------- staticscanner -------------------------

procedure Tfrmpointerscanner.drawtreeviewdone(var message: tmessage);
begin
  if DrawTreeviewThread<>nil then
    DrawTreeviewThread.done;
end;


procedure Tfrmpointerscanner.drawtreeviewAddToList(var message: tmessage); 
begin
  if DrawTreeviewThread<>nil then
    DrawTreeviewThread.AddToList;
end;


procedure Tfrmpointerscanner.drawtreeview;
begin
  file1.Enabled:=false;
  Pointerscanner1.Enabled:=false;
  treeview2.Items.BeginUpdate;
  DrawTreeviewthread:=TDrawtreeview.Create(true);
  DrawTreeviewThread.pointerlist:=pointerlist;
  DrawTreeviewThread.treeview:=treeview2;
  DrawTreeviewThread.progressbar:=progressbar1;
  DrawTreeviewThread.Resume;
end;

procedure TFrmpointerscanner.loadpointers;
begin
  if pointerlist=nil then pointerlist:=tmemorystream.create;
  pointerlist.loadfromfile(staticscanner.filenames[0]);
end;

procedure TFrmpointerscanner.doneui;
begin
  progressbar1.position:=0;

  panel2.Visible:=false;
  TreeView2.Visible:=true;
  TreeView2.Align:=alclient;
  open1.Enabled:=true;
  new1.enabled:=true;
  save1.Enabled:=true;
  rescanmemory1.Enabled:=true;
end;

procedure Tfrmpointerscanner.m_staticscanner_done(var message: tmessage);
var x: tfilestream;
    result: tfilestream;
    i: integer;
begin
  //update the treeview

  //now combile all thread results to 1 file
  result:=tfilestream.Create('result.ptr',fmcreate);
  for i:=0 to length(staticscanner.filenames)-1 do
  begin
    x:=tfilestream.Create(staticscanner.filenames[i],fmopenread);
    result.CopyFrom(x,0);
    x.free;
    deletefile(staticscanner.filenames[i]);
  end;
  result.Free;

  setlength(staticscanner.filenames,1);
  staticscanner.filenames[0]:='result.ptr';

  loadpointers;

  showresults1.Enabled:=true;
  showresults1.Click;

  doneui;  
end;

procedure TStaticScanner.method2scan(address:dword);
var i,j: integer;
begin
  //find one that is either empty or done

  j:=-1;
  method2semaphore.Aquire; //allow cpucount+1 to go through

  //this only gets called by 1 thread, so no threadchooser cs needed

  if terminated then
  begin
    method2semaphore.Release;
    exit;
  end;

  for i:=0 to length(method2scanners)-1 do
  begin
    if method2scanners[i].isdone then
    begin
      j:=i;
      break;
    end;
  end;

  if j<>-1 then //otherwhise impossible, but for debugging purposes...
  begin
    method2scanners[j].address:=address;
    method2scanners[j].addresstofind:=automaticaddress;
    method2scanners[j].maxlevel:=maxlevel;
    method2scanners[j].structsize:=sz;
    method2scanners[j].structsize0:=sz0;
    method2scanners[j].unalligned:=unalligned;
    method2scanners[j].filterstart:=filterstart;
    method2scanners[j].filterstop:=filterstop;
    method2scanners[j].method3:=method3;
    method2scanners[j].fast:=fast;
    method2scanners[j].psychotic:=psychotic;
    method2scanners[j].isdone:=false;
    method2scanners[j].Priority:=scannerpriority;

    method2scanners[j].startworking.SetEvent; //it should start working now...
  end
  else
    messagebox(0,'error','error',mb_ok);
end;

procedure TStaticScanner.execute;
var currentpos: ^Dword;
    i,j,k: integer;
    x,opcode:string;

    t:dword;
    hexcount,hexstart: integer;
    isstruct: boolean;
    isstatic: boolean;
    found: boolean;

    mbi: _MEMORY_BASIC_INFORMATION;

    tn,tempnode: ttreenode;
    th: TDissectData;
    lastnode: ttreenode;
    oldshowsymbols: boolean;
    oldshowmodules: boolean;


    bitcount: integer;

    scanregions: tmemoryregions;
    currentregion: integer;
    maxpos: dword;
    dw: byte;


begin
  if terminated then exit;

  //no vm so no phase 1
  
  currentpos:=pointer(start);

  phase:=2;

  Method2semaphore:=tsemaphore.create(threadcount);

  setlength(method2scanners,threadcount);
  for i:=0 to threadcount-1 do
    method2scanners[i]:=tmethod2scanner.create(false);

  i:=0;

  threadcount:=bitcount;

  try
    //create some regions it should scan
    setlength(scanregions,0);

    if not unallignedbase then
    begin
      if (start mod 4)>0 then
        start:=start+(4-(start mod 4));
    end;

    symhandler.fillMemoryRegionsWithModuleData(scanregions,start,stop-start);


    if length(scanregions)=0 then exit;   //no memory found
    currentregion:=0;
    currentpos:=pointer(scanregions[currentregion].BaseAddress);

    progressbar.Position:=0;
    progressbar.Max:=stop-start;

    maxpos:=scanregions[currentregion].BaseAddress+scanregions[currentregion].MemorySize-4;

    while (not terminated) and (dword(currentpos)<=stop) do
    begin
      isstatic:=false;

      try
        if ((currentpos^ mod 4)=0) or (unalligned) then //unaligned check after the first one so it includes a readable check
          isstatic:=true;

        if (writableonly) and ((dword(currentpos) mod 4096)=0) then
        begin
          if isbadwriteptr(currentpos,4) then
          begin
            inc(pbyte(currentpos),4096);

            isstatic:=false;
            if dword(currentpos)>=maxpos then
            begin
              inc(currentregion);
              if currentregion=length(scanregions) then break; //no more regions
              currentpos:=pointer(scanregions[currentregion].BaseAddress);

              maxpos:=scanregions[currentregion].BaseAddress+scanregions[currentregion].MemorySize-4;
            end;

            inc(i);
            i:=i mod 40;
            if i=0 then progressbar.position:=(dword(currentpos)-start);
            continue;

          end;

        end;


        if isstatic and (currentpos^>=filterstart) and (currentpos^<=filterstop) then
        begin
          method2scan(dword(currentpos));
        end;

        inc(i);
        i:=i mod 40;
        if i=0 then progressbar.position:=(dword(currentpos)-start);

        if unallignedbase then
          inc(pbyte(currentpos))
        else
          inc(pbyte(currentpos),4);

        if dword(currentpos)>=maxpos then
        begin
          inc(currentregion);
          if currentregion=length(scanregions) then break; //no more regions
          currentpos:=pointer(scanregions[currentregion].BaseAddress);

          maxpos:=scanregions[currentregion].BaseAddress+scanregions[currentregion].MemorySize-4;
        end;
      except
        inc(currentregion);
        if currentregion=length(scanregions) then break; //no more regions
        currentpos:=pointer(scanregions[currentregion].BaseAddress);

        maxpos:=scanregions[currentregion].BaseAddress+scanregions[currentregion].MemorySize-4;
      end;

    end;


    //done
  finally
    setlength(filenames,length(method2scanners));

    for i:=0 to length(method2scanners)-1 do
    begin
      method2scanners[i].stop:=true;
      method2scanners[i].startworking.SetEvent;  //run it in case it was waiting
      method2scanners[i].WaitFor; //wait till this thread has terminated because the main thread has termianted
      method2scanners[i].flushresults;
      filenames[i]:=method2scanners[i].filename; //save the filename
      method2scanners[i].Free;
    end;

    postmessage(frmpointerscanner.Handle,staticscanner_done,0,0);
    terminate;
    freeandnil(Method2semaphore);
  end;
end;



destructor TStaticscanner.destroy;
begin
  terminate;
  waitfor;
  inherited destroy;
end;

//---------------------------------main--------------------------

procedure Tfrmpointerscanner.Method3Fastspeedandaveragememoryusage1Click(
  Sender: TObject);
var
    i: integer;
begin

  start:=now;
  if frmpointerscannersettings=nil then
    frmpointerscannersettings:=tfrmpointerscannersettings.create(self);

  if frmpointerscannersettings.Visible then exit;

    if frmpointerscannersettings.Showmodal=mrok then
    begin
      treeview2.Visible:=false;

      panel2.Visible:=false;
      open1.Enabled:=false;
      new1.enabled:=false;
      save1.Enabled:=false;
      rescanmemory1.Enabled:=false;
      

      incorrectresult:=0;
      continued:=0;
      pointersfound:=0;

      label1.Caption:='Matches found:';
      label2.Left:=label1.Left+label1.Width+5;

      timer2.Enabled:=true;

      treenodeswithchildrenpos:=0;
      matchednodespos:=0;


      //free the old critical section
      for i:=0 to length(dissectedpointersLevelMREWS)-1 do
        dissectedpointersLevelMREWS[i].Free;

      setlength(dissectedpointersLevelpos,frmpointerscannersettings.maxlevel+1);
      setlength(dissectedpointersLevel,frmpointerscannersettings.maxlevel+1);
      setlength(dissectedpointersLevelMREWS,frmpointerscannersettings.maxlevel+1);

      for i:=0 to length(dissectedpointersLevelpos)-1 do
        dissectedpointersLevelpos[i]:=0;

      for i:=0 to length(dissectedpointersLevelMREWS)-1 do
        dissectedpointersLevelMREWS[i]:=TMultiReadExclusiveWriteSynchronizer.create;

      for i:=0 to length(dissectedpointerslevel)-1 do
        setlength(dissectedpointerslevel[i],1024*1024); //1mb default


      //possible paths cleaning and reinitialize
      for i:=0 to length(possiblepathslevelMREWS)-1 do
        possiblepathslevelMREWS[i].Free;

      setlength(possiblepathslevelpos,frmpointerscannersettings.maxlevel+1);
      setlength(possiblepathslevel,frmpointerscannersettings.maxlevel+1);
      setlength(possiblepathslevelMREWS,frmpointerscannersettings.maxlevel+1);

      for i:=0 to length(possiblepathslevelpos)-1 do
        possiblepathslevelpos[i]:=0;

      for i:=0 to length(possiblepathslevelMREWS)-1 do
        possiblepathslevelMREWS[i]:=TMultiReadExclusiveWriteSynchronizer.create;

      for i:=0 to length(possiblepathslevel)-1 do
        setlength(possiblepathslevel[i],1024*1024); //1mb default



      if staticscanner<>nil then staticscanner.free;

      staticscanner:=TStaticscanner.Create(true);

      try
        staticscanner.start:=frmpointerscannersettings.start;
        staticscanner.stop:=frmpointerscannersettings.Stop;
        staticscanner.filterstart:=frmpointerscannersettings.FilterStart;
        staticscanner.filterstop:=frmpointerscannersettings.FilterStop;
        staticscanner.unalligned:=frmpointerscannersettings.unalligned;
        staticscanner.codescan:=frmpointerscannersettings.codescan;

        staticscanner.automatic:=true;

        staticscanner.automaticaddress:=frmpointerscannersettings.automaticaddress;
        staticscanner.sz:=frmpointerscannersettings.structsize;
        staticscanner.sz0:=frmpointerscannersettings.level0structsize;
        staticscanner.maxlevel:=frmpointerscannersettings.maxlevel;
        staticscanner.method2:=true;
        staticscanner.method3:=true;
        staticscanner.fast:=frmpointerscannersettings.CheckBox1.Checked;
        staticscanner.psychotic:=frmpointerscannersettings.psychotic;
        staticscanner.writableonly:=frmpointerscannersettings.writableonly;
        staticscanner.unallignedbase:=frmpointerscannersettings.unallignedbase;

        staticscanner.progressbar:=progressbar1;
        staticscanner.threadcount:=frmpointerscannersettings.threadcount;
        staticscanner.scannerpriority:=frmpointerscannersettings.scannerpriority;

        progressbar1.Max:=staticscanner.stop-staticscanner.start;


        open1.Enabled:=false;
        staticscanner.starttime:=gettickcount;
        staticscanner.Resume;
        panel2.Visible:=true;
      except
        staticscanner.Free;
      end;
    end;
end;

procedure Tfrmpointerscanner.Timer2Timer(Sender: TObject);
var i,j,l: integer;
    s: string;
    a: string;
    smallestaddress: dword;
    todo: dword;
    done: dword;
    donetime,todotime: integer;
    oneaddresstime: double;
    _h,_m,_s: integer;
begin
  label8.Caption:=inttostr(continued);
  label2.Caption:=inttostr(pointersfound);
  label13.caption:=inttostr(skipped);

  if staticscanner<>nil then
  try
    l:=length(staticscanner.method2scanners);
    s:=inttostr(l);

    a:='';
    j:=0;
    for i:=0 to l-1 do
    begin
      if staticscanner.method2scanners[i].isdone then inc(j);
      if i=0 then smallestaddress:=staticscanner.method2scanners[i].address
      else
        if staticscanner.method2scanners[i].address<smallestaddress then smallestaddress:=staticscanner.method2scanners[i].address;

      a:=a+'   '+inttostr(i)+':'+inttohex(staticscanner.method2scanners[i].address,8);
    end;

    label11.caption:=s+' ('+inttostr(j)+')'+a;

    label15.Caption:=inttohex(smallestaddress,8);

    if staticscanner.phase=2 then
    begin
      todo:=staticscanner.stop-dword(staticscanner.currentpos);
      done:=dword(staticscanner.currentpos)-staticscanner.start;

      donetime:=gettickcount-staticscanner.starttime; //time(in ms) it did about 'done' addresses
      oneaddresstime:=donetime/done;
      todotime:=trunc(todo*oneaddresstime);

      _h:=todotime div 3600000;
      todotime:=todotime mod 3600000;

      _m:=todotime div 60000;
      todotime:=todotime mod 60000;

      _s:=todotime div 1000;

      label19.Caption:=inttostr(_h)+':'+inttostr(_m)+':'+inttostr(_s);      

      label18.Caption:='Scanning';
    end else label18.caption:='Copying memory';
  except
    label11.caption:='0 (0)';
  end else label18.Caption:='Idle';


  label4.Caption:=inttostr(scanaddresscount);
  label5.caption:=inttostr(incorrectresult);
end;

procedure Tfrmpointerscanner.Showresults1Click(Sender: TObject);
begin
  panel1.caption:='There are '+inttostr(pointersfound)+' pointers in the list';
  
  if pointersfound>15000 then
    if messagedlg('This is a huge ammount of pointers. ('+inttostr(pointersfound)+') Are you sure you want to show them? (It''ll take a while to update the list)',mtconfirmation,[mbyes,mbno],0)<>mryes then exit;

  drawtreeview;


end;

procedure Tfrmpointerscanner.Save1Click(Sender: TObject);
var y: tfilestream;
begin
  if savedialog1.execute then
  begin
    y:=tfilestream.Create(savedialog1.FileName,fmcreate);
    try
      y.CopyFrom(pointerlist,0);
    finally
      y.free;
    end;
  end;
end;

procedure Tfrmpointerscanner.Open1Click(Sender: TObject);
var x: tfilestream;
    offset: dword;
    offsetsize: dword;
    offsetlist: array of dword;
    stringlength: dword;
    s: pchar;
    ssize: dword;
    st: string;
begin
  ssize:=0;
  setlength(offsetlist,10);

  if opendialog1.Execute then
  begin
    if staticscanner=nil then
    begin
      staticscanner:=tstaticscanner.Create(true);
      staticscanner.Terminate;
      staticscanner.Resume;
    end;


    setlength(staticscanner.filenames,1);
    staticscanner.filenames[0]:=opendialog1.FileName;

    pointersfound:=0;
    x:=tfilestream.Create(opendialog1.FileName,fmopenread);

    getmem(s,100);
    ssize:=100;
    try
      while x.Position<x.Size do
      begin
        x.ReadBuffer(stringlength,sizeof(stringlength));
        if ssize<=stringlength then
        begin
          freemem(s);
          getmem(s,stringlength+1);
          ssize:=stringlength+1;
        end;

        x.ReadBuffer(s^,stringlength);
        s[stringlength]:=#0;

        x.ReadBuffer(offset,sizeof(offset));

        x.ReadBuffer(offsetsize,sizeof(offsetsize));
        if length(offsetlist)<(offsetsize+1) then
          setlength(offsetlist,offsetsize*2);

        x.ReadBuffer(offsetlist[0],offsetsize*sizeof(offsetlist[0]));

        inc(pointersfound);
      end;
    finally
      x.free;
      freemem(s);
    end;

    loadpointers;

    doneui;

    showresults1.Enabled:=true;
    Rescanmemory1.Enabled:=true;
    Save1.Enabled:=true;
    new1.Enabled:=true;

    showresults1.Click;
  end;
end;



procedure TRescanpointers.execute;
var offsetsize: dword;
    offsetlist: array of dword;
    x,br: dword;
    i: integer;
    mi: TModuleInfo;

    stringlength: dword;
    ssize: dword;
    s: pchar;
    offset: dword;

    b: ^dword;
begin
  pointersfound:=0;

  newpointerlist:=tmemorystream.Create;
  oldpointerlist.Seek(0,sofrombeginning);

  progressbar.Min:=0;
  progressbar.Max:=oldpointerlist.Size;

  getmem(s,100);
  ssize:=100;

  setlength(offsetlist,10);
  while oldpointerlist.Position<oldpointerlist.Size do
  begin
    oldpointerlist.ReadBuffer(stringlength,sizeof(stringlength));
    if ssize<=stringlength then
    begin
      freemem(s);
      getmem(s,stringlength+1);
      ssize:=stringlength+1;
    end;

    oldpointerlist.ReadBuffer(s^,stringlength);
    s[stringlength]:=#0;

    oldpointerlist.ReadBuffer(offset,sizeof(offset));


    oldpointerlist.ReadBuffer(offsetsize,sizeof(offsetsize));
    if length(offsetlist)<(offsetsize+1) then
      setlength(offsetlist,offsetsize*2);

    oldpointerlist.ReadBuffer(offsetlist[0],offsetsize*sizeof(offsetlist[0]));
    //now check if it matches, and if so, save it back to the newpointerlist


    try
      x:=symhandler.getAddressFromName(s,true)+offset;

      for i:=0 to offsetsize-1 do
      begin
        b:=pointer(x);
        x:=b^;
        inc(x,offsetlist[i]);
      end;

      if x=address then
      begin
        newpointerlist.WriteBuffer(stringlength,sizeof(stringlength));
        newpointerlist.WriteBuffer(s^,stringlength);
        newpointerlist.WriteBuffer(offset,sizeof(offset));

        newpointerlist.WriteBuffer(offsetsize,sizeof(offsetsize));
        newpointerlist.WriteBuffer(offsetlist[0],offsetsize*sizeof(offsetlist[0]));
        inc(pointersfound);
      end;
    except
      //not valid, so dont save
    end;

    progressbar.Position:=oldpointerlist.Position;
  end;

  postmessage(frmPointerScanner.Handle,rescan_done,0,0);
end;

procedure Tfrmpointerscanner.Rescanmemory1Click(Sender: TObject);
var address: dword;
    saddress: string;
begin
  saddress:='';
  if inputquery('Rescan pointers','What is the current address?',saddress) then
  begin
    address:=strtoint('$'+saddress);

    Rescanmemory1.Enabled:=false;
    Save1.Enabled:=false;
    new1.Enabled:=false;
    showresults1.Enabled:=false;

    //rescan the pointerlist
    rescan:=trescanpointers.create(true);
    rescan.progressbar:=progressbar1;
    rescan.oldpointerlist:=pointerlist;
    rescan.address:=address;
    rescan.resume;
  end;

end;

procedure tfrmpointerscanner.rescandone(var message: tmessage);
begin
  pointerlist.free;
  pointerlist:=rescan.newpointerlist;

  rescan.free;

  doneui;
    
  showresults1.Enabled:=true;
  Rescanmemory1.Enabled:=true;
  Save1.Enabled:=true;
  new1.Enabled:=true;

  showresults1.Click;  
end;

procedure Tfrmpointerscanner.Button1Click(Sender: TObject);
begin
  if staticscanner<>nil then
  begin
    staticscanner.Terminate;
    staticscanner.WaitFor;
  end;
end;

procedure Tfrmpointerscanner.FormClose(Sender: TObject;
  var Action: TCloseAction);
var i,j: integer;
begin
  if drawtreeviewthread<>nil then
  begin
    drawtreeviewthread.Terminate;
    drawtreeviewthread.WaitFor;
    drawtreeviewthread.free;
    drawtreeviewthread:=nil;
  end;

  button1.click;
  staticscanner.Free;
  staticscanner:=nil;

  setlength(staticlist,0);
  setlength(dissectedpointersLevelpos,0);
  setlength(dissectedpointersLevel,0);

  for i:=0 to length(dissectedpointersLevelMREWS)-1 do
    if dissectedpointersLevelMREWS[i]<>nil then
      freeandnil(dissectedpointersLevelMREWS[i]);

  setlength(dissectedpointersLevelMREWS,0);

  for i:=0 to length(treenodeswithchildren)-1 do
    freeandnil(treenodeswithchildren[i]);

  setlength(treenodeswithchildren,0);

  if matchednodescs<>nil then freeandnil(matchednodescs);

  for i:=0 to length(matchednodes)-1 do
    setlength(matchednodes[i],0);
  setlength(matchednodes,0);

  TreeView2.Items.Clear;

  for i:=0 to length(PossiblepathsLevelMREWS)-1 do
    freeandnil(PossiblepathsLevelMREWS[i]);
  setlength(PossiblepathsLevelMREWS,0);

  for i:=0 to length(possiblepathsLevel)-1 do
    setlength(possiblepathsLevel[i],0);
  setlength(possiblepathsLevel,0);

  setlength(possiblepathsLevelpos,0);

  if (method2semaphore<>nil) then
    freeandnil(method2semaphore); 

  action:=cafree;
  frmpointerscanner:=nil;
end;

procedure Tfrmpointerscanner.FormShow(Sender: TObject);
begin
  self.BringToFront;
  self.SetFocus;
  setforegroundwindow(self.handle);
  label9.Left:=0;
end;

procedure Tfrmpointerscanner.openscanner(var message: tmessage);
begin
  if frmpointerscannersettings=nil then
    frmpointerscannersettings:=tfrmpointerscannersettings.create(self);

  frmpointerscannersettings.edtAddress.text:=inttohex(message.WParam,8);
  Method3Fastspeedandaveragememoryusage1.Click;
end;


procedure Tfrmpointerscanner.TreeView2DblClick(Sender: TObject);
var ms: tmemorystream;
    x: dword;
    t: ttreenode;

    p: thandle;
    targetbuffer: pointer;
    CDS: COPYDATASTRUCT;
begin
  t:=treeview2.Selected;
  if t=nil then exit;
  if t.Level<>0 then exit;

  ms:=tmemorystream.Create;
  try
    x:=length(treeview2.Selected.Text);
    ms.WriteBuffer(x,sizeof(x));
    ms.WriteBuffer(treeview2.Selected.Text[1],x);

    t:=t.GetLastChild;

    while t<>nil do
    begin
      x:=strtoint('$'+t.Text);
      ms.WriteBuffer(x,4);
      t:=t.getPrevSibling;
    end;

    cds.dwData:=$ce;
    cds.cbData:=ms.Size;
    cds.lpData:=ms.Memory;
    sendmessage(scansettings.mainformHandle,WM_COPYDATA,handle,dword(@CDS));
  finally
    ms.free;
  end;
end;

procedure Tfrmpointerscanner.New1Click(Sender: TObject);
begin
  button1.Click;
  treeview2.Visible:=false;
  panel2.Visible:=false;
  panel1.Caption:='';
  open1.Enabled:=true;
  new1.enabled:=true;
  save1.Enabled:=false;
  rescanmemory1.Enabled:=false;
  showresults1.enabled:=false;
end;

end.

