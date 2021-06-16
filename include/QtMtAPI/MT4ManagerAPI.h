#pragma once

#define ManAPIProgramVersion  400
#define ManAPIProgramBuild    765
#define ManAPIVersion         MAKELONG(ManAPIProgramBuild,ManAPIProgramVersion)

#if _MSC_VER <= 1200
#define __time32_t time_t
#define __ptr32
#endif

enum
  {
   CONFIG_ALL=0,
   CONFIG_COMMON,
   CONFIG_ACCESS,
   CONFIG_SERVERS,
   CONFIG_TIME,
   CONFIG_HOLIDAYS,
   CONFIG_SYMBOLS,
   CONFIG_SYMB_GROUPS,
   CONFIG_GROUPS,
   CONFIG_MANAGERS,
   CONFIG_DATAFEEDS,
   CONFIG_BACKUP,
   CONFIG_LIVEUPDATE,
   CONFIG_SYNC,
   CONFIG_PLUGINS,
   CONFIG_GATEWAY_ACCOUNTS,
   CONFIG_GATEWAY_MARKUPS,
   CONFIG_GATEWAY_RULES,
   CONFIG_END=255
  };

struct ConCommon
  {
   char              owner[128];
   char              name[32];
   ULONG             address;
   int               port;
   DWORD             timeout;
   int               typeofdemo;
   int               timeofdemo;
   int               daylightcorrection;
   char              internal[64];
   int               timezone;
   char              timesync[64];
   int               minclient;
   int               minapi;
   DWORD             feeder_timeout;
   int               keepemails;
   int               endhour,endminute;
   int               optimization_time;
   int               optimization_lasttime;
   int               optimization_counter;
   int               optimization_unused[8];
   int               antiflood;
   int               floodcontrol;
   int               liveupdate_mode;
   int               lastorder;
   int               lastlogin;
   int               lostlogin;
   int               rollovers_mode;
   char              path_database[256];
   char              path_history[256];
   char              path_log[256];
   __time32_t        overnight_last_day;
   __time32_t        overnight_last_time;
   __time32_t        overnight_prev_time;
   __time32_t        overmonth_last_month;
   char              adapters[256];
   ULONG             bind_adresses[8];
   short             server_version;
   short             server_build;
   ULONG             web_adresses[8];
   int               statement_mode;
   int               monthly_state_mode;
   int               keepticks;
   int               statement_weekend;
   int               reserved[52];
  };

enum { DEMO_DISABLED, DEMO_PROLONG, DEMO_FIXED };

enum { ROLLOVER_NORMAL, ROLLOVER_REOPEN_BY_CLOSE_PRICE, ROLLOVER_REOPEN_BY_BID };

enum { LIVE_UPDATE_NO=0, LIVE_UPDATE_RELEASE=1, LIVE_UPDATE_NO_SERVER=2, LIVE_UPDATE_BETA=3 };

enum { STATEMENT_END_DAY, STATEMENT_START_DAY };

enum { MONTHLY_STATEMENT_END_MONTH, MONTHLY_STATEMENT_START_MONTH };

struct ConAccess
  {
   int               action;
   unsigned long     from,to;
   char              comment[64];
   int               reserved[17];
  };

enum { FW_BLOCK,FW_PERMIT };

struct ConDataServer
  {
   char              server[64];
   ULONG             ip;
   char              description[64];
   int               isproxy;
   int               priority;
   UINT              loading;
   ULONG             ip_internal;
   int               reserved[2];
   ConDataServer* __ptr32 next;
  };

struct ConTime
  {
   int               days[7][24];
   int               dayscontrol;
   int               reserved[3];
  };

struct ConBackup
  {
   char              fullbackup_path[256];
   int               fullbackup_period;
   int               fullbackup_store;
   __time32_t        fullbackup_lasttime;
   short             fullbackup_shift;
   char              external_path[256];
   int               archive_period;
   int               archive_store;
   __time32_t        archive_lasttime;
   char              export_securities[512];
   char              export_path[256];
   int               export_period;
   __time32_t        export_lasttime;
   int               watch_role;
   char              watch_password[16];
   char              watch_opposite[24];
   int               watch_ip;
   char              archive_shift;
   char              watch_state;
   char              watch_failover;
   unsigned char     watch_timeout;
   int               watch_login;
   __time32_t        watch_timestamp;
  };

enum { WATCH_STAND_ALONE,WATCH_MASTER,WATCH_SLAVE };
enum { BACKUP_1HOUR,BACKUP_4HOURS,BACKUP_1DAY };
enum { BU_STORE_1MONTH,BU_STORE_3MONTHS,BU_STORE_6MONTHS,BU_STORE_1YEAR };
enum { ARC_BACKUP_DISABLED,ARC_BACKUP_5MIN,ARC_BACKUP_15MIN,ARC_BACKUP_30MIN,ARC_BACKUP_1HOUR };
enum { ARC_STORE_1DAY,ARC_STORE_3DAYS,ARC_STORE_1WEEK,ARC_STORE_2WEEKS,ARC_STORE_1MONTH,ARC_STORE_3MONTH,ARC_STORE_6MONTH };
enum { EXPORT_1MIN,EXPORT_5MIN,EXPORT_15MIN,EXPORT_30MIN,EXPORT_1HOUR };

struct ConFeeder
  {
   char              name[64];
   char              file[256];
   char              server[64];
   char              login[32];
   char              pass[32];
   char              keywords[256];
   int               enable;
   int               mode;
   int               timeout;
   int               timeout_reconnect;
   int               timeout_sleep;
   int               attemps_sleep;
   int               news_langid;
   int               unused[33];
  };

enum { FEED_QUOTES=0, FEED_NEWS=1, FEED_QUOTESNEWS=2 };

#define MAX_SEC_GROUPS       (32)
#define MAX_SEC_GROPS_MARGIN (128)

struct ConGroupSec
  {
   int               show,trade;
   int               execution;
   double            comm_base;
   int               comm_type;
   int               comm_lots;
   double            comm_agent;
   int               comm_agent_type;
   int               spread_diff;
   int               lot_min,lot_max;
   int               lot_step;
   int               ie_deviation;
   int               confirmation;
   int               trade_rights;
   int               ie_quick_mode;
   int               autocloseout_mode;
   double            comm_tax;
   int               comm_agent_lots;
   int               freemargin_mode;
   int               reserved[3];
  };

