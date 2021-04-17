# SLIPS: SLIP Stream Encoding & Decoding C Library

SLIP is used for simple data framing / delimiting through byte stuffing.
See RFC-1055 for more details on SLIP (contains reference for this implementation).
https://tools.ietf.org/html/rfc1055.html

Copyright (c) 2021 Marco Satti. 
Released under the MIT licence (See LICENCE.md).

## Basic Usage

1. Initialize the library with the required configuration using `slips_init()`.

The configuration required a number of settings & encode/decode callbacks to be filled in. A `user_data` parameter is provided that will be forwarded directly into the callbacks for the application to use.

2. Use the API functions `slips_send_packet()` and `slips_recv_packet()` to send and receive packets as required.

3. De-initialize the library with `slips_deinit()` if required.
