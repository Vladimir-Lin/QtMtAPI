#include <QtCore>
#include <QtMtAPI>

int main(int argc,char * argv [])
{
  if ( argc < 4 ) return 0                                  ;
  #ifdef Q_OS_WIN
  WSADATA wsa                                               ;
  ::WSAStartup ( 0x0202 , &wsa )                            ;
  #endif
  ///////////////////////////////////////////////////////////
  QtMtAPI * qma = new QtMtAPI ( )                           ;
  QString   IP  ( argv [ 1 ] )                              ;
  bool      connected                                       ;
  ///////////////////////////////////////////////////////////
  qma -> Load       ( "" )                                  ;
  qma -> Initialize ( QtMtAPI::MtApiManager                 |
                      QtMtAPI::MtApiDealer                  |
                      QtMtAPI::MtApiPump                  ) ;
  qma -> Account  = QString ( argv [ 2 ] )                  ;
  qma -> Password = QString ( argv [ 3 ] )                  ;
  ///////////////////////////////////////////////////////////
  printf ( "Trying connect to %s...\r\n" , argv [ 1 ] )     ;
  connected = qma -> FullMount ( IP )                       ;
  if ( ! qma->isConnected() )                               {
    printf ( "%s can not be connected\r\n" , argv [ 1 ] )   ;
  } else                                                    {
    ConSymbol * cs                                          ;
    int         total = 0                                   ;
    printf ( "Get symbols\r\n" )                            ;
    qma->Manager->SymbolsRefresh()                          ;
    cs = qma->Manager->SymbolsGetAll(&total)                ;
    if ( total > 0 )                                        {
      for (int i=0;i<total;i++)                             {
        printf ( "%s\r\n" , cs[i].symbol )                  ;
      }                                                     ;
    }                                                       ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  printf ( "Disconnected\r\n" )                             ;
  qma -> disconnect ( )                                     ;
  delete qma                                                ;
  ///////////////////////////////////////////////////////////
  #ifdef Q_OS_WIN
  ::WSACleanup ( )                                          ;
  #endif
  ///////////////////////////////////////////////////////////
  return 1                                                  ;
}