struct ConGroupMargin
  {
   char              symbol[12];
   double            swap_long,swap_short;
   double            margin_divider;
   int               reserved[7];
  };

enum { EXECUTION_MANUAL, EXECUTION_AUTO, EXECUTION_ACTIVITY };
enum { COMM_TYPE_MONEY, COMM_TYPE_PIPS, COMM_TYPE_PERCENT };
enum { COMMISSION_PER_LOT, COMMISSION_PER_DEAL };
enum { TRADE_DENY_NONE=0, TRADE_DENY_CLOSEBY=1, TRADE_DENY_MUCLOSEBY=2 };

enum { CLOSE_OUT_NONE, CLOSE_OUT_HIHI, CLOSE_OUT_LOLO, CLOSE_OUT_HILO, CLOSE_OUT_LOHI, CLOSE_OUT_FIFO, CLOSE_OUT_LIFO, CLOSE_OUT_INTRDAY_FIFO };

struct ConGroup
  {
   char              group[16];
   int               enable;
   int               timeout;
   int               adv_security;
   char              company[128];
   char              signature[128];
   char              support_page[128];
   char              smtp_server[64];
   char              smtp_login[32];
   char              smtp_password[32];
   char              support_email[64];
   char              templates[32];
   int               copies;
   int               reports;
   int               default_leverage;
   double            default_deposit;
   int               maxsecurities;
   ConGroupSec       secgroups[MAX_SEC_GROUPS];
   ConGroupMargin    secmargins[MAX_SEC_GROPS_MARGIN];
   int               secmargins_total;
   char              currency[12];
   double            credit;
   int               margin_call;
   int               margin_mode;
   int               margin_stopout;
   double            interestrate;
   int               use_swap;
   int               news;
   int               rights;
   int               check_ie_prices;
   int               maxpositions;
   int               close_reopen;
   int               hedge_prohibited;
   int               close_fifo;
   int               hedge_largeleg;
   int               unused_rights[2];
   char              securities_hash[16];
   int               margin_type;
   int               archive_period;
   int               archive_max_balance;
   int               stopout_skip_hedged;
   int               archive_pending_period;
   UINT              news_languages[8];
   UINT              news_languages_total;
   int               reserved[17];
  };

enum { MARGIN_MODE_DONT_USE,MARGIN_MODE_USE_ALL,MARGIN_MODE_USE_PROFIT,MARGIN_MODE_USE_LOSS };
enum { MARGIN_TYPE_PERCENT, MARGIN_TYPE_CURRENCY };
enum { NEWS_NO, NEWS_TOPICS, NEWS_FULL  };
enum 
  { 
   ALLOW_FLAG_EMAIL      =1,
   ALLOW_FLAG_TRAILING   =2,
   ALLOW_FLAG_ADVISOR    =4,
   ALLOW_FLAG_EXPIRATION =8,
   ALLOW_FLAG_SIGNALS_ALL=16,
   ALLOW_FLAG_SIGNALS_OWN=32
  };

struct ConHoliday
  {
   int               year;
   int               month;
   int               day;
   int               from,to;
   char              symbol[32];
   char              description[128];
   int               enable;
   int               reserved[13];
   ConHoliday * __ptr32 next;
  };

#define LIVE_FILES_MAX (128)

struct LiveInfoFile
  {
   char              file[256];
   int               size;
   char              hash[36];
   int               reserved[10];
  };

struct ConLiveUpdate
  {
   char              company[128];
   char              path[256];
   int               version;
   int               build;
   int               maxconnect;
   int               connections;
   int               type;
   int               enable;
   int               totalfiles;
   LiveInfoFile      files[LIVE_FILES_MAX];
   int               reserved[16];
   ConLiveUpdate * __ptr32 next;
  };

enum
  {
   LIVE_UPDATE_CLIENT,
   LIVE_UPDATE_MANAGER,
   LIVE_UPDATE_ADMIN,
   LIVE_UPDATE_DATACENTER,
   LIVE_UPDATE_CLIENT_PPC2002,
   LIVE_UPDATE_CLIENT_PPC2003,
   LIVE_UPDATE_MULTI,
   LIVE_UPDATE_WD,
   LIVE_UPDATE_CLIENT_PHONE,
   LIVE_UPDATE_LAST 
  };

struct ConManagerSec
  {
   int               internal;
   int               enable;
   int               minimum_lots;
   int               maximum_lots;
   int               unused[16];
  };

struct ConManager
  {
   int               login;
   int               manager;
   int               money;
   int               online;
   int               riskman;
   int               broker;
   int               admin;
   int               logs;
   int               reports;
   int               trades;
   int               market_watch;
   int               email;
   int               user_details;
   int               see_trades;
   int               news;
   int               plugins;
   int               server_reports;
   int               techsupport;
   int               unused[11];
   int               ipfilter;
   unsigned long     ip_from,ip_to;
   char              mailbox[64];
   char              groups[1024];
   ConManagerSec     secgroups[MAX_SEC_GROUPS];
   DWORD             exp_time;
   char              name[32];
   int               info_depth;
   //---
   int               reserved[22];
  };

struct ConSession
  {
   short             open_hour,open_min;
   short             close_hour,close_min;
   int               open,close;
   short             align[7];
  };
//---
struct ConSessions
  {
   //---
   ConSession        quote[3];
   ConSession        trade[3];
   //---
   int               quote_overnight;
   int               trade_overnight;
   int               reserved[2];
  };

#define MAX_SYMBOLS 1024
//---
struct ConSymbol
  {
   char              symbol[12];
   char              description[64];
   char              source[12];
   char              currency[12];
   int               type;
   int               digits;
   int               trade;
   COLORREF          background_color;
   int               count;
   int               count_original;
   int               external_unused[7];
   int               realtime;
   __time32_t        starting;
   __time32_t        expiration;
   ConSessions       sessions[7];
   int               profit_mode;
   int               profit_reserved;
   int               filter;
   int               filter_counter;
   double            filter_limit;
   int               filter_smoothing;
   float             filter_reserved;
   int               logging;
   int               spread;
   int               spread_balance;
   int               exemode;
   int               swap_enable;
   int               swap_type;
   double            swap_long,swap_short;
   int               swap_rollover3days;
   double            contract_size;
   double            tick_value;
   double            tick_size;
   int               stops_level;
   int               gtc_pendings;
   int               margin_mode;
   double            margin_initial;
   double            margin_maintenance;
   double            margin_hedged;
   double            margin_divider;
   double            point;
   double            multiply;
   double            bid_tickvalue;
   double            ask_tickvalue;
   int               long_only;
   int               instant_max_volume;
   char              margin_currency[12];
   int               freeze_level;
   int               margin_hedged_strong;
   __time32_t        value_date;
   int               quotes_delay;
   int               swap_openprice;
   int               swap_variation_margin;
   int               unused[21];

  };

