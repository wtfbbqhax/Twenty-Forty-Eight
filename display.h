#include "game.h"

const char *colors[] = {
    "\033[48;5;63m",
    "\033[48;5;69m",  /* 4    */
    "\033[48;5;75m",  /* 8    */
    "\033[48;5;81m",  /* 16   */
    "\033[48;5;87m",  /* 32   */
    "\033[48;5;118m",  /* 64   */
    "\033[48;5;119m", /* 128  */
    "\033[48;5;120m", /* 256  */
    "\033[48;5;190m", /* 512  */
    "\033[48;5;220m", /* 1024 */
    "\033[48;5;226m", /* 2048 */
};

/* TODO: Identify out a oneliner bitshift to
 *       replace  this  brainfart   solution.
 */
static inline const char *
tile_color(Tile t)
{
    switch (t) {
    case 2: return colors[ 0 ];    
    case 4: return colors[ 1 ];    
    case 8: return colors[ 2 ];    
    case 16: return colors[ 3 ];   
    case 32: return colors[ 4 ];   
    case 64: return colors[ 5 ];   
    case 128: return colors[ 6 ];  
    case 256: return colors[ 7 ];  
    case 512: return colors[ 8 ];  
    case 1024: return colors[ 9 ]; 
    // Highlighting logic bombs after surpassing
    // 2048  tile.  Just  like  the  real  game!
    case 2048: return colors[ 10 ];
    default: return "";
    }
}

class Display: public Observer
{
public:
  Display(Game *g): Observer(g) { }
  void reset() {};
  void update()
  {
    Game *game = this->model();
    Grid grid = game->grid();
    const char *linecolors[4];
    int i;

    cout << "\033[?25h\033[0m\033[H\033[2J"
         << endl
         << "         SCORE: " << game->score() << endl << endl;

    for (auto const& line: grid)
    {
      i = 0;
      linecolors[0] = linecolors[1] = linecolors[2] = linecolors[3] = "";

      for (auto const& tile: line) 
      {
        if (!tile)
        {
          cout << "          ";
        }
        else
        {
          linecolors[i] = tile_color(tile);
          cout << tile_color(tile) << setw(10) << tile << "\033[0m";
        }
        ++i;
      }

      cout << endl
           << linecolors[0] << "          " << "\033[0m"
           << linecolors[1] << "          " << "\033[0m"
           << linecolors[2] << "          " << "\033[0m"
           << linecolors[3] << "          " << "\033[0m"
           << endl
           << linecolors[0] << "          " << "\033[0m"
           << linecolors[1] << "          " << "\033[0m"
           << linecolors[2] << "          " << "\033[0m"
           << linecolors[3] << "          " << "\033[0m"
           << endl
           << linecolors[0] << "          " << "\033[0m"
           << linecolors[1] << "          " << "\033[0m"
           << linecolors[2] << "          " << "\033[0m"
           << linecolors[3] << "          " << "\033[0m"
           << endl
           << linecolors[0] << "          " << "\033[0m"
           << linecolors[1] << "          " << "\033[0m"
           << linecolors[2] << "          " << "\033[0m"
           << linecolors[3] << "          " << "\033[0m"
           << endl;

    }

  if ( game->gameOver() )
    cout << "Game Over" << endl;
  }
};

