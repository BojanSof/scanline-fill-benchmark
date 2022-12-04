#include "lcd_ili9488.h"
#include "main.h"  // For pins
#include "spi.h"  // spi1

// Private functions declarations

// helper functions
static inline void LCD_StartCom(void)
{
	// set CS low
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

static inline void LCD_StopCom(void)
{
	// set CS high
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

static inline void LCD_SetDataWrite(void)
{
	// set DC pin high (data)
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
}

static inline void LCD_SetCmdWrite(void)
{
	// set DC pin low (command)
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Reset the LCD via its Reset pin
 */
static void LCD_Reset(void);

/**
 * @brief Send command to the LCD controller
 * @param cmd Command to send to the controller
 */
static void LCD_WriteCmd(uint8_t cmd);

/**
 * @brief Send 8-bit data to the LCD controller
 * @param data Data to send to the controller
 */
static void LCD_WriteData8(uint8_t data);

/**
 * @brief Send color data to the LCD controller
 * @param color Color to send
 */
static void LCD_WriteDataColor(const color_t color);

/**
 * @brief Send cmd to prepare for GRAM write
 */
static void LCD_PrepareGRAMWrite(void);

// Private functions definitions
void LCD_Reset(void)
{
	// set reset pin low
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	// set reset pin high
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
}

void LCD_WriteCmd(uint8_t cmd)
{
	LCD_StartCom();
	LCD_SetCmdWrite();
	// send command via SPI
	HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
	LCD_StopCom();
}

void LCD_WriteData8(uint8_t data)
{
	LCD_StartCom();
	LCD_SetDataWrite();
	// send data via SPI
	HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
	LCD_StopCom();
}

void LCD_WriteDataColor(const color_t color)
{
	// send red data
	LCD_WriteData8((color >> 8) & 0xFC);
	// send green data
	LCD_WriteData8((color >> 2) & 0xFC);
	// send blue data
	LCD_WriteData8((color << 2) & 0xFC);
}

void LCD_PrepareGRAMWrite(void)
{
	LCD_WriteCmd(0x2C);
}

// Public function definitions
void LCD_Init(Lcd *lcd)
{
	// reset the LCD
 	LCD_Reset();

 	// send initialization sequence
	LCD_WriteCmd(0XF7);
	LCD_WriteData8(0xA9);
	LCD_WriteData8(0x51);
	LCD_WriteData8(0x2C);
	LCD_WriteData8(0x82);
	LCD_WriteCmd(0xC0);
	LCD_WriteData8(0x11);
	LCD_WriteData8(0x09);
	LCD_WriteCmd(0xC1);
	LCD_WriteData8(0x41);
	LCD_WriteCmd(0XC5);
	LCD_WriteData8(0x00);
	LCD_WriteData8(0x0A);
	LCD_WriteData8(0x80);
	LCD_WriteCmd(0xB1);
	LCD_WriteData8(0xB0);
	LCD_WriteData8(0x11);
	LCD_WriteCmd(0xB4);
	LCD_WriteData8(0x02);
	LCD_WriteCmd(0xB6);
	LCD_WriteData8(0x02);
	LCD_WriteData8(0x42);
	LCD_WriteCmd(0xB7);
	LCD_WriteData8(0xc6);
	LCD_WriteCmd(0xBE);
	LCD_WriteData8(0x00);
	LCD_WriteData8(0x04);
	LCD_WriteCmd(0xE9);
	LCD_WriteData8(0x00);
	LCD_WriteCmd(0x36);
	LCD_WriteData8((1<<3)|(0<<7)|(1<<6)|(1<<5));
	LCD_WriteCmd(0x3A);
	LCD_WriteData8(0x66);
	LCD_WriteCmd(0xE0);
	LCD_WriteData8(0x00);
	LCD_WriteData8(0x07);
	LCD_WriteData8(0x10);
	LCD_WriteData8(0x09);
	LCD_WriteData8(0x17);
	LCD_WriteData8(0x0B);
	LCD_WriteData8(0x41);
	LCD_WriteData8(0x89);
	LCD_WriteData8(0x4B);
	LCD_WriteData8(0x0A);
	LCD_WriteData8(0x0C);
	LCD_WriteData8(0x0E);
	LCD_WriteData8(0x18);
	LCD_WriteData8(0x1B);
	LCD_WriteData8(0x0F);
	LCD_WriteCmd(0XE1);
	LCD_WriteData8(0x00);
	LCD_WriteData8(0x17);
	LCD_WriteData8(0x1A);
	LCD_WriteData8(0x04);
	LCD_WriteData8(0x0E);
	LCD_WriteData8(0x06);
	LCD_WriteData8(0x2F);
	LCD_WriteData8(0x45);
	LCD_WriteData8(0x43);
	LCD_WriteData8(0x02);
	LCD_WriteData8(0x0A);
	LCD_WriteData8(0x09);
	LCD_WriteData8(0x32);
	LCD_WriteData8(0x36);
	LCD_WriteData8(0x0F);
	LCD_WriteCmd(0x11);
	HAL_Delay(120);
	LCD_WriteCmd(0x29);

  	// set orientation
	LCD_SetOrientation(lcd, 0);

	// turn on backlight
	HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_Port, LCD_BACKLIGHT_Pin, GPIO_PIN_SET);

	// clear display with black color
	LCD_Clear(lcd, BLACK);

	// set cursor to (x, y)
	lcd->cursor.x = lcd->cursor.y = 0;
}

void LCD_SetCursor(Lcd *lcd, const Point p)
{
	LCD_SetArea(lcd, p, p);
}

void LCD_SetArea(Lcd *lcd, const Point startPoint, const Point endPoint)
{
	LCD_WriteCmd(0x2A);
	LCD_WriteData8(startPoint.x >> 8);
	LCD_WriteData8(startPoint.x & 0x00FF);
	LCD_WriteData8(endPoint.x >> 8);
	LCD_WriteData8(endPoint.x & 0x00FF);

	LCD_WriteCmd(0x2B);
	LCD_WriteData8(startPoint.y >> 8);
	LCD_WriteData8(startPoint.y & 0x00FF);
	LCD_WriteData8(endPoint.y >> 8);
	LCD_WriteData8(endPoint.y & 0x00FF);

	lcd->cursor.x = startPoint.x;
	lcd->cursor.y = startPoint.y;

	LCD_PrepareGRAMWrite();
}

void LCD_SetOrientation(Lcd *lcd, const uint8_t rotation)
{
	uint8_t memAccReg = 0;
	switch(rotation){
		case 0:
			lcd->width = LCD_WIDTH;
			lcd->height = LCD_HEIGHT;
			memAccReg = (1 << 3) | (0 << 6) | (0 << 7);
		break;
		case 1:
			lcd->width = LCD_HEIGHT;
			lcd->height = LCD_WIDTH;
			memAccReg = (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5);
		break;
		case 2:
			lcd->width = LCD_WIDTH;
			lcd->height = LCD_HEIGHT;
			memAccReg = (1 << 3) | (1 << 6) | (1 << 7);
		break;
		case 3:
			lcd->width = LCD_HEIGHT;
			lcd->height = LCD_WIDTH;
			memAccReg = (1 << 3) | (1 << 7) | (1 << 5);
		break;
		default:
		break;
	}
	if(memAccReg != 0)
	{
		LCD_WriteCmd(0x36);
		LCD_WriteData8(memAccReg);
	}
}

void LCD_Clear(Lcd *lcd, const color_t color)
{
	Point startPoint = {0, 0}, endPoint = {lcd->width - 1, lcd->height - 1};
	LCD_SetArea(lcd, startPoint, endPoint);

	LCD_StartCom();
	LCD_SetDataWrite();

	for(uint16_t x = 0; x < lcd->height; ++x)
	{
		for(uint16_t y = 0; y < lcd->width; ++y)
		{
			LCD_WriteDataColor(color);
		}
	}

	LCD_StopCom();
}

void LCD_DrawPoint(Lcd *lcd, const Point p, const color_t color)
{
	LCD_SetCursor(lcd, p);
	LCD_WriteDataColor(color);
}

void LCD_DrawArea(Lcd *lcd, const color_t *data, const size_t numRows, const size_t numCols)
{
	LCD_StartCom();
	LCD_SetDataWrite();

	for(uint16_t x = 0; x < numRows; ++x)
	{
		for(uint16_t y = 0; y < numCols; ++y)
		{
			LCD_WriteDataColor(data[y + x * numCols]);
		}
	}

	LCD_StopCom();
}
