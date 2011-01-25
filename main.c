#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <caca.h>
#include <mpd/client.h>
#include "main.h"


int cur_scr = halp;
struct mpd_connection *conn;
int cursor = 11;
int highlighted_song = 1;

int playlist_length;
int start_line = 0;

caca_canvas_t *playlist_box;

static char const ducks[] =
"                                __\n"
"                             ___( o)<\n"
"                            \\  <_  )\n"
"                             `___'\n"
" _0< _o<_O<_0<_o<_O<_o<_o<_0<_O<\n"
"'_) '_)'_)'_)'_)'_)'_)'_)'_)'_)\n";

void print_error_and_exit(){
    printf("BUTT\n");
    printf("%s\n", mpd_connection_get_error_message(conn));
    mpd_connection_free(conn);
    exit(1);
}

void response_finish(){
    if(!mpd_response_finish(conn)){
        print_error_and_exit();
    }
}

void draw_errorbox(char *error_msg, caca_canvas_t *cv){
    caca_canvas_t *error_box;
    error_box = caca_create_canvas(20, 20);
    caca_set_color_ansi(error_box, CACA_WHITE, CACA_BLUE);
    caca_printf(error_box, caca_get_canvas_width(error_box)/2,caca_get_canvas_height(error_box)/2,
        "%s", error_msg);
    //caca_set_attr(error_box, caca_get_attr(error_box, caca_get_canvas_width(error_box)/2,caca_get_canvas_height(error_box)/2));
    caca_blit(cv, caca_get_canvas_width(cv)/2, caca_get_canvas_height(cv)/2, error_box, NULL);
    //usleep(6000);
    //getchar();

    return;
}

void print_songtags(struct mpd_connection *conn, caca_canvas_t *cv){
    struct mpd_song *song;
    int y = 0;

    while(((song = mpd_recv_song(conn)) != NULL) && (y < caca_get_canvas_height(cv))){
        if((y++) == cursor){
            caca_set_color_ansi(cv, CACA_BLACK, CACA_GREEN);
            highlighted_song = y;
        }
        else
            caca_set_color_ansi(cv, CACA_GREEN, CACA_BLACK);
        caca_printf(cv, 0, y, "%s", mpd_song_get_tag(song, MPD_TAG_TITLE, 0));
        mpd_song_free(song);
    }
}

void draw_halp(caca_canvas_t *cv){
    caca_set_color_ansi(cv, CACA_CYAN, CACA_BLACK);
    caca_printf(cv, 0, 10, "Halp:");
    caca_set_color_ansi(cv, CACA_BLUE, CACA_BLACK);
    caca_printf(cv, 0, 12, "Global keys:");
    caca_printf(cv, 0, 13, "1: Halp screen");
    caca_printf(cv, 0, 14, "2: Playlist screen");
    caca_printf(cv, 0, 15, "3: Browse screen");
    caca_printf(cv, 0, 16, "<: Play previous song in playlist");
    caca_printf(cv, 0, 17, ">: Play next song in playlist");
    caca_printf(cv, 0, 18, "-: Decrease volume");
    caca_printf(cv, 0, 19, "+: Increase volume");
    caca_printf(cv, 0, 20, "j: Move cursor down");
    caca_printf(cv, 0, 21, "k: Move cursor up");

    caca_set_color_ansi(cv, CACA_CYAN, CACA_BLACK);
    caca_printf(cv, 0, 23, "Playlist screen:");
    caca_set_color_ansi(cv, CACA_BLUE, CACA_BLACK);
    caca_printf(cv, 0, 24, "Enter: Play highlighted song");

    return;
}

void draw_playlist(caca_canvas_t *cv){
    caca_canvas_t *sprite;
    struct mpd_status *status;
    //struct mpd_song *song;
    //int y = 0;
    //int j;
    //int i;

    sprite = caca_create_canvas(0, 0);
    playlist_box = caca_create_canvas(80, caca_get_canvas_height(cv)-11);

    caca_set_color_ansi(cv, CACA_RED, CACA_BLACK);
    caca_set_color_ansi(sprite, CACA_YELLOW, CACA_BLACK);
    caca_import_canvas_from_memory(sprite, ducks, strlen(ducks), "text");
    caca_blit(cv, 30, 6, sprite, NULL);
    caca_printf(cv, 0, 10, "BUTTTT");
    caca_set_color_ansi(cv, CACA_MAGENTA, CACA_BLACK);
    caca_printf(cv, 0, 10, "Playlist:");

    status = mpd_run_status(conn);
    playlist_length = mpd_status_get_queue_length(status);
    caca_printf(cv, 0, 11, "Playlist length:%i", playlist_length);
    if(!mpd_send_list_queue_meta(conn)){
        print_error_and_exit();
    }
    print_songtags(conn, playlist_box);

    response_finish();
    caca_blit(cv, 0, 12, playlist_box, NULL);

    return;
}

void draw_browse(caca_canvas_t *cv){
    const char *lsinfo_path = "/";
    caca_canvas_t *browse_box;
    //struct mpd_entity *entity;
    //initialize entity, run mpd_send_list_meta, then use recv entity?
    browse_box = caca_create_canvas(80, caca_get_canvas_height(cv)-11);

    caca_set_color_ansi(cv, CACA_BROWN, CACA_BLACK);
    caca_printf(cv, 0, 10, "Browse:");
    if(!mpd_send_list_meta(conn, lsinfo_path)){
        print_error_and_exit();
    }
    print_songtags(conn, browse_box);
    response_finish();
    caca_set_color_ansi(browse_box, CACA_GREEN, CACA_BLACK);
    caca_blit(cv, 0, 11, browse_box, NULL);

    return;
}

