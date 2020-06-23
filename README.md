# KECCAK_CPP
Keccak hash algorithm made on C++ as my course work in the university.
This program uses the original Keccak algorithm, that is discribed on Wikipedia or habr.com, with some additions:
- You can choose the hash length: 512, 384, 256, 224, 128 bits;
- It works with russian language as well as with english;
- In squeezing phase to get correct hash my algoritm swaps all elements symmetrically to the main diagonal in state string
(the reason why this is needed is unknown).

You can download the attached .rar archieve to use this program. 
To work properly program needs 3 files in the same directory with .exe file:
1) encode.txt - there is text you need to encode 
2) length.txt - there is neccesary hash length in bits
3) hash.txt - there is your encoded text

- If any of these files is not in the same directory with .exe or has other name you will get an error message in the console;
- By default the hash length is 512 bits, but if you write the wrong length (any length except noted above), you will get
an error message in the console;

