#ifndef SLIPS_H_INCLUDED
#define SLIPS_H_INCLUDED

/**
 * \file slips.h 
 * \brief SLIP Stream Encoding & Decoding C Library.
 * 
 * SLIP is used for simple data framing / delimiting through byte stuffing.
 * See RFC-1055 for more details on SLIP (contains reference for this implementation).
 * https://tools.ietf.org/html/rfc1055.html
 * 
 * \copyright Copyright (c) 2021 Marco Satti. 
 * Released under the MIT licence (See LICENCE.md).
 */

#include <stdbool.h>

/** 
 * \brief Encode send character callback.
 * \param c Character to send to output stream.
 * \param user_data User data.
 * \return If the callback succeeded or not.
 */
typedef bool (*slips_encode_send_char_fn_t)(char c, void *user_data);

/** 
 * \brief Encode buffer read byte callback.
 * \param c Character to encode.
 * \param eof Used to indicate to SLIPS if the read stream has no more data to be read.
 * \param user_data User data.
 * \return If the callback succeeded or not.
 */
typedef bool (*slips_encode_read_char_fn_t)(char *c, bool *eof, void *user_data);

/** 
 * \brief Receive character callback.
 * \param c Character to decode.
 * \param user_data User data.
 * \return If the callback succeeded or not.
 */
typedef bool (*slips_decode_recv_char_fn_t)(char *c, void *user_data);

/** 
 * \brief Decode buffer write byte callback.
 * \param c Decoded character.
 * \param user_data User data.
 * \return If the callback succeeded or not.
 */
typedef bool (*slips_decode_write_char_fn_t)(char c, void *user_data);

/** 
 * \brief SLIPS send context.
 */
typedef struct SLIPS_SEND_CONTEXT_T {
    slips_encode_send_char_fn_t encode_send_char_fn;
    slips_encode_read_char_fn_t encode_read_char_fn;
    bool send_start; /**< Send an initial END byte or not */
    void *user_data; /**< User data parameter forwarded to the callbacks. */
} slips_send_context_t;

/** 
 * \brief SLIPS receive context.
 */
typedef struct SLIPS_RECV_CONTEXT_T {
    slips_decode_recv_char_fn_t decode_recv_char_fn;
    slips_decode_write_char_fn_t decode_write_char_fn;
    bool check_start; /**< Check the start of the packet for the END byte. */
    void *user_data; /**< User data parameter forwarded to the callbacks. */
} slips_recv_context_t;

/** 
 * \brief Encode & send an arbitrary length packet with SLIP encoding.
 * \return Successful in encoding & sending a full SLIP's packet or not.
 */
bool slips_send_packet(const slips_send_context_t *context);

/** 
 * \brief Receive & decode an arbitrary length packet with SLIP encoding.
 * \return Successful in receiving & decoding a full SLIP's packet or not.
 */
bool slips_recv_packet(const slips_recv_context_t *context);

#endif
