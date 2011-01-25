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
void print_songtags(struct mpd_connection *conn, caca_canvas_t *cv);
void draw_errorbox(char *error_msg, caca_canvas_t *cv);
struct mpd_connection *mpd_init_connection();
