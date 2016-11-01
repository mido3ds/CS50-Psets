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
#include "tf.h"
#include "symbols.h"

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

    // return question type, or exits on failure
    QUESTIONTYPE Detect_Type(const std::string &line)
    {
        char sym = line[0];

        if (sym == static_cast<char>(TF::SYMBOL::START) )
            return TF_Question;
        else if (sym == static_cast<char>(MC::SYMBOL::START) )
            return MC_Question; 
        else if (sym == static_cast<char>(TY::SYMBOL::START) )
            return TY_Question;
        
        exit(static_cast<int>(ERROR::BAD_WRITTEN_LINE));
    }




    // reads file line by line and pases it to Parse
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
            QUESTIONTYPE type = Detect_Type(line);
            if (type == QUESTIONTYPE::MC_Question)
            {
                //
            }
            else if (type == QUESTIONTYPE::TF_Question)
            {
                using namespace TF;
                Line line_obj;
                
                // get question 
                for (int i = 0;;i++)
                {
                    // read untill end
                    if (line[i] == static_cast<char>(TF::SYMBOL::END))
                        break;
                    
                    // append the letter 
                    line_obj.question = std::append()
                    
                }
            }
            else if (type == QUESTIONTYPE::TY_Question)
            {
                //
            }

            // handle error !!
        }

        return SUCCESS;
    }


}

#endif