/****************************************************************************
 *
 * Copyright (C) 2015 Neutrino International Inc.
 *
 ****************************************************************************/

#include "qtmtmanapi.h"

#define IsNull(item)      (NULL==(item))
#define NotNull(item)     (NULL!=(item))
#define NotEqual(a,b)     ((a)!=(b))
#define IsMask(a,b)       (((a) & (b))==(b))
#define nEqual(VAR,CONST) ((CONST)==(VAR))
#define nGreater(A,B)     ((A)>(B))
#define nLess(A,B)        ((A)<(B))

QT_BEGIN_NAMESPACE

QtMtAPI:: QtMtAPI     ( void )
        : ApiChannels ( 0    )
        , Account     ( ""   )
        , Password    ( ""   )
        , IP          ( ""   )
        , Manager     ( NULL )
        , Dealers     ( NULL )
        , Pumping     ( NULL )
        , Codec       ( NULL )
        , fnVersion   ( NULL )
        , fnCreate    ( NULL )
{
  Codec = QTextCodec::codecForLocale ( ) ;
}

QtMtAPI:: QtMtAPI     ( int APIs )
        : ApiChannels (     APIs )
        , Account     ( ""       )
        , Password    ( ""       )
        , IP          ( ""       )
        , Manager     ( NULL     )
        , Dealers     ( NULL     )
        , Pumping     ( NULL     )
        , Codec       ( NULL     )
        , fnVersion   ( NULL     )
        , fnCreate    ( NULL     )
{
  Codec = QTextCodec::codecForLocale (      ) ;
  Initialize                         ( APIs ) ;
}

QtMtAPI:: QtMtAPI     ( QString filename , int APIs )
        : ApiChannels (                        APIs )
        , Account     ( ""                          )
        , Password    ( ""                          )
        , IP          ( ""                          )
        , Manager     ( NULL                        )
        , Dealers     ( NULL                        )
        , Pumping     ( NULL                        )
        , Codec       ( NULL                        )
        , fnVersion   ( NULL                        )
        , fnCreate    ( NULL                        )
{
  Codec = QTextCodec::codecForLocale (          ) ;
  Load                               ( filename ) ;
  Initialize                         ( APIs     ) ;
}

QtMtAPI::~QtMtAPI(void)
{
  Release ( ) ;
}

bool QtMtAPI::Load(QString filename)
{
  if ( filename . length ( ) <= 0 )                             {
    #ifdef _WIN64
    filename = "mtmanapi64.dll"                                 ;
    #else
    filename = "mtmanapi.dll"                                   ;
    #endif
  }                                                             ;
  ///////////////////////////////////////////////////////////////
  QLibrary lib ( filename )                                     ;
  fnVersion = (MtManVersion_t) lib . resolve ( "MtManVersion" ) ;
  fnCreate  = (MtManCreate_t ) lib . resolve ( "MtManCreate"  ) ;
  if ( NULL == fnVersion ) return false                         ;
  if ( NULL == fnCreate  ) return false                         ;
  return true                                                   ;
}

CManagerInterface * QtMtAPI::API(MtApiFlags api)
{
  switch ( api )                       {
    case MtApiManager : return Manager ;
    case MtApiDealer  : return Dealers ;
    case MtApiPump    : return Pumping ;
  }                                    ;
  return NULL                          ;
}

void QtMtAPI::Initialize(int APIs)
{
  if ( NULL == fnVersion ) return                            ;
  if ( NULL == fnCreate  ) return                            ;
  int version = fnVersion ( )                                ;
  if (version!=ManAPIVersion) return                         ;
  if (IsMask(APIs,MtApiManager)) fnCreate (version,&Manager) ;
  if (IsMask(APIs,MtApiDealer )) fnCreate (version,&Dealers) ;
  if (IsMask(APIs,MtApiPump   )) fnCreate (version,&Pumping) ;
  ApiChannels = APIs                                         ;
}

void QtMtAPI::Release(void)
{
  disconnect ( )                                                     ;
  if ( NULL != Manager ) { Manager -> Release ( ) ; Manager = NULL ; }
  if ( NULL != Dealers ) { Dealers -> Release ( ) ; Dealers = NULL ; }
  if ( NULL != Pumping ) { Pumping -> Release ( ) ; Pumping = NULL ; }
}

