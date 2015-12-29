#include <stdio.h>
#include <stdint.h>
#include <string.h>


/*  Constants   */
const unsigned int WORD_SIZE_IN_BYTE = 4;           //  32 bits
const unsigned int CHUNK_SIZE_IN_BYTE = 64;         //  512 bits
const unsigned int CHUNK_SIZE_IN_WORD = 16;         //  512 bits = 16 words * 32 bits/word 
const unsigned int EXTENSION_SIZE_IN_WORD = 80;       

const uint32_t H0 = 0x67452301;     //  0110 0111 0100 0101 0010 0011 0000 0001
const uint32_t H1 = 0xefcdab89;     //  1110 1111 1100 1101 1010 1011 1000 1001
const uint32_t H2 = 0x98badcfe;     //  1001 1000 1011 1010 1101 1100 1111 1110
const uint32_t H3 = 0x10325476;     //  0001 0000 0011 0010 0101 0100 0111 0110
const uint32_t H4 = 0xc3d2e1f0;     //  1100 0011 1101 0010 1110 0001 1111 0000

const uint32_t K1 = 0x5a827999;     //  0101 1010 1000 0010 0111 1001 1001 1001
const uint32_t K2 = 0x6ed9dba1;     //  0110 1110 1101 1001 1110 1011 1010 0001
const uint32_t K3 = 0x8f1bbcdd;     //  1000 1111 0001 1011 1011 1100 1101 1100
const uint32_t K4 = 0xca62c1d6;     //  1100 1010 0110 0010 1100 0001 1101 0110

const unsigned int THE_2_TO_32 = 2^32;


/*  Subfunctions   */

static uint32_t f1 (const uint32_t b, const uint32_t c, const uint32_t d) {     return (b & c) | (!b & d);              }
static uint32_t f2 (const uint32_t b, const uint32_t c, const uint32_t d) {     return b ^ c ^ d;                       }
static uint32_t f3 (const uint32_t b, const uint32_t c, const uint32_t d) {     return (b & c) | (b & d) | (c & d);     }
static uint32_t f4 (const uint32_t b, const uint32_t c, const uint32_t d) {     return b ^ c ^ d;                       }

static uint32_t rotate_left (const uint32_t num, const unsigned int dist) {
    /*
     *  <input>     32-bit number before rotate, distance.
     *  <output>    32-bit number after rotate.
     */
    
    uint32_t result;
    uint64_t buff;
    buff = num << dist;

    result = (buff % THE_2_TO_32) + (buff / THE_2_TO_32);

    return result;
}

hello

/*
 *  This function assume input chunk is preprocessed, so variable initialization, padding, and chunking are all done.   
 *  <input>     chunk.
 *  <inout>     sha1_variables.
 *  <output>    N/A.
 */
static void process_one_chunk (const uint32_t *chunk, uint32_t *sha1_variable) {

    int i;
    uint32_t (*f) (uint32_t b, uint32_t c, uint32_t d);
    uint32_t g, k;
    uint32_t buffer[EXTENSION_SIZE_IN_WORD];
    memcpy (buffer, chunk, CHUNK_SIZE_IN_BYTE);     //  memcpy unit: byte

    /*  Extension   */
    for (i = 16 ; i < 80 ; i ++)
        buffer[i] = buffer[i-16] & buffer[i-14] ^ buffer[i-8] ^ buffer[i-3];

    /*  Loop    */
    for (i = 0 ; i < 80 ; i ++) {
    
        if (i < 20) {
        
            k = K1;
            f = f1;
        }
        else if (i < 40) {
        
            k = K2;
            f = f2;
        }
        else if (i < 60) {
        
            k = K3;
            f = f3;
        }
        else {
        
            k = K4;
            f = f4;
        }

        //  A lrot 5 + E + F + K + word.
        g = (rotate_left(sha1_variable[0], 5) + sha1_variable[4] + f(sha1_variable[1], sha1_variable[2], sha1_variable[3]) + k + buffer[i]) % THE_2_TO_32;
    
        sha1_variable[4] = sha1_variable[3];
        sha1_variable[3] = sha1_variable[2];
        sha1_variable[2] = rotate_left (sha1_variable[1], 30);
        sha1_variable[1] = sha1_variable[0];
        sha1_variable[0] = g;
    }
}

/*
 *  <input>     message, message_length_in_byte.
 *  <output>    hash value (160 bits)
 */
unsigned char *sha1 (const unsigned char *message, const unsigned int message_length_in_byte) {

    /*  Declarations & Initializations  */
    uint32_t hash_vector[5];                        //  Hash vectors.
    uint32_t sha1_variable[5];                      //  Variables.
    uint32_t buffer1 [CHUNK_SIZE_IN_WORD], buffer2 [CHUNK_SIZE_IN_WORD];
    unsigned int chunk_index;
    const uint64_t suffix = message_length_in_byte * 8;     //  To append message length by bit.
    unsigned int space;
    unsigned int i, j, k;

    //  Init hash vectors.  
    hash_vector[0] = H0, hash_vector[1] = H1, hash_vector[2] = H2, hash_vector[3] = H3, hash_vector[4] = H4;
    sha1_variable[0] = H0, sha1_variable[1] = H1, sha1_variable[2] = H2, sha1_variable[3] = H3, sha1_variable[4] = H4;
    chunk_index = 0;

    for (chunk_index = 0, i = message_length_in_byte ; i > 0 ; i -= CHUNK_SIZE_IN_BYTE, chunk_index ++) {
     
        if (i <= CHUNK_SIZE_IN_BYTE) {
     
            /*  Padding */
            memcpy (buffer1, message + i, message_length_in_byte - i);  //  memcpy unit: byte

            space = 448 - ((message_length_in_byte * 8) % 512);
            if (space >= 0) {
            
                /*  buffer1 is enough!  */


                memcpy (buffer1 + i, suffix, 64);
            }
            else {
            
                /*  We need buffer2 */
                space += 512;

                if (i > 0)
                
            
                for (i = space ; i > 0 ; i ++)
            }
        }
        else
            process_one_chunk ((uint32_t *)(message + chunk_index * CHUNK_SIZE_IN_BYTE), sha1_variable);
    }


}

int main (int argc, char *argv[]) {

    /*
    FILE *input_file;
    char buffer[513];
    char prev_byte, byte;
    unsigned int read_amount_in_byte;
    unsigned char chunk[CHUNK_SIZE_IN_BYTE];

    input_file = fopen ("hello.txt", "r");
    read_amount_in_byte = 0;
    prev_byte = '\0';

    while (fscanf (input_file, "%c", &byte) != EOF) {
    
        read_amount_in_byte ++;
        if (prev_byte != '\0')
            snprintf (buffer + strlen(buffer), 512, "%x", (unsigned char)prev_byte);

        prev_byte = byte;

        if (read_amount_in_byte == CHUNK_SIZE_IN_BYTE) {

            //  Do chunk loop
            read_amount_in_byte = 0;
        }
    }
    
    printf ("%s\n", buffer);

    fclose (input_file);
    */
    
    const unsigned char *input = 0x48656c6c6f;
    unsigned char *output;
    output = sha1 (input, strlen(input));
    printf("Output: %s\n", output);

    return 0;
}
