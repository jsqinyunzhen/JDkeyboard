#ifndef _TA6932_H_
#define _TA6932_H_

#define TA6932_LEDCOUNT    20

typedef struct
{
    unsigned char num[TA6932_LEDCOUNT][3];
} Display_num;

extern u16 ta6932_number[];
extern unsigned char  channel_num;

void ta6932_channel_num_init(unsigned char num);

extern void TA6932_Init(void);

extern void TA6932_DisplayAllNumber(u16 *number);
extern void TA6932_DisplayPortNumber(u8 port, u16 number);

#endif

