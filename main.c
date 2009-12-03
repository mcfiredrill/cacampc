#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <caca.h>
#include <mpd/client.h>

static char const ducks[] =
"                                __\n"
"                        	  ___( o)<\n"
"                            \\  <_  )\n"
"                             `___'\n"
" _0< _o<_O<_0<_o<_O<_o<_o<_0<_O<\n"
"'_) '_)'_)'_)'_)'_)'_)'_)'_)'_)\n";

struct mpd_connection *mpd_init_connection(){
	struct mpd_connection *conn;

	conn = mpd_connection_new(NULL, 0, 30000);
	if(conn == NULL){
		printf("Out of memory, couldn't start mpd connection.\n");
		exit(1);
	}
	if(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS){
		printf("%s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		exit(1);
	}
	return conn;
}

int main(int argc, char **argv){
	struct mpd_connection *conn;
	struct mpd_status *status;
	struct mpd_song *song;

	caca_canvas_t *cv, *sprite, *banner;
	caca_display_t *dp;

	int quit = 0;
	const char *songtitle;

	conn = mpd_init_connection();


	cv = caca_create_canvas(0, 0);
	if(cv == NULL){
		printf("Couldn't create canvas!\n");
		exit(1);
	}
	dp = caca_create_display(cv);
	if(cv == NULL){
		printf("Couldn't create display!\n");
		exit(1);
	}
	sprite = caca_create_canvas(0, 0);
	banner = caca_create_canvas(0, 0);
	if(caca_canvas_set_figfont(banner, "/usr/share/figlet/smblock.tlf"))
		printf("Couldn't open figfont\n");
	caca_set_display_time(dp, 40000);
	caca_set_color_ansi(cv, CACA_RED, CACA_BLACK);
	caca_set_color_ansi(sprite, CACA_YELLOW, CACA_BLACK);
	caca_import_canvas_from_memory(sprite, ducks, strlen(ducks), "text");


	while(!quit){
		caca_event_t ev;
		caca_refresh_display(dp);
		caca_clear_canvas(cv);
		status = mpd_run_status(conn);
		caca_set_color_ansi(cv, CACA_RED, CACA_BLACK);
		caca_printf(cv, 0, 0, "vol: %i%", mpd_status_get_volume(status));
		caca_printf(cv, 0, 1, "%3i:%02i/%i:%02i",
			mpd_status_get_elapsed_time(status) / 60,
			mpd_status_get_elapsed_time(status) % 60,
			mpd_status_get_total_time(status) / 60,
			mpd_status_get_total_time(status) % 60);
		//mpd_send_current_song(conn);
		song = mpd_run_current_song(conn);
		songtitle = mpd_song_get_uri(song);
		caca_printf(cv, 0, 2, "%s", songtitle);
		caca_put_figchar(banner, songtitle[0]);
		caca_blit(cv, 30, 6, sprite, NULL);
		caca_blit(cv, 0, 10, banner, NULL);
		while(caca_get_event(dp, CACA_EVENT_ANY, &ev, 0)){
            if(caca_get_event_type(&ev) & CACA_EVENT_KEY_PRESS){
				switch(caca_get_event_key_ch(&ev)){
					case 'q':
					case 'Q':
					case CACA_KEY_ESCAPE:
						quit = 1;
						break;
					//case '+':
					//case '-':
				}
			}
		}
	}
	/*cleanup caca*/
	caca_free_canvas(cv);
	caca_free_display(dp);
	/*cleanup mpd*/
	mpd_status_free(status);
	mpd_song_free(song);
	mpd_connection_free(conn);
	return 0;
}