void __stdcall QtMtAPI::pumping(int code,int type,void * data,void * param)
{
  if (IsNull(param)) return          ;
  QtMtAPI * qma = (QtMtAPI *)param   ;
  qma -> Pour ( code , type , data ) ;
}

void QtMtAPI::Pour(int code,int type,void * data)
{
  switch ( code )                    {
    case PUMP_START_PUMPING          :
      StartPumping   ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_SYMBOLS         :
      UpdateSymbols  ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_GROUPS          :
      UpdateGroups   ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_USERS           :
      UpdateUsers    ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_ONLINE          :
      ObtainOnlines  ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_BIDASK          :
      UpdateBidAsk   ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_NEWS            :
      ObtainNews     ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_NEWS_BODY       :
      ObtainNewsBody ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_MAIL            :
      UpdateMails    ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_TRADES          :
      UpdateTrades   ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_REQUESTS        :
      UpdateRequests ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_PLUGINS         :
      UpdatePlugins  ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_ACTIVATION      :
      Activation     ( type , data ) ;
    break                            ;
    case PUMP_UPDATE_MARGINCALL      :
      MarginCalls    ( type , data ) ;
    break                            ;
    case PUMP_STOP_PUMPING           :
      StopPumping    ( type , data ) ;
    break                            ;
    case PUMP_PING                   :
      PumpingPing    ( type , data ) ;
    break                            ;
  }                                  ;
}

bool QtMtAPI::isConnected(void)
{
  #define CHECK(api) \
    if (IsMask(ApiChannels,api) && !isConnected(api)) return false
  CHECK ( MtApiManager ) ;
  CHECK ( MtApiDealer  ) ;
  CHECK ( MtApiPump    ) ;
  #undef  CHECK
  return true            ;
}

bool QtMtAPI::isConnected(MtApiFlags api)
{
  SafeMtAPI ( api , cmi , false ) ;
  return true                     ;
}

bool QtMtAPI::assure(MtApiFlags ApiFlag)
{
  CManagerInterface * cmi = API ( ApiFlag )     ;
  if ( NULL == cmi ) return false               ;
  if (cmi->IsConnected()) return true           ;
  int  result                                   ;
  char HostIP [256]                             ;
  ::strcpy ( HostIP , IP.toUtf8().constData() ) ;
  result  = cmi->Connect(HostIP)                ;
  if (result!=RET_OK) return false              ;
  return login (ApiFlag)                        ;
}

bool QtMtAPI::login(MtApiFlags ApiFlag)
{
  CManagerInterface * cmi = API (ApiFlag)           ;
  if (IsNull(cmi)) return false                     ;
  int  result = 0                                   ;
  int  login  = Account.toInt()                     ;
  char Pass [256]                                   ;
  ::strcpy ( Pass , Password.toUtf8().constData() ) ;
  result = cmi->Login(login,Pass)                   ;
  return nEqual(result,RET_OK)                      ;
}

bool QtMtAPI::mountPumping (void)
{
  if (!isConnected(MtApiPump)) return false                  ;
  Pumping -> PumpingSwitchEx ( QtMtAPI::pumping , 0 , this ) ;
  return true                                                ;
}

bool QtMtAPI::disconnect(MtApiFlags ApiFlag)
{
  if (!isConnected(ApiFlag)) return true  ;
  CManagerInterface * cmi = API (ApiFlag) ;
  if (IsNull(cmi)) return false           ;
  cmi -> Disconnect ( )                   ;
  return true                             ;
}

void QtMtAPI::disconnect(void)
{
  this -> disconnect ( MtApiManager ) ;
  this -> disconnect ( MtApiDealer  ) ;
  this -> disconnect ( MtApiPump    ) ;
}