enum { EXE_REQUEST,EXE_INSTANT,EXE_MARKET };
enum { TRADE_NO,TRADE_CLOSE,TRADE_FULL };
enum { SWAP_BY_POINTS,SWAP_BY_DOLLARS,SWAP_BY_INTEREST,SWAP_BY_MARGIN_CURRENCY };
enum { PROFIT_CALC_FOREX,PROFIT_CALC_CFD,PROFIT_CALC_FUTURES };
enum { MARGIN_CALC_FOREX,MARGIN_CALC_CFD,MARGIN_CALC_FUTURES,MARGIN_CALC_CFDINDEX,MARGIN_CALC_CFDLEVERAGE };
enum { ORDERS_DAILY, ORDERS_GTC, ORDERS_DAILY_NO_STOPS };

#define MAX_SEC_GROUP (32)
struct ConSymbolGroup
  {
   char              name[16];
   char              description[64];
  };

struct ConSync
  {
   char              server[64];
   int               unusedport;
   char              login[32];
   char              password[32];
   int               enable;
   int               mode;
   __time32_t        from,to;
   char              securities[1024];
   int               reserved[14];
   ConSync * __ptr32 next;
  };

enum { HB_ADD,HB_UPDATE,HB_INSERT,HB_DELETE,HB_LAST };

struct PluginInfo
  {
   char              name[128];
   unsigned int      version;
   char              copyright[128];
   int               reserved[32];
  };

struct PluginCfg
  {
   char              name[32];
   char              value[128];
   int               reserved[16];
  };

struct ConPlugin
  {
   char              file[256];
   PluginInfo        info;
   int               enabled;
   int               configurable;
   int               manager_access;
   int               reserved[62];
  };

struct ConPluginParam
  {
   ConPlugin         plugin;
   PluginCfg* __ptr32 params;
   int               total;
  };

struct ConGatewayAccount
  {
   int               enable;
   char              name[64];
   int               id;
   int               type;
   int               login;
   char              address[64];
   char              password[64];
   int               notify_logins[8];
   int               flags;
   int               reserved[23];
  };

enum EnGatewayAccountFlags { GATEWAY_FLAG_NONE=0, GATEWAY_FLAG_QUOTES=1 };

struct ConGatewayMarkup
  {
   int               enable;
   char              source[128];
   char              symbol[12];
   char              account_name[64];
   int               account_id;
   int               bid_markup;
   int               ask_markup;
   int               reserved[16];
  };

struct ConGatewayRule
  {
   int               enable;
   char              name[64];
   char              request_symbol[128];
   char              request_group[128];
   int               request_reserved[32];
   char              exe_account_name[64];
   int               exe_account_id;
   int               exe_max_deviation;
   int               exe_max_profit_slippage;
   int               exe_max_profit_slippage_lots;
   int               exe_max_losing_slippage;
   int               exe_max_losing_slippage_lots;
   int               exe_account_pos;
   int               exe_volume_percent;
   int               exe_reserved[26];
  };

enum
  {
   RET_OK                =0,
   RET_OK_NONE,
   RET_ERROR,
   RET_INVALID_DATA,
   RET_TECH_PROBLEM,
   RET_OLD_VERSION,
   RET_NO_CONNECT,
   RET_NOT_ENOUGH_RIGHTS,
   RET_TOO_FREQUENT,
   RET_MALFUNCTION,
   RET_GENERATE_KEY,
   RET_SECURITY_SESSION,
   RET_ACCOUNT_DISABLED  =64,
   RET_BAD_ACCOUNT_INFO,
   RET_PUBLIC_KEY_MISSING,
   RET_TRADE_TIMEOUT     =128,
   RET_TRADE_BAD_PRICES,
   RET_TRADE_BAD_STOPS,
   RET_TRADE_BAD_VOLUME,
   RET_TRADE_MARKET_CLOSED,
   RET_TRADE_DISABLE,
   RET_TRADE_NO_MONEY,
   RET_TRADE_PRICE_CHANGED,
   RET_TRADE_OFFQUOTES,
   RET_TRADE_BROKER_BUSY,
   RET_TRADE_REQUOTE,
   RET_TRADE_ORDER_LOCKED,
   RET_TRADE_LONG_ONLY,
   RET_TRADE_TOO_MANY_REQ,
   RET_TRADE_ACCEPTED,
   RET_TRADE_PROCESS,
   RET_TRADE_USER_CANCEL,
   RET_TRADE_MODIFY_DENIED,
   RET_TRADE_CONTEXT_BUSY,
   RET_TRADE_EXPIRATION_DENIED,
   RET_TRADE_TOO_MANY_ORDERS,
   RET_TRADE_HEDGE_PROHIBITED,
   RET_TRADE_PROHIBITED_BY_FIFO
  };

enum
  {
   CLIENT_FLAGS_HIDETICKS   =1,
   CLIENT_FLAGS_HIDENEWS    =2,
   CLIENT_FLAGS_HIDEMAIL    =4,
   CLIENT_FLAGS_SENDFULLNEWS=8,
   CLIENT_FLAGS_RESERVED    =16,
   CLIENT_FLAGS_HIDEONLINE  =32,
   CLIENT_FLAGS_HIDEUSERS   =64
  };

struct FeedDescription
  {
   int               version;
   char              name[128];
   char              copyright[128];
   char              web[128];
   char              email[128];
   char              server[128];
   char              username[32];
   char              userpass[32];
   int               modes;
   char              description[512];
   char              module[32];
   int               reserved[54];
  };

enum FeederModes
  {
   modeOnlyQuotes    =0,
   modeOnlyNews      =1,
   modeQuotesAndNews =2,
   modeQuotesOrNews  =3
  };

struct ServerFeed
  {
   char              file[256];
   FeedDescription   feed;
  };

