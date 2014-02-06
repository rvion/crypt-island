crypt-island
============

Weird binary matrix encoding and decoding system
Done as a kid :)
Don't look for good practice here.

## encrypted example:

```
25

2 2 0
4 4 0
4 3 3 4 0
25 0
4 3 3 4 0
4 2 2 2 4 0
2 2 4 2 2 0
3 4 3 0
4 2 3 0
11 0
10 0
8 0
6 0
5 0
5 0
5 0
7 0
8 0
9 0
4 3 0
5 2 0
6 2 0
4 2 0
2 2 0
3 4 0

1 0
5 0
5 0
7 0
7 0
1 3 0
7 4 0
8 2 1 0
3 4 3 0
1 4 2 0
1 4 4 0
1 2 13 0
1 16 0
1 15 0
1 2 10 0
1 10 0
3 5 4 0
9 9 0
7 8 0
1 1 0
7 1 0
7 0
5 0
5 0
1 0
```

## decrypted example:

```
1
$$$$$$$$$$
- - - * * - - - - - - - - - - - - - - - * * - - -
- * * * * - - - - - - - - - - - - - - - * * * * -
- * * * * - * * * - - - - - - - * * * - * * * * -
* * * * * * * * * * * * * * * * * * * * * * * * *
- * * * * - * * * - - - - - - - * * * - * * * * -
- * * * * - * * - - - - * * - - - * * - * * * * -
- - - * * - * * - - - * * * * - - * * - * * - - -
- - - - - - * * * - - * * * * - * * * - - - - - -
- - - - - - * * * * - - * * - - * * * - - - - - -
- - - - - - - * * * * * * * * * * * - - - - - - -
- - - - - - - - * * * * * * * * * * - - - - - - -
- - - - - - - - - * * * * * * * * - - - - - - - -
- - - - - - - - - - * * * * * * - - - - - - - - -
- - - - - - - - - - - * * * * * - - - - - - - - -
- - - - - - - - - - - * * * * * - - - - - - - - -
- - - - - - - - - - - * * * * * - - - - - - - - -
- - - - - - - - - - - * * * * * * * - - - - - - -
- - - - - - - - - - - * * * * * * * * - - - - - -
- - - - - - - - - - * * * * * * * * * - - - - - -
- - - - - - - - - - * * * * - - * * * - - - - - -
- - - - - - - - * * * * * - - - - * * - - - - - -
- - - - - - * * * * * * - - - - - * * - - - - - -
- - - - - * * * * - - - - - - - - * * - - - - - -
- - - - - * * - - - - - - - - - - * * - - - - - -
- - - - - * * * - - - - - - - - - * * * * - - - -
$$$$$$$$$$
```
