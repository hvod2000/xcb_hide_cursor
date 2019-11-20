#include <stdio.h>
#include <xcb/xcb.h>
#include <xcb/xfixes.h>
#include <xcb/xinput.h>

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
	struct {
	    xcb_input_event_mask_t head;
	    xcb_input_xi_event_mask_t mask;
	} mask;
	mask.head.deviceid = XCB_INPUT_DEVICE_ALL;
	mask.head.mask_len = sizeof(mask.mask) / sizeof(uint32_t);
	mask.mask = XCB_INPUT_XI_EVENT_MASK_MOTION | XCB_INPUT_XI_EVENT_MASK_BUTTON_PRESS;
	xcb_input_xi_select_events(conn, screen->root, 1, &mask.head);
	xcb_generic_event_t *event;
	xcb_flush(conn);
	while (event = xcb_wait_for_event(conn))
	    printf("Event: %p\n", event);
}
