/****************************************************************************
 *
 * Copyright (C) 2015 Neutrino International Inc.
 *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin
 *
 * QtCURL acts as an interface between Qt and cURL library.
 * Please keep QtMtAPI as simple as possible.
 *
 ****************************************************************************/

#ifndef QT_MTMANAPI_H
#define QT_MTMANAPI_H

#include <QtCore>
#include <QtNetwork>

#ifndef MTMANAPI_INCLUDED
#define MTMANAPI_INCLUDED
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#define _MT4MANDLL
#include <MT4ManagerAPI.h>
#endif

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTMTAPI_LIB)
#      define Q_MTAPI_EXPORT Q_DECL_EXPORT
#    else
#      define Q_MTAPI_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_MTAPI_EXPORT
#endif

#define SafeMtCall(api,cmi)                                     \
          CManagerInterface * cmi = API ( (MtApiFlags) api )  ; \
          if ( NULL == cmi              ) return ;              \
          if ( ! cmi -> IsConnected ( ) ) return

#define SafeMtAPI(api,cmi,rt)                                   \
          CManagerInterface * cmi = API ( (MtApiFlags) api )  ; \
          if ( NULL == cmi              ) return rt           ; \
          if ( ! cmi -> IsConnected ( ) ) return rt

#define SafeMtFree(cmi,obj)                                             \
          if ( ( NULL != cmi ) && ( NULL != obj ) ) cmi->MemFree(obj) ; \
          obj = NULL

class Q_MTAPI_EXPORT QtMtAPI
{
  public:

    typedef enum       {
      MtApiManager = 1 ,
      MtApiDealer  = 2 ,
      MtApiPump    = 4 }
      MtApiFlags       ;

    typedef enum       {
      NoConnection = 0 ,
      Success      = 1 ,
      NoAccount    = 2 ,
      TradeFailure = 3 ,
      NotBalance   = 4 }
      MtTradeResult    ;

    int                 ApiChannels ;
    QString             Account     ;
    QString             Password    ;
    QString             IP          ;
    CManagerInterface * Manager     ;
    CManagerInterface * Dealers     ;
    CManagerInterface * Pumping     ;
    QTextCodec        * Codec       ;
    MtManVersion_t      fnVersion   ;
    MtManCreate_t       fnCreate    ;

    explicit              QtMtAPI        (void) ;
    explicit              QtMtAPI        (int APIs) ;
    explicit              QtMtAPI        (QString filename            ,
                                          int     APIs = MtApiManager |
                                                         MtApiDealer  |
                                                         MtApiPump  ) ;
    virtual              ~QtMtAPI        (void) ;

    bool                  Load           (QString filename = QString("")) ;
    void                  Initialize     (int APIs) ; // create API functions
    void                  Release        (void) ; // remove API connectivities

    // API entries
    CManagerInterface *   API            (MtApiFlags ApiFlag) ;

    // Pumping acceptor
    virtual void          Pour           (int code,int type,void * data); // redirected from pumping(code,type,data)
    static void __stdcall pumping        (int code,int type,void * data,void * param);

    bool                  isConnected    (void);
    bool                  isConnected    (MtApiFlags ApiFlag); // check if API is connected
    bool                  assure         (MtApiFlags ApiFlag); // ensure API is connected to MT4
    bool                  login          (MtApiFlags ApiFlag); // login into MT4
    bool                  disconnect     (MtApiFlags ApiFlag); // disconnect API
    void                  disconnect     (void);
    bool                  mountPumping   (void); // connect pumping feeds

    int                   BytesSent      (MtApiFlags mtApiFlags = MtApiManager);
    int                   BytesReceived  (MtApiFlags mtApiFlags = MtApiManager);

    time_t                ServerTime     (MtApiFlags mtApiFlags = MtApiManager);
    int                   TimeZone       (MtApiFlags mtApiFlags = MtApiManager);

    // Ensure all assigned MT4 API connected to server
    bool                  FullMount      (QString ip,bool PumpingEx = true);

    // Miscellaneous
    ConCommon *           Common         (MtApiFlags mtApiFlags = MtApiManager) ;