bool QtMtAPI::FullMount(QString ip,bool PumpingEx)
{
  IP = ip                                                             ;
  if (IsMask(ApiChannels,MtApiManager) && !isConnected(MtApiManager)) {
    assure ( MtApiManager )                                           ;
  }                                                                   ;
  if (IsMask(ApiChannels,MtApiDealer ) && !isConnected(MtApiDealer )) {
    assure ( MtApiDealer  )                                           ;
  }                                                                   ;
  if (IsMask(ApiChannels,MtApiPump   ) && !isConnected(MtApiPump   )) {
    if ( assure ( MtApiPump ) )                                       {
      if ( PumpingEx ) mountPumping ( )                               ;
    }                                                                 ;
  }                                                                   ;
  return isConnected ( )                                              ;
}

int QtMtAPI::BytesSent(MtApiFlags api)
{
  SafeMtAPI ( api , cmi , 0 ) ;
  return cmi -> BytesSent ( ) ;
}

int QtMtAPI::BytesReceived(MtApiFlags api)
{
  SafeMtAPI ( api , cmi , 0 )     ;
  return cmi -> BytesReceived ( ) ;
}

time_t QtMtAPI::ServerTime(MtApiFlags api)
{
  SafeMtAPI ( api , cmi , 0 )             ;
  return (time_t)( cmi -> ServerTime () ) ;
}

int QtMtAPI::TimeZone(MtApiFlags api)
{
  ConCommon * cc = Common ( api ) ;
  if (IsNull(cc)) return 0        ;
  int tz = cc->timezone           ;
  delete cc                       ;
  return tz                       ;
}

ConCommon * QtMtAPI::Common(MtApiFlags api)
{
  SafeMtAPI ( api , cmi , NULL )              ;
  ConCommon CC                                ;
  int result = cmi->ManagerCommon(&CC)        ;
  if (nEqual(result,RET_OK))                  {
    ConCommon * cc = new ConCommon()          ;
    ::memcpy ( cc , &CC , sizeof(ConCommon) ) ;
    return cc                                 ;
  } else return NULL                          ;
}

QList<ConPluginParam *> QtMtAPI::Plugins(MtApiFlags api)
{
  QList<ConPluginParam *> plugins                ;
  SafeMtAPI ( api , cmi , plugins )              ;
  int              total = 0                     ;
  ConPluginParam * CPP   = NULL                  ;
  CPP = cmi -> CfgRequestPlugin (&total)         ;
  if (IsNull(CPP)) return plugins                ;
  for (int i=0;i<total;i++)                      {
    ConPluginParam * cpp                         ;
    cpp = new ConPluginParam()                   ;
    ::memcpy(cpp,&CPP[i],sizeof(ConPluginParam)) ;
    plugins << cpp                               ;
  }                                              ;
  SafeMtFree ( cmi , CPP )                       ;
  return plugins                                 ;
}

void QtMtAPI::Clear(QList<ConPluginParam *> & plugins)
{
  for (int i=0;i<plugins.count();i++) {
    if ( NotNull ( plugins [ i ] ) )  {
      delete plugins [ i ]            ;
    }                                 ;
  }                                   ;
  plugins . clear ( )                 ;
}

QStringList QtMtAPI::Symbols(MtApiFlags api)
{
  QStringList         S                  ;
  SafeMtAPI ( api , cmi , S )            ;
  ConSymbol * cs    = NULL               ;
  int         total = 0                  ;
  cmi -> SymbolsRefresh       (        ) ;
  cs   = cmi -> SymbolsGetAll ( &total ) ;
  for (int i=0;i<total;i++)              {
    S << QString::fromUtf8(cs[i].symbol) ;
  }                                      ;
  SafeMtFree ( cmi , cs )                ;
  return S                               ;
}

int QtMtAPI::Update(QStringList & symbols,MtApiFlags api)
{
  if (symbols.count()<0) return 0                      ;
  SafeMtAPI ( api , cmi , 0 )                          ;
  ConSymbol * cs    = NULL                             ;
  int         total = 0                                ;
  int         i                                        ;
  cs = cmi->SymbolsGetAll(&total)                      ;
  for (i=0;i<symbols.count();i++)                      {
    char * s = (char *)symbols[i].toUtf8().constData() ;
    cmi -> SymbolAdd (s)                               ;
  }                                                    ;
  SafeMtFree ( cmi , cs )                              ;
  return total                                         ;
}

