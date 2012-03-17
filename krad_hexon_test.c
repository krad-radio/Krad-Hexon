#include <krad_hexon.h>


int main ( int argc, char *argv[] ) {

	krad_hexon_t *krad_hexon;
	
	if (argc != 3) {
		printf("Usage: input.png output.png\n");
		exit(1);
	}
	
	krad_hexon = krad_hexon_create (1280, 720);
	
	krad_hexon_test_encode_png (	krad_hexon, argv[1], argv[2] );
	
	krad_hexon_destroy ( krad_hexon );

	return 0;

}
