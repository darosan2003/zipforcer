# zipforcer

![](https://img.shields.io/badge/made%20with-C-blue)

### Description
This program will attempt to open a password protected zip by bruteforcing.
In case of finding the password it will be presented on screen, otherwise
error message will be shown.

---

### Table of Content
* Usage
* Contribute
---

### Usage
 What follows is an explanation of how to use this program

#### Compilation
To compile just use Make with the following parameters:
```bash
make all
``` 
It will create a folder containing all the .o files, if you want to delete them all including the executable run:
```bash
make clean
```

---

#### Execution
This program takes two arguments. The zip you want to break and the passlist which
contains passwords, one at a line.
```bash
./zipforcer <zipfile> <passlist>
```

#### Example
![zipforcer-cap](https://user-images.githubusercontent.com/67020959/176444844-4249ad0a-e3f4-49b5-98df-d0822b940c21.png)

---

### Contribute
I feel this program could be optimized of some ways, whether it be another way of reading
the passfile more efficiently or getting rid of some unnecessary lines. I would appreciate
any kind of suggestions.
