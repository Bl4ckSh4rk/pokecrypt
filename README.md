# pokecrypt
crappy tool to decrypt/encrypt pkx files (gen 3 - 7)

## how to use
use on commandline
```
pokecrypt.exe inpkm [-d|-e] outpkm  
	-d      decrypt Pokemon file
	-e      encrypt Pokemon file
	inpkm   Pokemon file to decrypt/encrypt
	outpkm  decrypted/encrypted Pokemon file
```

## supported files
* 3rd Generation Pokemon (*.pk3)
	* 80 Bytes (box)
	* 100 Bytes (part)
* 4th Generation Pokemon (*.pk4)
	* 136 Bytes (box)
	* 236 Bytes (party)
* 5th Generation Pokemon (*.pk5)
	* 136 Bytes (box)
	* 220 Bytes (party)
* 6th and 7th Generation Pokemon (*.pk6, *.pk7)
	* 232 Bytes (box)
	* 260 Bytes (party)
