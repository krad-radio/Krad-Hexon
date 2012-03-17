#include <krad_hexon.h>



void krad_hexon_destroy(krad_hexon_t *krad_hexon) {

	free(krad_hexon);

}

void krad_hexon_set_resolution (krad_hexon_t *hexon, int width, int height) {

	hexon->width = width;
	hexon->height = height;	
	
	hexon->spp_hexrow = (hexon->width / HEXON_WIDTH) - 1;
	hexon->spp_hexcol = (hexon->height / HEXON_HEIGHT) - 1;

}


krad_hexon_t *krad_hexon_create(int width, int height) {

	krad_hexon_t *hexon = calloc(1, sizeof(krad_hexon_t));

	krad_hexon_set_resolution (hexon, width, height);
	
	hexon->hexoff[0] = 4;
	hexon->hexcnt[0] = 1;
	hexon->hexoff[1] = 3;
	hexon->hexcnt[1] = 3;
	hexon->hexoff[2] = 2;
	hexon->hexcnt[2] = 5;
	hexon->hexoff[3] = 1;
	hexon->hexcnt[3] = 7;
	hexon->hexoff[4] = 0;
	hexon->hexcnt[4] = 8;
	hexon->hexoff[5] = 0;
	hexon->hexcnt[5] = 8;
	hexon->hexoff[6] = 0;
	hexon->hexcnt[6] = 8;
	hexon->hexoff[7] = 0;
	hexon->hexcnt[7] = 8;
	hexon->hexoff[8] = 2;
	hexon->hexcnt[8] = 5;
	hexon->hexoff[9] = 3;
	hexon->hexcnt[9] = 3;
	hexon->hexoff[10] = 4;
	hexon->hexcnt[10] = 1;
	
	return hexon;

}

void krad_hexon_test_encode_png (krad_hexon_t *hexon, char *input_filename, char *output_filename) {
                                              
	cairo_surface_t *surface;
	cairo_surface_t *original_image;	
	cairo_t *cr;				  
			
	int stride;
	int bytes;
	unsigned char *data;
				
	original_image = cairo_image_surface_create_from_png ( input_filename );
		
	krad_hexon_set_resolution (hexon, 
							   cairo_image_surface_get_width ( original_image ),
							   cairo_image_surface_get_height ( original_image ));
	
	printf("Set resolution to %dx%d from input file %s\n", hexon->width, hexon->height, input_filename);

	stride = cairo_format_stride_for_width (CAIRO_FORMAT_RGB24, hexon->width);
	bytes = stride * hexon->height;
	data = calloc (1, bytes);
	surface = cairo_image_surface_create_for_data (data, CAIRO_FORMAT_RGB24, hexon->width, hexon->height, stride);

	cr = cairo_create (surface);

	cairo_set_source_surface ( cr, original_image, 0, 0 );
	cairo_paint ( cr);

	krad_hexon_encode (hexon, (int *)data, hexon->buffer);
	
	memset (data, '0', hexon->width * hexon->height * 4);
	
	krad_hexon_decode (hexon, (int *)data, hexon->buffer);

	cairo_surface_write_to_png (surface, output_filename);

	cairo_destroy(cr);

	cairo_surface_destroy(surface);
	cairo_surface_destroy(original_image);
	
	free(data);

}

void krad_hexon_encode (krad_hexon_t *hexon, int *rgba, unsigned char *encoded) {

	int sr;
	int sc;
	int hexrowoffset;
	int *encoded_pixels = (int *)encoded;
	
	for (sc = 0; sc < hexon->spp_hexcol; sc++) {
		if (sc % 2) {
			hexrowoffset = HEXON_WIDTH / 2;
		} else {
			hexrowoffset = 0;
		}
		for (sr = 0; sr < hexon->spp_hexrow; sr++) {	
			encoded_pixels[(sr + (sc * hexon->spp_hexrow))] = rgba[hexrowoffset + (sr * HEXON_WIDTH) + (sc * HEXON_HEIGHT * hexon->width + (HEXON_HEIGHT/2 * hexon->width))];
		}
	}

}



void krad_hexon_decode (krad_hexon_t *hexon, int *rgba, unsigned char *encoded) {
		
	int sr;
	int sc;
	int er;
	int ec;
	int hexrowoffset;
	int *encoded_pixels = (int *)encoded;
	
	for (sc = 0; sc < hexon->spp_hexcol; sc++) {
		if (sc % 2) {
			hexrowoffset = (HEXON_WIDTH / 2);
		} else {
			hexrowoffset = 0;
		}
		for (sr = 0; sr < hexon->spp_hexrow; sr++) {
		
			// this kludge just draws the hexel twice for "scaling"
		
			for (ec = 4; ec < HEXON_HEIGHT; ec++) {
				for (er = hexon->hexoff[ec]; er < hexon->hexoff[ec] + hexon->hexcnt[ec]; er++) {
					rgba[hexrowoffset + (er + (sr * HEXON_WIDTH)) + (((ec + 4) * hexon->width) + (sc * (HEXON_HEIGHT - 0) * hexon->width))] = encoded_pixels[(sr + (sc * hexon->spp_hexrow))];
				}
			}

			for (ec = 0; ec < HEXON_HEIGHT; ec++) {
				for (er = hexon->hexoff[ec]; er < hexon->hexoff[ec] + hexon->hexcnt[ec]; er++) {
					rgba[hexrowoffset + (er + (sr * HEXON_WIDTH)) + ((ec * hexon->width) + (sc * (HEXON_HEIGHT - 0) * hexon->width))] = encoded_pixels[(sr + (sc * hexon->spp_hexrow))];
				}
			}


		}
	}

}
