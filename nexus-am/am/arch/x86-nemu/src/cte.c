#include <am.h>
#include <x86.h>
#include<klib.h>
static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vectrap();
void vecnull();
void vecsys();
void get_cur_as(_Context *c);
void _switch(_Context *c);

_Context* irq_handle(_Context *tf) {
   // debug
   // printf("%d  %d  %d  %d  %d  %d  %d  %d\n",tf->eax,tf->ecx,tf->edx,tf->ebx,tf->esp,tf->ebp,tf->esi,tf->edi);
    get_cur_as(tf);
    _Context *next = tf;
  if (user_handler) {
    _Event ev={0};
    switch (tf->irq) {
      case 0x80: ev.event= _EVENT_SYSCALL; break;
      case 0x81: ev.event= _EVENT_YIELD; break;
      case 32: ev.event= _EVENT_IRQ_TIMER; break;
      default: ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, tf);
    if (next == NULL) {
        next = tf;
    }
  }

  _switch(next);
  return next;
}

static GateDesc idt[NR_IRQ];

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), vectrap, DPL_KERN);
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_KERN);


  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  _Context * ret_k=(_Context *)((uintptr_t)stack.end-sizeof(_Context)); //find the addr
    
    ret_k->prot=NULL;
    ret_k->edi=0;
    ret_k->esi=0;
    ret_k->ebp=(uintptr_t) stack.end;
    ret_k->esp=0;
    ret_k->ebx=0;
    ret_k->edx=0;
    ret_k->ecx=0;
    ret_k->eax=0;
    ret_k->irq=0x81;
    ret_k->err=0;
    ret_k->eip=(uintptr_t) entry;
    ret_k->cs=8;
    ret_k->eflags=0x2;
    return ret_k;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