QList<int> QtMtAPI::Logins(void)
{
  QList<int> logins                                ;
  SafeMtAPI ( MtApiManager , cmi , logins )        ;
  int total = 0                                    ;
  UserRecord * UR = cmi -> UsersRequest ( &total ) ;
  if (IsNull(UR)) return logins                    ;
  for (int i=0;i<total;i++) logins << UR[i].login  ;
  SafeMtFree ( cmi , UR )                          ;
  return logins                                    ;
}

QList<ConGroup *> QtMtAPI::Groups(void)
{
  QList<ConGroup *> groups                           ;
  SafeMtAPI ( MtApiManager , cmi , groups )          ;
  int                 total = 0                      ;
  ConGroup          * group = NULL                   ;
  group = cmi->GroupsRequest(&total)                 ;
  if (total>0 && group!=NULL)                        {
    for (int i=0;i<total;i++)                        {
      ConGroup * cg = new ConGroup()                 ;
      ::memcpy ( cg , &group[i] , sizeof(ConGroup) ) ;
      groups << cg                                   ;
    }                                                ;
  }                                                  ;
  SafeMtFree ( cmi , group )                         ;
  return groups                                      ;
}

QString QtMtAPI::UserGroup(int api,int login)
{
  SafeMtAPI ( api , cmi , "" )                       ;
  QString      grp   = ""                            ;
  UserRecord * UR    = NULL                          ;
  int          total = 1                             ;
  UR = cmi -> UserRecordsRequest ( &login , &total ) ;
  if ( ( total > 0 ) && ( NULL != UR ) )             {
    grp = QString::fromUtf8 ( UR -> group )          ;
  }                                                  ;
  SafeMtFree ( cmi , UR )                            ;
  return grp                                         ;
}

void QtMtAPI::Clear(QList<ConGroup *> & Records)
{
  for (int i=0;i<Records.count();i++) {
    if ( NotNull ( Records [ i ] ) )  {
      delete Records [ i ]            ;
    }                                 ;
  }                                   ;
  Records . clear ( )                 ;
}

bool QtMtAPI::Exists(int login)
{
  SafeMtAPI ( MtApiManager , cmi , false )       ;
  bool         success = false                   ;
  int          total   = 1                       ;
  UserRecord * guser   = NULL                    ;
  guser = cmi->UserRecordsRequest(&login,&total) ;
  success = (total>0 && guser!=NULL)             ;
  SafeMtFree ( cmi , guser )                     ;
  return success                                 ;
}

QList<int> QtMtAPI::Online(MtApiFlags api)
{
  QList<int> U                    ;
  SafeMtAPI ( api , cmi , U )     ;
  int          total = 0          ;
  OnlineRecord * OR  = NULL       ;
  OR = cmi->OnlineRequest(&total) ;
  if (IsNull(OR)) return U        ;
  if (total<=0  ) return U        ;
  for (int i=0;i<total;i++)       {
    U << OR [ i ] . login         ;
  }                               ;
  SafeMtFree ( cmi , OR )         ;
  return U                        ;
}

UserRecord * QtMtAPI::User(int login)
{
  SafeMtAPI ( MtApiManager , cmi , NULL )         ;
  UserRecord * user   = NULL                      ;
  UserRecord * qser   = NULL                      ;
  int          total  = 1                         ;
  qser = cmi->UserRecordsRequest(&login,&total)   ;
  if (total>0 && qser!=NULL)                      {
    user = new UserRecord ( )                     ;
    ::memcpy ( user , qser , sizeof(UserRecord) ) ;
  } else user = NULL                              ;
  SafeMtFree ( cmi , qser )                       ;
  return user                                     ;
}

MarginLevel * QtMtAPI::Margin(int login)
{
  SafeMtAPI ( MtApiManager , cmi , NULL )               ;
  MarginLevel * margin = new MarginLevel ( )            ;
  int result = cmi->MarginLevelRequest(login,margin)    ;
  if (result!=RET_OK) { delete margin ; return NULL ; } ;
  return margin                                         ;
}

bool QtMtAPI::New(UserRecord * user)
{
  SafeMtAPI ( MtApiManager , cmi , false ) ;
  int result = cmi->UserRecordNew(user)    ;
  return nEqual(result,RET_OK)             ;
}