struct ChartInfo
  {
   char              symbol[12];
   int               period;
   __time32_t        start;
   __time32_t        end;
   __time32_t        timesign;
   int               mode;
  };

enum { PERIOD_M1=1,  PERIOD_M5=5,   PERIOD_M15=15,  PERIOD_M30=30,
       PERIOD_H1=60, PERIOD_H4=240, PERIOD_D1=1440, PERIOD_W1=10080,
       PERIOD_MN1=43200 };

enum { CHART_RANGE_IN,CHART_RANGE_OUT,CHART_RANGE_LAST };

#pragma pack(push,1)
struct RateInfoOld
  {
   __time32_t        ctm;
   int               open;
   short             high,low,close;
   double            vol;
  };
struct RateInfo
  {
   __time32_t        ctm;
   int               open;
   int               high,low,close;
   double            vol;
  };
#pragma pack(pop)

#pragma pack(push,1)
struct TickRecord
  {
   __time32_t        ctm;
   double            bid,ask;
   int               datafeed;
   char              flags;
  };
#pragma pack(pop)

enum { TICK_FLAG_RAW=1, TICK_FLAG_NORMAL=2,TICK_FLAG_ALL=TICK_FLAG_RAW+TICK_FLAG_NORMAL };

#pragma pack(push,1)
struct TickRequest
  {
   char              symbol[12];
   __time32_t        from;
   __time32_t        to;
   char              flags;
  };
#pragma pack(pop)

#pragma pack(push,1)
struct PerformanceInfo
  {
   __time32_t        ctm;
   short             users;
   short             cpu;
   int               freemem;
   int               network;
   int               sockets;
  };
#pragma pack(pop)

struct BackupInfo
  {
   char              file[256];
   int               size;
   __time32_t        time;
   int               reserved[6];
  };

enum
  {
   BACKUPS_ALL,
   BACKUPS_PERIODICAL,
   BACKUPS_STARTUP,
   BACKUPS_DELETE
  };

enum { TRANS_ADD, TRANS_DELETE, TRANS_UPDATE, TRANS_CHANGE_GRP };

#define PUBLIC_KEY_SIZE    272
#define USER_COLOR_NONE    (0xFF000000)

struct UserRecord
  {
   int               login;
   char              group[16];
   char              password[16];
   int               enable;
   int               enable_change_password;
   int               enable_read_only;
   int               enable_reserved[3];
   char              password_investor[16];
   char              password_phone[32];
   char              name[128];
   char              country[32];
   char              city[32];
   char              state[32];
   char              zipcode[16];
   char              address[128];
   char              phone[32];
   char              email[48];
   char              comment[64];
   char              id[32];
   char              status[16];
   __time32_t        regdate;
   __time32_t        lastdate;
   int               leverage;
   int               agent_account;
   __time32_t        timestamp;
   int               last_ip;
   double            balance;
   double            prevmonthbalance;
   double            prevbalance;
   double            credit;
   double            interestrate;
   double            taxes;
   double            prevmonthequity;
   double            prevequity;
   double            reserved2[2];
   char              publickey[PUBLIC_KEY_SIZE];
   int               send_reports;
   unsigned int      mqid;
   COLORREF          user_color;
   char              unused[40];
   char              api_data[16];
  };

#pragma pack(push,1)
struct GroupCommandInfo
  {
   int               len;
   char              command;
   char              newgroup[16];
   int               leverage;
   int               reserved[8];
  };
#pragma pack(pop)

enum { GROUP_DELETE,GROUP_ENABLE,GROUP_DISABLE,GROUP_LEVERAGE,GROUP_SETGROUP };

struct OnlineRecord
  {
   int               counter;
   int               reserved;
   int               login;
   UINT              ip;
   char              group[16];
  };

#pragma pack(push,1)
struct TradeRecord
  {
   int               order;
   int               login;
   char              symbol[12];
   int               digits;
   int               cmd;
   int               volume;
   __time32_t        open_time;
   int               state;
   double            open_price;
   double            sl,tp;
   __time32_t        close_time;
   int               gw_volume;
   __time32_t        expiration;
   char              reason;
   char              conv_reserv[3];
   double            conv_rates[2];
   double            commission;
   double            commission_agent;
   double            storage;
   double            close_price;
   double            profit;
   double            taxes;
   int               magic;
   char              comment[32];
   int               gw_order;
   int               activation;
   short             gw_open_price;
   short             gw_close_price;
   double            margin_rate;
   __time32_t        timestamp;
   int               api_data[4];
   TradeRecord *__ptr32 next;
  };
#pragma pack(pop)

enum { OP_BUY=0,OP_SELL,OP_BUY_LIMIT,OP_SELL_LIMIT,OP_BUY_STOP,OP_SELL_STOP,OP_BALANCE,OP_CREDIT };
enum { TS_OPEN_NORMAL, TS_OPEN_REMAND, TS_OPEN_RESTORED, TS_CLOSED_NORMAL, TS_CLOSED_PART, TS_CLOSED_BY, TS_DELETED };
enum { TR_REASON_CLIENT, TR_REASON_EXPERT, TR_REASON_DEALER,TR_REASON_SIGNAL, TR_REASON_GATEWAY };

enum {
      ACTIVATION_NONE=0,
      ACTIVATION_SL,ACTIVATION_TP,ACTIVATION_PENDING,
      ACTIVATION_STOPOUT,
      ACTIVATION_SL_ROLLBACK     =-ACTIVATION_SL,
      ACTIVATION_TP_ROLLBACK     =-ACTIVATION_TP,
      ACTIVATION_PENDING_ROLLBACK=-ACTIVATION_PENDING,
      ACTIVATION_STOPOUT_ROLLBACK=-ACTIVATION_STOPOUT
     };

#pragma pack(push,1)
struct TradeRestoreResult
  {
   int               order;
   UCHAR             res;
  };
#pragma pack(pop)

#pragma pack(push,1)
struct TradeTransInfo
  {
   UCHAR             type;
   char              reserved;
   short             cmd;
   int               order,orderby;
   char              symbol[12];
   int               volume;
   double            price;
   double            sl,tp;
   int               ie_deviation;
   char              comment[32];
   __time32_t        expiration;
   int               crc;
  };
#pragma pack(pop)

