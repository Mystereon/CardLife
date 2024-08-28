//The Game of Life, also known simply as Life, is a cellular automaton
//devised by the British mathematician John Horton Conway in 1970.
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life

#include "M5Cardputer.h"

//ULTLORES
//        #define GRIDX 30
//        #define GRIDY 20
//        #define CELLXY 4

//LORES
//#define GRIDX = 60
// #define GRIDY = 40
// #define CELLXY = 2
int viewx = 240;
int viewy = 135;
//HIRES
#define GRIDX 240
#define GRIDY 135
#define CELLXY 1

#define TFT_GREY 0x1010
#define TFT_ORANGE 0xFFA5
int runt = 0;
int res = 1;

int rcolor = random(0xFFFF);



//Number of generations
#define NUMGEN 9999
int gens = NUMGEN;
uint16_t genCount = 0;

void setup()   {

    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(2);
    int textsize = M5Cardputer.Display.height() / 60;
    if (textsize == 0) {
        textsize = 1;
    }
    M5Cardputer.Display.setTextSize(textsize);

  M5Cardputer.Display.setCursor(0, 0);
  

  extern const unsigned char gImage_002[];

  M5Cardputer.Display.fillScreen(WHITE);
  M5Cardputer.Display.drawBitmap(0, 0, 80, 160, (uint16_t *)gImage_002);

  delay(2000);

  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.setTextColor(BLACK, WHITE);
  M5Cardputer.Display.setCursor(5, 50);
  M5Cardputer.Display.println(F(" CARD-LIFE "));

  delay(6000);
  M5Cardputer.Display.fillScreen(WHITE);


                                 
  
                      
  

}
//Current grid
uint8_t grid[GRIDX][GRIDY];

//The new grid for the next generation
uint8_t newgrid[GRIDX][GRIDY];

double vbat = 0.0;
int discharge, charge;
double temp = 0.0;
double bat_p = 0.0;
      void loop() {
      
       M5Cardputer.update();
        int GEN_DELAY = analogRead(G0) / 400;
        //Display a simple splash screen
      
      
       
        initGrid();
      
        genCount = NUMGEN;
      
        drawGrid();
      
        //Compute generations
        for (int gen = 0; gen < gens; gen++)
        {
          if (M5Cardputer.Keyboard.isChange()) {
          if (M5Cardputer.Keyboard.isKeyPressed('1')) {
             gen = 0; M5Cardputer.Display.setTextSize(2);  M5Cardputer.Display.setTextColor(TFT_WHITE, RED); M5Cardputer.Display.setCursor(35, 27);
            M5Cardputer.Display.println(F("  NUKE  ")); delay(200); rcolor = random(0xFFFF);runt--;initGrid(); int GEN_DELAY =  20;
          }
          if (M5Cardputer.Keyboard.isKeyPressed('2'))  {
            setres();
            M5Cardputer.Display.setRotation(1);
          }
          }
          vbat      = 0;
          
          M5Cardputer.Display.setTextColor(TFT_WHITE, BLACK);
          M5Cardputer.Display.setCursor(200, 5);
          M5Cardputer.Display.setTextSize(1);
          M5Cardputer.Display.println("GEN");
          M5Cardputer.Display.setCursor(180, 20);
          M5Cardputer.Display.setTextSize(2);
          M5Cardputer.Display.println(gen);
          M5Cardputer.Display.setCursor(200, 45);
          M5Cardputer.Display.setTextSize(1);
          M5Cardputer.Display.println("RUN");
          M5Cardputer.Display.setCursor(200, 60);
          M5Cardputer.Display.setTextSize(2);
          M5Cardputer.Display.println(runt);
          // M5Cardputer.Display.setCursor(0, 70);
          // M5Cardputer.Display.setTextSize(1);
          // M5Cardputer.Display.setTextColor(BLACK, RED);
          // M5Cardputer.Display.printf("%.3fV\r\n", vbat);
          //if (vbat <= 3.8 ) {
          //  M5Cardputer.Display.setCursor(20, 30);
          //  M5Cardputer.Display.setTextSize(3);
           // M5Cardputer.Display.printf("FEED ME");
            
            
          //}
          
          delay(GEN_DELAY);
      
          computeCA();
          drawGrid();
      
          for (int16_t x = 1; x < viewx - 1; x++) {
            for (int16_t y = 1; y < viewy - 1; y++) {
              grid[x][y] = newgrid[x][y];
            }
          }
      
        }
      }

