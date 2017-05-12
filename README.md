# pokecrypt
crappy tool to decrypt/encrypt pkx files (gen 3 - 7)

Usage:
pokecrypt.exe inpkm [-d|-e] outpkm
        -d      decrypt Pokemon file
        -e      encrypt Pokemon file
        inpkm   Pokemon file to decrypt/encrypt
        outpkm  decrypted/encrypted Pokemon file

3rd Generation Pokemon data should be:
	80 Bytes (box)
	100 Bytes (part)
4th Generation Pokemon data should be:
	136 Bytes (box)
	236 Bytes (party)
5th Generation Pokemon data should be:
	136 Bytes (box)
	220 Bytes (party)
6th and 7th Generation Pokemon data should be:
	232 Bytes (box)
	260 Bytes (party)