enum
  {
   TT_PRICES_GET,
   TT_PRICES_REQUOTE,
   TT_ORDER_IE_OPEN=64,
   TT_ORDER_REQ_OPEN,
   TT_ORDER_MK_OPEN,
   TT_ORDER_PENDING_OPEN,
   TT_ORDER_IE_CLOSE,
   TT_ORDER_REQ_CLOSE,
   TT_ORDER_MK_CLOSE,
   TT_ORDER_MODIFY,
   TT_ORDER_DELETE,
   TT_ORDER_CLOSE_BY,
   TT_ORDER_CLOSE_ALL,
   TT_BR_ORDER_OPEN,
   TT_BR_ORDER_CLOSE,
   TT_BR_ORDER_DELETE,
   TT_BR_ORDER_CLOSE_BY,
   TT_BR_ORDER_CLOSE_ALL,
   TT_BR_ORDER_MODIFY,
   TT_BR_ORDER_ACTIVATE,
   TT_BR_ORDER_COMMENT,
   TT_BR_BALANCE
  };

struct MarginLevel
  {
   int               login;
   char              group[16];
   int               leverage;
   int               updated;
   double            balance;
   double            equity;
   int               volume;
   double            margin;
   double            margin_free;
   double            margin_level;
   int               margin_type;
   int               level_type;
  };

enum { MARGINLEVEL_OK=0, MARGINLEVEL_MARGINCALL, MARGINLEVEL_STOPOUT };

struct RequestInfo
  {
   int               id;
   char              status;
   DWORD             time;
   int               manager;
   int               login;
   char              group[16];
   double            balance;
   double            credit;
   double            prices[2];
   TradeTransInfo    trade;
   int               gw_volume;
   int               gw_order;
   short             gw_price;
   RequestInfo* __ptr32 prev;
   RequestInfo* __ptr32 next;
  };

enum { DC_EMPTY,DC_REQUEST,DC_LOCKED,DC_ANSWERED,DC_RESETED,DC_CANCELED };

#define TIME_RATE         ((double)1.6777216)
#define STDTIME(custom_time) ((DWORD)((double)(custom_time)*TIME_RATE))
#define OURTIME(stdtime)     ((DWORD)((double)(stdtime)/TIME_RATE))

enum EnConfirmModes
  {
   CONFIRM_MODE_ADD_PRICES=0x00000001,
   CONFIRM_MODE_PACKET    =0x00000002
  };

struct DailyReport
  {
   int               login;
   __time32_t        ctm;
   char              group[16];
   char              bank[64];
   double            balance_prev;
   double            balance;
   double            deposit;
   double            credit;
   double            profit_closed;
   double            profit;
   double            equity;
   double            margin;
   double            margin_free;
   int               next;
   int               reserved[3];
  };

#pragma pack(push,1)
struct ReportGroupRequest
  {
   char              name[32];
   __time32_t        from;
   __time32_t        to;
   int               total;
  };
#pragma pack(pop)

struct DailyGroupRequest
  {
   char              name[32];
   __time32_t        from;
   __time32_t        to;
   int               total;
   int               reserved;
  };

struct SymbolInfo
  {
   char              symbol[12];
   int               digits;
   int               count;
   int               visible;
   int               type;
   double            point;
   int               spread;
   int               spread_balance;
   int               direction;
   int               updateflag;
   __time32_t        lasttime;
   double            bid,ask;
   double            high,low;
   double            commission;
   int               comm_type;
  };

enum { SDIR_UP,SDIR_DOWN,SDIR_NONE };

struct SymbolSummary
  {
   char              symbol[12];
   int               count;
   int               digits;
   int               type;
   int               orders;
   __int64           buylots;
   __int64           selllots;
   double            buyprice;
   double            sellprice;
   double            profit;
   int               covorders;
   __int64           covbuylots;
   __int64           covselllots;
   double            covbuyprice;
   double            covsellprice;
   double            covprofit;
  };

struct ExposureValue
  {
   char              currency[4];
   double            clients;
   double            coverage;
  };

#pragma pack(push,1)
struct SymbolPropertiesOld
  {
   char              symbol[12];
   COLORREF          color;
   int               spread;
   int               spread_balance;
   int               stops_level;
   int               exemode;
  };
#pragma pack(pop)

#pragma pack(push,1)
struct SymbolProperties
  {
   char              symbol[12];
   COLORREF          color;
   int               spread;
   int               spread_balance;
   int               stops_level;
   int               smoothing;
   int               exemode;
   int               reserved[8];
  };
#pragma pack(pop)

struct TickInfo
  {
   char              symbol[12];
   __time32_t        ctm;
   double            bid;
   double            ask;
  };

struct MailBox
  {
   __time32_t        time;
   int               sender;
   char              from[64];
   int               to;
   char              subject[128];
   int               readed;
   char* __ptr32     body;
   int               bodylen;
   short             build_min;
   short             build_max;
   int               reserved;
  };

struct NewsTopic
  {
   ULONG             key;
   __time32_t        time;
   char              ctm[32];
   char              topic[256];
   char              category[64];
   char              keywords[256];
   char* __ptr32     body;
   int               bodylen;
   int               readed;
   int               priority;
   int               langid;
   int               reserved[1];
  };

struct ServerLog
  {
   int               code;
   char              time[24];
   char              ip[20];
   char              message[512];
  };

enum EnErrLogTypes { 
                    CmdOK,
                    CmdTrade,
                    CmdLogin,
                    CmdWarn,
                    CmdErr,
                    CmdAtt
                   };

enum EnLogType     { 
                    LOG_TYPE_STANDARD,
                    LOG_TYPE_LOGINS,
                    LOG_TYPE_TRADES,
                    LOG_TYPE_ERRORS,
                    LOG_TYPE_FULL
                   };

enum EnLogMode     { 
                    LOG_MODE_ENABLED,
                    LOG_MODE_DISABLED,
                   };

enum
  {
   PUMP_START_PUMPING=0,
   PUMP_UPDATE_SYMBOLS,
   PUMP_UPDATE_GROUPS,
   PUMP_UPDATE_USERS,
   PUMP_UPDATE_ONLINE,
   PUMP_UPDATE_BIDASK,
   PUMP_UPDATE_NEWS,
   PUMP_UPDATE_NEWS_BODY,
   PUMP_UPDATE_MAIL,
   PUMP_UPDATE_TRADES,
   PUMP_UPDATE_REQUESTS,
   PUMP_UPDATE_PLUGINS,
   PUMP_UPDATE_ACTIVATION,
   PUMP_UPDATE_MARGINCALL,
   PUMP_STOP_PUMPING,
   PUMP_PING
  };

