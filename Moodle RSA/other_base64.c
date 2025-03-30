#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'}; // 64 caractères

static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

void build_decoding_table() {
    decoding_table = calloc(256, sizeof(char));
    if (!decoding_table) return;

    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

void base64_cleanup() {
    free(decoding_table);
    decoding_table = NULL;
}

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length) {
    *output_length = 4 * ((input_length + 2) / 3);
    char *encoded_data = malloc(*output_length + 1);
    if (!encoded_data) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 18) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 12) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 6) & 0x3F];
        encoded_data[j++] = encoding_table[triple & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    encoded_data[*output_length] = '\0';
    return encoded_data;
}

unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length) {
    if (!decoding_table) build_decoding_table();
    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (!decoded_data) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t sextet_a = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i++]];

        uint32_t triple = (sextet_a << 18) 
        + (sextet_b << 12) 
        + (sextet_c << 6) + sextet_d;

        if (j < *output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = triple & 0xFF;
    }
    return decoded_data;
}

void encode_file_base64(const char* input_filename, const char* output_filename) {
    FILE* input = fopen(input_filename, "rb");
    FILE* output = fopen(output_filename, "wb");
    if (!input || !output) {
        if (input) fclose(input);
        if (output) fclose(output);
        return;
    }

    uint8_t buffer[3];
    size_t bytes_read;
    size_t output_length;

    while ((bytes_read = fread(buffer, 1, 3, input)) > 0) {
        char* encoded = base64_encode(buffer, bytes_read, &output_length);
        fwrite(encoded, 1, output_length, output);
        free(encoded);
    }

    fclose(input);
    fclose(output);
}

void decode_file_base64(const char* input_filename, const char* output_filename) {
    FILE* input = fopen(input_filename, "r");
    FILE* output = fopen(output_filename, "wb");
    if (!input || !output) {
        if (input) fclose(input);
        if (output) fclose(output);
        return;
    }

    char buffer[4];
    size_t bytes_read;
    size_t output_length;

    while ((bytes_read = fread(buffer, 1, 4, input)) > 0) {
        uint8_t* decoded = base64_decode(buffer, bytes_read, &output_length);
        fwrite(decoded, 1, output_length, output);
        free(decoded);
    }
    fclose(input);
    fclose(output);
}