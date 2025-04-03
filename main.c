#include "stm32f10x.h"
#include "dht11.h"
#include "stdio.h"
#include "stm32f10x_usart.h"

		//TIMER
void timer2_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseInitTypeDef timerInit;
    timerInit.TIM_CounterMode = TIM_CounterMode_Up;// dem len 
    timerInit.TIM_Period = 0xFFFF; 								 // =65535
    timerInit.TIM_Prescaler = 36 - 1;              //36hia tan tu 72MHz xuong 1MHz 
    TIM_TimeBaseInit(TIM1, &timerInit);
    TIM_Cmd(TIM1, ENABLE);
}

		//DELAY 
void Delay_1ms(uint32_t time_ms){
    for (uint32_t i = 0; i < time_ms; i++) // Lap lai time_ms lan, moi lan tuong ung 1ms
    {
        TIM_SetCounter(TIM1, 0); // Dat lai bo dem Timer2 ve 0
        while (TIM_GetCounter(TIM1) < 1000); // Cho den khi Timer2 dem du 1000 xung (~1ms)
    }
}
		//UART
void UART_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //cap clk cho usart1 va gpioA

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//tx
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;          //AF: thay the chuc nang cua cong GPIO thong thuong -> UART,SPI...., PP: muc cao dc keo len 1, muc thap keo xuong 0
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//rx
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;    // thiet bi ben ngoai dieu khien nen ko can dien tro keo len/xuong
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruct; 																				// Khai bao mot bien cau truc de cau hinh USART
		USART_InitStruct.USART_BaudRate = 9600; 																		// Thiet lap toc do baud la 9600 bps
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; 										// Chon do dai du lieu la 8 bit
		USART_InitStruct.USART_StopBits = USART_StopBits_1; 												// Dat 1 bit stop
		USART_InitStruct.USART_Parity = USART_Parity_No; 														// Khong su dung bit chan le (parity)
		USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 								// Cho phep USART hoat dong ca truyen (Tx) va nhan (Rx)
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;// Khong su dung dieu khien luong phan cung (RTS/CTS)

    USART_Init(USART1, &USART_InitStruct);																			// khoi tao uart
    USART_Cmd(USART1, ENABLE);
}

void UART_SendChar(char c) {//gui 1 ki tu qua uart
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//ktra cong tx co san sang truyen data ko?
																																 //USART_FLAG_TXE co bao thanh ghi data trong va san sang nhan du kieu 
																																 //RESET la chua san sang (SET moi tiep tuc) 
    USART_SendData(USART1, c);
}

void UART_SendString(char *str) {//gui 1 chuoi ki tu 
    while (*str) {
        UART_SendChar(*str++);//lay gia tri ki tu tai dia chi str roi tang dia chi con tro ++, gap null thi dung 
    }
}

void UART_SendNumber(uint8_t number) {//chuyen so nguyen -> chuoi va gui qua uart 
    char buffer[10]; 
    sprintf(buffer, "%d", number); // ham chuyen doi so nuyen thanh chuoi ki tu
    UART_SendString(buffer); 
}

int main() {
    UART_Init();
    timer2_Init();
    uint8_t humidity, humtp, temperature, temtp;
    while (1) {
        DHT11_Init(&humidity, &humtp, &temperature, &temtp); //truyen dia chi cua humidity va temperature 
        UART_SendString("Temperature: ");
        UART_SendNumber(temperature);
				UART_SendString(".");
				UART_SendNumber(temtp);
        UART_SendString("*C\n");
        UART_SendString("Humidity: ");
        UART_SendNumber(humidity);
				UART_SendString(".");
				UART_SendNumber(humtp);
        UART_SendString("%\n");
        Delay_1ms(1000);
    }
}