#ifndef __XUNJI_H
#define __XUNJI_H



#define SEARCH_L_IO          GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14)
#define SEARCH_R_IO          GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define SEARCH_M_IO           GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)


void xunji_Init(void);


#endif













