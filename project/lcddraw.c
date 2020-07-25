#include "lcdutils.h"
#include "lcddraw.h"
#include "stateMachine.h"
#include "toggle.h"

/** next two functions are used to draw a string to the screen*/
void drawChar8x12(u_char rcol, u_char rrow,char c, u_int fgColorBGR, u_int bgColorBGR)
{
  u_char col = 0;
  u_int row = 0;    
  u_char bit = 0x080;
  u_char oc = c - 0x020;

  lcd_setArea(rcol, rrow, rcol + 7, rrow + 12);
  while(row < 12){
    while(col < 8){
      u_int colorBGR = (font_8x12[oc][row] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
      bit >>= 1;
    }
    col = 0;
    bit = 0x80;
    row++;
  }
 }

void drawString8x12(u_char col,u_char row, char *string,u_int fgColorBGR, u_int byColorBGR)
{
  u_char cols=col;
  while(*string){
    drawChar8x12(cols,row,*string++,fgColorBGR,byColorBGR);
    cols +=11;
  }
}

/** draw the space ship to the screen*/
void spaceShip(char col, char row, u_int colorBGR){
  //bottom part of the triangle
  for(int i=0; i<=17; i++){
    for(int j=0; j<=i; j++){
      drawPixel(16-j+47, 159-i,colorBGR);
      drawPixel(17+j+48, 159-i,colorBGR);
    }
  }

  //rectagle at the top of the spaceship
  for(int i=0; i <= 20; i+=2){
    for(int j=0; j <= 5; j+=2){
      drawPixel(i+col+10, row-j, colorBGR);
    }
  }

  //left triangle
  for(int i = 0; i <= 10; i++){
    for(int j =0; j <= i; j++){
      drawPixel(col+i, row-j+1, colorBGR);//+1 to make it come down one more row
    }
  }
  //since i want to push the r triangle to the right i add 30 more columns
 for(int i = 0; i <= 10; i++){
    for(int j = i; j <= 10; j++){
      drawPixel(col+i+27, row+j-9, colorBGR); 
    }
  }
}
/* funtion to draw enemy spaceships */
void enemy(char col, char row, u_int colorBGR){
  for(int r = 0; r <= 20; r++){
    // c+=2 to make the figure dotted,3 for pattern
    for(int c = r; c <= 10; c+=2){
      //right triangle of spaceship
      drawPixel(col+c, row+r, colorBGR);
      //left triangle of spaceship      
      drawPixel(col-c, row+r, colorBGR);//+4 in the column to make them overlap
    }
  }
}

/*draw a rectangle to simulate bullets of a fixed size*/
void bullet(u_char col, u_char row, u_int colorBGR){
fillRectangle(col,row, 3, 12, colorBGR);
}

/*Helper function to call functions and form an image*/
void customShape(u_int bgColor,u_int enemyColorBGR, u_int bulletRBG, u_int spaceship){
  clearScreen(bgColor);
 
  spaceShip(45,140,spaceship); /* spaceship */
  enemy(20,30, enemyColorBGR); /* enemy space ships */
  enemy(50,50, enemyColorBGR);
  enemy(90,20, enemyColorBGR);
  bullet(70,80,bulletRBG);
  bullet(90,50,bulletRBG);
  bullet(50,105,bulletRBG);
}

/**draws a string of the size 8x12*/
void drawCustomString(){
      demoSong_();   //  assembly state machine funtion
      //clearScreen(COLOR_WHITE);
      drawString8x12(5,20,"Miguel Mun.",COLOR_BLACK,COLOR_WHITE);
      drawString8x12(20,50,"Project-3",COLOR_BLACK,COLOR_WHITE);
      drawString8x12(30,80,"LCD TOY",COLOR_BLACK,COLOR_WHITE);
}

/** Draw single pixel at x,row 
 *
 *  \param col Column to draw to
 *  \param row Row to draw to
 *  \param colorBGR Color of pixel in BGR
 */
void drawPixel(u_char col, u_char row, u_int colorBGR) 
{
  lcd_setArea(col, row, col, row);
  lcd_writeColor(colorBGR);
}

/** Fill rectangle
 *
 *  \param colMin Column start
 *  \param rowMin Row start
 *  \param width Width of rectangle
 *  \param height height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void fillRectangle(u_char colMin, u_char rowMin, u_char width, u_char height, 
		   u_int colorBGR)
{
  u_char colLimit = colMin + width, rowLimit = rowMin + height;
  lcd_setArea(colMin, rowMin, colLimit - 1, rowLimit - 1);
  u_int total = width * height;
  u_int c = 0;
  while ((c++) < total) {
    lcd_writeColor(colorBGR);
  }
}

/** Clear screen (fill with color)
 *  
 *  \param colorBGR The color to fill screen
 */
void clearScreen(u_int colorBGR) 
{
  u_char w = screenWidth;
  u_char h = screenHeight;
  fillRectangle(0, 0, screenWidth, screenHeight, colorBGR);
}

/** 5x7 font - this function draws background pixels
 *  Adapted from RobG's EduKit
 */
void drawChar5x7(u_char rcol, u_char rrow, char c, 
     u_int fgColorBGR, u_int bgColorBGR) 
{
  u_char col = 0;
  u_char row = 0;
  u_char bit = 0x01;
  u_char oc = c - 0x20;

  lcd_setArea(rcol, rrow, rcol + 4, rrow + 7); /* relative to requested col/row */
  while (row < 8) {
    while (col < 5) {
      u_int colorBGR = (font_5x7[oc][col] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
    }
    col = 0;
    bit <<= 1;
    row++;
  }
}

/** Draw string at col,row
 *  Type:
 *  FONT_SM - small (5x8,) FONT_MD - medium (8x12,) FONT_LG - large (11x16)
 *  FONT_SM_BKG, FONT_MD_BKG, FONT_LG_BKG - as above, but with background color
 *  Adapted from RobG's EduKit
 *
 *  \param col Column to start drawing string
 *  \param row Row to start drawing string
 *  \param string The string
 *  \param fgColorBGR Foreground color in BGR
 *  \param bgColorBGR Background color in BGR
 */
void drawString5x7(u_char col, u_char row, char *string,
		u_int fgColorBGR, u_int bgColorBGR)
{
  u_char cols = col;
  while (*string) {
    drawChar5x7(cols, row, *string++, fgColorBGR, bgColorBGR);
    cols += 6;
  }
}


/** Draw rectangle outline
 *  
 *  \param colMin Column start
 *  \param rowMin Row start 
 *  \param width Width of rectangle
 *  \param height Height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void drawRectOutline(u_char colMin, u_char rowMin, u_char width, u_char height,
		     u_int colorBGR)
{
  /**< top & bot */
  fillRectangle(colMin, rowMin, width, 1, colorBGR);
  fillRectangle(colMin, rowMin + height, width, 1, colorBGR);

  /**< left & right */
  fillRectangle(colMin, rowMin, 1, height, colorBGR);
  fillRectangle(colMin + width, rowMin, 1, height, colorBGR);
}

