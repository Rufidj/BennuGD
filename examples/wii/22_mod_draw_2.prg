import "mod_draw"
import "mod_video"
import "mod_mouse"
import "mod_map"

GLOBAL
scr_width = 640, scr_height=480;
End;

Process main()
Begin
    // Check that we can set the video mode before actually setting it
    if(!mode_is_ok(scr_width, scr_height, 16, MODE_WINDOW))
        return -1;
    end;
    set_mode(scr_width, scr_height, 16, MODE_WINDOW);

    /* Draw three long straight lines */
    drawing_map(0, 0);
    drawing_color(rgb(255, 0, 0));
    draw_line(0, scr_height/4, scr_width, scr_height/4);
    drawing_color(rgb(0, 255, 0));
    draw_line(0, 2*scr_height/4, scr_width, 2*scr_height/4);
    drawing_color(rgb(0, 0, 255));
    draw_line(0, 3*scr_height/4, scr_width, 3*scr_height/4);

    while(! mouse.left)
        FRAME;
    end;

    // Delete all the lines
    delete_draw(0);
End;
