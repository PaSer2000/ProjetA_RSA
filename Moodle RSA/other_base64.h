#ifndef base64_H
#define base64

#include "rsa_common_header.h"

void build_decoding_table();

void base64_cleanup();

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);

unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);

void encode_file_base64(const char* input_filename, const char* output_filename);

void decode_file_base64(const char* input_filename, const char* output_filename);

#endif