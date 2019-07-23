#ifndef _INETRRUPT_H_
#define _INETRRUPT_H_

void interrupt_done(void);                                                                                                                                                              
void keyboard_interrupt(void);    
void timer_interrupt(void);  
#define irq0 0x60                                                                                                                                                        
#define irq1 0x61
 
#endif _INETRRUPT_H_
