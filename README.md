Just a little toy for encrpyting/decrypting BattleField 3 data files using a very simple algorithm I did not discover.

Made this for a friend, he sourced the algorithm. It's very hacky, but I apologise for nothing. It allows you to decrypt a file, edit it, and re-encrypt it. You will need to patch the CRC-32 checksum at the corresponding point in the file_crcs file, it's in little-endian format, and appears after the UTF-16 (16-bit wide char) filename.Not tested, since I don't have BF3, but the CRC's match if you don't edit the decrypted file.

The usage tells you how to re-encrypt, and decrypt respectively (which is likely the reverse order to how you'd go about it.)


```
Usage: EACryptrBF3.exe e <encrypted_file> <decrypted_file> <output_file>
       EACryptrBF3.exe d <encrypted_file> <decrpyted_file>
```