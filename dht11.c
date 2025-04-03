#include "dht11.h"
#include "stm32f10x.h"
void DHT11_Init(uint8_t *tem, uint8_t *temtp, uint8_t *hum, uint8_t *humtp) 
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Pin = GPIO_Pin_12; 
    gpioInit.GPIO_Mode = GPIO_Mode_Out_OD; // Cau hinh chan PB12 o che do Open Drain, khi can muc thap, mach keo xuong GND, khi can muc cao, can dien tro pullup ngoai/trong keo len 
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &gpioInit);

    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // Keo chan PB12 xuong muc thap de bat dau giao tiep
    Delay_1ms(20); // Gi? muc thap it nhat 18ms de DHT11 nhan tin hieu Start
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // Tha chan 00 len cao de chuan bi nhan phan hoi tu DHT11
		
		//ktra chan pb12 len cao hay chua
    TIM_SetCounter(TIM2, 0);
		while (TIM_GetCounter(TIM2) < 10) { 
			 	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)) { 
						break; 
				}
		}

		uint16_t u16Tim;
		uint8_t u8Buff[5]; // Du tru bo nho cho 5 byte du lieu (du lieu cua DHT11 se duoc luu vao day)
		
		//ktra xem thoat vvong lap la timeout hay break 
		u16Tim = TIM_GetCounter(TIM2); 
		if(u16Tim >= 10){ 
				while(1){ 
				}
		}
		//ktra dht11 keo xuong chua
		TIM_SetCounter(TIM2, 0); 
		while(TIM_GetCounter(TIM2) < 45){ // 
				if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){ 
						break; 
				}
		}
		//ktra thoi gian thoa man dieu kien
		u16Tim = TIM_GetCounter(TIM2); 
		if((u16Tim >= 45)||(u16Tim <= 5)){ 
				while(1){
				}
		}
		//ktra dht11 keo len chua
		TIM_SetCounter(TIM2, 0); 
		while(TIM_GetCounter(TIM2) < 90){ 
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){ 
						break;
				}
		}
		//ktra thoi gian thoa man dieu kien
		u16Tim = TIM_GetCounter(TIM2); 
		if((u16Tim >= 90)||(u16Tim <= 70)){ 
				while(1){ 
				}
		}
		//ktra thoi gian keo xuong thoa man dieu kien
		TIM_SetCounter(TIM2, 0); 
		while(TIM_GetCounter(TIM2) < 95){ 
				if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){ 
						break; 
				}
		}
		//ktra thoi gian thoa man dieu kien
		u16Tim = TIM_GetCounter(TIM2); 
		if((u16Tim >= 95)||(u16Tim <= 75)){ 
				while(1){ 
				}
		}
	
		//byte 1: 8 bit do am nguyen 
		for (int i=0;i<8;++i){
			TIM_SetCounter(TIM2,0);
			//ktra dht11 keo len cao sau 50us 
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
			//ktra thoi gian thoa man dieu kien 
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			//ktra dht11 keo xuong sau 26-28us hoac 70us tuy la bit 0 hay 1 
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
			//ktra thoi gian thoa man dieu kien 
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}
			u8Buff[0] <<=1;
			if(u16Tim>45) u8Buff[0] |= 1;  // |= dung khi muon bat bit
			else u8Buff[0] &= ~1; // &= khi muon tat bit 
		}
			
		//byte 2: 8 bit do am thap phan
		for (int b=0;b<8;++b){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}

			u8Buff[1] <<=1;
			if(u16Tim>45) u8Buff[1] |= 1;
			else u8Buff[1] &= ~1;
		}
		//byte 3: 8 bit nhiet do nguyen 
		for (int c=0;c<8;++c){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}

			u8Buff[2] <<=1;
			if(u16Tim>45) u8Buff[2] |= 1;
			else u8Buff[2] &= ~1;
		} 
		//byte 4: 8 bit nhiet do thap phan
		for (int d=0;d<8;++d){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}

			u8Buff[3] <<=1;
			if(u16Tim>45) u8Buff[3] |= 1;
			else u8Buff[3] &= ~1;
		}
		//byte 5: 8 bit check sum
		for (int e=0;e<8;++e){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}

			u8Buff[4] <<=1;
			if(u16Tim>45) u8Buff[4] |= 1;
			else u8Buff[4] &= ~1;
		}
		uint8_t u8CheckSum = u8Buff[0] + u8Buff[1] + u8Buff[2] + u8Buff[3];
		if(u8CheckSum != u8Buff[4]) while(1){};
		*tem=u8Buff[0];
		*temtp=u8Buff[1];		
		*hum=u8Buff[2];
		*humtp=u8Buff[3];
}