bool QtMtAPI::Update(UserRecord * user)
{
  SafeMtAPI ( MtApiManager , cmi , false ) ;
  int result = cmi->UserRecordUpdate(user) ;
  return nEqual(result,RET_OK)             ;
}

bool QtMtAPI::setPassword(int login,QString pass,bool modify)
{
  SafeMtAPI ( MtApiManager , cmi , false )                         ;
  int  result                                                      ;
  int  changing = modify ? 1 : 0                                   ;
  char password[16]                                                ;
  ::strcpy ( password , pass.toUtf8().constData() )                ;
  result = cmi->UserPasswordSet(login,(LPCSTR)password,changing,0) ;
  return nEqual(result,RET_OK)                                     ;
}

void QtMtAPI::Clear(UserRecord * user)
{
  if ( NotNull(user) ) delete user ;
}

void QtMtAPI::Clear(QList<UserRecord *> & Records)
{
  for (int i=0;i<Records.count();i++) {
    if ( NotNull ( Records [ i ] ) )  {
      delete Records [ i ]            ;
    }                                 ;
  }                                   ;
  Records . clear ( )                 ;
}

QList<TradeRecord *> QtMtAPI::Duplicate(QList<TradeRecord *> & Old)
{
  QList<TradeRecord *> T                   ;
  for (int i=0;i<Old.count();i++)          {
    TradeRecord * R = new TradeRecord()    ;
    ::memcpy(R,Old[i],sizeof(TradeRecord)) ;
    T << R                                 ;
  }                                        ;
  return T                                 ;
}

bool QtMtAPI::Contains(QList<TradeRecord *> & Records,TradeRecord * Trade)
{
  if (Records.count()<=0) return false                                    ;
  for (int i=0;i<Records.count();i++)                                     {
    TradeRecord * T = Records[i]                                          ;
    if ((Trade->login==T->login) && (Trade->order==T->order)) return true ;
  }                                                                       ;
  return true                                                             ;
}

void QtMtAPI::Clear(QList<TradeRecord *> & Records)
{
  for (int i=0;i<Records.count();i++) {
    if (NotNull(Records[i]))          {
      delete Records [ i ]            ;
    }                                 ;
  }                                   ;
  Records . clear ( )                 ;
}

QList<TradeRecord *> QtMtAPI::NewOrders(QList<TradeRecord *> & Old,QList<TradeRecord *> & New)
{
  QList<TradeRecord *> T                   ;
  for (int i=0;i<New.count();i++)          {
    if (!Contains(Old,New[i])) T << New[i] ;
  }                                        ;
  return T                                 ;
}

QList<TradeRecord *> QtMtAPI::ClosedOrders(QList<TradeRecord *> & Old,QList<TradeRecord *> & New)
{
  QList<TradeRecord *> T                   ;
  for (int i=0;i<Old.count();i++)          {
    if (!Contains(New,Old[i])) T << Old[i] ;
  }                                        ;
  return T                                 ;
}

QList<TradeRecord *> QtMtAPI::Orders(QList<int> & Logins,MtApiFlags api)
{
  QList<TradeRecord *> T                      ;
  SafeMtAPI ( api , cmi , T )                 ;
  int total = 0                               ;
  TradeRecord * TRX = cmi->TradesGet(&total)  ;
  if (IsNull(TRX)) return T                   ;
  if (total<=0) return T                      ;
  for (int i=0;i<total;i++)                   {
    if (Logins.contains(TRX[i].login))        {
      TradeRecord * R = new TradeRecord()     ;
      ::memcpy(R,&TRX[i],sizeof(TradeRecord)) ;
      T << R                                  ;
    }                                         ;
  }                                           ;
  SafeMtFree ( cmi , TRX )                    ;
  return T                                    ;
}

