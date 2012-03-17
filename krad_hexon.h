#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>

#include <cairo.h>

#define hexon_HEIGHT 11
#define hexon_WIDTH 8

typedef struct krad_hexon_St krad_hexon_t;

struct krad_hexon_St {

	int width;
	int height;
	
	int spp_hexrow;
	int spp_hexcol;
	
	int hexoff[hexon_HEIGHT];
	int hexcnt[hexon_HEIGHT];
	
	unsigned char buffer[1920 * 1080 * 4];

};

void krad_hexon_test_encode_png (krad_hexon_t *krad_hexon, char *input_filename, char *output_filename);

void krad_hexon_encode (krad_hexon_t *hexon, int *rgba, unsigned char *encoded);
void krad_hexon_decode (krad_hexon_t *hexon, int *rgba, unsigned char *encoded);

void krad_hexon_destroy(krad_hexon_t *hexon);
krad_hexon_t *krad_hexon_create(int width, int height);
