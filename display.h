#include <stdlib.h>
#include "game.h"

#include "cmds.h"
#include "cvar.h"
#include "prompt.h"

extern cvar_t * g_drawResetAnim;

#if 0
#include <string.h>
#include <ao/ao.h>
#include <math.h>

#define BUF_SIZE 4096

int Bzzz()
{
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	char *buffer;
	int buf_size;
	int sample;
	float freq = 440.0;
	int i;

	ao_initialize();
	default_driver = ao_default_driver_id();

    memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = 2;
	format.rate = 44100;
	format.byte_format = AO_FMT_LITTLE;

	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		return 1;
	}

	buf_size = format.bits/8 * format.channels * format.rate;
	buffer = (char*)calloc(buf_size, sizeof(char));

	for (i = 0; i < format.rate; i++)
    {
		sample = (int)(0.75 * 32768.0 *
			asin(2 * M_PI * freq * ((float) i/format.rate)));

		buffer[4*i]   = buffer[4*i+2] = sample & 0xff;
		buffer[4*i+1] = buffer[4*i+3] = (sample >> 8) & 0xff;
	}

	ao_play(device, buffer, buf_size);
	ao_close(device);
	ao_shutdown();

  return (0);
}
#endif

const char **colors;
const char *blues[] = {
    "\033[48;5;63m",  /* 2    */
    "\033[48;5;69m",  /* 4    */
    "\033[48;5;75m",  /* 8    */
    "\033[48;5;81m",  /* 16   */
    "\033[48;5;87m",  /* 32   */
    "\033[48;5;118m", /* 64   */
    "\033[48;5;119m", /* 128  */
    "\033[48;5;120m", /* 256  */
    "\033[48;5;190m", /* 512  */
    "\033[48;5;220m", /* 1024 */
    "\033[48;5;226m", /* 2048 */
};
const char *greens[] = {
    "\033[48;5;195m",  /* 2    */
    "\033[48;5;194m",  /* 4    */
    "\033[48;5;193m",  /* 8    */
    "\033[48;5;192m",  /* 16   */
    "\033[48;5;191m",  /* 32   */
    "\033[48;5;190m", /* 64   */
    "\033[48;5;229m", /* 128  */
    "\033[48;5;118m", /* 256  */
    "\033[48;5;228m", /* 512  */
    "\033[48;5;227m", /* 1024 */
    "\033[48;5;226m", /* 2048 */
};
const char *pinks[] = {
    "\033[48;5;225m",  /* 2    */
    "\033[48;5;219m",  /* 4    */
    "\033[48;5;213m",  /* 8    */
    "\033[48;5;207m",  /* 16   */
    "\033[48;5;212m",  /* 32   */
    "\033[48;5;200m", /* 64   */
    "\033[48;5;201m", /* 128  */
    "\033[48;5;199m", /* 256  */
    "\033[48;5;198m", /* 512  */
    "\033[48;5;197m", /* 1024 */
    "\033[48;5;196m", /* 2048 */
};
const char *browns[] = {
    "\033[48;5;181m",  /* 2    */
    "\033[48;5;183m",  /* 4    */
    "\033[48;5;189m",  /* 8    */
    "\033[48;5;194m",  /* 16   */
    "\033[48;5;192m",  /* 32   */
    "\033[48;5;228m", /* 64   */
    "\033[48;5;226m", /* 128  */
    "\033[48;5;202m", /* 256  */
    "\033[48;5;214m", /* 512  */
    "\033[48;5;208m", /* 1024 */
    "\033[48;5;202m", /* 2048 */
};
const char *purples[] = {
    "\033[48;5;5m",  /* 2    */
    "\033[48;5;4m",  /* 4    */
    "\033[48;5;3m",  /* 8    */
    "\033[48;5;10m",  /* 16   */
    "\033[48;5;2m",  /* 32   */
    "\033[48;5;1m", /* 64   */
    "\033[48;5;7m", /* 128  */
    "\033[48;5;8m", /* 256  */
    "\033[48;5;9m", /* 512  */
    "\033[48;5;3m", /* 1024 */
    "\033[48;5;11m", /* 2048 */
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
    case 64: return colors[ 7 ];  
    case 128: return colors[ 6 ];  
    case 256: return colors[ 5 ];   
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
    void clearScreen()
    { 
        cout << "\033[?25h\033[0m\033[H\033[2J" << endl;
    }

    void drawWaterfall()
    {
        const char *linecolors[4];

        for (int cnt = 0; cnt < 20; cnt++)
        {
            for (int i = 0; i < 4; ++i)
            {
                linecolors[0] = linecolors[1]
                              = linecolors[2]
                              = linecolors[3]
                              = "";
                int idx=0;

                for (int j = 0; j < 4; ++j) 
                {
# define MARGIN  60
                    Tile _2or4orEtc = (rand() % 100) < MARGIN ?    2
                                     : (rand() % 100) < MARGIN ?    4
                                     : (rand() % 100) < MARGIN ?   16
                                     : (rand() % 100) < MARGIN ?   64
                                     : (rand() % 100) < MARGIN ?  256
                                     : (rand() % 100) < MARGIN ? 1024
                                     : 2048; 

                    linecolors[idx] = tile_color(_2or4orEtc);
                    //cout << tile_color(_2or4orEtc) << setw(10) << "          "  << "\033[0m";
                    ++idx;
                }
            }

            for (int i=0; i < 5; ++i) {
            cout<< linecolors[0] << "          " << "\033[0m"
                << linecolors[1] << "          " << "\033[0m"
                << linecolors[2] << "          " << "\033[0m"
                << linecolors[3] << "          " << "\033[0m"
                << endl;
                usleep(9600);
            }
            //usleep(50000);
        }
    }

public:

    Display(Game *g): Observer(g) { }

    void Console() {
           char *in = simple_prompt("\033[1m\033[K|> ", 4096, true);
           //printf("\033[1;7m\n| INPUT: %s\033[0m\n", in);
           Cmd_Exec("| ", in);
           free(in);
    }

    /*****************************************
     * Reset handler
     *
     * 1. Selects a new color palette
     * 2. Draws animation that I call "Tile Waterfall".
     *****************************************/ 
    void reset()
    {
        clearScreen();

        int i = rand() % 5;
        switch (i) {
            case 0:
                colors = blues;
                break;
            case 1:
                colors = greens;
                break;
            case 3:
                colors = pinks;
                break;
            case 4:
                colors = browns;
                break;
            default:
                colors = purples;
                break;
        }

        if (g_drawResetAnim->integer == 1)
            drawWaterfall();

    };

    void update()
    {
        Game *game = this->model();
        Grid grid = game->grid();
        const char *linecolors[4];
        int i;

        clearScreen();
        cout << "SCORE: " << game->score() << endl << endl;

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

