#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

void generate_random(mpz_t r, mpz_t n) {
    // Generate a random number between 0 and n-1
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_urandomm(r, state, n);
    gmp_randclear(state);
}

int main() {
    mpz_t x, r1, r2, y1, y2, s, n, b;
    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Set the value of the secret number x
    mpz_init_set_ui(x, 42);

    // Set the value of the prime number n
    mpz_init_set_str(n, "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000023", 10);

    // Generate two random numbers r1 and r2
    mpz_init(r1);
    mpz_init(r2);
    generate_random(r1, n);
    generate_random(r2, n);

    // Compute y1 and y2
    mpz_init(y1);
    mpz_init(y2);
    mpz_pow_ui(y1, r1, 2);
    mpz_pow_ui(y2, r2, 2);
    mpz_mul(y2, y2, x);

    // Send y1 and y2 to the verifier
    printf("Prover sends y1 = ");
    mpz_out_str(stdout, 10, y1);
    printf("\nProver sends y2 = ");
    mpz_out_str(stdout, 10, y2);
    printf("\n");

    // Verifier generates a random bit b
    mpz_init(b);
    mpz_urandomb(b, state, 1);

    // Send b to the prover
    printf("Verifier sends b = ");
    mpz_out_str(stdout, 10, b);
    printf("\n");

    // Compute s
    mpz_init(s);
    mpz_powm_ui(s, r2, mpz_get_ui(b), n);
    mpz_mul(s, s, r1);

    // Send s to the verifier
    printf("Prover sends s = ");
    mpz_out_str(stdout, 10, s);
    printf("\n");

    // Verify the proof
    mpz_t lhs, rhs;
    mpz_init(lhs);
    mpz_init(rhs);
    mpz_powm_ui(lhs, s, 2, n);
    mpz_powm_ui(rhs, r1, 2, n);
    mpz_powm_ui(rhs, rhs, mpz_get_ui(b), n);
    mpz_mul(rhs, rhs, y2);

    if (mpz_cmp(lhs, rhs) == 0) {
        printf("Verification succeeded!\n");
    } else {
        printf("Verification failed.\n");
    }

    // Clear memory
    mpz_clear(x);
    mpz_clear(r1);
    mpz_clear(r2);
    mpz_clear(y1);
    mpz_clear(y2);
    mpz_clear(s);
    mpz_clear(n);
    mpz_clear(b);
    mpz_clear(lhs);
    mpz_clear(rhs);
    gmp_randclear(state);

    return 0;
}


//root auth (data =  user 1 and user 2)

//subtree(data = block information with key )
