gcc -Wall krad_hexon_test.c krad_hexon.c -o krad_hexon_test -I. `pkg-config --libs --cflags xcb x11 gl xext xcb-atom cairo`
