program KEY_PROG {
	version KEY_VERS {

		keystatus
		KEY_SET(keybuf) = 1;

		cryptkeyres
		KEY_ENCRYPT(cryptkeyarg) = 2;

		cryptkeyres
		KEY_DECRYPT(cryptkeyarg) = 3;

		des_block
		KEY_GEN(void) = 4;

		getcredres
		KEY_GETCRED(netnamestr) = 5;
	} = 1;
	version KEY_VERS2 {

		keystatus
		KEY_SET(keybuf) = 1;

		cryptkeyres
		KEY_ENCRYPT(cryptkeyarg) = 2;

		cryptkeyres
		KEY_DECRYPT(cryptkeyarg) = 3;

		des_block
		KEY_GEN(void) = 4;

		getcredres
		KEY_GETCRED(netnamestr) = 5;

		cryptkeyres
		KEY_ENCRYPT_PK(cryptkeyarg2) = 6;

		cryptkeyres
		KEY_DECRYPT_PK(cryptkeyarg2) = 7;

		keystatus
		KEY_NET_PUT(key_netstarg) = 8;

 		key_netstres
		KEY_NET_GET(void) = 9;

		cryptkeyres
		KEY_GET_CONV(keybuf) = 10;


	} = 2;
} = 100029;