QList<RequestInfo *> QtMtAPI::Requests(QList<int> & Logins,MtApiFlags api)
{
  QList<RequestInfo *> T                       ;
  SafeMtAPI ( api , cmi , T )                  ;
  int total = 0                                ;
  RequestInfo * TRX = cmi->RequestsGet(&total) ;
  if (IsNull(TRX)) return T                    ;
  if (total<=0) return T                       ;
  for (int i=0;i<total;i++)                    {
    if (Logins.contains(TRX[i].login))         {
      RequestInfo * R = new RequestInfo()      ;
      ::memcpy(R,&TRX[i],sizeof(RequestInfo))  ;
      T << R                                   ;
    }                                          ;
  }                                            ;
  SafeMtFree ( cmi , TRX )                     ;
  return T                                     ;
}

QtMtAPI::MtTradeResult QtMtAPI::Deposit(int login,double cash,QString comment)
{
  SafeMtAPI ( MtApiManager , cmi , NoConnection )      ;
  //////////////////////////////////////////////////////
  UserRecord * user = User(login)                      ;
  if (IsNull(user)) return NoAccount                   ;
  Clear(user)                                          ;
  //////////////////////////////////////////////////////
  TradeTransInfo info                                  ;
  char           cm[32]                                ;
  QByteArray     CM = comment.toUtf8()                 ;
  char         * dd = CM.data()                        ;
  ::memset ( cm , 0 , 32 )                             ;
  if (dd!=NULL)                                        {
    if (CM.size()<31) ::memcpy ( cm , dd , CM.size() ) ;
                 else ::memcpy ( cm , dd , 31        ) ;
  }                                                    ;
  ::memset ( &info , 0 , sizeof(TradeTransInfo) )      ;
  //////////////////////////////////////////////////////
  info.type    = TT_BR_BALANCE                         ;
  info.cmd     = OP_BALANCE                            ;
  info.orderby = login                                 ;
  info.price   = cash                                  ;
  ::strcpy(info.comment,cm)                            ;
  int result = cmi -> TradeTransaction(&info)          ;
  if (nEqual(result,RET_OK)) return Success            ;
  return TradeFailure                                  ;
}

QtMtAPI::MtTradeResult QtMtAPI::Withdraw(int login,double cash,QString comment)
{
  SafeMtAPI ( MtApiManager , cmi , NoConnection )      ;
  //////////////////////////////////////////////////////
  UserRecord * user = User(login)                      ;
  if (IsNull(user)) return NoAccount                   ;
  if (user->balance<cash) return NotBalance            ;
  Clear(user)                                          ;
  //////////////////////////////////////////////////////
  TradeTransInfo info                                  ;
  char           cm[32]                                ;
  QByteArray     CM = comment.toUtf8()                 ;
  char         * dd = CM.data()                        ;
  ::memset(cm,0,32)                                    ;
  if (dd!=NULL)                                        {
    if (CM.size()<31) ::memcpy ( cm , dd , CM.size() ) ;
                 else ::memcpy ( cm , dd , 31        ) ;
  }                                                    ;
  ::memset(&info,0,sizeof(TradeTransInfo))             ;
  //////////////////////////////////////////////////////
  info . type    =  TT_BR_BALANCE                      ;
  info . cmd     =  OP_BALANCE                         ;
  info . orderby =  login                              ;
  info . price   = -cash                               ;
  ::strcpy(info.comment,cm)                            ;
  int result = cmi -> TradeTransaction(&info)          ;
  if (nEqual(result,RET_OK)) return Success            ;
  return TradeFailure                                  ;
}

bool QtMtAPI::Trade(QList<TradeTransInfo *> & orders,bool clear)
{
  if ( orders . count ( ) <= 0 ) return false ;
  SafeMtAPI ( MtApiManager , cmi , false )    ;
  for (int i=0;i<orders.count();i++)          {
    cmi -> TradeTransaction ( orders[i] )     ;
  }                                           ;
  if ( clear ) Clear ( orders )               ;
  return true                                 ;
}

void QtMtAPI::Clear(QList<TradeTransInfo *> & orders)
{
  for (int i=0;i<orders.count();i++) delete orders[i] ;
  orders.clear()                                      ;
}

int QtMtAPI::Profit(TradeRecord * trade)
{
  SafeMtAPI ( MtApiManager , cmi , 0  ) ;
  return cmi->TradeCalcProfit ( trade ) ;
}

