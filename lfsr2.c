#include <stdio.h>
#include <stdlib.h>

#define STREAM (256 * 8)

/* short program to understand linear feed shift register
 * and its use in stream ciphers.
 */

int
main(int argc, char argv[]) {

        unsigned char in_s, cs, cp, p, nbit, s[STREAM];
        int i, j, k=0;


        in_s = 0xb4;     /* this can be any 8 bit value */
        p = 0x71;        /* max length polynomial x^8+x^4+x^3+x^2+1 = 0b01110001 */

        cs = in_s;      /* copy initial state */

        printf("\nByte values for lfsr with initial value of 0xb4, and bit mask 0x71.\n");
        printf("Should correspond to primitive polynomial x^8+x^4+x^3+x^2+1.\n");

        while (k < STREAM) {
                for (j = 0;j < 8;j++,k++) {
                        cp = nbit = cs & p;

                        for (i = 1;i < 8; i++) { /* xor all bits together */
                                nbit ^= (cp >> i);
                        }
                        s[k] = cs & 0x01;
                        cs = (cs >> 1) | (nbit << 7); /*  rotate in new bit */
                }
                printf(" %02x ",cs);
                if (cs == in_s) {
                         printf("\nreached duplicate at %d.\n", k);
                }

        }
/* print the stream and put it back together */

        printf("\n** stream cipher **\n");
        unsigned char out = 0;
        k = 7;
        for (i = 0;i < STREAM;i++) {
                printf("%d:",s[i]);
                out |= s[i]<<k;
                k--;
                if ((i+1) % 8 == 0) {
                        printf(" %02x ",out);
                        k = 7;out = 0;
                }
        }
}