//Draws the grid on the display
void drawGrid(void) {

int color = 0x0000;
  for (int16_t x = 1; x < 240 - 1; x++) {
    for (int16_t y = 1; y < 135 - 1; y++) {
      if ((grid[x][y]) != (newgrid[x][y])) {
        if (newgrid[x][y] == 1) color = rcolor; //random(0xFFFF);
        else color = 0;
        M5Cardputer.Display.fillRect(res * x, res * y, res, res, color);
      }
    }
  }
}

//Initialise Grid
void initGrid(void) {
  M5Cardputer.Display.setRotation(1);
   M5Cardputer.Display.fillScreen(BLACK);
M5Cardputer.Display.drawRect(0, 0, 240, 135, random(0xFFFF));
  for (int16_t x = 0; x < viewx; x++) {
    for (int16_t y = 0; y < viewy; y++) {
      newgrid[x][y] = 0;

      if (x == 0 || x == viewx - 1 || y == 0 || y == viewy - 1) {
        grid[x][y] = 0;
      }
      else {
        if (random(3) == 1)
          grid[x][y] = 1;
        else
          grid[x][y] = 0;
      }

    }
  }
  runt++;
  rcolor = random(0xFFFF);
  
}

//Compute the CA. Basically everything related to CA starts here
void computeCA() {
  for (int16_t x = 1; x < viewx; x++) {
    for (int16_t y = 1; y < viewy; y++) {
      int neighbors = getNumberOfNeighbors(x, y);
      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3  ))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 1)  newgrid[x][y] = 0;
      if (grid[x][y] == 0 && (neighbors == 3|| neighbors == 7))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 0) newgrid[x][y] = 0;
    }
  }
}

// Check the Moore neighborhood
int getNumberOfNeighbors(int x, int y) {
  return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
}

void statusSys() {
  M5Cardputer.Display.setRotation(1);
  extern const unsigned char gImage_002[];
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.fillScreen(BLUE);
  M5Cardputer.Display.drawBitmap(0, 0, 135, 240, (uint16_t *)gImage_002);



 

  M5Cardputer.Display.setTextColor(BLACK, WHITE);
  if (M5Cardputer.Keyboard.isChange()) {
  if (M5Cardputer.Keyboard.isKeyPressed('3')) {


    

    vbat      = 0;
    temp      = 0;
    bat_p     =  0;
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setCursor(0, 15, 1);
    M5Cardputer.Display.printf("batt:%.3fV\r\n", vbat); //battery voltage
    M5Cardputer.Display.printf("temp:%.1fC\r\n", temp); //axp192 inside temp
    M5Cardputer.Display.printf("power:%.3fmW\r\n", bat_p); //battery power
    delay(500);

  }
  }
  delay(200);
  setres();
}


// menu select resolution
void setres(){                           extern const unsigned char gImage_002[];
                                         M5Cardputer.Display.setRotation(2);
                                         rcolor == random(0xFFFF);
                                        M5Cardputer.Display.setTextSize(2);  
                                        M5Cardputer.Display.drawBitmap(0, 0, 80, 160, (uint16_t *)gImage_002);
                                         M5Cardputer.Display.setRotation(1);
                                         if (M5Cardputer.Keyboard.isChange()) {
                                        if (M5Cardputer.Keyboard.isKeyPressed('5')){
                                          M5Cardputer.Display.setCursor(5, 5);
                                          M5Cardputer.Display.setTextColor(BLACK, WHITE);
                                        M5Cardputer.Display.println(F(" CARD-LIFE  "));
                                        M5Cardputer.Display.setCursor(5, 60);
                                        M5Cardputer.Display.println(F(" set Res"));
                                        M5Cardputer.Display.setCursor(120, 60);
                                        M5Cardputer.Display.println(res );
                                        if (M5Cardputer.Keyboard.isKeyPressed('4')) { res++; rcolor == random(0xFFFF); }
                                        if (res == 5) { res = 1;}
                                        CELLXY == res;
                                        if (res ==1) { viewx = 240; viewy = 135;gens = 3999;}
                                        if (res ==2) { viewx = 120; viewy = 67;gens = 1999;}
                                        if (res ==3) { viewx = 40; viewy = 30; gens = 800;}
                                        if (res ==4) { viewx = 30; viewy = 20; gens = 500;}
                                        delay(50);
                                        }
                                       
                                        }
                                        M5Cardputer.Display.fillScreen(BLACK);
}
