#define LOG_FILE_PATH "./error.log"
#define LOG_FILE  log

/* debugging macros */
#define LOG(x, fmt) \
{\
  FILE *LOG_FILE = fopen(LOG_FILE_PATH, "a"); \
  fprintf(log, "LOG: %s : %" #fmt "\n", #x, x);\
  fclose(LOG_FILE);\
}

#define CHECK(x) \
{\
  FILE *LOG_FILE = fopen(LOG_FILE_PATH, "a"); \
  fprintf(log, "%s\n", #x);\
  fclose(LOG_FILE);\
}

enum screens {
	halp,
	playlist,
	browse,
	library,
	search,
	visuals
};


void draw_status(caca_canvas_t *cv, struct mpd_status *status, struct mpd_song *song);
void draw_halp(caca_canvas_t *cv);
void draw_playlist(caca_canvas_t *cv);
void draw_browse(caca_canvas_t *cv);
void draw_library();
void draw_search();
void draw_visuals();
void print_songtags(caca_canvas_t *cv);
void draw_errorbox(char *error_msg, caca_canvas_t *cv);
struct mpd_connection *mpd_init_connection();
