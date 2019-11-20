#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xfixes.h>
#include <xcb/xinput.h>

xcb_connection_t *conn;
xcb_screen_t *screen;

void
show_cursor()
{
	xcb_xfixes_show_cursor(conn, screen->root);
	xcb_flush(conn);
}

void
hide_cursor()
{
	xcb_xfixes_hide_cursor(conn, screen->root);
	xcb_flush(conn);
}

int
main(int argc, char *argv[])
{
	char opt;
	int idle_time = 0;
	if (argc == 2)
		idle_time = atoi(argv[1]) * 1000;

	conn = xcb_connect(NULL, NULL);
	screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	xcb_xfixes_query_version(conn, 4, 0);
	if (xcb_connection_has_error(conn))
		return 1;

	if (idle_time < 1) { // always hide cursor if -t is not set
		hide_cursor();
		sleep(-1);
	}

	struct {
		xcb_input_event_mask_t head;
		xcb_input_xi_event_mask_t mask;
	} mask;
	mask.head.deviceid = XCB_INPUT_DEVICE_ALL;
	mask.head.mask_len = sizeof(mask.mask) / sizeof(uint32_t);
	mask.mask = XCB_INPUT_XI_EVENT_MASK_MOTION | XCB_INPUT_XI_EVENT_MASK_BUTTON_PRESS;
	xcb_input_xi_select_events(conn, screen->root, 1, &mask.head);
	xcb_generic_event_t *event;
	while(1) {
		hide_cursor();
		event = xcb_wait_for_event(conn);
		show_cursor();
		do {
			do free(event);
			while (event = xcb_poll_for_event(conn));
			usleep(idle_time);
		} while (event = xcb_poll_for_event(conn));
	}
}
