/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <memory/vaddr.h>
#include <memory/paddr.h>

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
	uint8_t cols_max = 4; // maximum number of columns
	word_t word[4];
	int line = len ? ((len - 1) / cols_max) + 1 : 1;
	int num = len;

	for(int i = 0; i < line; i++) {
		for(int j = 0; j < 4; j++) {
			word[j] = vaddr_read(vaddr + i * cols_max + j * type, type);
		}

		switch (num) {
		case 1:
			printf("0x%.8x: 0x%.8x\n", vaddr + i * cols_max * type, word[0]);
			num -= 1;
		break;
		case 2:
			printf("0x%.8x: 0x%.8x 0x%.8x\n", vaddr + i * cols_max * type, word[0], word[1]);
			num -= 2;
		break;
		case 3:
			printf("0x%.8x: 0x%.8x 0x%.8x 0x%.8x\n", vaddr + i * cols_max * type, word[0], word[1], word[2]);
			num -= 3;
		break;
		default:
			printf("0x%.8x: 0x%.8x 0x%.8x 0x%.8x 0x%.8x\n", vaddr + i * cols_max * type, word[0], word[1], word[2], word[3]);
			num -= 4;
		break;
		}
	}

  return MEM_RET_OK;
}
