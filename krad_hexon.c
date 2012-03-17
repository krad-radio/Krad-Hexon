#include <krad_hexon.h>



void krad_hexon_destroy(krad_hexon_t *krad_hexon) {

	free(krad_hexon);

}

krad_hexon_t *krad_hexon_create(int width, int height) {

	krad_hexon_t *krad_hexon = calloc(1, sizeof(krad_hexon_t));

	krad_hexon->width = width;
	krad_hexon->height = height;	
	
	krad_hexon->spp_hexrow = (krad_hexon->width / hexon_WIDTH) - 1;
	krad_hexon->spp_hexcol = (krad_hexon->height / hexon_HEIGHT) - 1;
	
	krad_hexon->hexoff[0] = 4;
	krad_hexon->hexcnt[0] = 1;
	krad_hexon->hexoff[1] = 3;
	krad_hexon->hexcnt[1] = 3;
	krad_hexon->hexoff[2] = 2;
	krad_hexon->hexcnt[2] = 5;
	krad_hexon->hexoff[3] = 1;
	krad_hexon->hexcnt[3] = 7;
	krad_hexon->hexoff[4] = 0;
	krad_hexon->hexcnt[4] = 8;
	krad_hexon->hexoff[5] = 0;
	krad_hexon->hexcnt[5] = 8;
	krad_hexon->hexoff[6] = 0;
	krad_hexon->hexcnt[6] = 8;

	krad_hexon->hexoff[7] = 0;
	krad_hexon->hexcnt[7] = 8;
	krad_hexon->hexoff[8] = 2;
	krad_hexon->hexcnt[8] = 5;
	krad_hexon->hexoff[9] = 3;
	krad_hexon->hexcnt[9] = 3;
	krad_hexon->hexoff[10] = 4;
	krad_hexon->hexcnt[10] = 1;
	
	return krad_hexon;

}

void krad_hexon_test_encode_png (krad_hexon_t *hexon, char *input_filename, char *output_filename) {
                                              
	cairo_surface_t *surface;
	cairo_surface_t *original_image;	
	cairo_t *cr;				  
			
	int stride;
	int bytes;
	unsigned char *data;
				
	original_image = cairo_image_surface_create_from_png ( input_filename );

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
			hexrowoffset = hexon_WIDTH / 2;
		} else {
			hexrowoffset = 0;
		}
		for (sr = 0; sr < hexon->spp_hexrow; sr++) {	
			encoded_pixels[(sr + (sc * hexon->spp_hexrow))] = rgba[hexrowoffset + (sr * hexon_WIDTH) + (sc * hexon_HEIGHT * hexon->width + (hexon_HEIGHT/2 * hexon->width))];
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
			hexrowoffset = (hexon_WIDTH / 2);
		} else {
			hexrowoffset = 0;
		}
		for (sr = 0; sr < hexon->spp_hexrow; sr++) {
		
			// this kludge just draws the hexel twice for "scaling"
		
			for (ec = 4; ec < hexon_HEIGHT; ec++) {
				for (er = hexon->hexoff[ec]; er < hexon->hexoff[ec] + hexon->hexcnt[ec]; er++) {
					rgba[hexrowoffset + (er + (sr * hexon_WIDTH)) + (((ec + 4) * hexon->width) + (sc * (hexon_HEIGHT - 0) * hexon->width))] = encoded_pixels[(sr + (sc * hexon->spp_hexrow))];
				}
			}

			for (ec = 0; ec < hexon_HEIGHT; ec++) {
				for (er = hexon->hexoff[ec]; er < hexon->hexoff[ec] + hexon->hexcnt[ec]; er++) {
					rgba[hexrowoffset + (er + (sr * hexon_WIDTH)) + ((ec * hexon->width) + (sc * (hexon_HEIGHT - 0) * hexon->width))] = encoded_pixels[(sr + (sc * hexon->spp_hexrow))];
				}
			}


		}
	}

}
