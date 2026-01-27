/* TERMS OF USE
 * This source code is subject to the terms of the MIT License.
 * Copyright(c) 2026 Vladimir Vasilich Tregub
*/
#pragma once
#include <stdint.h>

#define TAPE_LEN (0x10000)

int32_t POS;
uint32_t shifts;

uint8_t* TAPE;

double execTime;

int cc_BB(char* jsRuleString);