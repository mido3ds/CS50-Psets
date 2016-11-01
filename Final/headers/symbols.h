/**
*   symbols in .qzm files
*   written by Mahmoud Adas
*   21/9/2016
*/

#ifndef SYMBOLS_H
#define SYMBOLS_H

#define NEW_LINE '\n'

enum QUESTIONTYPE{TF_Question, MC_Question, TY_Question};

// True or False Question (TF)

/**
* ascii symbols used when writing 
* TF Questions in file
*/
namespace TF {
    enum class SYMBOL {
        START = 230,      // æ
        END = START,    // æ
        TRUE_ANS = 116,     // t
        FALSE_ANS = 102,        // f
        EXPLAIN = 58        // :
    };
}

// Multi-Choice Questions (MC)

/**
* ascii symbols used when writing 
* MC Questions in file
*/
namespace MC {
    enum class SYMBOL {
        START = 182,        // ¶
        END = START,      // ¶
        CHOICE = 187,       // »
        ANSWER = 185        // ¹
    };
};

// Typing Questions (TY)

/**
* ascii symbols used when writing 
* TY Questions in file
*/
namespace TY {
    enum class SYMBOL {
        START = 191,        // ¿
        END = START,        // ¿
        ANSWER = 186        // º
    };
}

#endif