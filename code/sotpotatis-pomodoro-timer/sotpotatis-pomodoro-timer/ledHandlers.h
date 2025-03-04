/* ledHandlers.h
Defines the interface between C and ASM for ledHandlers.S.
That is, defines C templates for everything in ledHandlers.S.
*/

#ifndef LEDHANDLERS_H_
    #define LEDHANDLERS_H_
    extern void setCharlieplexingState(unsigned char ledNumber);
#endif