#include <string>
#include <gmp.h>
#include "secure_rsa.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
using std::string;
/*
m_value: message sent by the user
e_value: public exponent
d_value: private exponent
c_value: decrypted message
n_value: modulus

*/
void Initialize()
{
mpz_init(e_value);
mpz_init(constant_value);
mpz_init(m_value);
mpz_init(p_value);
mpz_init(q_value);
mpz_init(n_value);
mpz_init(rand_num);
mpz_init(d_value);
mpz_init(c_value);
mpf_init(float_value_of_n);
mpz_init(phi_value);
mpz_init(p_minusvalue);
mpz_init(q_minusvalue);
mpf_init(float_value_of_d);
mpz_init(gcd_value_of_e_and_phi);
mpf_init(check_d_value);
mpz_set_ui(e_value, 65537);
mpz_set_ui(constant_value, 1);
mpz_init(zero_constant_value);
gmp_randinit_mt(r_state);
}
void Encryption(mpz_t m_value)
{
    mpz_powm(c_value, m_value, e_value, n_value); 
    gmp_printf("%Zd encrypted value of the message is::", c_value);
}
void Decryption(mpz_t c_value, mpz_t d_value)
{
    mpz_powm(m_value, c_value, d_value, n_value);
    gmp_printf("%Zd decrypted value of the encrypted message is::", m_value);

}
bool CheckForDValue()
{
    mpf_set_z(float_value_of_n, n_value);                       
    mpf_sqrt(check_d_value, float_value_of_n);
    mpf_set_z(float_value_of_d, d_value);
    if(mpf_cmp(float_value_of_d, check_d_value) > 0)
    {
        return true;
    }
    return false;
}
void GeneratePrimes() 
{
    int count_of_primes =0;

    seed = 1234;
    gmp_randseed_ui(r_state, seed);
    while(true)
    {

        mpz_urandomb(rand_num,r_state,2048);
 	if((mpz_even_p(rand_num) == 0))
	{
	    if(mpz_probab_prime_p(rand_num, 100) == 1)
	    {	
                count_of_primes++;
		if(count_of_primes == 1)
		{
		    mpz_set(p_value,rand_num);
		}
		if(count_of_primes == 2)
		{
		    mpz_set(q_value,rand_num);
		    mpz_mul(n_value, p_value, q_value);
    		    mpz_sub(p_minusvalue, p_value, constant_value);
    		    mpz_sub(q_minusvalue, q_value, constant_value);
    		    mpz_mul(phi_value, p_minusvalue, q_minusvalue);
                    mpz_gcd(gcd_value_of_e_and_phi, e_value, phi_value);
    		    mpz_invert(d_value, e_value, phi_value);
                    bool d_value_greater_than_n_power_half = CheckForDValue();
		    if(mpz_cmp(gcd_value_of_e_and_phi, constant_value) == 0)
		    {
			if(!(mpz_cmp(d_value,zero_constant_value) == 0))
			{
			    if(d_value_greater_than_n_power_half)
                              break;
			}
                    }
		    else
		    {
		        count_of_primes =0;		
	            }
			   	
		} 
	    }		
	}
	
    }

}

int main (int argc, char* argv[]) 
{
    clock_t start, end;
    start = clock();
    Initialize();
    unsigned long int i =0;
    char* input;
    printf("Enter Integer value that should get encrypted::");
    scanf("%s", input);
    std::string input_string = std::string(input); 
    i = std::atoi(input_string.c_str());

    mpz_set_ui(m_value, i);
    GeneratePrimes();
    Encryption(m_value);
    Decryption(c_value, d_value); 
    mpz_clear(rand_num);

    gmp_randclear(r_state);
    end = clock();
    double msecs;
    msecs = ((double) (end-start)) * 1000/CLOCKS_PER_SEC;
    printf("%f Execution Time Of The Entire Program::-->",msecs);
    printf("Below are the private key, public key, Modulus and encrypted values of the message given by the user::\n");
    gmp_printf("value of d is --%Zd\n", d_value);
    gmp_printf("value of e is --%Zd\n", e_value);
    gmp_printf("value of n is --%Zd\n", n_value);
    gmp_printf("value of c is --%Zd\n", c_value);
    
    return 0;
}