    // System requests
    QList<ConPluginParam *> Plugins      (MtApiFlags mtApiFlags = MtApiManager) ;
    void                  Clear          (QList<ConPluginParam *> & plugins);

    // Symbols
    QStringList           Symbols        (MtApiFlags mtApiFlags = MtApiManager); // get supported symbols
    int                   Update         (QStringList & symbols,MtApiFlags mtApiFlags = MtApiPump);

    // Query
    QList<int>            Logins         (void) ; // accounts in MtApiManager
    QList<ConGroup *>     Groups         (void) ; // obtain trade groups
    QString               UserGroup      (int api,int login) ;
    void                  Clear          (QList<ConGroup *> & Records); // clear ConGroup items memory

    // User query and operations
    bool                  Exists         (int login) ; // check if account (login) exists
    QList<int>            Online         (MtApiFlags mtApiFlags = MtApiManager); // get online accounts(login)
    UserRecord     *      User           (int login) ; // obtain UserRecord by login
    MarginLevel    *      Margin         (int login) ; // margin level of user (login)
    bool                  New            (UserRecord * user) ; // append a new user account
    bool                  Update         (UserRecord * user) ; // update an account information
    bool                  setPassword    (int login,QString pass,bool modify); // set account(login) password
    void                  Clear          (UserRecord * user) ; // clear an user item memory
    void                  Clear          (QList<UserRecord *> & Records); // clear user items memory

    // orders
    bool                  Contains       (QList<TradeRecord *> & Records,TradeRecord * Trade); // return true if Trade within Records
    void                  Clear          (QList<TradeRecord *> & Records); // clear TradeRecord items memory
    QList<TradeRecord *>  Orders         (QList<int          > & Logins,MtApiFlags mtApiFlags = MtApiManager); // get accounts(Logins) trade items
    QList<TradeRecord *>  Duplicate      (QList<TradeRecord *> & Old);
    QList<TradeRecord *>  NewOrders      (QList<TradeRecord *> & Old,QList<TradeRecord *> & New);
    QList<TradeRecord *>  ClosedOrders   (QList<TradeRecord *> & Old,QList<TradeRecord *> & New);

    QList<RequestInfo *>  Requests       (QList<int          > & Logins,MtApiFlags mtApiFlags = MtApiPump);

    MtTradeResult         Deposit        (int login,double cash,QString comment);
    MtTradeResult         Withdraw       (int login,double cash,QString comment);
    bool                  Trade          (QList<TradeTransInfo *> & orders,bool Clear = false);
    void                  Clear          (QList<TradeTransInfo *> & orders);
    int                   Profit         (TradeRecord * trade);
    double                Depository     (int api,int login,QDateTime from,QString commentContains) ;
    bool                  CloseTrade     (int api,int order,double lots = -1,QString comment = "") ;
    bool                  CloseAllOrders (int api,int login,QString commentPattern = "") ;
    bool                  setReadOnly    (int api,int login,bool readOnly) ;

    unsigned char         TradeCommand   (int cmd) ;

  protected:

    virtual void          StartPumping   (int type,void * data) ;
    virtual void          UpdateSymbols  (int type,void * data) ;
    virtual void          UpdateGroups   (int type,void * data) ;
    virtual void          UpdateUsers    (int type,void * data) ;
    virtual void          ObtainOnlines  (int type,void * data) ;
    virtual void          UpdateBidAsk   (int type,void * data) ;
    virtual void          ObtainNews     (int type,void * data) ;
    virtual void          ObtainNewsBody (int type,void * data) ;
    virtual void          UpdateMails    (int type,void * data) ;
    virtual void          UpdateTrades   (int type,void * data) ;
    virtual void          UpdateRequests (int type,void * data) ;
    virtual void          UpdatePlugins  (int type,void * data) ;
    virtual void          Activation     (int type,void * data) ;
    virtual void          MarginCalls    (int type,void * data) ;
    virtual void          StopPumping    (int type,void * data) ;
    virtual void          PumpingPing    (int type,void * data) ;

  private:

};

#endif
