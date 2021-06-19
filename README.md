# Hide text in binary file
c program that adds encrypted text to the end of a copy of the binary

password protected. test password is `pass`, salt is `sult` and uses sha512

encryption is AES256. IV is `a test ini value`

'iiiiiiii' is used to denote text section at end of file

## TODO:
text is currently unencrypted
doesn't overwrite properly (should the user just generate a new file every time?)

## WARNING:
this is an experiment/joke

it is not intended for production use

it will not protect you from the nsa
