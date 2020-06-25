# Didn't care about my grammar, sorry not sorry.

# Payload
https://tux-fanpage.2020.redpwnc.tf/page?path=f&path=/../../index.js

### Explanation
The code was assuming that `path` was always a string. It didn't check for that
so when we send path as a list. It was possible to trick `preventDirectoryTraversal` function.
Also before reading, `prepare` function was joining './public' with `path`, which
will convert the path list to a str with `,` as delimiter. So we need an extra `../` to escape from the `path[0]+','` mocked directory.

### Takeaway
* Look into the root directory of the app for the flag.
* Download the same file that you have to check any difference (Here the flag was there but could not be the case).
* Some ways to get param list
  * path=A&path=B
  * path[]=??
  * path[0]=A&path[1]=B
