/**
*   read, encrypt and write a .qzm file 
*   Written by: Mahmoud Adas
*   21/9/2016
*/

#ifndef READ_H
#define READ_H

#include <iostream>
#include <fstream>
#include "error.h"

namespace qzm {

    // XOR encryption/decryption to prevent users from reading answers
    // it is not meant to be high sucered
    std::string EncryptDecrypt(std::string &input)
    {
        std::string key = "This is a key to encrypt the file, plz be honest and do not try to share it with hackers so they do not  cheat, I could not find any key so I wrote this :D :D";

        for (int i = 0; i < input.length(); i++)
        {
            input[i] ^= key[i % key.length()];
        }
        return input;
    }

    // detect question type and pass its content to 
    // question object members
    // takes a line from file, returns 0 on success 
    int Detect_Type(std::string &line)
    {
        
        return SUCCESS;
    }

    // reads file line by line and pases it to Parse
    // returns zero on success
    int Read(std::ifstream &infile)
    {
        // check file is open
        if (!infile.is_open())
        {
            return -1;  // error
        }

        // read each line
        std::string line;
        while (std::getline(infile, line))
        {
            //Parse
        }

        return SUCCESS;
    }


}

#endif