enum
  {
   DEAL_START_DEALING=0,
   DEAL_REQUEST_NEW,
   DEAL_STOP_DEALING
  };

typedef void (__stdcall *MTAPI_NOTIFY_FUNC)(int code);
typedef void (__stdcall *MTAPI_NOTIFY_FUNC_EX)(int code,int type,void * data,void *param);

class CManagerInterface
  {
public:
   virtual int    __stdcall QueryInterface(REFIID riid,LPVOID* obj)=0;
   virtual int    __stdcall AddRef() =0;
   virtual int    __stdcall Release()=0;
   virtual void   __stdcall MemFree(void* ptr)              =0;
   virtual LPCSTR __stdcall ErrorDescription(const int code)=0;
   virtual void   __stdcall WorkingDirectory(LPCSTR path)   =0;
   virtual int  __stdcall Connect(LPCSTR server)                =0;
   virtual int  __stdcall Disconnect()                          =0;
   virtual int  __stdcall IsConnected()                         =0;
   virtual int  __stdcall Login(const int login,LPCSTR password)=0;
   virtual int  __stdcall LoginSecured(LPCSTR key_path)         =0;
   virtual int  __stdcall KeysSend(LPCSTR key_path)             =0;
   virtual int  __stdcall Ping()                                =0;
   virtual int  __stdcall PasswordChange(LPCSTR pass,const int is_investor)=0;
   virtual int  __stdcall ManagerRights(ConManager *man)        =0;
   virtual int  __stdcall SrvRestart()        =0;
   virtual int  __stdcall SrvChartsSync()     =0;
   virtual int  __stdcall SrvLiveUpdateStart()=0;
   virtual int  __stdcall SrvFeedsRestart()   =0;
   virtual int  __stdcall CfgRequestCommon(ConCommon *cfg)=0;
   virtual int  __stdcall CfgRequestTime(ConTime *cfg)    =0;
   virtual int  __stdcall CfgRequestBackup(ConBackup *cfg)=0;
   virtual int  __stdcall CfgRequestSymbolGroup(ConSymbolGroup *cfg)=0;
   virtual ConAccess*      __stdcall CfgRequestAccess(int *total)    =0;
   virtual ConDataServer*  __stdcall CfgRequestDataServer(int *total)=0;
   virtual ConHoliday*     __stdcall CfgRequestHoliday(int *total)   =0;
   virtual ConSymbol*      __stdcall CfgRequestSymbol(int *total)    =0;
   virtual ConGroup*       __stdcall CfgRequestGroup(int *total)     =0;
   virtual ConManager*     __stdcall CfgRequestManager(int *total)   =0;
   virtual ConFeeder*      __stdcall CfgRequestFeeder(int *total)    =0;
   virtual ConLiveUpdate*  __stdcall CfgRequestLiveUpdate(int *total)=0;
   virtual ConSync*        __stdcall CfgRequestSync(int *total)      =0;
   virtual ConPluginParam* __stdcall CfgRequestPlugin(int *total)    =0;
   virtual int  __stdcall CfgUpdateCommon(const ConCommon* cfg)                        =0;
   virtual int  __stdcall CfgUpdateAccess(const ConAccess* cfg,const int pos)          =0;
   virtual int  __stdcall CfgUpdateDataServer(const ConDataServer *cfg,const int pos)  =0;
   virtual int  __stdcall CfgUpdateTime(const ConTime *cfg)                            =0;
   virtual int  __stdcall CfgUpdateHoliday(const ConHoliday *cfg,const int pos)        =0;
   virtual int  __stdcall CfgUpdateSymbol(const ConSymbol *cfg)                        =0;
   virtual int  __stdcall CfgUpdateSymbolGroup(const ConSymbolGroup *cfg,const int pos)=0;
   virtual int  __stdcall CfgUpdateGroup(const ConGroup *cfg)                          =0;
   virtual int  __stdcall CfgUpdateManager(const ConManager *cfg)                      =0;
   virtual int  __stdcall CfgUpdateFeeder(const ConFeeder *cfg)                        =0;
   virtual int  __stdcall CfgUpdateBackup(const ConBackup *cfg)                        =0;
   virtual int  __stdcall CfgUpdateLiveUpdate(const ConLiveUpdate *cfg)                =0;
   virtual int  __stdcall CfgUpdateSync(const ConSync *cfg)                            =0;
   virtual int  __stdcall CfgUpdatePlugin(const ConPlugin *cfg,const PluginCfg *parupd,const int total)=0;
   virtual int  __stdcall CfgDeleteAccess(const int pos)    =0;
   virtual int  __stdcall CfgDeleteDataServer(const int pos)=0;
   virtual int  __stdcall CfgDeleteHoliday(const int pos)   =0;
   virtual int  __stdcall CfgDeleteSymbol(const int pos)    =0;
   virtual int  __stdcall CfgDeleteGroup(const int pos)     =0;
   virtual int  __stdcall CfgDeleteManager(const int pos)   =0;
   virtual int  __stdcall CfgDeleteFeeder(const int pos)    =0;
   virtual int  __stdcall CfgDeleteLiveUpdate(const int pos)=0;
   virtual int  __stdcall CfgDeleteSync(const int pos)      =0;
   virtual int  __stdcall CfgShiftAccess(const int pos,const int shift)    =0;
   virtual int  __stdcall CfgShiftDataServer(const int pos,const int shift)=0;
   virtual int  __stdcall CfgShiftHoliday(const int pos,const int shift)   =0;
   virtual int  __stdcall CfgShiftSymbol(const int pos,const int shift)    =0;
   virtual int  __stdcall CfgShiftGroup(const int pos,const int shift)     =0;
   virtual int  __stdcall CfgShiftManager(const int pos,const int shift)   =0;
   virtual int  __stdcall CfgShiftFeeder(const int pos,const int shift)    =0;
   virtual int  __stdcall CfgShiftLiveUpdate(const int pos,const int shift)=0;
   virtual int  __stdcall CfgShiftSync(const int pos,const int shift)      =0;
   virtual int  __stdcall CfgShiftPlugin(const int pos,const int shift)    =0;
   virtual ServerFeed* __stdcall SrvFeeders(int *total)            =0;
   virtual LPSTR       __stdcall SrvFeederLog(LPCSTR name,int *len)=0;
   virtual RateInfoOld* __stdcall ChartRequestObsolete(const ChartInfo *chart,__time32_t *timesign,int *total)            =0;
   virtual int          __stdcall ChartAddObsolete(LPCSTR symbol,const int period,const RateInfoOld* rates,int *count)   =0;
   virtual int          __stdcall ChartUpdateObsolete(LPCSTR symbol,const int period,const RateInfoOld* rates,int *count)=0;
   virtual int          __stdcall ChartDeleteObsolete(LPCSTR symbol,const int period,const RateInfoOld* rates,int *count)=0;
   virtual PerformanceInfo* __stdcall PerformaneRequest(__time32_t from,int *total)=0;
   virtual BackupInfo*  __stdcall BackupInfoUsers(const int mode,int *total) =0;
   virtual BackupInfo*  __stdcall BackupInfoOrders(const int mode,int *total)=0;
   virtual UserRecord*  __stdcall BackupRequestUsers(LPCSTR file,LPCSTR request,int *total) =0;
   virtual TradeRecord* __stdcall BackupRequestOrders(LPCSTR file,LPCSTR request,int *total)=0;
   virtual int          __stdcall BackupRestoreUsers(const UserRecord *users,const int total)     =0;
   virtual TradeRestoreResult* __stdcall BackupRestoreOrders(const TradeRecord *trades,int *total)=0;
   virtual UserRecord*  __stdcall AdmUsersRequest(LPCSTR group,int *total)          =0;
   virtual TradeRecord* __stdcall AdmTradesRequest(LPCSTR group,const int open_only,int *total)=0;
   virtual int          __stdcall AdmBalanceCheck(int *logins,int *total)           =0;
   virtual int          __stdcall AdmBalanceFix(const int *logins,const int total)  =0;
   virtual int          __stdcall AdmTradesDelete(const int *orders,const int total)=0;
   virtual int          __stdcall AdmTradeRecordModify(const TradeRecord *trade)    =0;
   virtual int          __stdcall SymbolsRefresh()                           =0;
   virtual ConSymbol*   __stdcall SymbolsGetAll(int *total)                  =0;
   virtual int          __stdcall SymbolGet(LPCSTR symbol,ConSymbol *cs)     =0;
   virtual int          __stdcall SymbolInfoGet(LPCSTR symbol,SymbolInfo *si)=0;
   virtual int          __stdcall SymbolAdd(LPCSTR symbol)                   =0;
   virtual int          __stdcall SymbolHide(LPCSTR symbol)                  =0;
   virtual int          __stdcall SymbolChangeObsolete(const SymbolPropertiesOld *prop)=0;
   virtual int          __stdcall SymbolSendTick(LPCSTR symbol,const double bid,const double ask)=0;
   virtual ConGroup*    __stdcall GroupsRequest(int *total)                      =0;
   virtual int          __stdcall MailSend(const MailBox* mail,const int *logins)=0;
   virtual int          __stdcall NewsSend(const NewsTopic *news)                =0;
   virtual ServerLog*   __stdcall JournalRequest(const int mode,const __time32_t from,const __time32_t to,LPCSTR filter,int *total)=0;
   virtual UserRecord*  __stdcall UsersRequest(int *total)                        =0;
   virtual UserRecord*  __stdcall UserRecordsRequest(const int *logins,int *total)=0;
   virtual int          __stdcall UserRecordNew(UserRecord *user)                 =0;
   virtual int          __stdcall UserRecordUpdate(const UserRecord *user)        =0;
   virtual int          __stdcall UsersGroupOp(const GroupCommandInfo *info,const int *logins)=0;
   virtual int          __stdcall UserPasswordCheck(const int login,LPCSTR password)=0;
   virtual int          __stdcall UserPasswordSet(const int login,LPCSTR password,const int change_investor,const int clean_pubkey)=0;
   virtual OnlineRecord* __stdcall OnlineRequest(int *total)              =0;
   virtual int          __stdcall TradeTransaction(TradeTransInfo *info)           =0;
   virtual TradeRecord* __stdcall TradesRequest(int *total)                        =0;
   virtual TradeRecord* __stdcall TradeRecordsRequest(const int *orders,int *total)=0;
   virtual TradeRecord* __stdcall TradesUserHistory(const int login,const __time32_t from,const __time32_t to,int *total)=0;
   virtual int          __stdcall TradeCheckStops(const TradeTransInfo *trade,const double price)=0;
   virtual TradeRecord* __stdcall ReportsRequest(const ReportGroupRequest *req,const int *logins,int *total)    =0;
   virtual DailyReport* __stdcall DailyReportsRequest(const DailyGroupRequest *req,const int *logins,int *total)=0;
   virtual int          __stdcall ExternalCommand(LPCSTR data_in,const int size_in,LPSTR *data_out,int *size_out)=0;
   virtual int          __stdcall PluginUpdate(const ConPluginParam *plugin)=0;
   virtual int          __stdcall PumpingSwitch(MTAPI_NOTIFY_FUNC pfnFunc,const HWND destwnd,const UINT eventmsg,const int flags)=0;
   virtual ConGroup*    __stdcall GroupsGet(int *total)                                    =0;
   virtual int          __stdcall GroupRecordGet(LPCSTR name,ConGroup *group)              =0;
   virtual int          __stdcall SymbolInfoUpdated(SymbolInfo *si,const int max_info)     =0;
   virtual UserRecord*  __stdcall UsersGet(int *total)                                     =0;
   virtual int          __stdcall UserRecordGet(const int login,UserRecord *user)          =0;
   virtual OnlineRecord* __stdcall OnlineGet(int *total)                                   =0;
   virtual int          __stdcall OnlineRecordGet(const int login,OnlineRecord *user)      =0;
   virtual TradeRecord* __stdcall TradesGet(int *total)                                    =0;
   virtual TradeRecord* __stdcall TradesGetBySymbol(LPCSTR symbol,int *total)              =0;
   virtual TradeRecord* __stdcall TradesGetByLogin(const int login,LPCSTR group,int *total)=0;
   virtual TradeRecord* __stdcall TradesGetByMarket(int *total)                            =0;
   virtual int          __stdcall TradeRecordGet(const int order,TradeRecord *trade)       =0;
   virtual int          __stdcall TradeClearRollback(const int order)                      =0;
   virtual MarginLevel* __stdcall MarginsGet(int *total)                                   =0;
   virtual int          __stdcall MarginLevelGet(const int login,LPCSTR group,MarginLevel *margin)=0;
   virtual RequestInfo* __stdcall RequestsGet(int *total)                                  =0;
   virtual int          __stdcall RequestInfoGet(const int pos,RequestInfo *info)          =0;
   virtual ConPlugin*   __stdcall PluginsGet(int *total)                                   =0;
   virtual int          __stdcall PluginParamGet(const int pos,ConPluginParam *plugin)     =0;
   virtual int          __stdcall MailLast(LPSTR path,int *length)                         =0;
   virtual NewsTopic*   __stdcall NewsGet(int *total)                                      =0;
   virtual int          __stdcall NewsTotal()                                              =0;
   virtual int          __stdcall NewsTopicGet(const int pos,NewsTopic *news)              =0;
   virtual void         __stdcall NewsBodyRequest(const int key)                           =0;
   virtual LPSTR        __stdcall NewsBodyGet(const int key)                               =0;
   virtual int          __stdcall DealerSwitch(MTAPI_NOTIFY_FUNC pfnFunc,const HWND destwnd,const UINT eventmsg)=0;
   virtual int          __stdcall DealerRequestGet(RequestInfo *info)   =0;
   virtual int          __stdcall DealerSend(const RequestInfo *info,const int requote,const int mode)=0;
   virtual int          __stdcall DealerReject(const int id)            =0;
   virtual int          __stdcall DealerReset(const int id)             =0;
   virtual TickInfo*    __stdcall TickInfoLast(LPCSTR symbol,int *total)=0;
   virtual int          __stdcall SymbolsGroupsGet(ConSymbolGroup *grp) =0;
   virtual __time32_t   __stdcall ServerTime()                          =0;
   virtual MailBox*     __stdcall MailsRequest(int *total)              =0;
   virtual SymbolSummary* __stdcall SummaryGetAll(int *total)                            =0;
   virtual int          __stdcall SummaryGet(LPCSTR symbol,SymbolSummary *info)          =0;
   virtual int          __stdcall SummaryGetByCount(const int symbol,SymbolSummary *info)=0;
   virtual int          __stdcall SummaryGetByType(const int sectype,SymbolSummary *info)=0;
   virtual int          __stdcall SummaryCurrency(LPSTR cur,const int maxchars)          =0;
   virtual ExposureValue* __stdcall ExposureGet(int *total)                              =0;
   virtual int          __stdcall ExposureValueGet(LPCSTR cur,ExposureValue *info)       =0;
   virtual int          __stdcall MarginLevelRequest(const int login,MarginLevel *level) =0;
   virtual int          __stdcall HistoryCorrect(LPCSTR symbol,int *updated)             =0;
   virtual RateInfo *   __stdcall ChartRequest(const ChartInfo *chart,__time32_t *timesign,int *total)            =0;
   virtual int          __stdcall ChartAdd(LPCSTR symbol,const int period,const RateInfo *rates,int *count)   =0;
   virtual int          __stdcall ChartUpdate(LPCSTR symbol,const int period,const RateInfo *rates,int *count)=0;
   virtual int          __stdcall ChartDelete(LPCSTR symbol,const int period,const RateInfo *rates,int *count)=0;
   virtual TickRecord*  __stdcall TicksRequest(const TickRequest *request,int *total)=0;
   virtual int          __stdcall PumpingSwitchEx(MTAPI_NOTIFY_FUNC_EX pfnFunc,const int flags,void *param)=0;
   virtual int          __stdcall UsersSyncStart(const __time32_t timestamp)=0;
   virtual UserRecord*  __stdcall UsersSyncRead(int* users_total)=0;
   virtual int*         __stdcall UsersSnapshot(int* users_total)=0;
   virtual int          __stdcall TradesSyncStart(const __time32_t timestamp)=0;
   virtual TradeRecord* __stdcall TradesSyncRead(int* trades_total)=0;
   virtual int*         __stdcall TradesSnapshot(int* trades_total)=0;
   virtual int          __stdcall DailySyncStart(const __time32_t timestamp)=0;
   virtual DailyReport* __stdcall DailySyncRead(int* daily_total)=0;
   virtual int          __stdcall TradeCalcProfit(TradeRecord *trade)=0;
   virtual int          __stdcall SymbolChange(const SymbolProperties *prop) =0;
   virtual int          __stdcall BytesSent()=0;
   virtual int          __stdcall BytesReceived()=0;
   virtual int          __stdcall ManagerCommon(ConCommon *common)=0;
   virtual void         __stdcall LogsOut(const int code,LPCSTR source,LPCSTR msg)=0;
   virtual void         __stdcall LogsMode(const int mode)=0;
   virtual int         __stdcall  LicenseCheck(LPCSTR license_name)=0;
   virtual ConGatewayAccount* __stdcall CfgRequestGatewayAccount(int *total)=0;
   virtual ConGatewayMarkup*  __stdcall CfgRequestGatewayMarkup(int *total)=0;
   virtual ConGatewayRule*    __stdcall CfgRequestGatewayRule(int *total)=0;
   virtual int          __stdcall CfgUpdateGatewayAccount(const ConGatewayAccount* cfg)=0;
   virtual int          __stdcall CfgUpdateGatewayMarkup(const ConGatewayMarkup* cfg)=0;
   virtual int          __stdcall CfgUpdateGatewayRule(const ConGatewayRule *cfg)=0;
   virtual int          __stdcall CfgDeleteGatewayAccount(const int pos)=0;
   virtual int          __stdcall CfgDeleteGatewayMarkup(const int pos)=0;
   virtual int          __stdcall CfgDeleteGatewayRule(const int pos)=0;
   virtual int          __stdcall CfgShiftGatewayAccount(const int pos,const int shift)=0;
   virtual int          __stdcall CfgShiftGatewayMarkup(const int pos,const int shift)=0;
   virtual int          __stdcall CfgShiftGatewayRule(const int pos,const int shift)=0;
  };

typedef int (*MtManVersion_t)(void);
typedef int (*MtManCreate_t)(int version,CManagerInterface **man);
