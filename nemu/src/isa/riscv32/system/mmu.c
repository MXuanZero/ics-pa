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

static bool isa_is_mem(vaddr_t addr, int type);

static inline bool isa_is_mem(vaddr_t addr, int type)
{
  return addr >= PMEM_LEFT && (addr + type) <= PMEM_RIGHT;
}

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
	uint8_t cols_max = 4; // maximum number of columns
  vaddr_t addr, read_addr;
	word_t word[4];
	int line = len ? ((len - 1) / cols_max) + 1 : 1;
	int num = len;

	for(int i = 0; i < line; i++) {
    addr = vaddr + i * cols_max * type;

		for(int c = 0; c < 4; c++) {
      read_addr = addr + c * type;
      if(isa_is_mem(read_addr, type) == false) {
        printf("address = 0x%x is out of bound of mem \n", read_addr);
        return MEM_RET_FAIL;
      }
			word[c] = vaddr_read(read_addr, type);
		}

		switch (num) {
		case 1:
			printf("0x%.8x: 0x%.8x\n", addr, word[0]);
			num -= 1;
		break;
		case 2:
			printf("0x%.8x: 0x%.8x 0x%.8x\n", addr, word[0], word[1]);
			num -= 2;
		break;
		case 3:
			printf("0x%.8x: 0x%.8x 0x%.8x 0x%.8x\n", addr, word[0], word[1], word[2]);
			num -= 3;
		break;
		default:
			printf("0x%.8x: 0x%.8x 0x%.8x 0x%.8x 0x%.8x\n", addr, word[0], word[1], word[2], word[3]);
			num -= 4;
		break;
		}
	}

  return MEM_RET_OK;
}
