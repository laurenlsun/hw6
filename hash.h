#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here

        int numBlocks = k.length()/6 + 1;
        int idx = k.length()-1;
        unsigned long long *w = new unsigned long long[5];
        unsigned long long sum, baseval;
        for (int i=0; i< 5; i++) {
            w[i] = 0;
        }
        
        for (int i=4; i> 5-numBlocks; i--) { // in array, start at last index
            std::cout << "filling w at index " << i << std::endl;
            baseval=1; // start at 36^0
            sum = 0;
            for (int j = 0; j<6; j++) { // get 6 letters
                sum+=letterDigitToNumber(k[idx]) * baseval; //b36 digit * 36^n 
                baseval*=36;
                --idx; // move back 1
            }
            w[i] = sum;
            // w[i] = b36 expansion of k[length-6:length]

        }
        // remainder
        sum = 0;
        // for the remaining characters
        baseval = 1;
        while(idx>=0) {
            std::cout << "converted letter " << k[idx] << " to " << letterDigitToNumber(k[idx]) << std::endl;
            sum+=letterDigitToNumber(k[idx]) * baseval;
            std::cout << "sum is now " << sum << std::endl;
            baseval*=36;
            --idx;
        }
        w[5-numBlocks] = sum;
        for (int i=0; i<5; i++) {
            std::cout << "w[" << i << "] = " << w[i] << std::endl;
        }

        unsigned long long result = 0;
        for (int i=4; i>=0; i--) {
            result+= w[i]*rValues[i];
        }
        delete [] w;
        return result;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if ((letter >= 'a') && (letter <= 'z')) { // is a letter
            return letter-'a'; // = 0 for a, 1, for b, ...
        }
        if ((letter >= 'A') && (letter <= 'Z')) {
            int diff = 'a'-'A';
            return letter+diff-'a';
        }
        else {
            return letter-'0' + 26; // '0' -> 26, '1'->27, ...
        }

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
