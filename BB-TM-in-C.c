/* TERMS OF USE
 * This source code is subject to the terms of the MIT License.
 * Copyright(c) 2026 Vladimir Vasilich Tregub
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define TAPE_LEN (0x10000)

int main(int argc, char* argv[]) {
    int32_t ruleScrArr[12];
    int32_t ruleBidirArr[12];
    uint32_t ruleStateArr[12];

    int32_t POS;
    uint32_t MIN_POS;
    uint32_t MAX_POS;
    uint32_t tmrule = 0;
    uint32_t ruleix = 0;
    uint32_t shifts = 0;

    uint8_t* TAPE = malloc(TAPE_LEN * sizeof(uint8_t));

    uint8_t* domRulesArr = (uint8_t*)malloc(3 * 12 * sizeof(uint8_t*));

    //const char* domRuleStr = "1RH 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ"; // bb(2,1)
    //const char* domRuleStr = "1RB 1LB 1LA 1RH 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ"; // bb(2,2)
    //const char* domRuleStr = "1RB 1RH 0RC 1RB 1LC 1LA 0RZ 0RZ 0RZ 0RZ 0RZ 0RZ"; // bb(2,3)
    //const char* domRuleStr = "1RB 1LB 1LA 0LC 1RH 1LD 1RD 0RA 0RZ 0RZ 0RZ 0RZ"; // bb(2,4)
    //const char* domRuleStr = "1RB 1LC 1RC 1RB 1RD 0LE 1LA 1LD 1RH 0LA 0RZ 0RZ"; // bb(2,5)
    const char* domRuleStr = "1RB 0LD 1RC 0RF 1LC 1LA 0LE 1RH 1LF 0RB 0RC 0RE"; // bb(2,6)

    uint8_t ruleStr[3 * 12 + 1];

    char rulechar;
    int charix = 0;
    int rulecharix = 0;
    if (argc == 1) {
        while ((rulechar = domRuleStr[charix++]) != 0) {
            if (rulechar == ' ' || rulechar == '\t') continue;
            ruleStr[rulecharix++] = rulechar;
            if (rulecharix > 36) break;
        }
    }
    else {
        strcpy_s(ruleStr, 37, argv[1]);
    }
    for (int i = 0; i < 12; ++i)
    {
        rulechar = ruleStr[3 * i];
        if (rulechar == '1' || rulechar == '0')
        {
            ruleScrArr[i] = rulechar - 0x30;
        }
        else
            exit(-1);
        rulechar = ruleStr[3 * i + 1];
        if (rulechar == 'R')
            ruleBidirArr[i] = 1;
        else if (rulechar == 'L')
            ruleBidirArr[i] = -1;
        else
            exit(-1);
        rulechar = ruleStr[3 * i + 2];
        if (rulechar > 0x40 && rulechar < 0x47)
            ruleStateArr[i] = (rulechar - 0x41);
        else
            ruleStateArr[i] = 7;
    }

    POS = TAPE_LEN / 2;
    for (int i = 0; i < TAPE_LEN; i++) {
        TAPE[i] = 0;
    }

    clock_t loopstarted = clock();
    do {
        ruleix = 2 * tmrule + ((TAPE[POS] == 0x31) ? 1 : 0); // TAPE[POS] value selects from two rules per state 
        tmrule = ruleStateArr[ruleix];
        TAPE[POS] = ruleScrArr[ruleix] | 0x30;
        POS += ruleBidirArr[ruleix];
        if (POS < 0 || POS >= TAPE_LEN)
            tmrule = 7; // 'H'
        shifts++;
    } while (tmrule != 7);
    clock_t loopexited = clock();

    int iter = 0;
    while (TAPE[iter] == 0 && iter < TAPE_LEN) {
        iter++;
    }
    MIN_POS = iter;
    while (TAPE[iter] != 0 && iter < TAPE_LEN) {
        iter++;
    }
    MAX_POS = iter;

    uint8_t* tapedisplay = malloc((MAX_POS - MIN_POS + 3) * sizeof(char));
    int tdix = 0;
    int ZEROPOS = TAPE_LEN / 2;
    if (POS < ZEROPOS) {
        for (int i = MIN_POS; i <= POS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
        tapedisplay[tdix++] = '.';
        for (int i = POS + 1; i < ZEROPOS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
        tapedisplay[tdix++] = '^';
        for (int i = ZEROPOS; i <= MAX_POS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
    }
    else {
        for (int i = MIN_POS; i < ZEROPOS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
        tapedisplay[tdix++] = '^';
        for (int i = ZEROPOS; i <= POS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
        tapedisplay[tdix++] = '.';
        for (int i = POS + 1; i <= MAX_POS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
    }
    tapedisplay[MAX_POS - MIN_POS + 2] = 0;

    free(TAPE);

    free(domRulesArr);

    printf("MIN_POS: %d; POS: %d; MAX_POS: %d; \n", MIN_POS, POS, MAX_POS);

    printf("%.1024s\nTape length = %d\n", tapedisplay, (int)strlen((char*)tapedisplay) - 2);

    free(tapedisplay);

    printf("TM loop execution time: %f ms\n", (double)(loopexited - loopstarted) / CLOCKS_PER_SEC);
}
