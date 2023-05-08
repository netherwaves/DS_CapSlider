#include "wifi.h"

namespace daisy
{

WiFi::Result WiFi::Init(Config &config)
{
    this->mux = config.mux;

    // initialize USART port
    uart.Init(config.uart_cfg);
    uart.DmaReceiveFifo();

    // run a test to make sure that the ESP responds
    return SendCommand(AT_TEST);
}

WiFi::Result WiFi::SetMode(Config::Mode mode)
{
    char cmd[100];
    sprintf(cmd, AT_MODE, mode);

    return SendCommand(cmd);
}

WiFi::Result WiFi::Connect(const char *ssid, const char *pw)
{
    char cmd[100];
    sprintf(cmd, AT_CONNECT, ssid, pw);

    return SendCommand(cmd);
}

WiFi::Result WiFi::SendCommand(const char *cmd)
{
    // send command to serial port
    uart.BlockingTransmit((uint8_t *)cmd, strlen(cmd), 1000);

    // prepare variables
    Result result;
    wait_response = true;

    // TODO: move this to another thread so it doesnt block everything else..
    while(wait_response)
    {
        if(uart.ReadableFifo())
        {
            // set rx size for blocking receive
            rx_size = uart.ReadableFifo();
            for(size_t i = 0; i < rx_size; i++)
            {
                buf[i] = uart.PopFifo();
            }
            // make it null-terminated
            buf[rx_size] = '\0';

            // check if return value is OK
            if(strstr((const char *)buf, "OK\r\n"))
            {
                result        = Result::OK;
                wait_response = false;
            }
            // or if there was an error
            else if(strstr((const char *)buf, "ERROR\r\n"))
            {
                result        = Result::ERR;
                wait_response = false;
            }
        }
    }

    // return result
    return result;
}

} // namespace daisy