double QtMtAPI::Depository(int api,int login,QDateTime from,QString comment)
{
  SafeMtAPI ( api , cmi , 0 )                                   ;
  ///////////////////////////////////////////////////////////////
  QDateTime     Now      = QDateTime::currentDateTime()         ;
  time_t        fromT    = from . toTime_t ( )                  ;
  time_t        toT      = Now  . toTime_t ( ) + 86400          ;
  int           total    = 0                                    ;
  TradeRecord * TRX      = NULL                                 ;
  double        balance  = 0                                    ;
  bool          matching = ( comment . length ( ) > 0 )         ;
  ///////////////////////////////////////////////////////////////
  TRX = cmi -> TradesUserHistory (login , fromT , toT , &total) ;
  if ( ( total > 0 ) && ( NULL != TRX ) )                       {
    for (int i=0;i<total;i++)                                   {
      if ( OP_BALANCE == TRX [ i ] . cmd )                      {
        if ( matching )                                         {
          QString c = QString::fromUtf8 ( TRX [ i ] . comment ) ;
          if ( c . contains ( comment ) )                       {
            balance += TRX [ i ] . profit                       ;
          }                                                     ;
        } else                                                  {
          balance += TRX [ i ] . profit                         ;
        }                                                       ;
      }                                                         ;
    }                                                           ;
  }                                                             ;
  SafeMtFree ( cmi , TRX )                                      ;
  ///////////////////////////////////////////////////////////////
  return balance                                                ;
}

bool QtMtAPI::CloseTrade(int api,int order,double lots,QString c)
{
  SafeMtAPI ( api , cmi , false )                       ;
  ///////////////////////////////////////////////////////
  TradeRecord   TRX                                     ;
  TradeRecord * TRU                                     ;
  int           r                                       ;
  int           total = 1                               ;
  char          comment [ 32 ]                          ;
  ///////////////////////////////////////////////////////
  ::memset( &TRX , 0 , sizeof(TradeRecord) )            ;
  TRU = cmi -> TradeRecordsRequest ( &order , &total )  ;
  if ( ( total <= 0 ) || ( NULL == TRU ) ) return false ;
  ::memcpy ( &TRX , TRU , sizeof(TradeRecord) )         ;
  cmi -> MemFree ( TRU )                                ;
  ///////////////////////////////////////////////////////
  TradeTransInfo TTI                                    ;
  int            volume = (int) ( lots * 100 )          ;
  ::memset( &TTI , 0 , sizeof(TradeTransInfo) )         ;
  if ( c.length() > 0 )                                 {
    c = c . left ( 31 )                                 ;
    ::memset ( comment , 0 , 32 )                       ;
    ::strcpy ( comment , c . toUtf8().constData() )     ;
  }                                                     ;
  TTI . order   = TRX . order                           ;
  TTI . orderby = TRX . login                           ;
  TTI . cmd     = TRX . cmd                             ;
  TTI . price   = TRX . close_price                     ;
  if ( ( lots < 0 ) || ( volume >= TRX . volume ) )     {
    TTI . type    = TT_BR_ORDER_CLOSE                   ;
    TTI . volume  = TRX . volume                        ;
  } else                                                {
    TTI . type    = TT_BR_ORDER_CLOSE                   ;
    TTI . volume  = volume                              ;
  }                                                     ;
  ::strcpy ( TTI . symbol  , TRX . symbol )             ;
  ::strcpy ( TTI . comment , comment      )             ;
  ///////////////////////////////////////////////////////
  r = cmi -> TradeTransaction ( &TTI )                  ;
  return ( r == RET_OK )                                ;
}

