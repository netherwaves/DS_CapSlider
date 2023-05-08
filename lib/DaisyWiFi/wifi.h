#pragma once

#include "daisy.h"
#include "daisy_seed.h"

namespace daisy
{
// commands
#define AT_TEST "AT\r\n"
#define AT_MODE "AT+CWMODE=%d\r\n"
#define AT_CONNECT "AT+CWJAP=\"%s\",\"%s\"\r\n"
#define AT_MUX "AT+CIPMUX=%d\r\n"
#define AT_SERVERINIT "AT+CIPSERVER=%d,%d\r\n"
#define AT_UDPSTART "AT+CIPSTART=%d,\"UDP\",\"%s\",%d,%d,%d\r\n"
#define AT_UDPSEND "AT+CIPSEND=0,%d,\"%s\",%d\r\n"

class WiFi
{
  public:
    struct Config
    {
        enum Mode
        {
            STA = 1,
            AP,
            BOTH
        };

        UartHandler::Config uart_cfg;

        bool mux  = true;
        Mode mode = Mode::STA;
    };

    enum Result
    {
        OK,
        ERR
    };

    Result Init(Config &);
    Result SetMode(Config::Mode);
    Result Connect(const char *, const char *);

  private:
    Result SendCommand(const char *cmd);

    // config parameters
    UartHandler uart;
    bool        mux;

    bool    wait_response = false;
    uint8_t buf[1024];
    size_t  rx_size;
};
} // namespace daisy