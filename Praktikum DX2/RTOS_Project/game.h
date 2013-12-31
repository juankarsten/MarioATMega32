#define TIME_INTERVAL 2
#define GAME_PLAY 0
#define GAME_FINAL 1
#define WIN_PASS 10

#define FAST_MODE 2
#define SLOW_MODE 1

void update_game();
void draw_game();
void init_game();
void set_mario(int loc);
int getpass();
int get_mario();
void set_mode(int mod);
