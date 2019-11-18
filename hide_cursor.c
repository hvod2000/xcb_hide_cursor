#include <unistd.h>
#include <stdio.h>
#include <xcb/xcb.h>
#include <xcb/xfixes.h>

xcb_connection_t *conn;
xcb_screen_t *screen;

int
main(int argc, char *argv[])
{
	conn = xcb_connect(NULL, NULL);
	screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	xcb_xfixes_query_version(conn, 4, 0);
	if (xcb_connection_has_error(conn)) {
		fprintf(stderr, "Cannot open display\n");
		return 1;
	}
	xcb_xfixes_hide_cursor(conn, screen->root);
	xcb_flush(conn);
	while (1);
}