bool QtMtAPI::CloseAllOrders(int api,int login,QString pattern)
{
  SafeMtAPI ( api , cmi , false )                               ;
  ///////////////////////////////////////////////////////////////
  UserRecord UR                                                 ;
  int        RT                                                 ;
  RT = cmi -> UserRecordGet     ( login , &UR                 ) ;
  if ( RET_OK != RT ) return false                              ;
  ///////////////////////////////////////////////////////////////
  TradeRecord * TRX                                             ;
  int           total = 0                                       ;
  TRX = cmi -> TradesGetByLogin ( login , UR . group , &total ) ;
  if ( total <= 0   ) return false                              ;
  if ( NULL  == TRX ) return false                              ;
  ///////////////////////////////////////////////////////////////
  QList<int> ORDERs                                             ;
  int   id                                                      ;
  for (int i=0;i<total;i++) ORDERs << TRX [ i ] . order         ;
  SafeMtFree ( cmi , TRX )                                      ;
  ///////////////////////////////////////////////////////////////
  foreach ( id , ORDERs )                                       {
    QString comment = ""                                        ;
    if ( pattern . length ( ) > 0 )                             {
      comment = QString ( pattern ) . arg ( id )                ;
    }                                                           ;
    this -> CloseTrade ( api , id , -1 , comment )              ;
  }                                                             ;
  ///////////////////////////////////////////////////////////////
  return true                                                   ;
}

bool QtMtAPI::setReadOnly(int api,int login,bool readOnly)
{
  SafeMtAPI ( api , cmi , false )                    ;
  UserRecord * UR      = NULL                        ;
  int          total   = 1                           ;
  UserRecord   UU                                    ;
  ////////////////////////////////////////////////////
  UR           = User   ( login )                    ;
  UR = cmi -> UserRecordsRequest ( &login , &total ) ;
  if ( NULL == UR ) return false                     ;
  memcpy ( &UU , UR , sizeof(UserRecord) )           ;
  UU . enable_read_only = readOnly ? 1 : 0           ;
  SafeMtFree ( cmi , UR )                            ;
  ////////////////////////////////////////////////////
  int result = cmi -> UserRecordUpdate ( &UU )       ;
  return nEqual ( result , RET_OK )                  ;
}

unsigned char QtMtAPI::TradeCommand(int cmd)
{
  char c = 0                   ;
  switch ( cmd )               {
    case TT_PRICES_GET         :
    break                      ;
    case TT_PRICES_REQUOTE     :
    break                      ;
    case TT_ORDER_IE_OPEN      :
    break                      ;
    case TT_ORDER_REQ_OPEN     :
    break                      ;
    case TT_ORDER_MK_OPEN      :
    break                      ;
    case TT_ORDER_PENDING_OPEN :
    break                      ;
    case TT_ORDER_IE_CLOSE     :
      c = 'D'                  ;
    break                      ;
    case TT_ORDER_REQ_CLOSE    :
      c = 'E'                  ;
    break                      ;
    case TT_ORDER_MK_CLOSE     :
      c = 'F'                  ;
    break                      ;
    case TT_ORDER_MODIFY       :
    break                      ;
    case TT_ORDER_DELETE       :
    break                      ;
    case TT_ORDER_CLOSE_BY     :
    break                      ;
    case TT_ORDER_CLOSE_ALL    :
    break                      ;
    case TT_BR_ORDER_OPEN      :
    break                      ;
    case TT_BR_ORDER_CLOSE     :
      c = 'L'                  ;
    break                      ;
    case TT_BR_ORDER_DELETE    :
      c = 'H'                  ;
    break                      ;
    case TT_BR_ORDER_CLOSE_BY  :
      c = 'N'                  ;
    break                      ;
    case TT_BR_ORDER_CLOSE_ALL :
    break                      ;
    case TT_BR_ORDER_MODIFY    :
    break                      ;
    case TT_BR_ORDER_ACTIVATE  :
    break                      ;
    case TT_BR_ORDER_COMMENT   :
    break                      ;
    case TT_BR_BALANCE         :
    break                      ;
  }                            ;
  return (unsigned char) c     ;
}

void QtMtAPI::StartPumping(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::UpdateSymbols(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::UpdateGroups(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::UpdateUsers(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::ObtainOnlines(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::UpdateBidAsk(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::ObtainNews(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::ObtainNewsBody(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::UpdateMails(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::UpdateTrades(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::UpdateRequests(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::UpdatePlugins(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::Activation(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::MarginCalls(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::StopPumping(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

void QtMtAPI::PumpingPing(int type,void * data)
{
  Q_UNUSED ( type ) ;
  Q_UNUSED ( data ) ;
}

QT_END_NAMESPACE
