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
typedef bool (*slips_encode_send_char_fn)(char c, void *user_data);

/** 
 * \brief Encode buffer read byte callback.
 * \param c Character to encode.
 * \param eof Used to indicate to SLIPS if the read stream has no more data to be read.
 * \param user_data User data.
 * \return If the callback succeeded or not.
 */
typedef bool (*slips_encode_read_char_fn)(char *c, bool *eof, void *user_data);

/** 
 * \brief Receive character callback.
 * \param c Character to decode.
 * \param user_data User data.
 * \return If the callback succeeded or not.
 */
typedef bool (*slips_decode_recv_char_fn)(char *c, void *user_data);

/** 
 * \brief Decode buffer write byte callback.
 * \param c Decoded character.
 * \param user_data User data.
 * \return If the callback succeeded or not.
 */
typedef bool (*slips_decode_write_char_fn)(char c, void *user_data);

/** 
 * \brief SLIPS configuration.
 */
typedef struct SLIPS_CONFIG {
    slips_encode_send_char_fn encode_send_char;
    slips_encode_read_char_fn encode_read_char;
    slips_decode_recv_char_fn decode_recv_char;
    slips_decode_write_char_fn decode_write_char;
    bool send_start; /**< When encoding a packet, send an initial END byte */
    bool check_start; /**< Check the start of the packet for the END byte. */
    void *user_data; /**< User data parameter forwarded to the callbacks. */
} slips_config;

/** 
 * \brief SLIPS library initialization. 
 * \warning Use this function first before any other library functions.
 * \param config Initialize with the given configuration.
 */
void slips_init(slips_config *config);

/** 
 * \brief SLIPS library deinitialization. 
 * \warning Do not use any other library functions after calling until re-initialized.
 */
void slips_deinit(void);

/** 
 * \brief Encode & send an arbitrary length packet with SLIP encoding.
 * \return Successful or not (callback error).
 */
bool slips_send_packet(void);

/** 
 * \brief Receive & decode an arbitrary length packet with SLIP encoding.
 * \return Successful or not (callback error).
 */
bool slips_recv_packet(void);

#endif
