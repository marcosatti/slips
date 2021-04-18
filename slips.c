#include <stddef.h>
#include <stdbool.h>
#include "slips.h"

#define END 0300
#define ESC 0333
#define ESC_END 0334
#define ESC_ESC 0335

bool slips_send_packet(const slips_send_context_t *context)
{
    char c;
    bool eof;

    if (context->send_start)
    {
        if (!context->encode_send_char_fn(END, context->user_data))
            return false;
    }

    while (true)
    {
        if (!context->encode_read_char_fn(&c, &eof, context->user_data))
            return false;

        if (eof)
            break;

        switch (c)
        {
        case END:
            if (!context->encode_send_char_fn(ESC, context->user_data))
                return false;
            if (!context->encode_send_char_fn(ESC_END, context->user_data))
                return false;
            break;

        case ESC:
            if (!context->encode_send_char_fn(ESC, context->user_data))
                return false;
            if (!context->encode_send_char_fn(ESC_ESC, context->user_data))
                return false;
            break;

        default:
            if (!context->encode_send_char_fn(c, context->user_data))
                return false;
        }
    }

    if (!context->encode_send_char_fn(END, context->user_data))
        return false;

    return true;
}

bool slips_recv_packet(const slips_recv_context_t *context)
{
    char c;
    bool initial = true;

    while (true)
    {
        if (!context->decode_recv_char_fn(&c, context->user_data))
            return false;

        if (initial && context->check_start) 
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
            if (!context->decode_recv_char_fn(&c, context->user_data))
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
            if (!context->decode_write_char_fn(c, context->user_data))
                return false;
        }
    }

    return true;
}
