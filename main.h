enum screens {
	halp,
	playlist,
	browse,
	library,
	search,
	visuals
};

void draw_halp(caca_display_t *dp, caca_canvas_t *cv);
void draw_playlist(caca_display_t *dp, caca_canvas_t *cv);
void draw_browse();
void draw_library();
void draw_search();
void draw_visuals();
struct mpd_connection *mpd_init_connection();
