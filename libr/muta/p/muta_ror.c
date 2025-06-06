/* radare - LGPL - Copyright 2016-2024 - pancake */

#include <r_muta.h>

#define NAME "ror"

enum { MAX_ROR_KEY_SIZE = 32768 };

struct ror_state {
	ut8 key[MAX_ROR_KEY_SIZE];
	int key_size;
};

static bool ror_init(struct ror_state *const state, const ut8 *key, int keylen) {
	if (!state || !key || keylen < 1 || keylen > MAX_ROR_KEY_SIZE) {
		return false;
	}
	int i;
	state->key_size = keylen;
	for (i = 0; i < keylen; i++) {
		state->key[i] = key[i];
	}
	return true;
}

static void ror_crypt(struct ror_state *const state, const ut8 *inbuf, ut8 *outbuf, int buflen) {
	int i;
	for (i = 0; i < buflen; i++) {
		ut8 count = state->key[i % state->key_size] & 7;
		ut8 inByte = inbuf[i];
		outbuf[i] = (inByte >> count) | (inByte << ((8 - count) & 7));
	}
}

static bool ror_set_key(RMutaSession *cj, const ut8 *key, int keylen, int mode, int direction) {
	cj->flag = direction;
	free (cj->data);
	cj->data = R_NEW0 (struct ror_state);
	struct ror_state *st = (struct ror_state*)cj->data;
	return ror_init (st, key, keylen);
}

static int ror_get_key_size(RMutaSession *cj) {
	struct ror_state *st = (struct ror_state*)cj->data;
	return st->key_size;
}

static bool ror_check(const char *algo) {
	return !strcmp (algo, NAME);
}

static bool update(RMutaSession *cj, const ut8 *buf, int len) {
	if (cj->flag != R_CRYPTO_DIR_ENCRYPT) {
		return false;
	}
	ut8 *obuf = calloc (1, len);
	if (!obuf) {
		return false;
	}
	struct ror_state *st = (struct ror_state*)cj->data;
	ror_crypt (st, buf, obuf, len);
	r_muta_session_append (cj, obuf, len);
	free (obuf);
	return true;
}

static bool fini(RMutaSession *cj) {
	R_FREE (cj->data);
	return true;
}

RMutaPlugin r_muta_plugin_ror = {
	.type = R_MUTA_TYPE_CRYPTO,
	.meta = {
		.name = NAME,
		.desc = "Rotate Right N bits",
		.author = "pancake",
		.license = "LGPL-3.0-only",
	},
	.set_key = ror_set_key,
	.get_key_size = ror_get_key_size,
	.check = ror_check,
	.update = update,
	.end = update,
	.fini = fini,
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_CRYPTO,
	.data = &r_muta_plugin_ror,
	.version = R2_VERSION
};
#endif
