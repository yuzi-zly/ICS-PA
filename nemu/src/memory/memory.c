#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

extern int is_mmio(paddr_t addr);
extern uint32_t mmio_read(paddr_t addr,int len,int map_NO);
extern void mmio_write(paddr_t addr,int len,uint32_t data,int map_NO);

/*
uint32_t paddr_read(paddr_t addr, int len) {
  return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
}
*/
uint32_t paddr_read(paddr_t addr, int len) {
     if(is_mmio(addr)!=-1)
        return mmio_read(addr,len,is_mmio(addr)); 
     else
        return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
}

/*
void paddr_write(paddr_t addr, uint32_t data, int len) {
  memcpy(guest_to_host(addr), &data, len);
}
*/
void paddr_write(paddr_t addr, uint32_t data, int len) {
    if(is_mmio(addr)!=-1)
        mmio_write(addr,len,data,is_mmio(addr));
    else
        memcpy(guest_to_host(addr), &data, len);
}


uint32_t vaddr_read(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_write(addr, data, len);
}
