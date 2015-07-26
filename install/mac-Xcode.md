## 1st ##
install `brew` [http://brew.sh/](http://brew.sh/)  

```
$ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

## 2nd ##

install `cmake` [http://www.cmake.org/](http://www.cmake.org/)

```
$ brew install cmake
```

### Maybe Error Case 1 ###

console.log like below :

```
/usr/local/bin/brew: /usr/local/Library/brew.rb: /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/bin/ruby: bad interpreter: No such file or directory
/usr/local/bin/brew: line 26: /usr/local/Library/brew.rb: Undefined error: 0
```

solution : [from here](http://ryantvenge.com/2014/09/ruby-homebrea-yosemite/)

```
$ cd /System/Library/Frameworks/Ruby.framework/Versions
$ sudo ln -s Current 1.8
$ brew update
$ sudo rm 1.8
```

### Maybe Error Case 2 ###

```
cmake: command not found
```

#### Solution 1 ####

```
$ brew link cmake
```

#### Solution 2 ####

find `cmake` dictionary path, can use `$ pwd` to check current path, maybe like below:

```
/usr/local/Cellar/cmake/3.3.0/bin/
```

and then, set PATH to link cmake

```
$ export PATH=/usr/local/Cellar/cmake/3.3.0/bin/:$PATH
```

## 3rd ##

install Xcode & command line tool on App Store.

## 4th ##

```
$ cd ~/Documents/opencv-2.4.11/
```

below `/opencv-2.4.11/`

```
$ mkdir release
$ cd release
$ cmake -G "Unix Makefiles" ..
$ make
$ sudo make install
```

## 5th ##

work on Xcode, build project and set library&linker path.

[reference](http://blogs.wcode.org/2014/11/howto-setup-xcode-6-1-to-work-with-opencv-libraries/)