void draw_status(caca_canvas_t *cv, struct mpd_status *status, struct mpd_song *song){
    const struct mpd_audio_format *audio_format;
    const char *songtitle;

    status = mpd_run_status(conn);
    if(status){
        caca_set_color_ansi(cv, CACA_RED, CACA_BLACK);
        //caca_printf(cv, caca_get_canvas_width(cv)-8, 0, "vol: %i%", mpd_status_get_volume(status));
        caca_printf(cv, 0, 0, "vol: %i%", mpd_status_get_volume(status));
        caca_printf(cv, 0, 1, "%3i:%02i/%i:%02i",
            mpd_status_get_elapsed_time(status) / 60,
            mpd_status_get_elapsed_time(status) % 60,
            mpd_status_get_total_time(status) / 60,
            mpd_status_get_total_time(status) % 60);
        song = mpd_run_current_song(conn);
        songtitle = mpd_song_get_uri(song);
        caca_printf(cv, 0, 2, "%s", songtitle);
        audio_format = mpd_status_get_audio_format(status);
        if(audio_format != NULL){
            caca_printf(cv, 0, 3, "samplerate: %i", audio_format->sample_rate);
            caca_printf(cv, 0, 4, "bits: %i", audio_format->bits);
            caca_printf(cv, 0, 5, "channels: %i", audio_format->channels);
        }
    }
}

struct mpd_connection *mpd_init_connection(){
    struct mpd_connection *conn;

    conn = mpd_connection_new(NULL, 0, 30000);
    if(conn == NULL){
        printf("Out of memory, couldn't start mpd connection.\n");
        exit(1);
    }
    if(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS){
        print_error_and_exit();
 /*       printf("%s\n", mpd_connection_get_error_message(conn));
        mpd_connection_free(conn);
        exit(1); */
    }
    return conn;
}

int main(int argc, char **argv){
    struct mpd_song *song;
    struct mpd_status *status;
    int vol = 0;
    //int playlist_length;
    //int error = 0;

    caca_canvas_t *cv;
    caca_display_t *dp;

    int quit = 0;

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
    caca_set_display_time(dp, 40000);


    while(!quit){
        caca_event_t ev;
        caca_refresh_display(dp);
        caca_clear_canvas(cv);
        draw_status(cv, status, song);
        switch(cur_scr){
        case halp:
            draw_halp(cv);
            break;
        case playlist:
            draw_playlist(cv);
            break;
        case browse:
            draw_browse(cv);
            break;
        }
        while(caca_get_event(dp, CACA_EVENT_ANY, &ev, 0)){
            if(caca_get_event_type(&ev) & CACA_EVENT_KEY_PRESS){
                switch(caca_get_event_key_ch(&ev)){
                case '1':
                    cur_scr = halp;
                    break;
                case '2':
                    cur_scr = playlist;
                    break;
                case '3':
                    cur_scr = browse;
                    break;
                case 'j':
                    switch(cur_scr){
                    case playlist:
                        if(cursor < playlist_length){
                            cursor++;
                        }
                        if((cursor >= caca_get_canvas_height(playlist_box)) && 
                            (start_line < caca_get_canvas_height(playlist_box) - playlist_length))
                            start_line++;
                    default:
                        break;
                    }
                    break;
                case 'k':
                    if(cursor)
                        cursor--;
                    break;
                case 'q':
                case 'Q':
                case CACA_KEY_ESCAPE:
                    quit = 1;
                    break;
                case '+': //increase volume
                    status = mpd_run_status(conn);
                    if((vol = mpd_status_get_volume(status)) >= 0){
                        if(vol + 1 <= 100)
                            if(!mpd_run_set_volume(conn, mpd_status_get_volume(status)+1))
                                print_error_and_exit();
                    }else{
                        draw_errorbox("Problems setting volume! X_x", cv);
                        caca_refresh_display(dp);
                        caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, -1);
                    }
                    break;
                case '-': //decrease volume
                    status = mpd_run_status(conn);
                    if((vol = mpd_status_get_volume(status)) >= 0){
                        vol = mpd_status_get_volume(status);
                        if(vol - 1 <= 100)
                            if(!mpd_run_set_volume(conn, mpd_status_get_volume(status)-1))
                                print_error_and_exit();
                    }else{
                        draw_errorbox("Problems setting volume! X_x", cv);
                        caca_refresh_display(dp);
                        caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, -1);
                    }
                    break;
                case '>': //next song in playlist
                    mpd_run_next(conn);
                    break;
                case '<': //previous song in playlist
                    mpd_run_previous(conn);
                    break;
                case 's':
                    mpd_run_stop(conn);
                    response_finish();
                    break;
                case 'p':
                    mpd_send_toggle_pause(conn);
                    response_finish();
                    break;
                case '\r':
                    switch(cur_scr){
                    case playlist:
                        mpd_run_play_pos(conn, cursor);
                        break;
                    default:
                        break;
                    }
                    break;
                }
            }
        }
    }
    /*cleanup caca*/
    caca_free_canvas(cv);
    caca_free_display(dp);
    /*cleanup mpd*/
    //mpd_status_free(status);
    //mpd_song_free(song);
    mpd_connection_free(conn);
    return 0;
}
