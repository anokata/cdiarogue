#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_framerate.h>
#include <glib.h>

uint32_t rmask, gmask, bmask, amask;

//TODO: make demo scene

#define FRAMES_PER_SECOND 60

typedef struct {
    SDL_Color text_color;
    SDL_Color rect_color;
	uint32_t padx;
	uint32_t pady;
} node_options;

typedef struct {
    char* text;
    //char texts[32];
	char selected;
	node_options* options;
	// pointer to option struct
} list_node;
// struct for common node options like padding, colors
// constructor for default

void render_list(SDL_Surface* surface, GList* list, uint32_t x, uint32_t y);
void add_node(char* text);
void demo_run();
void demo(int tick, SDL_Surface*);

static FPSmanager frames;
static SDL_Surface* screen = NULL;
static TTF_Font* font = NULL;

static GList* test_list = NULL;
static node_options opts = {{0x60, 0x80, 0xC0}, {0x30, 0, 0x40}, 30, 10};
static const int width = 640;
static const int height = 480;

int 
sdl_init() {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    SDL_Init( SDL_INIT_EVERYTHING );
    TTF_Init();
    screen = SDL_SetVideoMode( width, height, 32, SDL_SWSURFACE );
    SDL_initFramerate(&frames);
    SDL_setFramerate(&frames, FRAMES_PER_SECOND);
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 20);
    if (!font) {
        perror("openfont");
        return -1;
    }
    return 0;
}

int 
init_render() {
    SDL_Rect r = {200, 200};
    SDL_Flip( screen );
    return 0;
}

// State System
#define STATE_DEFAULT 1
#define STATE_EDIT 2
#define STATE_MAX_HANDLERS 100
static int state = STATE_DEFAULT;
typedef void (*state_handler)(void);
static state_handler handlers[STATE_MAX_HANDLERS] = {0};
//state_handler[STATE_EDIT] = //
//static 
void state_set(int s) {
	state = s;
}

void state_set_handler(int st, int event, state_handler fun) {

}

void edit_current() {
	state_set(STATE_EDIT);
}

void key_handler(char k) {
	switch (k) {
		case 'e': edit_current(); break;
	}
}

void render() {
	SDL_Surface* surf = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
	boxColor(surf, 0, 0, width, height, 0x0FF);

    render_list(surf, test_list, 100, 50);
    demo_run(surf);

	SDL_BlitSurface(surf, NULL, screen, NULL);
    SDL_Flip(screen);
    SDL_FreeSurface(surf);
}

int 
main_loop() {
    SDL_Event event;
    int quit = 0;
    while (quit == 0) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = 1;
            if (event.type == SDL_KEYDOWN) {
                uint8_t *keystates = SDL_GetKeyState( NULL );
                //if (keystates[SDLK_C 
                //GetModState
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: quit = 1; break;
                    default: printf("key%d %c\n", event.key.keysym.sym, event.key.keysym.sym); 
                }
				char key = event.key.keysym.sym;
				key_handler(key);
				if (key >= 'a' && key <= 'z') {
					add_node("key");
				}
            }
        }
        render();
        SDL_framerateDelay(&frames);
    }
    return 0;
}

void 
sdl_free() {
    SDL_Quit();
}

void add_node(char* text) {
	list_node *node_new = calloc(1, sizeof(list_node));
	node_new->text = text;
	node_new->options = &opts;
    test_list = g_list_append(test_list, node_new);
}

void render_list(SDL_Surface* surface, GList* list, uint32_t x, uint32_t y) {
    uint32_t height = 30;
    uint32_t width = 20;
    list_node* p = NULL;
    GList* cur = list;
	node_options* options = NULL;
    SDL_Surface* img = NULL;
    SDL_Rect r;
	r.y = y;
    while (cur != NULL) {
        // void render_boxnode(list_node* p)
		r.x = x;
        p = cur->data;
		options = p->options;
		char* text = p->text; 
        img = TTF_RenderText_Shaded(font, text, options->text_color, options->rect_color);
		width = img->w + options->padx; 
		height = img->h + options->pady;
        Uint32 rgbColor = SDL_MapRGB(surface->format, 
				options->rect_color.r, 
				options->rect_color.g, 
				options->rect_color.b);
        boxRGBA(surface, r.x, r.y, r.x + width, r.y+height, 
                options->rect_color.r,
                options->rect_color.g,
                options->rect_color.b,
                255
                );
		r.x += (width - img->w) / 2;
		r.y += (height - img->h) / 2;
        SDL_BlitSurface(img, NULL, surface, &r);
        cur = cur->next;
        r.y += height;
    }
}

void demo_run(SDL_Surface* screen) {
	static int t = 0;
	const int m = 5;
	//if (t % m == 0) {
		demo(t++ / m, screen);
	//} else t++;
}
void demo(int tick, SDL_Surface* screen) {
    if (tick < 100) {
    SDL_Surface* img = NULL;
	char text[20];
	sprintf(text, "%d", tick);
	img = TTF_RenderText_Shaded(font, text, opts.text_color, opts.rect_color);
	SDL_BlitSurface(img, NULL, screen, NULL);
    SDL_Surface* surf;
	surf = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 100, 32, rmask, gmask, bmask, amask);
	rectangleColor(surf, 50, 50, 20, 20, 0xFF00FFFF);
	rectangleColor(surf, 49, 49, 21, 21, 0xFF00FFFF);
	rectangleColor(surf, 49-tick, 49-tick, 21+tick, 21+tick, 0xFF00FFFF*tick);
	SDL_BlitSurface(surf, NULL, screen, NULL);
    }
}

void glibtest() {
	add_node("Abcd_");
	add_node("[text node]");
	add_node("add with func");
}

int 
main( int argc, char* args[] )
{
    glibtest();
    sdl_init();
    init_render();
    main_loop();
    sdl_free();
	g_list_free(test_list);
    return 0;
}
