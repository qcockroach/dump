// C++ program to convert binary to decimal
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

// Function to convert binary to decimal
int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }

    return dec_value;
}

int ascii_to_digit (char c)
{
    return c - 48;
}

int binaryToDecimal2(char *n)
{
    char *num = n;
    int size = strlen (n);
    int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;
    for (int i = 0; i < size; ++i) {
        int last_digit = (num[size - i - 1]) - 48; /* convert ascii to digit.  */
        dec_value += last_digit * base;
        base = base * 2;
    }

    return dec_value;
}

/*
 * C Program to Find the Sum of two Binary Numbers
 */
#include <stdio.h>
 
int add2bin()
{
 
    long binary1, binary2;
    int i = 0, remainder = 0, sum[20];
 
    printf("Enter the first binary number: ");
    scanf("%ld", &binary1);
    printf("Enter the second binary number: ");
    scanf("%ld", &binary2);
    while (binary1 != 0 || binary2 != 0)
    {
        sum[i++] =(binary1 % 10 + binary2 % 10 + remainder) % 2;
        remainder =(binary1 % 10 + binary2 % 10 + remainder) / 2;
        binary1 = binary1 / 10;
        binary2 = binary2 / 10;
    }
    if (remainder != 0)
        sum[i++] = remainder;
    --i;
    printf("Sum of two binary numbers: ");
    while (i >= 0)
        printf("%d", sum[i--]);
    return 0;
}


int subtractOne(int x)
{
    int m = 1;
 
    // Flip all the set bits
    // until we find a 1
    while (!(x & m))
    {
        x = x ^ m;
        m <<= 1;
    }
 
    // Flip the rightmost 1 bit
    x = x ^ m;
    return x;
}

string subtract(string minuend, string subtrahend)
{
    string min = minuend;
    string sub = subtrahend;
    string ans;
    bool carry = false;
    for (int i = min.length() - 1; i >= 0; --i)
    {
        // ans[i] = min[i] - sub[i];
        ans[i] = min[i] - sub[i] + int('0');
        cout << "i: " << ans[i] << "\n";
        if(carry == true)
        {
            ans[i]-1;
        }
        if (ans[i]==-1)
        {
            ans[i]+2;
            carry = true;
        }
    }

    cout << "res: "<< ans << endl;
    return ans;
}

int bin2dec(char *n)
{
    char *num = n;
    int size = strlen (n);
    int decval = 0;
    int sign = 1;

    if (n[0] == '1') {
        sign = -1;
    }

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    /* binary convertion with 2's complement */
    for (int i = size - 1; i > 0; --i) {
        int last_digit = (num[i]) - 48; /* convert ascii to digit.  */
        decval += last_digit * base;
        base = base * 2;
    }

    return sign * decval;
}


/* 
    1001
 */


string addBinary(string a, string b)
{
    string result = ""; // Initialize result
    int s = 0;          // Initialize digit sum
 
    // Traverse both strings starting from last
    // characters
    int i = a.size() - 1, j = b.size() - 1;
    while (i >= 0 || j >= 0 || s == 1)
    {
        // Comput sum of last digits and carry
        s += ((i >= 0)? a[i] - '0': 0);
        s += ((j >= 0)? b[j] - '0': 0);
 
        // If current digit sum is 1 or 3, add 1 to result
        result = char(s % 2 + '0') + result;
 
        // Compute carry
        s /= 2;
 
        // Move to next digits
        i--; j--;
    }
    return result;
}

// Driver program
int driver1()
{
    string a = "1101", b="100";
    cout << addBinary(a, b) << endl;
    return 0;
}


char *binsub1 (char *s)
{
    int size = strlen (s);

    for (int i = size - 1; i >= 0; --i) {
        if (s[i] == '1') {
            s[i] = '0';
            return s;
        }

        printf ("[%c]\n", s[i]);
    }

    /* flip all bits.  */
    for (int i = 0; i < size; ++i) {
        s[i] = '1';
    }

    return s;
}




// Driver program to test above function
int main()
{
    char s[] = "1100";
    char *res = binsub1 (s);

    printf ("[%s]\n", res);



    return 0;
}
