#include <stddef.h>
#include <stdbool.h>
#include "slips.h"

#define END 0300
#define ESC 0333
#define ESC_END 0334
#define ESC_ESC 0335

static slips_config *config;

void slips_init(slips_config *config_) {
    config = config_;
}

void slips_deinit(void) {
    config = NULL;
}

bool slips_send_packet(void)
{
    char c;
    bool eof;

    if (config->send_start)
    {
        if (!config->encode_send_char(END, config->user_data))
            return false;
    }

    while (true)
    {
        if (!config->encode_read_char(&c, &eof, config->user_data))
            return false;

        if (eof)
            break;

        switch (c)
        {
        case END:
            if (!config->encode_send_char(ESC, config->user_data))
                return false;
            if (!config->encode_send_char(ESC_END, config->user_data))
                return false;
            break;

        case ESC:
            if (!config->encode_send_char(ESC, config->user_data))
                return false;
            if (!config->encode_send_char(ESC_ESC, config->user_data))
                return false;
            break;

        default:
            if (!config->encode_send_char(c, config->user_data))
                return false;
        }
    }

    if (!config->encode_send_char(END, config->user_data))
        return false;

    return true;
}

bool slip_recv_packet(void)
{
    char c;
    bool initial = true;

    while (true)
    {
        if (!config->decode_recv_char(&c, config->user_data))
            return false;

        if (initial && config->check_start) 
        {
            initial = false;

            if (c == END)
                continue;
            else
                return false;
        }

        switch (c)
        {
        case END:
            break;

        case ESC:
            if (!config->decode_recv_char(&c, config->user_data))
                return false;

            switch (c)
            {
            case ESC_END:
                c = END;
                break;
            case ESC_ESC:
                c = ESC;
                break;
            /* Fallthrough intentional */
            }

        default:
            if (!config->decode_write_char(c, config->user_data))
                return false;
        }
    }

    return true;
}
